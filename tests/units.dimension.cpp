#include <catch2/catch_test_macros.hpp>

#include <sec21/units/dimension.h>

TEST_CASE("dimension test", "[sec21][units]")
{
   using namespace sec21::units;

   struct d0 : base_dimension<0>
   {
   };
   struct d1 : base_dimension<1>
   {
   };
   struct d2 : base_dimension<2>
   {
   };

   SECTION("invert exponents")
   {
      STATIC_REQUIRE(std::is_same_v<invert_exponent_t<exponent<d0, 1>>, exponent<d0, -1>>);
      STATIC_REQUIRE(std::is_same_v<invert_exponent_t<exponent<d1, -2>>, exponent<d1, 2>>);
   }
   SECTION("add exponents")
   {
      using e1 = add_exponent_t<exponent<d0, 1>, exponent<d0, 3>>;
      STATIC_REQUIRE(std::is_same_v<e1, exponent<d0, 4>>);

      using e2 = add_exponent_t<exponent<d0, 1>, exponent<d0, -3>>;
      STATIC_REQUIRE(std::is_same_v<e2, exponent<d0, -2>>);
   }

   using dim0 = dimension<>;
   using dim1 = dimension<exponent<d0, 1>>;
   using dim2 = dimension<exponent<d0, 1>, exponent<d1, 2>>;
   using dim3 = dimension<exponent<d0, 1>, exponent<d1, 2>, exponent<d2, 3>>;
   using dim4 = dimension<exponent<d0, -1>, exponent<d1, -2>, exponent<d2, 3>>;

   SECTION("multiply dimensions")
   {
      using r1 = multiply_dimension_t<dim0, dim1>;
      STATIC_REQUIRE(std::is_same_v<r1, dimension<exponent<d0, 1>>>);

      using r2 = multiply_dimension_t<dim1, dim1>;
      STATIC_REQUIRE(std::is_same_v<r2, dimension<exponent<d0, 2>>>);

      using r3 = multiply_dimension_t<dim1, dim2>;
      STATIC_REQUIRE(std::is_same_v<r3, dimension<exponent<d0, 2>, exponent<d1, 2>>>);

      using r4 = multiply_dimension_t<dim2, dim3>;
      STATIC_REQUIRE(std::is_same_v<r4, dimension<exponent<d0, 2>, exponent<d1, 4>, exponent<d2, 3>>>);

      using r5 = multiply_dimension_t<dim1, dim4>;
      STATIC_REQUIRE(std::is_same_v<r5, dimension<exponent<d1, -2>, exponent<d2, 3>>>);
   }
   SECTION("invert dimensions")
   {
      using r1 = invert_dimension_t<dim1>;
      STATIC_REQUIRE(std::is_same_v<r1, dimension<exponent<d0, -1>>>);

      using r2 = invert_dimension_t<dim2>;
      STATIC_REQUIRE(std::is_same_v<r2, dimension<exponent<d0, -1>, exponent<d1, -2>>>);

      using r4 = invert_dimension_t<dim4>;
      STATIC_REQUIRE(std::is_same_v<r4, dimension<exponent<d0, 1>, exponent<d1, 2>, exponent<d2, -3>>>);
   }
}
