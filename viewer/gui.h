#pragma once

#include "debug_data.h"
#include "events.h"
#include "selectable.h"

#include <imgui.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "imgui_impl/imgui_impl_sdl.h"
#pragma GCC diagnostic pop

#include <entt/entt.hpp>

#include <filesystem>
#include <string_view>

namespace ImGui
{
   template <typename T>
   static bool getter(void* vec, int idx, const char** out)
   {
      const auto& v = *static_cast<const T*>(vec);

      if (idx < 0 || idx >= static_cast<int>(size(v)))
         return false;

      *out = v.at(static_cast<std::size_t>(idx)).c_str();

      return true;
   }

   //! \todo const correctness
   bool Combo(std::string_view label, int* index, std::vector<std::string>& values)
   {
      if (values.empty())
         return false;

      return Combo(label.data(), index, getter<std::vector<std::string>>, static_cast<void*>(values.data()),
                   static_cast<int>(values.size()));
   }

   //! \todo const correctness
   bool ListBox(std::string_view label, int* index, std::vector<std::string>& values)
   {
      if (values.empty())
         return false;

      return ListBox(label.data(), index, getter<std::vector<std::string>>, static_cast<void*>(&values),
                     static_cast<int>(size(values)));
   }

#if 0
   template <typename... Args>
   void Text(std::string_view format, Args &&... args) noexcept
   {
      ImGui::TextUnformated(fmt::format(format, std::forward<Args>(args)...).c_str());
   }
#endif
} // namespace ImGui

namespace sec21::viewer
{
   static constexpr auto popup_window_decoration =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

   static constexpr auto popup_window_transparency = 0.35f;

#if 0
   static void debug_overlay()
   {
      static bool p_open = true;
      static constexpr auto kDistanceFromBorder = 10.0f;
      static int corner = 2;
      ImGuiIO& io = ImGui::GetIO();

      if (corner != -1)
      {
         auto window_pos = ImVec2{ 
            (corner & 1) ? io.DisplaySize.x - kDistanceFromBorder : kDistanceFromBorder, 
            (corner & 2) ? io.DisplaySize.y - kDistanceFromBorder : kDistanceFromBorder };
         auto window_pos_pivot = ImVec2{ (corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f };
         ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
      }
      ImGui::SetNextWindowBgAlpha(popup_window_transparency);
      if (ImGui::Begin("debug overlay", &p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
      {
         if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", static_cast<double>(io.MousePos.x), static_cast<double>(io.MousePos.y));
         else
            ImGui::Text("Mouse Position: <invalid>");
         if (ImGui::BeginPopupContextWindow())
         {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            ImGui::EndPopup();
         }
      }
      ImGui::End();
   }
#endif

   template <typename Window, typename Data>
   auto render_ui(entt::registry& registry, entt::dispatcher& dispatcher, Data const& data, Window&& window) noexcept
   {
      namespace fs = std::filesystem;

      // clang-format off
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(window);
      ImGui::NewFrame();

      ///
      ImGui::ShowDemoWindow();
      ImGui::ShowMetricsWindow();

      if (ImGui::BeginMainMenuBar())
      {
         if (ImGui::BeginMenu("File"))
         {
            if (ImGui::MenuItem("New")) { dispatcher.enqueue<event_clear_entites>(); }
            // if (ImGui::MenuItem("Quit", "Alt+F4")) { dispatcher.enqueue<event_quit_application>(); }

            ImGui::EndMenu();
         }
         // if (ImGui::BeginMenu("Edit"))
         // {
         // 	if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
         // 	if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
         // 	ImGui::Separator();
         // 	if (ImGui::MenuItem("Cut", "CTRL+X")) {}
         // 	if (ImGui::MenuItem("Copy", "CTRL+C")) {}
         // 	if (ImGui::MenuItem("Paste", "CTRL+V")) {}
         // 	ImGui::EndMenu();
         // }
         ImGui::EndMainMenuBar();
      }

      ImGui::Begin("Project");
      {
         auto files = data;
         static int current_file = 0;
         if (ImGui::Combo("Projects", &current_file, files))
         {
            dispatcher.enqueue<event_clear_entites>();
            dispatcher.enqueue<event_load_project>(current_file);
         }

         // auto loads = active_debug_data.example_load_files;
         // static int current_load = 0;
         // if (ImGui::Combo("Loads", &current_load, loads))
         // {
         //    dispatcher.trigger<event_load_model_load>(loads[static_cast<std::size_t>(current_load)]);
         // }
         // if (ImGui::CollapsingHeader("Visibility"))
         // {		
         //    ImGui::Checkbox("show nodes", &active_settings.show_nodes);
         //    ImGui::Checkbox("show members", &active_settings.show_members);
         //    ImGui::Checkbox("show supports", &active_settings.show_supports);
         //    ImGui::Checkbox("show deformation", &active_settings.show_deformation);
         //    ImGui::Checkbox("show load", &active_settings.show_load);
         // }
         // if (ImGui::CollapsingHeader("Camera"))
         // {			
         //    ImGui::PushItemWidth(60);
         //    ImGui::Text("Position");
         //    ImGui::SameLine(); ImGui::SliderFloat("X", &active_camera.position.x, -50.0f, 50.0f);
         //    ImGui::SameLine(); ImGui::SliderFloat("Y", &active_camera.position.y, -50.0f, 50.0f);
         //    ImGui::SameLine(); ImGui::SliderFloat("Z", &active_camera.position.z, -50.0f, 50.0f);
         //    ImGui::PopItemWidth();
         // }
      }
      ImGui::End();

      ImGui::Render();
      // clang-format on
   }
} // namespace sec21::viewer