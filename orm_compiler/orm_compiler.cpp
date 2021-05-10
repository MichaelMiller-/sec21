#include "config.h"
#include "entity.h"

#include <sec21/for_each_indexed.h>

#pragma warning(push)
// Dll interface
#pragma warning(push)
#pragma warning(disable: 4275)
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

// #include <fmt/ranges.h>
#pragma warning(pop)

#include <boost/program_options.hpp>

#include <numeric>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std::literals;

namespace v1
{
   const auto table_column_template = R"(
         struct {0} : data_mapper<{1},{2}>
         {{
            using constraints_t = std::tuple<>;
            static constexpr inline column_index = {3};
            static constexpr inline column_name = {0};
            static value_t get({4}::{5} const& obj) {{ return map(obj.{0}); }}
            static source_t upcast(value_t const& v) {{ return map(v); }}
         }};
   )"s;

   //

   const auto output_template = R"(#pragma once
//
// generated file from orm_compiler version {}.{}
// date: {}
//
namespace sec21::database
{{
   template <>
   struct table<{}::{}>
   {{
      struct columns 
      {{
         {}
      }};
      using metainfo = std::tuple<{}>;
   }};
}})"s;
}

namespace sec21::orm
{
   inline auto formatted_timepoint() 
   {
      const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
      auto time = std::chrono::system_clock::to_time_t(now);

      std::stringstream ss;
      ss << std::put_time(std::gmtime(&time), "%F %T%z");
      return ss.str();
   }

   auto generate(entity const& ent, std::string_view out_template)
   {
      std::string columns;
      for_each_indexed(begin(ent.columns), end(ent.columns), [&](auto i, auto v) {
         columns += fmt::format(
            v1::table_column_template,
            v.name,
            v.database_type,
            v.source_type,
            i,
            ent.full_typename,
            ent.name);
         });

      const auto prefix_colum_name = [](auto v)
      {
         return fmt::format("columns::{}", v.name);
      };

      std::vector<std::string> names;
      std::transform(begin(ent.columns), end(ent.columns), back_inserter(names), prefix_colum_name);

      const auto colum_names = std::accumulate(begin(names), end(names), std::string{}, [](auto init, auto v) {
         if (empty(init))
            init += v;
         else
            init += decltype(init){","} += v;
         return init;
         });

      return fmt::format(
         out_template, 
         VERSION_MAJOR, 
         VERSION_MINOR,
         formatted_timepoint(), 
         ent.full_typename, 
         ent.name,
         columns,
         colum_names);
   }

   void write(entity const& ent)
   {
      const auto s = fmt::format("{}_database_table.h", ent.name);
      std::ofstream ofs{s};
      ofs << orm::generate(ent, v1::output_template);
   }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
   try
   {
      spdlog::set_level(spdlog::level::debug);
      spdlog::info("starting application sec21::orm_compiler {}.{}", VERSION_MAJOR, VERSION_MINOR);

      std::string input_file{};

      namespace po = boost::program_options;
      po::options_description desc("Allowed options");
      desc.add_options()
         ("help", "produce help message")
         ("I,inputfile", po::value<decltype(input_file)>(), "input filename");

      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
      po::notify(vm);

      if (vm.count("help"))
      {
         std::cout << desc << "\n";
         return 1;
      }

      if (vm.count("input-file"))
         input_file = vm["input-file"].as<decltype(input_file)>();
      else
      {
         spdlog::critical("input-file argument is missing");
         return -1;
      }

      using namespace sec21;

      std::ifstream ifs{ input_file };
      std::string input{};
      std::copy(std::istreambuf_iterator<char>(ifs), {}, std::back_inserter(input));

      if (empty(input_file))
      {
         spdlog::critical("invalid input-file");
         return -2;
      }
      const auto ent = orm::parse(input);
      //! \todo check if entity is valid 
      orm::write(ent);
   }
   catch (std::exception& ex) 
   {
      spdlog::critical("Exception {}", ex.what());
      return -1;
   }
   return 0;
}

