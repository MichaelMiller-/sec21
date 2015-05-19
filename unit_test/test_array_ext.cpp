#include "sec21/array_ext.h"

using namespace sec21::array_ext;

struct StdArrayAsVec2IntExample : public Test 
{
   typedef std::array<int, 2> type;

   const type a;
   const type b;
   const type a_minus_b;

   StdArrayAsVec2IntExample()
   : a{ {1, 1} }
   , b{ {5, 3} }
   , a_minus_b{ {-4, -2} }
   { }
};

struct StdArrayAsVec3FloatExample : public Test 
{
   typedef std::array<float, 3> type;

   const type a;
   const type b;
   const type a_minus_b;

   StdArrayAsVec3FloatExample()
   : a{ {0, 1, 0} }
   , b{ {1, 2, 0} }
   , a_minus_b{ {-1, -1, 0} }
   { }
};

struct StdArrayAsVec4FloatExample : public Test 
{
   typedef std::array<float, 4> type;

   const type a;
   const type b;
   const type a_minus_b;
   const type a_cross_b;

   StdArrayAsVec4FloatExample()
   : a{ {1, 0, 0, 3} }
   , b{ {0, 1, 0, 7} }
   , a_minus_b{ {1, -1, 0, 0} }
   , a_cross_b{ {0, 0, 1, 0} }
   { }
};

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

TEST(StdArrayAsRandomLengthIntVector, ValuePtrAtBeginingFromArray)
{
//   typedef std::array<int, boost::random::mt11213b()> type;

//   const auto ptr_a = &a[0];
//   ASSERT_THAT(value_ptr(a), Eq(ptr_a));
}

TEST_F(StdArrayAsVec2IntExample, ValuePtrAtBeginingFromArray)
{
   const auto ptr_a = &a[0];
   ASSERT_THAT(value_ptr(a), Eq(ptr_a));
}

TEST_F(StdArrayAsVec2IntExample, ValuePtrAtBeginingFromVectorOfArrays)
{
   auto tmp = std::vector<StdArrayAsVec2IntExample::type>();
   tmp.push_back(a);
   const auto ptr_tmp = &tmp[0][0];

   ASSERT_THAT(value_ptr(tmp), Eq(ptr_tmp));
}

TEST_F(StdArrayAsVec3FloatExample, ValuePtrAtBeginingFromArray)
{
   const auto ptr_a = &a[0];
   ASSERT_THAT(value_ptr(a), Eq(ptr_a));
}

TEST_F(StdArrayAsVec3FloatExample, ValuePtrAtBeginingFromVectorOfArrays)
{
   auto tmp = std::vector<StdArrayAsVec3FloatExample::type>();
   tmp.push_back(a);
   const auto ptr_tmp = &tmp[0][0];

   ASSERT_THAT(value_ptr(tmp), Eq(ptr_tmp));
}

TEST_F(StdArrayAsVec4FloatExample, ValuePtrAtBeginingFromArray)
{
   const auto ptr_a = &a[0];
   ASSERT_THAT(value_ptr(a), Eq(ptr_a));
}

TEST_F(StdArrayAsVec4FloatExample, ValuePtrAtBeginingFromVectorOfArrays)
{
   auto tmp = std::vector<StdArrayAsVec4FloatExample::type>();
   tmp.push_back(a);
   const auto ptr_tmp = &tmp[0][0];

   ASSERT_THAT(value_ptr(tmp), Eq(ptr_tmp));
}

using namespace sec21::array_ext::math;

TEST_F(StdArrayAsVec2IntExample, GeometricLess)
{
   ASSERT_THAT(geometric_less(a, b), Eq(true));
}

TEST_F(StdArrayAsVec2IntExample, MathMinus)
{
   ASSERT_THAT(a - b, Eq(a_minus_b));
}

TEST_F(StdArrayAsVec3FloatExample, GeometricLess)
{
   ASSERT_THAT(geometric_less(a, b), Eq(true));
}

TEST_F(StdArrayAsVec3FloatExample, MathMinus)
{
   ASSERT_THAT(a - b, Eq(a_minus_b));
}

TEST_F(StdArrayAsVec4FloatExample, GeometricLess)
{
   ASSERT_THAT(geometric_less(a, b), Eq(true));
}

TEST_F(StdArrayAsVec4FloatExample, MathMinus)
{
   ASSERT_THAT(a - b, Eq(a_minus_b));
}

TEST_F(StdArrayAsVec4FloatExample, MathCross)
{
   ASSERT_THAT(cross(a, b), Eq(a_cross_b));
}
