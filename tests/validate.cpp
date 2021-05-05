#include <catch.hpp>

#include <sec21/overloaded.h>
#include <sec21/validate.h>

#include <functional>
#include <tuple>
#include <variant>
#include <optional>

// begin-snippet: validate.user.action
struct action1 {};

template <>
struct sec21::validate<action1>
{
   std::optional<action1> operator()(std::string_view input) const noexcept
   {
      if (input == "action1")
         return action1{};
      return std::nullopt;
   }
};
// end-snippet

TEST_CASE("action1 validation", "[sec21][core]")
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

struct action2 { std::string arg1{}; };

#include <boost/algorithm/string.hpp>

template <>
struct sec21::validate<action2>
{
   std::optional<action2> operator()(std::string_view input) const noexcept
   {
      std::vector<std::string> results;
      boost::split(results, input, [](auto c) { return c == '='; });

      if (std::size(results) == 2 && results[0] == "action2")
         return action2{ results[1] };
      return std::nullopt;
  }
};

TEST_CASE("action2 validation", "[sec21][core]")
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
void to_json(nlohmann::json &j, input_json const &obj)
{
   j = nlohmann::json{{"i", obj.i}, {"s", obj.s }};
}
void from_json(nlohmann::json const &j, input_json &obj)
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
      }
      catch(...) {
         return std::nullopt;
      }
      return std::nullopt;
   }
};

TEST_CASE("validate a json object", "[sec21][core]")
{
   using namespace sec21;
   validate<input_json> process;
   const auto result = process(R"({"i":42,"s":"foobar"})");

   REQUIRE(result.has_value());
   
   const auto obj = *result;
   REQUIRE(obj.i == 42);
   REQUIRE(obj.s == "foobar");
}

#include <boost/algorithm/string.hpp>

// request actions
struct listfiles {};
struct filecontent { std::string filename{}; };

using action_t = std::variant<
	listfiles,
	filecontent>;

template <>
struct sec21::validate<listfiles>
{
   std::optional<listfiles> operator()(std::string_view input) const noexcept
   {
      if (input == "listfiles")
         return listfiles{};
      return std::nullopt;
   }
};

template <>
struct sec21::validate<filecontent>
{
   std::optional<filecontent> operator()(std::string_view input) const noexcept
   {
      std::vector<std::string> results;
      boost::split(results, input, [](auto c) { return c == '='; });

      if (std::size(results) == 2 && results[0] == "filecontent")
         return filecontent{ results[1] };
      return std::nullopt;
   }
};

std::string response(action_t action) noexcept
{
	return std::visit(
        sec21::overloaded{
			[](filecontent) { return std::string{ "example filecontent" }; },
			[](listfiles) { return std::string{ "list of files[]" }; }
		},
		action
	);
}

TEST_CASE("dispatch the user input to a action", "[sec21][core]")
{
   const auto process_input = sec21::input_dispatcher<std::string, listfiles, filecontent>([](auto action) -> std::string {
         if (action) {
            return response(*action);
         }
         return "invalid input";
      });

   REQUIRE(process_input("filecontent") == "invalid input");
   REQUIRE(process_input("filecontent=/home/bar/foo.file") == "example filecontent");
   REQUIRE(process_input("listfiles") == "list of files[]");
}