#include <catch.hpp>

#include <sec21/event/events.h>


#if __cpp_nontype_template_args >= 201911L

#include <sec21/event/input_manager.h>

class test_backend
{
   std::size_t counter{ 0 };
public:    
   std::tuple<bool, sec21::event::event_t> poll_event()
   {
      using namespace sec21::event;
      counter++;

      if (counter == 1) {
         return { true, pressed<mouse_button>{} };
      }
      if (counter == 2) {
         return { true, pressed<key>{} };
      }
      if (counter == 3) {
         return { true, time_elapsed{} };
      }
      if (counter == 4) {
         return { true, close_window{} };
      }
      return { false, {} };
   }
};


TEST_CASE("simulate some events", "[sec21][event]")
{
   using namespace sec21::event;

   input_manager<test_backend> input{};

   {
      auto result = input.next_event();
      REQUIRE(std::holds_alternative<pressed<mouse_button>>(result));
   }
   {
      auto result = input.next_event();
      REQUIRE(std::holds_alternative<pressed<key>>(result));
   }
   {
      auto result = input.next_event();
      REQUIRE(std::holds_alternative<time_elapsed>(result));
   }
   {
      auto result = input.next_event();
      REQUIRE(std::holds_alternative<close_window>(result));
   }
   {
      auto result = input.next_event();
      REQUIRE(std::holds_alternative<time_elapsed>(result));
   }
}
#endif