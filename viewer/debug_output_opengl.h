#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <spdlog/spdlog.h>

namespace sec21::viewer
{
   void debug_output_opengl(
      [[maybe_unused]] GLenum source, 
      [[maybe_unused]] GLenum type, 
      unsigned int id, 
      [[maybe_unused]] GLenum severity, 
      [[maybe_unused]] GLsizei length, 
      const char* message, 
      const void*)
   {
      // ignore non-significant error/warning codes
      // if(id == 131169 || id == 131185 || id == 131218 || id == 131204) 
      //    return; 

      spdlog::debug("OpenGL debug message ({}): {} ", id, message);

      // switch (source)
      // {
      //    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
      //    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
      //    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
      //    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
      //    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
      //    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
      // } std::cout << std::endl;

      // switch (type)
      // {
      //    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
      //    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
      //    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
      //    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
      //    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
      //    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
      //    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
      //    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
      //    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
      // } std::cout << std::endl;
      
      // switch (severity)
      // {
      //    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
      //    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
      //    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
      //    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
      // } std::cout << std::endl;
      // std::cout << std::endl;
   }

	auto enable_opengl_debug_output()
	{
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
		spdlog::debug("OpenGL debug output flag not available");
	}   
} // sec21::viewer
