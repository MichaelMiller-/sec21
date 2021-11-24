#pragma once

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <chrono>

namespace sec21
{
   // milliseconds wrapper
   template <typename T>
   class timestamp
   {
      using value_t = T;
      value_t value{0};

    public:
      explicit timestamp(std::string s)
      {
         using namespace boost::gregorian;
         using namespace boost::local_time;
         using namespace boost::posix_time;
         const auto tp = time_period(ptime{date(1970, 1, 1)}, time_from_string(std::move(s)));
         value = tp.length().total_milliseconds();
      }

      operator value_t() const noexcept { return value; }

      static value_t now() noexcept
      {
         const auto now = std::chrono::system_clock::now();
         return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
      }
   };
} // namespace sec21