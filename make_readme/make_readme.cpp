#include "config.h"

#include <fmt/format.h>

#include <spdlog/spdlog.h>

#include <exception>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <regex>

struct content
{
   std::string title;
   std::string anchor;
   std::string document;
};

using namespace std::literals;

const auto output_template = R"(
# sec21
[![GCC](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml) [![Clang](https://github.com/MichaelMiller-/sec21/actions/workflows/clang.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/clang.yml) [![MSVC](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml)

**sec21** is a collection of useful functions and classes.
{}

Examples
- [Viewer](viewer/README.md)

{}
)"s;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
   try {
      spdlog::set_level(spdlog::level::debug);
      spdlog::info("Starting application sec21::{} {}.{}", SEC21_PROGRAM_NAME, VERSION_MAJOR, VERSION_MINOR);

      const auto document_directory = std::filesystem::path{SEC21_ROOT_DIRECTORY} / "docs" / "api";
      spdlog::info("Read all files from: {}", document_directory.string());

      std::vector<std::string> filenames{};
      std::transform(std::filesystem::directory_iterator{document_directory}, {}, std::back_inserter(filenames),
                     [](auto const& e) { return e.path().string(); });
      spdlog::info("{} files found", size(filenames));

      const auto read_content = [](std::filesystem::path filename) -> content {
         std::ifstream ifs{filename};

         if (ifs.good() == false)
            throw std::runtime_error(fmt::format("Failed to read: {}", filename.string()));

         const auto file_content = std::string{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

         std::string first_line{};
         getline(ifs, first_line);

         const std::regex expr(R"(##\s([^<]*)<span id=\"(\w*)\"><\/span>)");
         std::smatch string_match;
         regex_search(file_content, string_match, expr);

         if (size(string_match) == 0)
            throw std::runtime_error(fmt::format(
               "First line in {} should match the pattern: '## Title <span id=\"Anchor\"></span>'", filename.string()));

         return {string_match[1], string_match[2], file_content};
      };

      std::vector<content> content_list{};
      std::transform(begin(filenames), end(filenames), std::back_inserter(content_list), read_content);

      spdlog::info("Generate 'Table of Content'");
      const auto toc = std::accumulate(begin(content_list), end(content_list), std::string{}, [](auto init, auto v) {
         return init + "\n" + fmt::format("- [{}](#{})", v.title, v.anchor);
      });

      spdlog::info("Generate content");
      const auto docs =
         std::accumulate(begin(content_list), end(content_list), std::string{},
                         [document_directory](auto init, auto v) { return init + "\n--------------\n" + v.document; });

      spdlog::info("Write README.md");
      std::ofstream ofs{std::filesystem::path{SEC21_ROOT_DIRECTORY} / "README.md"};
      ofs << fmt::format(output_template, toc, docs);

   } catch (std::exception& ex) {
      spdlog::critical("Exception {}", ex.what());
      return -1;
   }
   return 0;
}