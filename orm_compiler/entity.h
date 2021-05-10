#pragma once

#pragma warning(push)
#pragma warning(disable: 4275)
#include <fmt/format.h>
#pragma warning(pop)

#include <boost/algorithm/string/trim.hpp>

#include <string>
#include <vector>
#include <regex>

namespace sec21::orm
{
   struct entity
   {
      struct column
      {
         std::string name;
         std::string source_type;
         std::string database_type;

         friend auto& operator << (std::ostream& os, column const& obj)
         {
            return os << fmt::format(
               "column name:'{}' srctype:'{}' datatype:'{}'",
               obj.name,
               obj.source_type,
               obj.database_type);
         }

      };

      std::string name;
      std::string full_typename;
      std::vector<column> columns;

      friend auto& operator << (std::ostream& os, entity const& obj)
      {
         return os << fmt::format(
            "entity\n\t name:'{}'\n\t full:'{}'\n\t col N:{}",
            obj.name,
            obj.full_typename,
            std::size(obj.columns));
      }
   };

   namespace detail
   {
      auto entity_name(std::string_view input) -> std::string
      {
         const std::regex expr(R"(\[\[Entity\]\]\s*(struct|class)\s*(\w+))");
         const auto str = std::string{ input };
         std::smatch smtch;
         regex_search(str, smtch, expr);
         return smtch[2];
      }

      auto entity_namespace(std::string_view input) -> std::string
      {
         const auto entity_pos = input.find("[[Entity]]");
         const auto namespace_pos = input.find("namespace", 0, entity_pos);
         const auto it_first = std::find(std::next(begin(input), namespace_pos), end(input), ' ');
         const auto it_last = std::find(std::next(it_first), end(input), '{');

         return boost::algorithm::trim_copy(std::string(it_first, it_last));
      }
   }

   auto parse(std::string_view input) -> entity
   {
      entity result;
      result.name = detail::entity_name(input);
      result.full_typename = detail::entity_namespace(input);

      const std::regex expr(R"(\[\[Column\]\]\s*(\[\[\w+\]\])*\s*([\w:]*)\s*(\w*)\s*;)");
      std::smatch smtch;

      const auto str = std::string{ input };
      const auto column_pos = str.find("[[Column]]");

      auto first = std::next(begin(str), column_pos);
      const auto last = end(str);
      //! \todo while loop is ugly as hell
      while (regex_search(first, last, smtch, expr))
      {
         if (smtch.length(0) > 0)
         {
            entity::column obj;
            obj.database_type = smtch[1];
            obj.source_type = smtch[2];
            obj.name = smtch[3];

            const auto type_mapper = [](auto t) {
               if (t == "[[SqlString]]")
                  return "std::string";
               return "InvalidTypeCheckAttributes";
            };
            // postprocess datatype
            obj.database_type = empty(obj.database_type) ? obj.source_type : type_mapper(obj.database_type);
            result.columns.push_back(obj);
            first = std::next(first, smtch.length(0));
         }
         else if (first != last)
            ++first;
         else
            break;
      }
      return result;
   }

}
