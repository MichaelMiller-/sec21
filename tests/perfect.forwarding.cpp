﻿#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes” 
// https://www.youtube.com/watch?v=PNRju6_yn3o
// std::string_view ctor -> more malloc's   video: t > 50:00min

#include <type_traits>
#include <string>

class Customer
{
   std::string first;
   std::string last;
   int id;
public:
   template <typename T, typename U = std::string
#ifndef __cpp_concepts
   , typename = std::enable_if_t<std::is_convertible_v<T, std::string>>>
#else   
   > requires std::is_constructible_v<std::string, T>
#endif
   Customer(T&& first_, U&& last_ = "", int id_ = 0) 
      : first{ std::forward<T>(first_) }
      , last{ std::forward<U>(last_) }
      , id{ id_ }
   { }

};

class VIP : public Customer
{
   using Customer::Customer;
};


TEST_CASE("perfect forwarding", "[compilation-only]") 
{
   using namespace std::literals::string_literals;

   Customer foo_bar("foo"s, "bar"s, 42);             // 2 malloc ( 2 creation(cr) )
   {
      std::string s{ "baz" };                       // 1 malloc                       <--------
      Customer baz_bar(s, "bar"s, 42);              // 2 malloc ( 1 cp + 2 cr )               |
      Customer baz_baz(std::move(s), "baz"s, 42);   // 1 malloc ( 1 cr + 1 mv )  + (1 malloc) |
      Customer only_foo("foo"s);

      Customer copy_foo_bar( foo_bar );
      const Customer const_c( "c"s );
      Customer copy_c( const_c );

      Customer assign_c = "c"s;
   }
   {
      std::string s{ "baz" };
      VIP baz_bar( s, "bar"s, 42 );
      VIP baz_baz( std::move(s), "baz"s, 42 );
      VIP only_foo( "foo"s );

      const VIP const_c( "c"s );
      VIP copy_c( const_c );

      VIP assign_c = "c"s;
   
      Customer copy_vip( baz_bar );
   }
   SUCCEED("Nothing to test. Compiletests");
}
