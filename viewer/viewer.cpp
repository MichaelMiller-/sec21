#include "config.h"
#include "preferences.h"
#include "initialize_sdl_opengl3.h"
#include "camera.h"
#include "gui.h"
#include "events.h"
#include "shader_loader.h"
#include "process_event.h"
// debug
#include "debug_data.h"
#include "debug_output_opengl.h"
//
#include "vertex_buffer_loader.h"
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
#include "ocetree_listener.h"
#include "model_listener.h"

// sec21
#include <sec21/resource.h>
#include <sec21/zip.h>
#include <sec21/access.h>
#include <sec21/all_of.h>
#include <sec21/file_loader.h>
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
#include <variant>

using namespace std::literals;

template <typename Iterator>
auto dump(Iterator first, Iterator last, std::string_view name)
{
   std::cout << name.data() << " n: "<< std::distance(first, last) << "\n(";
   std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(std::cout, ", "));
   std::cout << ")\n" << std::endl;
}
template <typename Sequence>
auto dump(Sequence&& seq, std::string_view name)
{
   dump(std::begin(seq), std::end(seq), std::move(name));
}

inline std::ostream& operator << (std::ostream& os, sec21::viewer::event::mouse_move const& evt) 
{
	os << std::boolalpha << evt.button_pressed[0] << ',' << evt.button_pressed[1] << ',' << evt.button_pressed[2] << ' ';
	os << "relative to window (" << evt.relative_to_window[0] << "," << evt.relative_to_window[1] << ") ";
	os << "relative motion (" << evt.relative_motion[0] << "," << evt.relative_motion[1] << ")";
	return os;
}  

inline std::ostream& operator << (std::ostream& os, sec21::viewer::event::mouse_button_down const& evt) 
{
	os << std::boolalpha << evt.pressed[0] << ',' << evt.pressed[1] << ',' << evt.pressed[2] << ' ';
	os << "relative to window (" << evt.relative_to_window[0] << "," << evt.relative_to_window[1] << ") ";
	os << "clicks: " << evt.clicks;  
	return os;
}  
inline std::ostream& operator << (std::ostream& os, sec21::viewer::event::mouse_button_up const& evt) 
{
	os << std::boolalpha << evt.released[0] << ',' << evt.released[1] << ',' << evt.released[2] << ' ';
	os << "relative to window (" << evt.relative_to_window[0] << "," << evt.relative_to_window[1] << ") ";
	os << "clicks: " << evt.clicks;  
	return os;
}  


//! \todo clang
#if defined(GCC)
	#pragma GCC diagnostic push
	// #pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wshadow"
#endif
#include <boost/sml.hpp>
#include <boost/sml/utility/dispatch_table.hpp>

template <SDL_EventType Id>
decltype(boost::sml::event<sdl_event_impl<Id>>) sdl_event{};

constexpr auto is_key(int key) noexcept {
	return [=](auto event) { return event.data.key.keysym.sym == key; };
}

constexpr auto is_mouse_btn(int button) noexcept {
	return [=](auto event) { return event.data.button.button == button; };
}

// constexpr auto is_mouse_wheel_scroll(int button) noexcept {
// 	return [=](auto event) { return event.data.button.button == button; };
// }


struct sdl2 
{
	// entt::registry& registry;
	// explicit sdl2(entt::registry& reg) : registry{ reg } {}

	auto operator()() const noexcept 
	{
		using namespace boost::sml;
		// clang-format off
		return make_transition_table(
		  "wait_for_user_input"_s <= *"idle"_s / [] { spdlog::debug("idle");}

		, "idle"_s <= "wait_for_user_input"_s + sdl_event<SDL_MOUSEWHEEL> / [] { spdlog::debug("wheel"); }

		, "pan_first_pick"_s <= "wait_for_user_input"_s + sdl_event<SDL_KEYUP>[is_key(SDLK_p)] / [] { spdlog::debug("p pressed -> pan enabled"); }
		, "pan_second_pick"_s <= "pan_first_pick"_s + sdl_event<SDL_MOUSEBUTTONDOWN>[is_mouse_btn(SDL_BUTTON_MIDDLE)] / [] { spdlog::debug("pan started: pirst point picked"); }
		, "pan_first_pick"_s <= "pan_second_pick"_s + sdl_event<SDL_MOUSEBUTTONUP>[is_mouse_btn(SDL_BUTTON_MIDDLE)] / []{ spdlog::debug("pan exited"); }
		, "idle"_s <= "pan_first_pick"_s + sdl_event<SDL_KEYUP>[is_key(SDLK_ESCAPE)] / []{ spdlog::debug("pan aborted"); }

		, X <= "wait_for_user_input"_s + sdl_event<SDL_KEYUP>[is_key(SDLK_ESCAPE)] / [] { spdlog::debug("escape key"); }
		, X <= *"waiting_for_quit"_s + sdl_event<SDL_QUIT> / [] { spdlog::debug("quit event"); }
		);
		// clang-format on
	}
};

#if defined(GCC)
	#pragma GCC diagnostic pop
#endif

namespace sec21::viewer
{
#if 0	
   //! \todo consider to be a entt::resource -> reload -> observe -> create new model
   //! \todo replace filename type with std::filesystem::path
   bool load_from_json(entt::registry& registry, std::string const& filename, preferences const& settings)
   {
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
      return true;
   }
#endif

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

	namespace event
	{
		struct quit_application 
		{
			bool value{ false };
		};
	}

	struct viewer_state
	{
		struct close_window {};

		struct joystick
		{
			unsigned int id;
			std::string name;
			// unsigned int button_count;
			std::array<bool, 32> button_state;
			std::array<float, 8> axis_state;
		};

		using event_t = std::variant<event::quit_application, joystick, event::mouse_move, std::nullopt_t>;
	};

	template <typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	// explicit deduction guide (not needed as of C++20)
	template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

	struct pan
	{

	};

	struct select
	{

	};

	using command_state_t = std::variant<pan, select, std::nullopt_t>;
 
#if 0
		void new_node(event_new_node const& evt)
		{
			spdlog::debug("event: create new node");
			make_node(
				registry,
				//! \todo hardcoded
				0.5,
				glm::vec3{ std::get<0>(evt.value), std::get<1>(evt.value), std::get<2>(evt.value) });
		}
	#endif
}

struct command_listener
{
	entt::registry& registry;
	sec21::viewer::preferences const& settings;

	void zoom(sec21::viewer::event_mouse_wheel const& evt)
	{
		using namespace sec21::viewer;
		spdlog::debug("event: mouse wheel: {}", evt.value);

		auto camera_view = registry.view<camera>();
		auto& active_camera = camera_view.get(*camera_view.begin());

		active_camera.position += active_camera.front * active_camera.movement_speed * static_cast<float>(evt.value);
	}

	// void pan(sec21::viewer::event_mouse_wheel const& evt)
	// {
	// 	using namespace sec21::viewer;
	// 	spdlog::debug("event: mouse wheel: {}", evt.value);

	// 	auto camera_view = registry.view<camera>();
	// 	auto& active_camera = camera_view.get(*camera_view.begin());

	// 	active_camera.position += active_camera.front * active_camera.movement_speed * static_cast<float>(evt.value);
	// }	
};


int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	try
	{
		using namespace sec21::viewer;

		// viewer_state state{};

		entt::registry registry{};
		entt::dispatcher dispatcher{};

		spdlog::set_level(spdlog::level::debug);
		spdlog::info("starting application sec21::viewer {}.{}", VERSION_MAJOR, VERSION_MINOR);

		spdlog::info("load settings");
		const auto settings = sec21::load_from_json<preferences>("viewer/settings.json");

		spdlog::info("setup dispatcher");
		//! \todo rename
		model_listener listener{ registry, settings };
		command_listener cmd_listener{ registry, settings };
		ocetree_listener bbox_listender{ registry };

		dispatcher.sink<event_load_model>().connect<&model_listener::load_model>(listener);
		dispatcher.sink<event_load_model_load>().connect<&model_listener::load_model_load>(listener);
		dispatcher.sink<event_clear_entites>().connect<&model_listener::clear>(listener);

		dispatcher.sink<event_mouse_wheel>().connect<&command_listener::zoom>(cmd_listener);

		registry.on_construct<aabb>().connect<&ocetree_listener::insert>(bbox_listender);
		// registry.on_construct<position>().connect<&my_free_function>();

		entt::observer selectable_observer{registry, entt::collector.update<selectable>()};

		{
			auto data = debug_data{};

			std::vector<std::string> filelist{};
			std::transform(std::filesystem::directory_iterator("viewer"), {}, std::back_inserter(filelist), [](auto const& p){
				return p.path().string();
			});

			const auto is_example = [](auto const& filename){ return filename.find("example") != std::string::npos; };
			const auto is_load = [](auto const& filename){ return filename.find("load") != std::string::npos; };

			std::copy_if(
				begin(filelist), 
				end(filelist), 
				back_inserter(data.example_files), 
				is_example);

			data.example_files.erase(
				std::remove_if(begin(data.example_files), end(data.example_files), is_load),
				std::end(data.example_files));

			std::copy_if(
				begin(filelist), 
				end(filelist), 
				back_inserter(data.example_load_files), 
				sec21::all_of{ is_example, is_load });

			spdlog::debug("add debug-data: {} example files and {} load files", std::size(data.example_files), std::size(data.example_load_files));

			auto entity = registry.create();
			registry.emplace<debug_data>(entity, data);
		}

		auto [window, context] = initialize_sdl_opengl3(settings);

		enable_opengl_debug_output();

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, settings.screen_width, settings.screen_height);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ImGui::CreateContext();
		ImGui::StyleColorsClassic();
		ImGui_ImplSDL2_InitForOpenGL(window.get(), context);
		ImGui_ImplOpenGL3_Init(glsl_version_140::version.data());

		spdlog::info("SDL start text input");
		SDL_StartTextInput();

		spdlog::info("initizialize resource cache");
		cache_shader_t cache_shader{};
		cache_vertex_buffer_t cache_vertex_buffer{};

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
		//! \todo: make_axis(registry);
		//! \todo: make_viewcube(registry);

		{
			auto entity = registry.create();
			auto& trans = registry.emplace<transformation>(entity);
			trans.viewport = glm::ivec4{ 0, 0, settings.screen_width, settings.screen_height };
		}
		{
			//! \todo should be handled via events
			auto entity = registry.create();
			registry.emplace<input_data>(entity);
		}

		spdlog::info("application initialized successful");

		SDL_Event event{};
		// bool quit{ false }; 

		boost::sml::sm<sdl2> sm; ///{ registry };
		auto dispatch_event = boost::sml::utility::make_dispatch_table<SDL_Event, SDL_QUIT, SDL_MOUSEWHEEL>(sm);

		while (!sm.is(boost::sml::X))
		{
			// auto delta_frame_time = 10.0f / ImGui::GetIO().Framerate;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				dispatch_event(event, event.type);
			}
			// update_settings(registry);	// controls visibility
			//! \todo update_transformation instead of update_camera()
			update_camera(registry);
			update_collision(registry);

			render_ui(registry, dispatcher, window.get());
			SDL_GL_MakeCurrent(window.get(), context);

			glClearColor(settings.clear_color[0], settings.clear_color[1], settings.clear_color[2], 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			using namespace sec21::viewer;
			auto camera_view = registry.view<camera>();
			const auto& active_camera = camera_view.get(*camera_view.begin());

			auto transformation_view = registry.view<transformation>();
			const auto& active_transformation = transformation_view.get(*transformation_view.begin());

			auto h1 = cache_shader.handle(handle_shader);
			if (!h1) {
				throw std::runtime_error("invalid shader handle");
			}
			auto &active_shader = h1.get();

			active_shader.enable();
			active_shader.uniform("light.position", 3.0f, 20.0f, 20.0f);
			active_shader.uniform("light.ambient",  0.2f, 0.2f, 0.2f);
			active_shader.uniform("light.diffuse", 1.0f, 1.0f, 1.0f);
			// active_shader.uniform("light.specular", 1.0f, 1.0f, 1.0f);
			active_shader.uniform_mat("projection"sv, active_transformation.projection);
			active_shader.uniform_mat("view"sv, active_transformation.view);
			active_shader.uniform("camera.position", active_camera.position.x, active_camera.position.y, active_camera.position.z);

			registry.view<material, viewable, vertex_buffer_id, model_transformation>().each([&](auto, auto const& mat, auto const& visible, const auto& vertex_buff_id, auto const& model_matrix) 
			{
				if (visible.value)
				{
					active_shader.uniform("material.ambient", mat.ambient);
					active_shader.uniform("material.diffuse", mat.diffuse);
					active_shader.uniform("material.specular", mat.specular);
					active_shader.uniform("material.shininess", mat.shininess);
					active_shader.uniform_mat("model"sv, model_matrix.value);

					if (auto handle = cache_vertex_buffer.handle(vertex_buff_id.value); handle)
					{
						handle->bind();
						//! \todo hardcoded implicit triangle
						handle->draw();
					}
				}
			});

			// auto zz{0};
			// selectable_observer.each([&zz](const auto entity) 
			// {
			// 	spdlog::debug("zz: {}", zz);
			// });
					//! \todo render text
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(window.get());
		}

		spdlog::info("stop application");

		cache_shader.clear();
		cache_vertex_buffer.clear();

		SDL_StopTextInput();
		SDL_Quit();
	}
	catch (std::exception &ex)
	{
		spdlog::critical("exception {}", ex.what());
		return -1;
	}
	return 0;
}
