#pragma once

#include "preferences.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <spdlog/spdlog.h>

template <>
struct std::default_delete<SDL_Window> 
{
	default_delete() = default;

	template <class U, class = std::enable_if_t<std::is_convertible_v<U*, SDL_Window*>>>
	constexpr default_delete(default_delete<U>) noexcept {}
	
	void operator()(SDL_Window* p) const noexcept 
	{ 
		spdlog::debug("destroy SDL window pointer");
		SDL_DestroyWindow(p);
	}
};

template <SDL_EventType Id>
struct sdl_event_impl 
{
	static constexpr auto id = Id;

	SDL_Event data{};

	explicit sdl_event_impl(SDL_Event const& data) noexcept 
		: data{ data }
	{}
};

namespace sec21::viewer
{
	struct glsl_version_140
	{
		static constexpr std::string_view version{"#version 140"};
	};

	auto initialize_sdl_opengl3(preferences settings = {}) -> std::tuple<std::unique_ptr<SDL_Window>, SDL_GLContext>
	{
		spdlog::info("initialize SDL/OpenGL window");
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw std::runtime_error(fmt::format("SDL could not initialize! SDL Error: {}", SDL_GetError()));

		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			spdlog::warn("Linear texture filtering not enabled!");

		// Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_Window* window{ nullptr };
		if (window = SDL_CreateWindow(
					"sec21::viewer",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					settings.screen_width,
					settings.screen_height,
					SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); window == nullptr)
		{
			throw std::runtime_error(fmt::format("Window could not be created! SDL Error: {}",SDL_GetError()));
		}

		spdlog::info("create OpenGL context");
		SDL_GLContext context{nullptr};
		if ((context = SDL_GL_CreateContext(window)) == nullptr)
			throw std::runtime_error(fmt::format("OpenGL context could not be created! SDL Error: {}",SDL_GetError()));

		glewExperimental = GL_TRUE;
		if (auto glew_error = glewInit() != GLEW_OK)
			throw std::runtime_error(fmt::format("Error initializing GLEW: {}", glewGetErrorString(glew_error)));

		if (SDL_GL_SetSwapInterval(1) < 0)
			spdlog::warn("Warning: Unable to set VSync! SDL Error {}", SDL_GetError());

		return { std::unique_ptr<SDL_Window>(window), context };
	}
}
