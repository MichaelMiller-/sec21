#include <catch2/catch_test_macros.hpp>

#include <sec21/overloaded.h>
#include <sec21/validate.h>

#include <functional>
#include <optional>
#include <tuple>
#include <variant>

// begin-snippet: validate.user.action
struct action1
{
};

template <>
struct sec21::validate<action1>
{
   std::optional<action1> operator()(std::string_view input) const noexcept
   {
      if (input == "action1") {
         return action1{};
      }
      return std::nullopt;
   }
};
// end-snippet

TEST_CASE("validate action1", "[sec21]")
{
   using namespace sec21;

   SECTION("matches with input string")
   {
      validate<action1> obj;
      REQUIRE(obj("action1").has_value());
   }
   SECTION("misspelling 1")
   {
      validate<action1> obj;
      REQUIRE(obj("action21").has_value() == false);
   }
   SECTION("misspelling 2")
   {
      validate<action1> obj;
      REQUIRE(obj("action11").has_value() == false);
   }
}

struct action2
{
   std::string arg1{};
};

#include <boost/algorithm/string/split.hpp>

template <>
struct sec21::validate<action2>
{
   std::optional<action2> operator()(std::string_view input) const noexcept
   {
      std::vector<std::string> results;
      boost::split(results, input, [](auto c) { return c == '='; });

      if (std::size(results) == 2 && results[0] == "action2") {
         return action2{results[1]};
      }
      return std::nullopt;
   }
};

TEST_CASE("validate action2", "[sec21]")
{
   using namespace sec21;

   SECTION("match")
   {
      validate<action2> obj;
      REQUIRE(obj("action2=/home/user/data.file").has_value());
   }
   SECTION("no filename")
   {
      validate<action2> obj;
      REQUIRE(obj("action2").has_value() == false);
   }
   SECTION("action11 not match")
   {
      validate<action2> obj;
      REQUIRE(obj("action2=/home/user/data.file=bar.file").has_value() == false);
   }
}

#include <nlohmann/json.hpp>

struct input_json
{
   int i{};
   std::string s{};
};
void to_json(nlohmann::json& j, input_json const& obj) { j = nlohmann::json{{"i", obj.i}, {"s", obj.s}}; }
void from_json(nlohmann::json const& j, input_json& obj)
{
   j.at("i").get_to(obj.i);
   j.at("s").get_to(obj.s);
}

template <>
struct sec21::validate<input_json>
{
   std::optional<input_json> operator()(std::string_view input) const noexcept
   {
      try {
         const auto j = nlohmann::json::parse(input);
         return j.get<input_json>();
      } catch (...) {
         return std::nullopt;
      }
      return std::nullopt;
   }
};

TEST_CASE("validate a json object", "[sec21]")
{
   using namespace sec21;
   validate<input_json> process;
   const auto result = process(R"({"i":42,"s":"foobar"})");

   REQUIRE(result.has_value());

   const auto obj = *result;
   REQUIRE(obj.i == 42);
   REQUIRE(obj.s == "foobar");
}

// actions
struct list_files
{
};
struct input_file
{
   std::string filename{};
};

template <>
struct sec21::validate<list_files>
{
   std::optional<list_files> operator()(std::string_view input) const noexcept
   {
      if (input == "list_files") {
         return list_files{};
      }
      return std::nullopt;
   }
};

template <>
struct sec21::validate<input_file>
{
   std::optional<input_file> operator()(std::string_view input) const
   {
      std::vector<std::string> parts{};
      boost::split(parts, input, [](auto c) { return c == '='; });

      if (size(parts) == 2 && parts[0] == "input_file") {
         return input_file{parts[1]};
      }
      return std::nullopt;
   }
};

TEST_CASE("dispatch the user input to a action", "[sec21]")
{
   auto dispatcher = sec21::input_dispatcher<list_files, input_file>{};

   SECTION("test 'list_files' input value")
   {
      auto result = dispatcher("list_files");
      REQUIRE(std::get_if<list_files>(&result) != nullptr);
   }
   SECTION("test 'input_file' input value")
   {
      auto result = dispatcher("input_file=foo.bar");
      auto ptr = std::get_if<input_file>(&result);
      REQUIRE(ptr != nullptr);
      REQUIRE(ptr->filename == "foo.bar");
   }
}