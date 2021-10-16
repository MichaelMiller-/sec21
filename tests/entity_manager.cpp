#include <catch.hpp>

#include <sec21/entity_manager.h>

namespace ns
{
   struct counter : public sec21::component
   {
      int value{ 0 };

      void update(float elapsed_time) override
      {
         std::ignore = elapsed_time;
         value++;
      }
   };

   struct hitpoints : public sec21::component
   {
      counter* c{ nullptr };

      void init() override
      {
         c = &owner->get_component<counter>();
      }

      void update(float elapsed_time) override
      {
         std::ignore = elapsed_time;
         if (c->value > 3)
            owner->destroy();
      }
   };
}

TEST_CASE("entity manager: component id check", "[core]")
{
   using namespace ns;
   using namespace sec21;

   REQUIRE(create_component_id<counter>() == 0);
   REQUIRE(create_component_id<hitpoints>() == 1);
   REQUIRE(create_component_id<counter>() == 0);
}

TEST_CASE("entity manager tests", "[core]")
{
   using namespace ns;
   using namespace sec21;

   entity_manager em;

   auto& entity1 = em.create();

   entity1.add_component<counter>();
   entity1.add_component<hitpoints>();

   em.update(1.0f);
   REQUIRE(entity1.is_alive() == true);
   em.update(1.0f);
   REQUIRE(entity1.is_alive() == true);
   em.update(1.0f);
   REQUIRE(entity1.is_alive() == true);
   em.update(1.0f);
   REQUIRE(entity1.is_alive() == false);
}
