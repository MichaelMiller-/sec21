#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sec21::viewer
{
   struct camera
   {
      // default camera values
      static constexpr auto kYaw = -90.0f;
      static constexpr auto kPitch = 0.0f;
      static constexpr auto kSpeed = 2.5f;
      static constexpr auto kSensitivity = 0.1f;
      static constexpr auto kZoom = 45.0f;

      glm::vec3 position{ 0.0f, 0.0f, 0.0f };
      glm::vec3 front{ 0.0f, 0.0f, -1.0f };
      glm::vec3 up{ 0.0f, 1.0f, 0.0f };
      glm::vec3 right{ 1.0f, 0.0f, 0.0f };
      glm::vec3 world_up{ 0.0f, 1.0f, 0.0f };
      // Euler Angles
      float yaw{ kYaw };
      float pitch { kPitch };
      // camera options
      float movement_speed{ kZoom };
      float mouse_sensitivity{ kSensitivity };
      float zoom{ kZoom };
   };

   //! \brief returns the view matrix calculated using Euler Angles and the LookAt Matrix
   //           | Rx  Ry  Rz  0 |   | 1  0  0 -Px |
   // look at = | Ux  Uy  Uz  0 | * | 0  1  0 -Py |
   //           | Dx  Dy  Dz  0 |   | 0  0  1 -Pz |
   //           | 0   0   0   1 |   | 0  0  0  1  |
   [[nodiscard]] inline auto view_matrix(camera const& cam) noexcept
   {
      return glm::lookAt(cam.position, cam.position + cam.front, cam.up);
   }
} // namespace sec21::viewer



#include <nlohmann/json.hpp>

namespace glm
{
   void to_json(nlohmann::json &j, vec3 const& value)
   {
      j = nlohmann::json{
         {"x", value.x},
         {"y", value.y},
         {"z", value.z}};
   }
   void from_json(nlohmann::json const &j, vec3 &value)
   {
      j.at("x").get_to(value.x);
      j.at("y").get_to(value.y);
      j.at("z").get_to(value.z);
   }
}

namespace sec21::viewer
{
   void to_json(nlohmann::json &j, camera const& value)
   {
      j = nlohmann::json{
         {"position", value.position},
         {"front", value.front},
         {"up", value.up},
         {"right", value.right},
         {"world_up", value.world_up},
         {"yaw", value.yaw},
         {"pitch", value.pitch},
         {"speed", value.movement_speed},
         {"mouse_sensitivity", value.mouse_sensitivity},
         {"zoom", value.zoom}
      };
   }
   void from_json(nlohmann::json const &j, camera &value)
   {
      j.at("position").get_to(value.position);
      j.at("front").get_to(value.front);
      j.at("up").get_to(value.up);
      j.at("right").get_to(value.right);
      j.at("world_up").get_to(value.world_up);
      j.at("yaw").get_to(value.yaw);
      j.at("pitch").get_to(value.pitch);
      j.at("speed").get_to(value.movement_speed);
      j.at("mouse_sensitivity").get_to(value.mouse_sensitivity);
      j.at("zoom").get_to(value.zoom);
   }
}
