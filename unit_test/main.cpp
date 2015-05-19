#include <gmock/gmock.h>

using namespace testing;

#include "test_array_ext.cpp"

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}