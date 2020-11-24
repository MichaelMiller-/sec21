#include <nonius/nonius.h++>

#include <tuple>
#include <vector>
#include <chrono>

struct tridiagonal
{
   std::vector<double> values;

   tridiagonal() = default;
   tridiagonal(std::initializer_list<double> l) : values{ l } {}

   template <typename Source>
   //! \todo constrain Source to expression
   tridiagonal& operator = (Source const& src)
   {
      std::size_t const n = std::size(values);
      for (auto i = 0; i < n; ++i)
         values[i] = src[i];
      return *this;
   }
};

namespace conventional
{
   //! \todo is not corret at all; what if rhs.values.size is greater than lhs?
   tridiagonal operator + (tridiagonal const& lhs, tridiagonal const& rhs)
   {
      tridiagonal result;
      std::transform(
         std::begin(lhs.values),
         std::end(lhs.values),
         std::begin(rhs.values),
         std::back_inserter(result.values),
         [](auto const& l, auto const& r) { return l + r; });
      return result;
   }

   tridiagonal operator * (double const& lhs, tridiagonal const& rhs)
   {
      tridiagonal result{ rhs };
      std::transform(
         std::begin(result.values),
         std::end(result.values),
         std::begin(result.values),
         [lhs](auto const& v) { return v * lhs; });
      return result;
   }
}

namespace expression_template
{
   struct base_expression {};


   //! \todo something like a sequence type, more like "has operator []"
   template <typename T>
   struct is_sequence : std::false_type {};

   template <typename T>
   struct is_sequence<std::vector<T>> : std::true_type {};

   template <typename T>
   inline constexpr bool is_sequence_v = is_sequence<std::remove_cvref_t<T>>::value;

   template <typename T>
   inline constexpr bool is_expression_v = std::is_base_of_v<base_expression, std::remove_cvref_t<T>>;

   template <typename T>
   inline constexpr bool is_sequence_or_expression_v = is_sequence_v<T> || is_expression_v<T>;

   template <typename T, typename U>
   inline constexpr bool is_binary_op_v = is_sequence_or_expression_v<T> || is_sequence_or_expression_v<U>;


   template <typename Operand>
   auto subscript(Operand const& v, std::size_t const index)
   {
      if constexpr (is_sequence_or_expression_v<Operand>)
         return v[index];
      else
         return v;
   }

   template <typename Callable, typename... Operands>
   class expression : base_expression
   {
      std::tuple<Operands const&...> args;
      Callable func;
   public:
      constexpr expression(Callable f, Operands const&... ops) : args{ ops... }, func{ f } {}

      auto operator [](std::size_t const index) const
      {
         auto const call_at_index = [this, index](Operands const&... a) {
            return func(subscript(a, index)...);
         };
         return std::apply(call_at_index, args);
      }
   };

   template <typename T, typename U, typename = std::enable_if_t<is_binary_op_v<T, U>>>
   [[nodiscard]] constexpr auto operator * (T const& lhs, U const& rhs) noexcept
   {
      return expression{
          [](auto const& l, auto const& r) { return l * r; },
          lhs,
          rhs
      };
   }
   template <typename T, typename U, typename = std::enable_if_t<is_binary_op_v<T, U>>>
   [[nodiscard]] constexpr auto operator + (T const& lhs, U const& rhs) noexcept
   {
      return expression{
          [](auto const& l, auto const& r) { return l + r; },
          lhs,
          rhs
      };
   }

   auto operator + (tridiagonal const& lhs, tridiagonal const& rhs) noexcept
   {
      return lhs.values + rhs.values;
   }

   auto operator * (double const& lhs, tridiagonal const& rhs) noexcept
   {
      return lhs * rhs.values;
   }
}

std::string today()
{
   auto const now = std::chrono::system_clock::now();
   auto const tt = std::chrono::system_clock::to_time_t(now);

   std::stringstream ss;
   ss << std::put_time(std::localtime(&tt), "%Y-%m-%d");
   return ss.str();
}

//! \todo save benchmark results to a persistent database 

int main() 
{
   nonius::configuration cfg;
   cfg.output_file = today() + "_expression_templates.html";
   
   double a{ 1.23 };
   double b{ 3.14 };

   tridiagonal A{ { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 } };
   tridiagonal B{ { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 } };

   // const auto expected = std::vector{ 3.36, 6.73, 10.09, 13.46, 16.82, 20.19, 23.55, 26.92, 30.28 };

   const nonius::benchmark benchmarks[] = 
   {
      nonius::benchmark("conventional", [&] { 
         using namespace conventional;
         A = 0.77 * (a * A + b * B); 
         return A;
      }),
      nonius::benchmark("use expression templates", [&] {
         using namespace expression_template;
         A = 0.77 * (a * A + b * B);
         return A;
      }),
   };

   nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter{});
   //nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::csv_reporter());

   return 0;
}
