#pragma once

#include <sec21/is_specialized.h>

#include <functional>
#include <tuple>
#include <variant>
#include <string_view>
#include <optional>

namespace sec21
{
   // customization point for input validation
   template <typename T>
   struct validate;

   namespace detail
   {
      template<typename...Ts, typename Function, size_t... Is>
      auto transform(std::tuple<Ts...> const& inputs, Function function, std::index_sequence<Is...>)
      {
         return std::tuple<std::result_of_t<Function(Ts)>...>{function(std::get<Is>(inputs))...};
      }

      template<typename... Ts, typename Function>
      auto transform(std::tuple<Ts...> const& inputs, Function function)
      {
         return transform(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
      }

      template <typename... InputDescriptions>
      [[nodiscard]] auto transform_inputs(std::string_view input)
      {
#if __cpp_generic_lambdas > 201304         
         const auto validate_input = [&input]<typename T>(T) { return std::invoke(validate<T>{}, input); };
#else         
         const auto validate_input = [&input](auto v) {
            return std::invoke(validate<std::decay_t<decltype(v)>>{}, input); 
         };
#endif
         return transform(std::tuple<InputDescriptions...>{}, validate_input);
      }
   }

   template <typename Response, typename... InputDescriptions>
   class input_dispatcher
   {
      static_assert(sizeof...(InputDescriptions) > 0);
      static_assert(std::conjunction_v<is_specialized<validate, InputDescriptions>...>);

   public:
      using action_t = std::variant<InputDescriptions...>;
      using process_t = std::function<Response(std::optional<action_t>)>;

   private:
      process_t func;

   public:
      constexpr explicit input_dispatcher(process_t&& process) : func{process} {}

      [[nodiscard]] auto operator()(std::string_view input) const noexcept -> Response
      {
         const auto validated_inputs = detail::transform_inputs<InputDescriptions...>(input);

         // check if at least one input is valid 
         //! \todo count is maybe saver: condition= c == 1 -> true; else is false
         //! \todo any_of with v.has_value()
         if (std::apply([](auto const &... v) { return (v.has_value() || ...); }, validated_inputs) == false) 
            return std::invoke(func, std::nullopt);

         // extract action
         action_t result;
         const auto transform_to_action = [&result](auto t) { 
            if (t.has_value())
               result = *t;
         };
         std::apply([&](auto const &... v) { (transform_to_action(v),...); }, validated_inputs);

         return std::invoke(func, result);
      }
   };
}
