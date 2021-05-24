#pragma once

#include <array>

namespace sec21::viewer
{
   enum class ForceAppearance { XY, Real };

   //! \todo different kind of settings, some of them are user-settungs and some of them are viewer-settings -> split up
   struct preferences
   {
      int screen_width{800};
      int screen_height{600};

      bool headless{false};
      int max_frames{0};

      bool show_nodes{true};
      bool show_members{true};
      bool show_supports{true};
      bool show_deformation{true};
      bool show_load{false};

      //! \todo not implemented yet
      ForceAppearance force_appearance{ForceAppearance::XY};

      float radius_node{0.3f};
      float radius_member{0.3f};
      float scale_load{1.0f};

      std::array<float, 3> clear_color{0.6f, 0.6f, 0.6f};
   };
} // namespace sec21::viewer

#include <nlohmann/json.hpp>

namespace sec21::viewer
{
   NLOHMANN_JSON_SERIALIZE_ENUM(ForceAppearance, {
                                                    {ForceAppearance::XY, "xy"},
                                                    {ForceAppearance::Real, "real"},
                                                 })

   void to_json(nlohmann::json& j, preferences const& e)
   {
      j = nlohmann::json{{"screen_width", e.screen_width},   {"screen_height", e.screen_height},
                         {"headless", e.headless},           {"max_frames", e.max_frames},
                         {"show_nodes", e.show_nodes},       {"show_members", e.show_members},
                         {"show_supports", e.show_supports}, {"show_deformation", e.show_deformation},
                         {"show_load", e.show_load},         {"force_appearance", e.force_appearance},
                         {"radius_node", e.radius_node},     {"radius_member", e.radius_member},
                         {"scale_load", e.scale_load},       {"clear_color", e.clear_color}};
   }

   void from_json(nlohmann::json const& j, preferences& e)
   {
      j.at("screen_width").get_to(e.screen_width);
      j.at("screen_height").get_to(e.screen_height);
      j.at("headless").get_to(e.headless);
      j.at("max_frames").get_to(e.max_frames);
      j.at("show_nodes").get_to(e.show_nodes);
      j.at("show_members").get_to(e.show_members);
      j.at("show_supports").get_to(e.show_supports);
      j.at("show_deformation").get_to(e.show_deformation);
      j.at("show_load").get_to(e.show_load);
      j.at("force_appearance").get_to(e.force_appearance);
      j.at("radius_node").get_to(e.radius_node);
      j.at("radius_member").get_to(e.radius_member);
      j.at("scale_load").get_to(e.scale_load);
      j.at("clear_color").get_to(e.clear_color);
   }
} // namespace sec21::viewer