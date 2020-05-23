#pragma once

#include "read_file.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <spdlog/spdlog.h>

#include <memory>
#include <tuple>
#include <string_view>
#include <type_traits>
#include <filesystem>

namespace sec21::viewer
{
	template <auto I>
	struct shader_type : std::integral_constant<GLenum, I> {};

	using vertex_shader_t = shader_type<GL_VERTEX_SHADER>;
	using fragment_shader_t = shader_type<GL_FRAGMENT_SHADER>;

	//! \todo noexcept if logger is noexcept
	template <typename ShaderType>
	auto load_and_compile_shader(std::string_view source) noexcept -> std::tuple<bool, GLuint>
	{
		//! \todo static_assert(std::ShaderType == shader_type)

		spdlog::info("load_and_compile_shader() from source ...");
		spdlog::debug("shader type: {} ...", ShaderType::value);

		auto shader = glCreateShader(ShaderType::value);

		if (auto result = glIsShader(shader); result != GL_TRUE)
		{
			spdlog::error("isShader() returns: {}", result);
			return { false, 0 };
		}

		const auto *ptr_source = reinterpret_cast<const GLchar*>(source.data());
		glShaderSource(shader, 1, &ptr_source, nullptr);

		GLint length{ 0 };
		glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
		spdlog::debug("shader source length: {} ...", length);

		if (length > 0)
		{
			GLint isCompiled = GL_FALSE;

			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				spdlog::error("compile error: {}", isCompiled);
				glDeleteShader(shader);
				return { false, 0 };
			}
		}
		return { true, shader };
	}

	template <typename T>
	struct uniform_apply_impl;

	template <>
	struct uniform_apply_impl<float>
	{
		void operator()(int id, float v0) const
		{
			glUniform1f(id, v0);
		}
		void operator()(int id, float v0, float v1, float v2) const
		{
			glUniform3f(id, v0, v1, v2);
		}    
	};

	template <>
	struct uniform_apply_impl<int>
	{
		void operator()(int id, int v) const
		{
			glUniform1i(id, v);
		}
	};

	template <typename T, auto N>
	struct uniform_apply_impl<std::array<T, N>>
	{
		void operator()(int id, std::array<T, N> const& v) const
		{
			static_assert(std::is_floating_point_v<T>);
			//! \todo only float at this point
			glUniform3f(id, std::get<0>(v), std::get<1>(v), std::get<2>(v));
		}
	};


	template <typename T, std::size_t N>
	struct uniform_impl;

	template <typename T>
	struct uniform_impl<T, 1> 
	{
		static void apply(GLint id, T v) noexcept 
		{
			uniform_apply_impl<T>()(id, v);
		}
	};

	template <typename T>
	struct uniform_impl<T, 3> 
	{
		static void apply(GLint id, T v0, T v1, T v2) noexcept 
		{
			uniform_apply_impl<T>()(id, v0, v1, v2);
		}
	};

	class shader_resource
	{
		//! \todo rename
		GLuint m_program{ 0 };
		
		GLuint vertex_shader_id{ 0 };
		GLuint fragment_shader_id{ 0 };

		std::map<std::string, GLuint> m_fileVertexShaders;
		std::map<std::string, GLuint> m_fileFragmentShaders;

	private:
		//! \todo is spdlog noexcept? -> so this method can also be noexcept
		[[nodiscard]] auto uniform_location(std::string_view name) const
		{
			auto location = glGetUniformLocation(m_program, name.data());
			if (location == -1)
			{
				if (auto index = name.find("gl_"); index == 0)
					spdlog::error("{} starts with the reserved prefix \"gl_\".", name);
				else
					spdlog::error("{} does not correspond to an active uniform variable", name);
			}
			return location;
		}

		void create_program(GLuint shader) // noexcept
		{
			spdlog::info("create shader program: {} ...", m_program);
			if (!m_program)
			{
				GLuint program = glCreateProgram();
				spdlog::debug("new shaderprogram is required. id: {}", program);

				if (glIsProgram(program) == GL_TRUE)
					m_program = program;
				else
					spdlog::error("a fatal error occurred creating the GLSL program object.");
			}
			spdlog::debug("attach shader: {} to program: {}", shader, m_program);
			glAttachShader(m_program, shader);

			spdlog::info("shader program successful created");
		}

	public:
		bool link()
		{
			spdlog::info("link shader");
			auto is_linked = GL_FALSE;

			glLinkProgram(m_program);
			glGetProgramiv(m_program, GL_LINK_STATUS, &is_linked);
			spdlog::debug("shader is linked: {}", static_cast<bool>(is_linked));

			// Mark all attached shaders for deletion. These shaders aren't deleted
			// right now because they are already attached to a shader program.
			// When the shader program is deleted these shaders will be
			// automatically detached and deleted.

			//! \todo good approch?
			// delete_shader(vertex_shader_id);
			// delete_shader(fragment_shader_id);

			if (is_linked == GL_FALSE)
			{
				spdlog::error("error linking GLSL shaders into a shader program: {}", m_program); //, get_program_info_log(m_program));
				glDeleteProgram(m_program);
				m_program = 0;
				return false;
			}
			spdlog::info("shader linked to id: {} ...", m_program);
			return true;
		}

	public:
		~shader_resource()
		{
			spdlog::info("begin to destroy shader: {}", m_program);

			delete_shader(vertex_shader_id);
			delete_shader(fragment_shader_id);

			if (m_program != 0)
				glDeleteProgram(m_program);

			spdlog::info("shader destroyed");
		}	

		static shader_resource& load_from_file(
			std::filesystem::path const& vertex_shader_file, 
			std::filesystem::path const& fragment_shader_file)
		{
			spdlog::info("load shader from files: {}, {}", vertex_shader_file.string(), fragment_shader_file.string());

			const auto vertex_shader_content = read_file(vertex_shader_file.string()); // "viewer/shader1.vs");
			const auto fragment_shader_content = read_file(fragment_shader_file.string()); // "viewer/shader1.fs");

			auto instance{ new shader_resource };

			auto [result_vertex_loader, vertex_shader_id] = load_and_compile_shader<vertex_shader_t>(vertex_shader_content);

			if (result_vertex_loader)
			{
				instance->vertex_shader_id = vertex_shader_id;
				instance->create_program(vertex_shader_id);
			}

			auto [result_fragment_loader, vertex_fragment_id] = load_and_compile_shader<fragment_shader_t>(fragment_shader_content);

			if (result_fragment_loader)
				instance->create_program(vertex_fragment_id);

			instance->link();

			return *instance;
		}

		void disable() const noexcept
		{
			glUseProgram(0);
		}

		void enable() const noexcept
		{
			glUseProgram(m_program);
		}

		template <typename First, typename... Rest>
		void uniform(std::string_view name, First first, Rest... rest) noexcept
		{
			uniform_impl<First, sizeof...(Rest) + 1>::apply(uniform_location(name), first, std::forward<Rest>(rest)...);
		}

		void uniform_mat(std::string_view name, glm::mat4 const& matrix) const noexcept
		{
			glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}

		auto attrib_location(std::string_view name) const noexcept -> GLint //-> std::invoke_result_t<getAttribLocation()> { 
		{
			return glGetAttribLocation(m_program, name.data());
		}		

	private:
		void delete_shader(GLuint id) const
		{
			spdlog::debug("delete shader: {} ", id);
			GLuint shader = id;
			GLint isFlaggedForDeletion = GL_FALSE;

			glGetShaderiv(id, GL_DELETE_STATUS, &isFlaggedForDeletion);

			if (isFlaggedForDeletion == GL_FALSE)
			{
				spdlog::debug("shader: {} is flagged for deletion", shader);
				glDeleteShader(shader);
			}
		}
	};
}
