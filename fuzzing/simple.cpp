// showcase of the boilerplate code
#include "boilerplate.h"

#include <string>

struct Employee
{
   int id{0};
   std::string firstName{};
   std::string lastName{};
   std::string address{};
};

void do_something(Employee const&)
{
   //
}

void do_another_thing(std::uint32_t, int)
{
   //
}

extern "C" int LLVMFuzzerTestOneInput([[maybe_unused]] const uint8_t* data, [[maybe_unused]] std::size_t count)
{
   {
      auto [employee, remaining] = make_fuzzed_object<Employee>({data, count});
      do_something(employee);
   }
   {
      // std::tuple interface
      auto [args, remaining] = make_fuzzed_objects<std::uint32_t, int>({data, count});
      std::apply(do_another_thing, args);
   }

   return 0;
}
