#pragma once

#include "events.h"
#include "debug_data.h"
#include "input_data.h"
#include "selectable.h"

#include <imgui.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
	#include "imgui_impl/imgui_impl_sdl.h"
	#include "imgui_impl/imgui_impl_opengl3.h"
#pragma GCC diagnostic pop

#include <entt/entt.hpp>

#include <filesystem>
#include <string_view>


namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out) 
	{
		auto& v = *static_cast<std::vector<std::string>*>(vec);

		if (idx < 0 || idx >= static_cast<int>(v.size()))
			return false;

		*out = v.at(static_cast<std::size_t>(idx)).c_str();

		return true;
	};

	bool Combo(const char* label, int* index, std::vector<std::string>& values)
	{
		if (values.empty())
			return false;

		return Combo(label, index, vector_getter, static_cast<void*>(&values), static_cast<int>(values.size()));
	}

	bool ListBox(const char* label, int* index, std::vector<std::string>& values)
	{
		if (values.empty())
			return false;

		return ListBox(
			label, 
			index, 
			vector_getter, 
			static_cast<void *>(&values), 
			static_cast<int>(values.size()));
	}
} // namespace ImGui

namespace sec21::viewer
{
	static constexpr auto popup_window_decoration =
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoFocusOnAppearing | 
		ImGuiWindowFlags_NoNav;

	static constexpr auto popup_window_transparency = 0.35f;

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

	template <typename Window>
	auto render_ui(entt::registry& registry, entt::dispatcher& dispatcher, Window &&window) noexcept
	{
		namespace fs = std::filesystem;

		auto view = registry.view<debug_data>();
		const auto& active_debug_data = view.get(*view.begin());		

		std::vector<std::string> files;
		files = active_debug_data.input_files;

		auto settings_view = registry.view<preferences>();
		auto& active_settings = settings_view.get(*settings_view.begin());

		auto input_data_view = registry.view<input_data>();
		const auto& active_input_data = input_data_view.get(*input_data_view.begin());	

		auto camera_view = registry.view<camera>();
		auto& active_camera = camera_view.get(*camera_view.begin());

		// clang-format off
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		auto selected_entites_view = registry.view<selectable>();
		for (auto entity : selected_entites_view)
		{
			if (selected_entites_view.get<selectable>(entity).selected)
			{
				static bool p_open = true;
				auto window_pos = ImVec2{ 
					static_cast<float>(active_input_data.mouse_position.x), 
					static_cast<float>(active_input_data.mouse_position.y) };

				//! \todo 
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
				ImGui::SetNextWindowBgAlpha(popup_window_decoration); // Transparent background
				if (ImGui::Begin("Properties", &p_open, ImGuiWindowFlags_NoMove))
				{
					ImGui::Text("Mouse Position: <invalid>");
				}
				ImGui::End();
			}
		}

		// auto selected_entites_view = registry.view<selectable, load_tag, >();
		// for (auto entity : selected_entites_view)
		// {
		// 	static auto open = true;
		// 	// ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
		// 	// ImGui::SetNextWindowBgAlpha(popup_window_decoration); // Transparent background
		// 	if (ImGui::Begin("Properties", &open, ImGuiWindowFlags_NoMove))
		// 	{
		// 		ImGui::Text("Value: "); 
		// 		ImGui::SameLine(); if (ImGui::Button("Edit")) { dispatcher.trigger<event_new_node>({ node_x_pos, node_y_pos, 0.0 }); }
		// 	}
		// 	ImGui::End();
		// }

		ImGui::ShowDemoWindow();
		ImGui::ShowMetricsWindow();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) { dispatcher.trigger<event_clear_entites>(); }
				if (ImGui::MenuItem("Quit", "Alt+F4")) { dispatcher.trigger<event_quit_application>(); }

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

		debug_overlay();

		ImGui::Begin("sec21::viewer debug window");
		{
			static int current_file = 0;
			if (ImGui::Combo("Filelist", &current_file, files))
			{
				dispatcher.trigger<event_clear_entites>();
				dispatcher.trigger<event_load_model>(files[static_cast<std::size_t>(current_file)]);
			}

			if (ImGui::CollapsingHeader("Visibility"))
			{		
				ImGui::Checkbox("show nodes", &active_settings.show_nodes);
				ImGui::Checkbox("show members", &active_settings.show_members);
				ImGui::Checkbox("show supports", &active_settings.show_supports);
				ImGui::Checkbox("show deformation", &active_settings.show_deformation);
				ImGui::Checkbox("show load", &active_settings.show_load);
			}
			if (ImGui::CollapsingHeader("Camera"))
			{			
				ImGui::PushItemWidth(60);
				ImGui::Text("Position");
				ImGui::SameLine(); ImGui::SliderFloat("X", &active_camera.position.x, -50.0f, 50.0f);
				ImGui::SameLine(); ImGui::SliderFloat("Y", &active_camera.position.y, -50.0f, 50.0f);
				ImGui::SameLine(); ImGui::SliderFloat("Z", &active_camera.position.z, -50.0f, 50.0f);
				ImGui::PopItemWidth();
			}
		}
		ImGui::End();

		ImGui::Render();
		// clang-format on
	}
} // namespace sec21::viewer