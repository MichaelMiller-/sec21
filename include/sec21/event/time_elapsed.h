#pragma once

#include <nlohmann/json.hpp>

#include <chrono>
#include <string_view>

namespace sec21::event
{
   struct time_elapsed
   {
      static constexpr std::string_view name{ "time elapsed" };

      using clock_t = std::chrono::steady_clock;
      clock_t::duration elapsed{};
   };
}

namespace nlohmann 
{
	template<> 
	struct adl_serializer<sec21::event::time_elapsed::clock_t::duration>
	{
		static void to_json(nlohmann::json& j, const sec21::event::time_elapsed::clock_t::duration& duration)
		{
			j = nlohmann::json{ { "nanoseconds", std::chrono::nanoseconds{duration}.count() } };
		}

		static void from_json(const nlohmann::json& j, sec21::event::time_elapsed::clock_t::duration& duration)
		{
			//! \todo get unterlying type instead of uint64_t
			const std::uint64_t value = j.at("nanoseconds");
			duration = std::chrono::nanoseconds{value};
		}
	};
}

namespace sec21::event
{
   void to_json(nlohmann::json& j, time_elapsed const& obj)
   {
      j = nlohmann::json{ { obj.name, obj.elapsed } };
   }

   void from_json(nlohmann::json const& j, time_elapsed& obj)
   {
      j.at(obj.name.data()).get_to(obj.elapsed);
   }
}