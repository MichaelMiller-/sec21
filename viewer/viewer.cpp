#include "camera.h"
#include "config.h"
#include "events.h"
#include "gui.h"
#include "initialize_sdl_opengl3.h"
#include "preferences.h"
#include "process_event.h"
#include "project.h"
#include "shader_loader.h"
// debug
#include "debug_data.h"
#include "debug_output_opengl.h"
//
#include "vertex_buffer_loader.h"
#include "vertex_factory/generated_arrow.h"
#include "vertex_factory/generated_cylinder.h"
#include "vertex_factory/generated_sphere.h"
#include "vertex_factory/generated_support_fixed.h"
#include "vertex_factory/generated_support_roller.h"
// entities
#include "make_axis.h"
#include "make_camera.h"
// #include "make_load.h"
#include "make_member.h"
#include "make_node.h"
#include "make_support.h"
#include "tags.h"
#include "transformation.h"
// collision
#include "intersection.h"
#include "model_listener.h"
#include "ocetree_listener.h"

// sec21
#include <algorithm>
#include <iterator>
#include <sec21/access.h>
#include <sec21/all_of.h>
#include <sec21/event/backend/sdl2.h>
#include <sec21/event/events.h>
#include <sec21/event/input_manager.h>
#include <sec21/file_loader.h>
#include <sec21/resource.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/support.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/zip.h>

// imgui
#include <imgui.h>

// logging
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

#include <nlohmann/json.hpp>

#include <entt/entt.hpp>

#include <boost/math/constants/constants.hpp>

#include <array>
#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

//! \todo take a look: raylib
//! \todo take a look: boost::json
//! \todo take a look: https://libigl.github.io/tutorial/

using namespace std::literals;

#include <boost/core/demangle.hpp>
#include <iostream>
#include <optional>
template <typename T>
inline auto name() -> std::string
{
   return boost::core::demangle(typeid(T).name());
}
template <typename T>
inline void print_type(std::string_view n)
{
   std::cout << n << ": " << name<T>() << std::endl;
}

template <typename Iterator>
auto dump(Iterator first, Iterator last, std::string_view name)
{
   std::cout << name.data() << " n: " << std::distance(first, last) << "\n(";
   std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(std::cout, ", "));
   std::cout << ")\n" << std::endl;
}
template <typename Sequence>
auto dump(Sequence&& seq, std::string_view name)
{
   dump(std::begin(seq), std::end(seq), std::move(name));
}

namespace sec21::viewer
{
   auto update_settings(entt::registry& registry)
   {
      //! \todo there has to be a better way to store and get access to only one T
      const auto settings_view = registry.view<preferences>();
      const auto tuple = settings_view.get(*settings_view.begin());
      const auto active_settings = std::get<0>(tuple);

      // order is important
      registry.view<node_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_nodes; });
      registry.view<member_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_members; });
      registry.view<support_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_supports; });
      registry.view<displacement_tag, viewable>().each(
         [&](auto, auto& v) { v.value = active_settings.show_deformation; });
      registry.view<load_tag, viewable>().each([&](auto, auto& v) { v.value = active_settings.show_load; });
   }

#if 0
   auto update_camera(entt::registry& registry, transformation& trans)
   {
      const auto camera_view = registry.view<camera>();
      const auto tuple = camera_view.get(*camera_view.begin());
      const auto active_camera = std::get<0>(tuple);

      registry.view<transformation>().each([&](auto& trans) {
         //! \todo remove magic numbers -> near and far plane
         trans.projection = glm::perspective(glm::radians(active_camera.zoom), viewport_ratio(trans), 0.1f, 1000.0f);
         trans.view = view_matrix(active_camera);
      });

      trans.projection = glm::perspective(glm::radians(active_camera.zoom), viewport_ratio(trans), 0.1f, 1000.0f);
      trans.view = view_matrix(active_camera);

   }
#endif
   auto update_camera(camera const& cam, transformation& trans)
   {
      trans.projection = glm::perspective(glm::radians(cam.zoom), viewport_ratio(trans), 0.1f, 1000.0f);
      trans.view = view_matrix(cam);
   }

   // struct pan {};
   // struct select {};
   // using command_state_t = std::variant<std::monostate, pan, select>;

} // namespace sec21::viewer

struct command_listener
{
   entt::registry& registry;
   sec21::viewer::preferences const& settings;
   sec21::viewer::camera& current_camera;

   void zoom(sec21::viewer::event_mouse_wheel const& evt)
   {
      using namespace sec21::viewer;
      spdlog::debug("event: mouse wheel: {}", evt.value);
#if 0
      auto camera_view = registry.view<camera>();
      auto& active_camera = camera_view.get(*camera_view.begin());

      active_camera.position += active_camera.front * active_camera.movement_speed * static_cast<float>(evt.value);
#else
      current_camera.position += current_camera.front * current_camera.movement_speed * static_cast<float>(evt.value);
#endif
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

void screenshot_to_targa(int width, int height)
{
   const int numberOfPixels = width * height * 3;
   unsigned char pixels[numberOfPixels];

   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   glReadBuffer(GL_FRONT);
   glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

   FILE* outputFile = fopen("screenshot.tga", "w");
   short header[] = {0, 2, 0, 0, 0, 0, static_cast<short>(width), static_cast<short>(height), 24};

   fwrite(&header, sizeof(header), 1, outputFile);
   fwrite(pixels, numberOfPixels, 1, outputFile);
   fclose(outputFile);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
   try {
      using namespace sec21::viewer;

      entt::registry registry{};
      entt::dispatcher dispatcher{};

      spdlog::set_level(spdlog::level::debug);
      spdlog::info("starting application sec21::viewer {}.{}", VERSION_MAJOR, VERSION_MINOR);

      spdlog::info("load settings");
      const auto settings = sec21::load_from_json<preferences>("viewer/settings.json");

      spdlog::info("setup dispatcher");

#if 0
      make_camera(registry);
#else
      auto active_camera = sec21::load_from_json<camera>("viewer/camera.json");
#endif
      const auto get_projects = []() {
         sec21::http_connection conn("http://localhost:3003");
         return conn.get("/projects");
      };

      const auto projects = get_projects().get<std::vector<project>>();
      spdlog::debug("{} projects found", size(projects));

      std::vector<std::string> list_of_project_names{};
      std::transform(begin(projects), end(projects), std::back_inserter(list_of_project_names),
                     [](auto const& v) { return v.name; });

      //! \todo rename
      model_listener listener{registry, settings};
      command_listener cmd_listener{registry, settings, active_camera};
      ocetree_listener bbox_listender{registry};

      dispatcher.sink<event_load_project>().connect<&model_listener::load_model>(listener);
      // dispatcher.sink<event_load_model_load>().connect<&model_listener::load_model_load>(listener);
      dispatcher.sink<event_clear_entites>().connect<&model_listener::clear>(listener);

      dispatcher.sink<event_mouse_wheel>().connect<&command_listener::zoom>(cmd_listener);

      registry.on_construct<aabb>().connect<&ocetree_listener::insert>(bbox_listender);
      // registry.on_construct<position>().connect<&my_free_function>();

      entt::observer selectable_observer{registry, entt::collector.update<selectable>()};

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

      cache_shader.load<shader_loader>(handle_shader,
                                       shader_resource::load_from_file("viewer/shader1.vs", "viewer/shader1.fs"));

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
            .attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{12})
            .attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{24})
            .from_vertex_array(std::begin(sphere_vertices), std::end(sphere_vertices),
                               sphere_indices)); // sphere_creator.get_indices()));

      cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
         handle_vertexbuffer_cylinder,
         vertex_buffer_t::builder()
            .attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
            .attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{12})
            .attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{24})
            .from_vertex_array(std::begin(cylinder_vertices), std::end(cylinder_vertices), cylinder_indices));

      cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
         handle_vertexbuffer_support_fixed,
         vertex_buffer_t::builder()
            .attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
            .attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{12})
            .attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{24})
            .from_vertex_array(std::begin(support_fixed_vertices), std::end(support_fixed_vertices),
                               support_fixed_indices));

      cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
         handle_vertexbuffer_support_roller,
         vertex_buffer_t::builder()
            .attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
            .attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{12})
            .attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{24})
            .from_vertex_array(std::begin(support_roller_vertices), std::end(support_roller_vertices),
                               support_roller_indices));

      cache_vertex_buffer.load<vertex_buffer_loader<vertex>>(
         handle_vertexbuffer_arrow,
         vertex_buffer_t::builder()
            .attribute(vertex_attribute_tag_t::position, vertex_attribute_t::float_3)
            .attribute(vertex_attribute_tag_t::normale, vertex_attribute_t::float_3, std::byte{12})
            .attribute(vertex_attribute_tag_t::texture_coords, vertex_attribute_t::float_2, std::byte{24})
            .from_vertex_array(std::begin(arrow_vertices), std::end(arrow_vertices), arrow_indices));

      spdlog::info("setup enitites ...");

      //! \todo: make_axis(registry);
      //! \todo: make_viewcube(registry);

      transformation active_transformation;
      active_transformation.viewport = glm::ivec4{0, 0, settings.screen_width, settings.screen_height};

      spdlog::info("application initialized successful");

      using namespace sec21::event;

      bool quit{false};
      input_manager<backend::sdl2> input;
      std::vector<sec21::event::event_t> input_events{time_elapsed{}};

      auto frames = 0;

      while (!quit) {
         const auto evt = input.next_event();
         if (const auto sdl2_event = backend::sdl2::to_event(evt); sdl2_event) {
            ImGui_ImplSDL2_ProcessEvent(&(*sdl2_event));
         }

         using namespace sec21::event;

         // compress events (merge adjacent time_elapsed events)
         std::visit(
            sec21::overloaded{[](time_elapsed& prev, time_elapsed const& next) { prev.elapsed += next.elapsed; },
                              [&](auto const& /*prev*/, std::monostate const&) {},
                              [&](auto const& /*prev*/, auto const& next) { input_events.push_back(next); }},
            input_events.back(), evt);

         bool render = false;
         //! \note: overloaded pattern hat den vorteil das alle events abgehandelt werden müssen <-> compiletimefehler
         sec21::match(
            evt, [&](close_window const&) { quit = true; }, [](pressed<mouse_button> const&) {},
            [](released<mouse_button> const&) {}, [](moved<mouse> const&) {},
            [&](mouse_wheel const& evt) { dispatcher.enqueue<event_mouse_wheel>(evt.y); },
            [&](pressed<key> const& value) {
               if (value.source.code == Keycode::Escape) {
                  quit = true;
               }
               if (value.source.code == Keycode::M) {
                  dispatcher.enqueue<event_load_project>(1);
               }
            },
            [](released<key> const&) {}, [&](time_elapsed const&) { render = true; }, [](std::monostate const&) {});

         if (!render) {
            // todo: something more with a linear flow here
            // right now this is just saying "no reason to update the render yet"
            continue;
         }

         // emits all the events queued so far at once
         dispatcher.update();

         // update_settings(registry);	// controls visibility
         //! \todo update_transformation instead of update_camera()
         // update_camera(registry, active_transformation);
         update_camera(active_camera, active_transformation);
         // update_collision(registry);

         render_ui(registry, dispatcher, list_of_project_names, window.get());
         SDL_GL_MakeCurrent(window.get(), context);

         glClearColor(settings.clear_color[0], settings.clear_color[1], settings.clear_color[2], 1.f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         using namespace sec21::viewer;
         // auto camera_view = registry.view<camera>();
         // const auto& camera_tuple = camera_view.get(*camera_view.begin());
         // const auto& active_camera = std::get<0>(camera_tuple);

         auto h1 = cache_shader.handle(handle_shader);
         if (!h1) {
            throw std::runtime_error("invalid shader handle");
         }
         auto& active_shader = h1.get();

         active_shader.enable();
         active_shader.uniform("light.position", 3.0f, 20.0f, 20.0f);
         active_shader.uniform("light.ambient", 0.2f, 0.2f, 0.2f);
         active_shader.uniform("light.diffuse", 1.0f, 1.0f, 1.0f);
         // active_shader.uniform("light.specular", 1.0f, 1.0f, 1.0f);
         active_shader.uniform_mat("projection"sv, active_transformation.projection);
         active_shader.uniform_mat("view"sv, active_transformation.view);
         active_shader.uniform("camera.position", active_camera.position.x, active_camera.position.y,
                               active_camera.position.z);

         registry.view<material, viewable, vertex_buffer_id, model_transformation>().each(
            [&](auto, auto const& mat, auto const& visible, const auto& vertex_buff_id, auto const& model_matrix) {
               if (visible.value) {
                  active_shader.uniform("material.ambient", mat.ambient);
                  active_shader.uniform("material.diffuse", mat.diffuse);
                  active_shader.uniform("material.specular", mat.specular);
                  active_shader.uniform("material.shininess", mat.shininess);
                  active_shader.uniform_mat("model"sv, model_matrix.value);

                  if (auto handle = cache_vertex_buffer.handle(vertex_buff_id.value); handle) {
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

         ++frames;

         if (settings.headless == true && frames > settings.max_frames) {
            screenshot_to_targa(settings.screen_width, settings.screen_height);
            quit = true;
         }
      }
      {
         spdlog::debug("Events processed: {}", std::size(input_events));
         std::ofstream ofs{"output_events.json"};
         ofs << std::setw(2) << nlohmann::json(input_events);
      }
      spdlog::info("stop application");

      cache_shader.clear();
      cache_vertex_buffer.clear();

      ImGui::DestroyContext();

      SDL_StopTextInput();
      SDL_Quit();
   } catch (std::exception& ex) {
      spdlog::critical("exception {}", ex.what());
      return -1;
   }
   return 0;
}
