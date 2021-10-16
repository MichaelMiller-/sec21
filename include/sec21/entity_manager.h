#pragma once

#include <memory>
#include <bitset>
#include <vector>
#include <array>
#include <type_traits>
#include <cassert>
#include <algorithm>

namespace sec21
{
   // forward declaration
   struct component;

   using component_id_t = std::uint32_t;

   namespace detail
   {
      inline auto unique_component_id() noexcept
      {
         static component_id_t last_id{ 0 };
         return last_id++;
      }
   } // namespace detail

   // every Component has a unique ID
   template <typename T>
   inline auto create_component_id() noexcept
   {
      static_assert(std::is_base_of<component, T>::value, "T must inherit from component");

      static component_id_t id{ detail::unique_component_id() };
      return id;
   }

   static constexpr std::size_t max_components{ 32 };

   // forward declaration
   class entity;
   //! \brief every component type has an unique component id
   struct component
   {
      entity* owner{ nullptr };

      virtual ~component() = default;

      virtual void init() { }
      virtual void update([[maybe_unused]] float elapsed_time) { }
      virtual void draw() { }
   };

   //! \brief every entity has a only one instance of a certain component type
   class entity
   {
      std::vector<std::unique_ptr<component>>   components;
      std::array<component*, max_components>    component_array;
      std::bitset<max_components>               component_bitset;
      // e.g:
      // Component-Array
      //
      // [0] = Component Type #0
      // [1] = nullptr
      // [2] = nullptr
      // [3] = Component Type #3
      // [4] = nullptr
      // [5] = nullptr
      // [6] = nullptr
      // 
      // related Component-Bitset
      // [ 0, 0, 0, 1, 0, 0, 1 ] 
      //            |        |
      //            |     Component Type #0
      //       Component Type #3
      //

      // life-time-tracker
      bool alive{ true };

   public:
      entity() = default;

      entity(entity const&) = delete;
      entity& operator = (entity const&) = delete;

      template <typename T>
      void update(T elapsed_time)
      {
         for (auto& c : components)
            c->update(elapsed_time);
      }

      void draw()
      {
         for (auto& c : components)
            c->draw();
      }

      void destroy() noexcept { alive = false; }

      [[nodiscard]] bool is_alive() const noexcept { return alive; }

      template <typename T>
      [[nodiscard]] inline bool has_component() const noexcept
      {
         return component_bitset[create_component_id<T>()];
      }

      template <typename T, typename... Args>
      void add_component(Args &&... args)
      {
         // check if entity already has this component
         assert(!has_component<T>());

         auto c{ new T{std::forward<Args>(args)...} };
         c->owner = this;

         //decltype(components)::value_type ptr{ c };
         std::unique_ptr<component> ptr{ c };
         components.emplace_back(std::move(ptr));

         component_array[create_component_id<T>()] = c;
         component_bitset[create_component_id<T>()] = true;

         c->init();
         // return *c;
      }

      template <typename T>
      T& get_component() const
      {
         assert(has_component<T>());

         auto ptr(component_array[create_component_id<T>()]);
         return *static_cast<T*>(ptr);
      }
   };

   //! \see https://en.wikipedia.org/wiki/Entity_component_system
   //
   class entity_manager final
   {
      //! \todo also possible to set the number 
      std::vector<std::unique_ptr<entity>>   entities;

   private:
      inline void clear() noexcept {
         //! \todo test 
         entities.clear();
      }

   public:
      entity_manager() = default;
      // rule of 5
      ~entity_manager() { clear(); }

      entity_manager(entity_manager const&) = delete;
      entity_manager& operator = (entity_manager const&) = delete;

      //! \todo move ctor and move assignment

      template <typename T>
      void update(T elapsed_time)
      {
         const auto is_alive = [](auto&& e) { return !e->is_alive(); };
         // remove all "dead" entities
#ifdef __cpp_lib_erase_if
         std::erase_if(entities, is_alive);
#else
         entities.erase(
            std::remove_if(begin(entities), end(entities), is_alive),
            end(entities));
#endif
         //! \todo: std::for_each(parallel, std::begin(), std::end(), );
         for (auto& e : entities)
            e->update(elapsed_time);
      }

      void draw() {
         for (auto& e : entities)
            e->draw();
      }

      template <typename Function>
      void apply(Function&& func) const
      {
         for (const auto& e : entities)
            std::invoke(std::forward<Function>(func), e);
      }

#if 0
      //! \todo 
      template <typename Component>
      auto filter() const
      {

      }
#endif
      entity& create()
      {
         entity* e = new entity;
         decltype(entities)::value_type ptr{ e };
         entities.push_back(std::move(ptr));
         return *e;
      }
   };
}
