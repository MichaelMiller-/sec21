#pragma once

#include <boost/uuid/uuid.hpp>

#include <nlohmann/json.hpp>

#include <sec21/json_serializer_optional.h>
#include <sec21/json_serializer_uuid.h>
#include <sec21/type_traits/is_std_string.h>
#include <sec21/timestamp.h>

#include <iomanip>
#include <optional>
#include <string>
#include <regex>

namespace sec21::log
{
   enum class Kind { Info, Error, Warning, Metric, Debug };

   struct entry
   {
      Kind kind{Kind::Info};
      //
      //! \todo timestamp<long>
      std::uint64_t timestamp{0}; // 1970-01-01
      //
      int process_id{-1};
      //
      std::string message{};
      // contains the UUID of the item to which the message is related
      std::optional<boost::uuids::uuid> element{};

#ifdef __cpp_lib_three_way_comparison
      bool operator == (entry const& ) const = default;
#else
      bool operator == (entry const& rhs) const
      {
         return std::tie(kind, timestamp, process_id, message, element) ==
                std::tie(rhs.kind, rhs.timestamp, rhs.process_id, rhs.message, rhs.element);
      }
#endif
   };

   constexpr bool operator<(entry const& lhs, entry const& rhs) noexcept { return lhs.timestamp < rhs.timestamp; }

   NLOHMANN_JSON_SERIALIZE_ENUM(Kind, {
                                         {Kind::Info, "Info"},
                                         {Kind::Error, "Error"},
                                         {Kind::Warning, "Warning"},
                                         {Kind::Metric, "Metric"},
                                         {Kind::Debug, "Debug"},
                                      })

   inline void to_json(nlohmann::json& j, entry const& obj)
   {
      j = nlohmann::json{{"kind", obj.kind},
                         {"timestamp", obj.timestamp},
                         {"process_id", obj.process_id},
                         {"message", obj.message},
                         {"element", obj.element}};
   }
   inline void from_json(nlohmann::json const& j, entry& obj)
   {
      j.at("kind").get_to(obj.kind);
      j.at("timestamp").get_to(obj.timestamp);
      j.at("process_id").get_to(obj.process_id);
      j.at("message").get_to(obj.message);
      j.at("element").get_to(obj.element);
   }

   [[nodiscard]] inline auto to_kind(std::string s) -> Kind
   {
      std::transform(begin(s), next(begin(s)), begin(s), [](unsigned char c){ return std::toupper(c); } );
      nlohmann::json j = s;
      return j.get<Kind>();
   }

   template <typename Iterator>
   [[nodiscard]] auto to_entries(Iterator first, Iterator last)
   {
      static_assert(type_traits::is_std_string_v<typename std::iterator_traits<Iterator>::value_type>);

      static const std::regex expr(R"(\[(.*)\]\s\[(\w*)\]\slog:\s(\d+)\smessage:\s(?:(.*))?$)");

      std::vector<entry> result(static_cast<std::size_t>(std::distance(first, last)));

      const auto to_value = [](auto line)-> entry
      {
        std::smatch string_match;
        if (std::regex_search(line, string_match, expr))
        {
           return {
              to_kind(string_match[2].str()),
              timestamp<std::uint64_t>(string_match[1].str()),
              std::atoi(string_match[3].str().c_str()),
              string_match[4].str()
           };
        }
        return {};
      };
      std::transform(first, last, begin(result), to_value);
      return result;
   }

} // namespace sec21::log
