#include "config.h"
#include "preferences.h"
#include "initialize_sdl_opengl3.h"
#include "camera.h"
#include "gui.h"
#include "events.h"
#include "shader.h"
// debug
#include "debug_data.h"
#include "debug_output_opengl.h"
//
#include "vertex.h"
#include "vertex_buffer.h"
#include "vertex_factory/generated_sphere.h"
#include "vertex_factory/generated_cylinder.h"
#include "vertex_factory/generated_support_fixed.h"
#include "vertex_factory/generated_support_roller.h"
#include "vertex_factory/generated_arrow.h"
// entities
#include "tags.h"
#include "transformation.h"
#include "make_camera.h"
#include "make_axis.h"
#include "make_node.h"
#include "make_member.h"
#include "make_support.h"
#include "make_load.h"
//! \todo replace -> events hsould handle this
#include "input_data.h"
// collision
#include "intersection.h"

// sec21
#include <sec21/resource.h>
#include <sec21/zip.h>
#include <sec21/access.h>
#include <sec21/all_of.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/support.h>

// imgui
#include <imgui.h>
// logging
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nlohmann/json.hpp>

#include <entt/entt.hpp>

#include <boost/math/constants/constants.hpp>

#include <string>
#include <string_view>
#include <array>
#include <tuple>
#include <functional>
#include <type_traits>
#include <chrono>
#include <filesystem>
#include <vector>

using namespace std::literals;

namespace sec21::viewer
{
	using vertex_buffer_t = vertex_buffer<vertex>;

	using cache_shader_t = entt::cache<shader_resource>;
	using cache_vertex_buffer_t = entt::cache<vertex_buffer_t>;

	struct shader_loader final : entt::loader<shader_loader, shader_resource>
	{
		template <typename... Args>
		auto load(Args &&... args) const
		{
			return std::shared_ptr<shader_resource>(new shader_resource{ std::forward<Args>(args)... });
		}
	};

	template <typename Vertex>
	struct vertex_buffer_loader final : entt::loader<vertex_buffer_loader<Vertex>, vertex_buffer<Vertex>>
	{
		template <typename... Args>
		auto load(Args &&... args) const // -> std::shared_ptr<vertex_buffer<Vertex>
		{
			spdlog::debug("vertex_buffer_loader::load()");
			return std::shared_ptr<vertex_buffer<Vertex>>(new vertex_buffer<Vertex>{ std::forward<Args>(args)... });
		}
	};

   //! \todo consider to be a entt::resource -> reload -> observe -> create new model
   //! \todo replace filename type with std::filesystem::path
   bool load_from_json(entt::registry& registry, std::string const& filename)
   {
      structural_analysis::space_truss sys{};
      try {
         spdlog::info("load model from json file: {} ...", filename);
         std::ifstream ifs{filename};
         nlohmann::json j;
         ifs >> j;
         sys = j.get<decltype(sys)>();
      }
      catch (std::exception& ex)
      {
         spdlog::error("failed to load file: {} exception: {} ", filename, ex.what());
         return false;
      }
      catch (...)
      {
         spdlog::error("failed to load model: unkown exception");
         return false;
      }

      auto settings_view = registry.view<preferences>();
      auto& active_settings = settings_view.get(*settings_view.begin());

      for (auto const& node : sys.nodes)
         make_node(registry, active_settings.radius_node, node.position);

      decltype(sys.nodes) filtered_support_nodes{};
      std::copy_if(
         std::begin(sys.nodes),
         std::end(sys.nodes),
         std::back_inserter(filtered_support_nodes),
         [](auto const& node) { return node.global_support.has_value(); });

      spdlog::debug("{} supports found", std::size(filtered_support_nodes));
      for (auto const& node : filtered_support_nodes)
      {
         //! \todo consider all support kinds
         //! \todo hardcoded scale
         // if (node.global_support == structural_analysis::Support::Roller)
         //    make_support_roller(registry, active_settings.radius_node, node.position);
         // else
            make_fixed_support(registry, 0.4f, node.position);
      }

      for (auto [k, v] : sys.coincidence_table)
      {
         auto [s, e] = v;
         spdlog::debug("lookup descriptor {} and {} from coincidence table", s, e);

         if (valid_descriptor(sys.nodes, s) == false)
            spdlog::error("descriptor: {} is invalid", s);

         if (valid_descriptor(sys.nodes, e) == false)
            spdlog::error("descriptor: {} is invalid", e);

         const auto from = structural_analysis::get_element(sys.nodes, s);
         const auto to = structural_analysis::get_element(sys.nodes, e);

         make_member(registry, active_settings.radius_member, from->position, to->position);
      }

#if 0
      const auto result_filename = [](auto const& fn){
         auto result = fn;
         if (const auto pos = result.find_last_of('.'); pos != decltype(result)::npos)
            result.insert(pos, "_result");
         return result;
      };

      //! \todo seperate loader code -> there could be one system with different kind of results
      structural_analysis::system_result<decltype(sys)> sys_results{};
      try
      {
         spdlog::info("try to load results from json file: {}", result_filename(filename));
         if (std::ifstream ifs{result_filename(filename)}; ifs)
         {
            nlohmann::json j;
            ifs >> j;
            sys_results = j.get<decltype(sys_results)>();
         }
      }
      catch (std::exception& ex)
      {
         spdlog::error("failed to load file: {} exception: {} ", filename, ex.what());
      }
      catch (...)
      {
         spdlog::error("failed to load results: unkown exception");
      }

      if (std::empty(sys_results.nodes)) {
         spdlog::warn("no results found");
         return false;
      }

      using node_descriptor_t = size_t; // sec21::structural_analysis::space_truss::node_descriptor_t;
      using pt_t = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;
      // using pt_t = decltype(sys)::node_t::point_t;
      std::map<node_descriptor_t, pt_t> node_displacement_lookup{};

      //! \todo consider unit
      static constexpr auto kunit = 50;
      for (auto const& e : sys_results.nodes)
         node_displacement_lookup[e.id] = { X(e.displacement).value() / kunit, Y(e.displacement).value() / kunit };

      if (std::size(sys.nodes) != std::size(node_displacement_lookup))
      {
         spdlog::error("loaded results mismatches with inputdata");
         //! \todo return ?
		}

      //! \todo only a view of ranges
      for (auto [k, v] : node_displacement_lookup)
      {
         const auto p = structural_analysis::get_element(sys.nodes, k)->position;
         make_displaced_node(registry, active_settings.radius_node, p + v);
      }

      for (auto [k, v] : sys.coincidence_table)
      {
         auto [s, e] = v;
         const auto from = structural_analysis::get_element(sys.nodes, s);
         const auto to = structural_analysis::get_element(sys.nodes, e);

         make_displaced_member(
            registry,
            active_settings.radius_member,
            //! \todo opportunite for boost::qvm
            from->position + node_displacement_lookup.at(from->id),
            to->position + node_displacement_lookup.at(to->id)
         );
      }
#endif
      return true;
   }

	auto update_settings(entt::registry& registry)
	{
		auto settings_view = registry.view<preferences>();
		const auto& active_settings = settings_view.get(*settings_view.begin());

		// order is important
		registry.view<node_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_nodes; });
		registry.view<member_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_members; });
		registry.view<support_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_supports; });
		registry.view<displacement_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_deformation; });
		registry.view<load_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_load; });
	}

	auto update_camera(entt::registry& registry)
	{
		auto camera_view = registry.view<camera>();
		const auto& active_camera = camera_view.get(*camera_view.begin());

		registry.view<transformation>().each([&](auto& trans)
		{
			//! \todo remove magic numbers -> near and far plane
			trans.projection = glm::perspective(glm::radians(active_camera.zoom), viewport_ratio(trans), 0.1f, 1000.0f);
			trans.view = view_matrix(active_camera);
		});
	}

	auto clear_render_window(entt::registry& registry)
	{
		auto settings_view = registry.view<preferences>();
		const auto& active_settings = settings_view.get(*settings_view.begin());

		glClearColor(active_settings.clear_color[0], active_settings.clear_color[1], active_settings.clear_color[2], 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void update_collision(entt::registry& registry)
	{
		auto transformation_view = registry.view<transformation>();
		const auto& active_transformation = transformation_view.get(*transformation_view.begin());

		auto input_data_view = registry.view<input_data>();
		const auto& active_input_data = input_data_view.get(*input_data_view.begin());

		const auto win_x = active_input_data.mouse_position.x;
		const auto win_y = viewport_height(active_transformation) - active_input_data.mouse_position.y;

		const auto start_pos = glm::unProject(
			{ win_x, win_y, 0.0f },
			active_transformation.view,
			active_transformation.projection,
			active_transformation.viewport);

		const auto end_pos = glm::unProject(
			{ win_x, win_y, 1.0f },
			active_transformation.view,
			active_transformation.projection,
			active_transformation.viewport);

		const auto r = ray{ start_pos, end_pos - start_pos };

		//! \todo quadtree (octree) to reduce the collision test
		registry.view<selectable, sphere>().each([&](auto& obj, auto const& bounding_volume)
		{
			const auto intersects = intersect(r, bounding_volume);

			obj.highlight = intersects;
			obj.selected = intersects && active_input_data.mouse_btn_right_clicked;
		});

		//! \todo highlight objects
		// registry.view<selectable, material>().each([&](auto const& obj, auto& mat)
		// {
		// 	//! \todo use some kind of emission
		// 	mat = obj.highlight ? emerald : mat;
		// });
	}

	///
	///
	class sdl_application final
	{
		SDL_Window *window{ nullptr };
		SDL_GLContext context{ nullptr };

		cache_shader_t cache_shader{};
		cache_vertex_buffer_t cache_vertex_buffer{};

		entt::registry registry{};
		entt::dispatcher dispatcher{};

		bool quit{ false };

		preferences settings;
		shader_resource shader_cube;

	private:
		void render()
		{
			auto camera_view = registry.view<camera>();
			const auto& active_camera = camera_view.get(*camera_view.begin());

			auto transformation_view = registry.view<transformation>();
			const auto& active_transformation = transformation_view.get(*transformation_view.begin());

			// auto shader_resource_view = registry.view<shader_resource>();
			// auto& active_shader = shader_resource_view.get(*shader_resource_view.begin());

			// if (auto handle = cache_shader.handle(handle_shader); handle)
			// {
			// 	spdlog::debug("shader handle valid");
			// }

			//! \todo use "shader_resource"
			auto& active_shader = shader_cube;

			active_shader.enable();
			active_shader.uniform("light.position", 3.0f, 20.0f, 20.0f);
			active_shader.uniform("light.ambient",  0.2f, 0.2f, 0.2f);
			active_shader.uniform("light.diffuse", 1.0f, 1.0f, 1.0f);
			// active_shader.uniform("light.specular", 1.0f, 1.0f, 1.0f);
			active_shader.uniform_mat("projection"sv, active_transformation.projection);
			active_shader.uniform_mat("view"sv, active_transformation.view);
			active_shader.uniform("camera.position", active_camera.position.x, active_camera.position.y, active_camera.position.z);

			auto view = registry.view<material, viewable, vertex_buffer_id, model_transformation>();
			for (auto entity : view)
			{
				if (view.get<viewable>(entity).value)
				{
					const auto& mat = view.get<material>(entity);
					const auto& model_matrix = view.get<model_transformation>(entity);
					const auto& vertex_buff_id = view.get<vertex_buffer_id>(entity);

					active_shader.uniform("material.ambient", mat.ambient);
					active_shader.uniform("material.diffuse", mat.diffuse);
					active_shader.uniform("material.specular", mat.specular);
					active_shader.uniform("material.shininess", mat.shininess);
					active_shader.uniform_mat("model"sv, model_matrix.value);

					if (auto handle = cache_vertex_buffer.handle(vertex_buff_id.value); handle)
					{
						handle->bind();
						//! \todo hardcoded
						handle->draw<triangles>();
					}
				}
			}
		}

		void stop()
		{
			spdlog::debug("event: quit application");
			quit = true;
		}

		void receive(event_clear_entites)
		{
			spdlog::debug("event: clear all entities");
			{
				auto view = registry.view<node_tag>();
				spdlog::debug("delete {} nodes", std::size(view));
				registry.destroy(std::begin(view), std::end(view));
			}
			{
				auto view = registry.view<member_tag>();
				spdlog::debug("delete {} members", std::size(view));
				registry.destroy(std::begin(view), std::end(view));
			}
			{
				auto view = registry.view<support_tag>();
				spdlog::debug("delete {} supports", std::size(view));
				registry.destroy(std::begin(view), std::end(view));
			}
			{
				auto view = registry.view<load_tag>();
				spdlog::debug("delete {} loads", std::size(view));
				registry.destroy(std::begin(view), std::end(view));
			}
		}

		void update_ocetree()
		{
			//! \todo insert aabb into octree
			//! \todo arguments for observer
		}

		void load_model(event_load_model const& evt)
		{
			spdlog::debug("event: load model");
			load_from_json(registry, evt.filename);
		}

		void show_load(event_load_model_load const& evt)
		{
			spdlog::debug("event: load model");

	      structural_analysis::space_truss sys{};
	      structural_analysis::loadcase<decltype(sys)> load{};
			{
				std::ifstream ifs{evt.model};
				nlohmann::json j;
				ifs >> j;
				sys = j.get<decltype(sys)>();
			}
			{
				std::ifstream ifs{evt.load};
				nlohmann::json j;
				ifs >> j;
				load = j.get<decltype(load)>();
			}

			auto settings_view = registry.view<preferences>();
			auto& active_settings = settings_view.get(*settings_view.begin());

			for (auto e : load.node_load)
			{
				auto it = std::find_if(begin(sys.nodes), end(sys.nodes), [id = e.first](auto const& n){ return n.id == id; });

				if (it == end(sys.nodes))
					continue;

				auto v = e.second;

				if (auto fx = std::get<0>(v).value(); fx != 0.0)
				{
					spdlog::debug("load in x direction: {}", fx);
					make_single_load(registry, active_settings.scale_load, it->position, glm::vec3{fx, 0, 0});
				}
				if (auto fy = std::get<1>(v).value(); fy != 0.0)
				{
					spdlog::debug("load in y direction: {}", fy);
					make_single_load(registry, active_settings.scale_load, it->position, glm::vec3{0, fy, 0});
				}
			}
		}

		//! \todo there is not really a need for this -> could be
		void load_settings(event_load_settings const& evt)
		{
			spdlog::debug("event: load settings");
			try
			{
				auto settings_view = registry.view<preferences>();
				auto& active_settings = settings_view.get(*settings_view.begin());

				spdlog::info("load settings from file: {}", evt.filename);
				std::ifstream ifs(evt.filename);
				nlohmann::json j;
				ifs >> j;
				active_settings = j.get<preferences>();
			}
			catch (...)
			{
				spdlog::warn("failed to load settings from file");
			}
		}

		void new_node(event_new_node const& evt)
		{
			spdlog::debug("event: create new node");
			make_node(
				registry,
				//! \todo hardcoded
				0.5,
				glm::vec3{ std::get<0>(evt.value), std::get<1>(evt.value), std::get<2>(evt.value) });
		}

		void initialize_debug_data(event_load_debug_data)
		{
			auto view = registry.view<debug_data>();
			auto& active_debug_data = view.get(*view.begin());

			active_debug_data.example_files.clear();
			active_debug_data.example_load_files.clear();

			std::vector<std::string> filelist{};
			for (const auto &p : std::filesystem::directory_iterator("viewer"))
			{
				if (p.path().extension() == ".json")
					filelist.push_back(p.path().string());
			}

			const auto is_example = [](auto const& filename){ return filename.find("example") != std::string::npos; };
			const auto is_load = [](auto const& filename){ return filename.find("load") != std::string::npos; };

			std::copy_if(
				begin(filelist), 
				end(filelist), 
				back_inserter(active_debug_data.example_files), 
				is_example);

			active_debug_data.example_files.erase(
				std::remove_if(begin(active_debug_data.example_files), end(active_debug_data.example_files), is_load),
				std::end(active_debug_data.example_files));

			std::copy_if(
				begin(filelist), 
				end(filelist), 
				back_inserter(active_debug_data.example_load_files), 
				all_of{ is_example, is_load });
		}

		void process_mouse_input(event_mouse_move)
		{
			// spdlog::debug("event: mouse move");
		}

		void zoom(event_mouse_wheel const& evt)
		{
			spdlog::debug("event: mouse wheel: {}", evt.value);

			auto camera_view = registry.view<camera>();
			auto& active_camera = camera_view.get(*camera_view.begin());

			active_camera.position += active_camera.front * active_camera.movement_speed * static_cast<float>(evt.value);
		}

		void process_key_down(event_key_down const& evt)
		{
			spdlog::debug("event: process key down: {}", evt.value);

			//! \todo
			// command -> pan, new node, new member, edit properties  -> enable state
			// command instant -> zoom extend,
			// command spawn -> calculate

			// auto camera_view = registry.view<camera>();
			// auto& active_camera = camera_view.get(*camera_view.begin());
		}

	public:
		sdl_application() = default;

		~sdl_application()
		{
			spdlog::info("stop SDL application");

			cache_shader.clear();
			cache_vertex_buffer.clear();

			SDL_StopTextInput();
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		bool init()
		{
			spdlog::info("init application");

			//! \todo observer
			registry.on_construct<aabb>().connect<&sdl_application::update_ocetree>(*this);

			//
			dispatcher.sink<event_quit_application>().connect<&sdl_application::stop>(*this);
			dispatcher.sink<event_load_model>().connect<&sdl_application::load_model>(*this);
			dispatcher.sink<event_load_model_load>().connect<&sdl_application::show_load>(*this);
			//! \todo is the dispatcher the right way to do this? load_settings and debug_data ...?
			dispatcher.sink<event_load_settings>().connect<&sdl_application::load_settings>(*this);
			dispatcher.sink<event_load_debug_data>().connect<&sdl_application::initialize_debug_data>(*this);
			// camera
			dispatcher.sink<event_mouse_move>().connect<&sdl_application::process_mouse_input>(*this);
			dispatcher.sink<event_mouse_wheel>().connect<&sdl_application::zoom>(*this);
			dispatcher.sink<event_key_down>().connect<&sdl_application::process_key_down>(*this);
			//
			dispatcher.sink<event_clear_entites>().connect<&sdl_application::receive>(*this);
			dispatcher.sink<event_new_node>().connect<&sdl_application::new_node>(*this);

			auto entity_setting = registry.create();
			auto& active_settings = registry.assign<preferences>(entity_setting);

			{
				auto entity = registry.create();
				registry.assign<debug_data>(entity);
			}
			//! \todo possible cache with reload
			dispatcher.trigger<event_load_settings>("viewer/settings.json");
			dispatcher.trigger<event_load_debug_data>();

			std::tie(window, context) = initialize_sdl_opengl3(active_settings);

			//! \todo test
			GLint flags{ 0 };
			if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags) != 0)
				spdlog::error("failed to retrieve OpenGL context");

			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				spdlog::debug("enable OpenGL debug output");
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(debug_output_opengl, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
				//! \todo only get error messages from OpenGL
				// glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
			}

			glEnable(GL_DEPTH_TEST);
			glViewport(0, 0, active_settings.screen_width, active_settings.screen_height);

			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//! \todo -> set shader_loader
			shader_cube = shader_resource::load_from_file("viewer/shader1.vs", "viewer/shader1.fs");

			ImGui::CreateContext();
			// ImGuiIO &io = ImGui::GetIO();
			// std::ignore = io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

			ImGui::StyleColorsClassic();
			ImGui_ImplSDL2_InitForOpenGL(window, context);
			ImGui_ImplOpenGL3_Init(glsl_version_140::version.data());

			spdlog::info("SDL start text input");
			SDL_StartTextInput();

			spdlog::info("initizialize cache");
			cache_shader.load<shader_loader>(handle_shader, shader_resource::load_from_file("viewer/shader1.vs", "viewer/shader1.fs"));

			const auto sphere_vertices = vertex_factory::generated_sphere::vertices<vertex>();
			const auto sphere_indices = vertex_factory::generated_sphere::indices();

			const auto cylinder_vertices = vertex_factory::generated_cylinder::vertices<vertex>();
			const auto cylinder_indices = vertex_factory::generated_cylinder::indices();

			const auto support_fixed_vertices = vertex_factory::generated_support_fixed::vertices<vertex>();
			const auto support_fixed_indices = vertex_factory::generated_support_fixed::indices();

			const auto support_roller_vertices = vertex_factory::generated_support_roller::vertices<vertex>();
			const auto support_roller_indices = vertex_factory::generated_support_roller::indices();

			const auto arrow_vertices = vertex_factory::generated_arrow::vertices<vertex>();
			const auto arrow_indices = vertex_factory::generated_arrow::indices();

			cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
				handle_vertexbuffer_sphere,
				vertex_buffer_t::builder()
					.attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
					.attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{ 12 })
					.attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{ 24 })
					.from_vertex_array(std::begin(sphere_vertices), std::end(sphere_vertices), sphere_indices)); //sphere_creator.get_indices()));

			cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
				handle_vertexbuffer_cylinder,
				vertex_buffer_t::builder()
					.attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
					.attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{ 12 })
					.attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{ 24 })
					.from_vertex_array(std::begin(cylinder_vertices), std::end(cylinder_vertices), cylinder_indices));

			cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
				handle_vertexbuffer_support_fixed,
				vertex_buffer_t::builder()
					.attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
					.attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{ 12 })
					.attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{ 24 })
					.from_vertex_array(std::begin(support_fixed_vertices), std::end(support_fixed_vertices), support_fixed_indices));

			cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
				handle_vertexbuffer_support_roller,
				vertex_buffer_t::builder()
					.attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
					.attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{ 12 })
					.attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{ 24 })
					.from_vertex_array(std::begin(support_roller_vertices), std::end(support_roller_vertices), support_roller_indices));

			cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
				handle_vertexbuffer_arrow,
				vertex_buffer_t::builder()
					.attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
					.attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{ 12 })
					.attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{ 24 })
					.from_vertex_array(std::begin(arrow_vertices), std::end(arrow_vertices), arrow_indices));

			spdlog::info("setup enitites ...");

			make_camera(registry);
			//! \todo:
			make_axis(registry);
			//! \todo: make_viewcube(registry);

			{
				auto entity = registry.create();
				auto& trans = registry.assign<transformation>(entity);
				trans.viewport = glm::ivec4{ 0, 0, active_settings.screen_width, active_settings.screen_height };
			}
			{
				//! \todo should be handled via events
				auto entity = registry.create();
				registry.assign<input_data>(entity);
			}

			spdlog::info("application init successful");
			return true;
		}

		auto run() // main_loop
		{
			SDL_Event event{};

			while (!quit)
			{
				// auto delta_frame_time = 10.0f / ImGui::GetIO().Framerate;

				while (SDL_PollEvent(&event) != 0)
				{
					ImGui_ImplSDL2_ProcessEvent(&event);

					auto input_data_view = registry.view<input_data>();
					auto& active_input_data = input_data_view.get(*input_data_view.begin());

					// reset data
					active_input_data.mouse_btn_left_clicked = false;

					if (event.type == SDL_QUIT)
					{
						dispatcher.trigger<event_quit_application>();
					}
					else if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						active_input_data.mouse_btn_left_clicked = event.button.button == SDL_BUTTON_LEFT;
						active_input_data.mouse_btn_right_clicked = event.button.button == SDL_BUTTON_RIGHT;
						active_input_data.mouse_position = { event.button.x, event.button.y };
					}
					else if (event.type == SDL_MOUSEMOTION)
					{
						dispatcher.trigger<event_mouse_move>({ event.motion.x, event.motion.y });
						active_input_data.mouse_position = { event.button.x, event.button.y };
					}
					else if (event.type == SDL_MOUSEWHEEL)
					{
						dispatcher.trigger<event_mouse_wheel>(event.wheel.y);
					}
					else if (event.type == SDL_KEYDOWN)
					{
						dispatcher.trigger<event_key_down>(event.key.keysym.sym);

						// if (event.key.keysym.sym == SDLK_w)
						// 	process_keyboard(active_camera, Direction::kForward, delta_frame_time);
						// if (event.key.keysym.sym == SDLK_s)
						// 	process_keyboard(active_camera, Direction::BACKWARD, delta_frame_time);
						// if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
						// 	process_keyboard(active_camera, Direction::LEFT, delta_frame_time);
						// if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
						// 	process_keyboard(active_camera, Direction::RIGHT, delta_frame_time);

						// if (event.key.keysym.sym == SDLK_UP)
						// 	process_keyboard(active_camera, Direction::kUp, delta_frame_time);
						// if (event.key.keysym.sym == SDLK_DOWN)
						// 	process_keyboard(active_camera, Direction::kDown, delta_frame_time);

						if (event.key.keysym.sym == SDLK_ESCAPE)
						{
							// dispatcher.trigger<event_quit_application>();
							quit = true;
						}
					}
				}

				// process_input(registry)
				update_settings(registry);	// controls visibility
				//! \todo update_transformation instead of update_camera()
				update_camera(registry);
				update_collision(registry);

				render_ui(registry, dispatcher, window);
				SDL_GL_MakeCurrent(window, context);

				clear_render_window(registry); //! \todo could be a part from render
				render();
				//! \todo render text

				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				SDL_GL_SwapWindow(window);
			}
		}
	};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	try
	{
		spdlog::set_level(spdlog::level::debug);
		spdlog::info("starting application sec21::viewer {}.{}", VERSION_MAJOR, VERSION_MINOR);

		sec21::viewer::sdl_application app;

		if (!app.init())
		{
			spdlog::critical("failed to initialize application!");
			return -2;
		}
		app.run();
	}
	catch (std::exception &ex)
	{
		spdlog::critical("exception {}", ex.what());
		return -1;
	}
	return 0;
}
