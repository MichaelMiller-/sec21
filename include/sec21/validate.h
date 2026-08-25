#pragma once

#include <tuple>
#include <variant>
#include <string_view>
#include <functional>

namespace sec21
{
   // customization point for 'actions'. Any 'action' has to specialize the 'validate' template
   // and has to provide a function call operator which has to return a std::optional of the action type.
   template <typename T>
   struct validate;

   struct invalid_input {};

   // transform any given input into a 'action'. if input validation fails the variant holds 'invalid_input'.
   //! \todo guard Action with a concept
   template <typename... Actions>
   struct input_dispatcher
   {
      static_assert(sizeof...(Actions) > 0);

      using action_t = std::variant<invalid_input, Actions...>;

      template <typename... Args>
      [[nodiscard]] auto operator()(Args &&... args) const -> action_t
      {
         auto validate_input = [args...](auto v) {
            return std::invoke(validate<std::decay_t<decltype(v)>>{}, std::forward<Args>(args)...);
         };

         auto validated_inputs = std::apply(
            [&](auto const &... v) { return std::make_tuple(validate_input(v)...); },
            std::tuple<Actions...>{});

         action_t result{};
         auto transform_to_action = [&result](auto t) {
            if (t.has_value()) {
               result = *t;
            }
         };
         std::apply([&](auto const &... v) { (transform_to_action(v),...); }, validated_inputs);

         return result;
      }
   };
}
