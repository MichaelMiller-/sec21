#pragma once

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>

namespace sec21::viewer 
{
   static auto make_camera(entt::registry& registry)
   {
      spdlog::info("create camera ...");

      auto id = registry.create();
      auto& cam = registry.assign<camera>(id);

      cam.position = glm::vec3{ 0.0f, 0.0f, 8.0f };
      // camera.UpdateWindowSize(sdl::Graphics::Window());

      try
      {
         spdlog::debug("try to load camera settings from file");
         std::ifstream ifs{ "viewer/camera.json" };
         nlohmann::json j;
         ifs >> j;
         cam = j.get<camera>();
      }
      catch (...)
      {
         spdlog::warn("camera setup from file failed");
      }
      spdlog::info("camara initialzied successfull");
   }
}