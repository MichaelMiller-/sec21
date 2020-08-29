#pragma once

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <sec21/file_loader.h>

namespace sec21::viewer 
{
   static inline auto make_camera(entt::registry& registry)
   {
      spdlog::info("create camera ...");

      auto id = registry.create();
      auto& cam = registry.emplace<camera>(id);

      cam.position = glm::vec3{ 0.0f, 0.0f, 8.0f };

      try
      {
         spdlog::debug("try to load camera settings from file");
			cam = sec21::load_from_json<camera>("viewer/camera.json");
      }
      catch (...)
      {
         spdlog::warn("camera setup from file failed");
      }
      spdlog::info("camara initialzied");
   }
}