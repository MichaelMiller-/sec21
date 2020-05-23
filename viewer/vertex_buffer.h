#pragma once

#include <sec21/for_each_indexed.h>
#include <sec21/resource.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

#include <vector>
#include <tuple>
#include <memory>
#include <cstddef>

namespace sec21::viewer
{
   template <auto Value>
   struct render_primitive_t : std::integral_constant<GLenum, Value>{};

   struct triangles : render_primitive_t<GL_TRIANGLES>{};

   enum class vertex_attribute_tag_t
   {
      position,
      normale,
      texture_coords,
      color
   };

   constexpr auto to_string(vertex_attribute_tag_t value) noexcept
   {
      using namespace std::literals;
      switch (value)
      {
         case vertex_attribute_tag_t::position:
            return "position"sv;
         case vertex_attribute_tag_t::normale:
            return "normale"sv;
         case vertex_attribute_tag_t::texture_coords:
            return "texture coordinates"sv;
         case vertex_attribute_tag_t::color:
            return "color"sv;
      }
      return "invalid"sv;
   }

   enum class vertex_attribute_t
   {
      float_2,
      float_3,
      ubyte_4
   };

   constexpr auto number_of_components(vertex_attribute_t value) noexcept
   {
      switch (value)
      {
         case vertex_attribute_t::float_2:
            return 2;
         case vertex_attribute_t::float_3:
            return 3;
         case vertex_attribute_t::ubyte_4:
            return 4;
      }
      return 0;
   }

   constexpr auto data_type(vertex_attribute_t value) noexcept -> GLenum 
   {
      switch (value)
      {
         case vertex_attribute_t::float_2:
         case vertex_attribute_t::float_3:
            return GL_FLOAT;
         case vertex_attribute_t::ubyte_4:
            return GL_UNSIGNED_BYTE;
      }
      return 0;
   }

   constexpr auto to_string(vertex_attribute_t value) noexcept
   {
      using namespace std::literals;
      switch (value)
      {
         case vertex_attribute_t::float_2:
            return "float_2"sv;
         case vertex_attribute_t::float_3:
            return "float_3"sv;
         case vertex_attribute_t::ubyte_4:
            return "ubyte_4"sv;
      }
      return "invalid"sv;
   }

   template <typename Vertex>
   class vertex_buffer
   {
      struct attribute
      {
         vertex_attribute_tag_t tag{};
         vertex_attribute_t type{};
         std::byte offset{};
         std::byte stride{ sizeof(Vertex) };
         bool normalized{ false };
      };

      std::uint32_t vertex_count{ 0 };
      std::uint32_t index_count{ 0 };

      std::uint32_t vao{ 0 };
      std::uint32_t vbo{ 0 };
      std::uint32_t ebo{ 0 };

      std::vector<attribute> attributes{};

   public:
      vertex_buffer() noexcept = default;
      // vertex_buffer(vertex_buffer const&) = delete;

      ~vertex_buffer()
      {
         spdlog::debug("delete vertexbuffer: {}", vao);
         glDeleteVertexArrays(1, &vao);
         glDeleteBuffers(1, &vbo);
         glDeleteBuffers(1, &ebo);
      }

      inline void bind() const noexcept
      {
         glBindVertexArray(vao);
      }

      template <typename RenderPrimitive>
      void draw() const noexcept
      {
         if (index_count == 0)
            glDrawArrays(RenderPrimitive::value, 0, static_cast<GLsizei>(vertex_count));
         else
            glDrawElements(RenderPrimitive::value, static_cast<GLint>(index_count), GL_UNSIGNED_INT, 0);
      }

      class builder
      {
         // std::unique_ptr<vertex_buffer> instance{new vertex_buffer};
         vertex_buffer* instance;
      private:
         template <typename Iterator>
         auto impl(Iterator first, Iterator last, std::vector<unsigned int> idx)
         {
            spdlog::info("build vertexbuffer impl ...");

            using value_t = typename std::iterator_traits<Iterator>::value_type;
            static_assert(std::is_same_v<value_t, Vertex>);

            instance->index_count = static_cast<decltype(instance->index_count)>(std::size(idx));
            instance->vertex_count = static_cast<decltype(instance->vertex_count)>(std::distance(first, last));
            // instance->ptr = std::addressof(*first);
            auto n = static_cast<GLsizeiptr>(std::distance(first, last));

            glGenVertexArrays(1, &instance->vao);
            glGenBuffers(1, &instance->vbo);
            glGenBuffers(1, &instance->ebo);

            glBindVertexArray(instance->vao);
            glBindBuffer(GL_ARRAY_BUFFER, instance->vbo);
            glBufferData(
               GL_ARRAY_BUFFER, 
               n * sizeof(value_t), 
               std::addressof(*first), 
               GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(idx.size() * sizeof(unsigned int)), std::addressof(idx[0]), GL_STATIC_DRAW);

            for_each_indexed(instance->attributes, [](GLuint index, auto const& attr) 
            {
               // log
               spdlog::debug(
                  "attribute: {} tag: {} type: {} offset: {}_byte stride: {}_byte", 
                  index, 
                  to_string(attr.tag), 
                  to_string(attr.type), 
                  std::to_integer<int>(attr.offset), 
                  std::to_integer<int>(attr.stride));

               glEnableVertexAttribArray(index);	
               glVertexAttribPointer(
                  index, 
                  number_of_components(attr.type), 
                  data_type(attr.type), 
                  GL_FALSE, 
                  std::to_integer<int>(attr.stride), 
                  reinterpret_cast<void*>(std::to_integer<int>(attr.offset)));
            });

            glBindVertexArray(0);
            spdlog::info("vertexbuffer: {} created with: {} vertices", instance->vao, std::distance(first, last));
         }    
      public:
         builder() : instance{ new vertex_buffer } {}

         template <typename T> 
         builder& vertex_count(T count) noexcept
         {
            instance->vertex_count = static_cast<decltype(instance->vertex_count)>(count);
            return *this;
         }

         template <typename... Args>
         builder& attribute(Args &&... args)
         {
            instance->attributes.push_back({ std::forward<Args>(args)... });
            return *this;
         }

         template <typename Iterator>
         auto from_vertex_array(Iterator first, Iterator last, std::vector<unsigned int> idx) -> vertex_buffer&
         {
            impl(first, last, std::move(idx));
            return *instance;
         }

         //! \todo arguments, -> index array / only pointer to the first index 
         template <typename Iterator>
         auto build(Iterator first, Iterator last, std::vector<unsigned int> idx) -> std::unique_ptr<vertex_buffer>
         {
            impl(first, last, std::move(idx));
            return std::unique_ptr<vertex_buffer>{ instance };
         }
      };
   };
}