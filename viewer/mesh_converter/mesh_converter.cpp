#include "../vertex.h"
#include "config.h"

#include <sec21/enlose.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <boost/program_options.hpp>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

// static constexpr int kVersionMajor{ VERSION_MAJOR };
// static constexpr int kVersionMinor{ VERSION_MINOR };

namespace fs = std::filesystem;
using namespace std::literals;

template <typename T, auto Q>
auto format_vec(glm::vec<2, T, Q> const& v)
{
   return fmt::format("{{ {:f}, {:f} }}", v.x, v.y);
}

template <typename T, auto Q>
auto format_vec(glm::vec<3, T, Q> const& v)
{
   return fmt::format("{{ {:f}, {:f}, {:f} }}", v.x, v.y, v.z);
}

namespace sec21::viewer
{
   std::ostream& operator<<(std::ostream& os, vertex const& v)
   {
      return os << "\n{ " << format_vec(v.position) << ", " << format_vec(v.normal) << ", " << format_vec(v.tex_coords)
                << " }";
   }
   +
} // namespace sec21::viewer

struct mesh
{
   std::vector<sec21::viewer::vertex> vertices{};
   std::vector<unsigned int> indices{};
};

std::vector<mesh> meshes;

//! \todo generate std::array instead of std::vector
constexpr auto output_file_content = R"(#pragma once
//
// generated file from sec21::viewer::mesh_converter version {:d}.{:d}
// date: {}
//
#include <vector>

namespace {} 
{{
   struct {} 
   {{
      template <typename T>
      static auto vertices() -> std::vector<T> {{
         return {};         
      }}
      static auto indices() -> std::vector<unsigned int> {{
         return {};
      }}
   }};
}})"sv;

auto generate_output_string(mesh const& m, std::string mesh_name = "generated_mesh",
                            std::string full_namespace = "sec21::viewer::vertex_factory")
{
   std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
   auto time = std::chrono::system_clock::to_time_t(now);

   std::stringstream ss;
   ss << std::put_time(std::gmtime(&time), "%F %T%z");
   auto generated_timepoint = ss.str();

   auto output_string =
      fmt::format(output_file_content, kVersionMajor, kVersionMinor, generated_timepoint, full_namespace, mesh_name,
                  //! \todo use fmt::format
                  fmt::join(std::begin(data.value), std::end(data.value), ","),
                  // sec21::enclose(m.vertices),
                  sec21::enclose(m.indices));

   return output_string;
}

auto process_mesh(aiMesh* input_mesh, [[maybe_unused]] aiScene const* scene) -> mesh
{
   decltype(mesh::vertices) vertices;
   decltype(mesh::indices) indices;

   spdlog::debug("mesh process {} vertices", input_mesh->mNumVertices);
   for (decltype(input_mesh->mNumVertices) i = 0; i < input_mesh->mNumVertices; i++) {
      decltype(vertices)::value_type result{
         {input_mesh->mVertices[i].x, input_mesh->mVertices[i].y, input_mesh->mVertices[i].z},
         {input_mesh->mNormals[i].x, input_mesh->mNormals[i].y, input_mesh->mNormals[i].z}};
      // does the mesh contain texture coordinates?
      if (input_mesh->mTextureCoords[0])
         result.tex_coords = {input_mesh->mTextureCoords[0][i].x, input_mesh->mTextureCoords[0][i].y};
      else
         result.tex_coords = glm::vec2(0.0f, 0.0f);

      vertices.push_back(result);
   }

   for (decltype(input_mesh->mNumFaces) i = 0; i < input_mesh->mNumFaces; i++) {
      auto face = input_mesh->mFaces[i];
      // retrieve all indices of the face and store them in the indices vector
      for (decltype(face.mNumIndices) j = 0; j < face.mNumIndices; j++)
         indices.push_back(face.mIndices[j]);
   }

   return {vertices, indices};
}

void process_node(aiNode* node, aiScene const* scene)
{
   spdlog::debug("process {} meshes", node->mNumMeshes);
   // process each mesh located at the current node
   for (decltype(node->mNumMeshes) i = 0; i < node->mNumMeshes; i++) {
      spdlog::debug("process mesh: {}", i);
      // the node object only contains indices to index the actual objects in the scene.
      // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
      auto mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(process_mesh(mesh, scene));
   }

   spdlog::debug("process {} child nodes", node->mNumChildren);
   for (decltype(node->mNumChildren) i = 0; i < node->mNumChildren; i++) {
      spdlog::debug("process node: {}", i);
      process_node(node->mChildren[i], scene);
   }
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void convert_model(std::string const& path, std::string output_file)
{
   spdlog::info("convert model from file: {}", path);

   Assimp::Importer importer{};
   //! \todo flags as command line arguments
   const auto* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      spdlog::error("error while loading model: {}", importer.GetErrorString());
      return;
   }

   spdlog::info("process root node ...");
   process_node(scene->mRootNode, scene);

   spdlog::debug("meshes found: {}", std::size(meshes));
   if (std::size(meshes) != 1) {
      spdlog::error("mesh converter works only with a single mesh");
      return;
   }

   auto m = *std::begin(meshes);
   spdlog::debug("export {} vertices", std::size(m.vertices));
   spdlog::debug("export {} indices", std::size(m.indices));

   auto out_content = generate_output_string(m, output_file);

   output_file.append(".h");
   spdlog::info("write modeldata to file: {}", output_file);
   //! \todo filename
   std::ofstream ofs{output_file};
   ofs << out_content << std::endl;

   spdlog::debug("generated file\n {}", out_content);
}

int main(int argc, const char** argv)
{
   try {
      std::string input_file{};
      std::string output_file{"generated_mesh"};

      namespace po = boost::program_options;
      po::options_description desc("Allowed options");
      desc.add_options()("help", "produce help message")("input-file", po::value<decltype(input_file)>(),
                                                         "input filename")(
         "output-file,O", po::value<decltype(output_file)>(&output_file)->default_value(output_file),
         "name of the struct and output filename");

      po::positional_options_description p;
      p.add("input-file", -1);

      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
      po::notify(vm);

      if (vm.count("help")) {
         std::cout << desc << "\n";
         return 1;
      }

      if (vm.count("input-file"))
         input_file = vm["input-file"].as<decltype(input_file)>();
      else {
         spdlog::critical("input-file is missing");
         return -1;
      }

      spdlog::set_level(spdlog::level::debug);
      spdlog::info("Starting application sec21::viewer::mesh converter {}.{}", VERSION_MAJOR, VERSION_MINOR);

      convert_model(input_file, output_file);
   } catch (std::exception& ex) {
      spdlog::critical("Exception {}", ex.what());
      return -1;
   }
   return 0;
}
