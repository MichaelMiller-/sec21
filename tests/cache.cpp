#define CATCH_CONFIG_MAIN
#include "catch.hpp"


namespace policies
{
   template <std::size_t N>
   //! \todo rename
   struct clear_after_N_entries
   {
      template <typename T>
      static void apply(T& t) 
      {
         if (t.size() > N)
            t.clear();
      }
   };

   //! \todo policies -> clear cache if current size > MB

}

TEST_CASE("test clear N entries", "[cache][policies]")
{
   std::vector<int> result{ 1, 2, 3 };
   using policy = policies::clear_after_N_entries<5>;

   policy::apply(result);
   REQUIRE(result.size() == 3);

   result.push_back(4);
   policy::apply(result);
   REQUIRE(result.size() == 4);

   result.push_back(5);
   policy::apply(result);
   REQUIRE(result.size() == 5);

   result.push_back(6);
   policy::apply(result);
   REQUIRE(result.size() == 0);

   result.push_back(7);
   policy::apply(result);
   REQUIRE(result.size() == 1);
}

template <typename Key, typename Value, typename ClearPolicy>
class cache
{
   std::map<Key, Value> data{};

public:
   auto operator()(Key const& key)
   {
      const auto it{ data.find(key) };

      if (it != std::end(data))
         return it->second;

      // call ftor
      auto result = int(52);
      // add key + value to data
      data.emplace(std::make_pair(key, result));
      // call policy
      ClearPolicy::apply(data);

      return result;
   }

};

int real_function(std::string key)
{
   return 42;
}


TEST_CASE("intersection test", "[cache]")
{
   cache<
      std::string, 
      int, 
      policies::clear_after_N_entries<5> > c;

   REQUIRE(c("foo") == 42);
}