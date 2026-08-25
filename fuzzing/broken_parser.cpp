// this test demonstrates how fuzz-testing works

#include <iostream>
#include <string_view>
#include <iomanip>
#include <optional>

// for demo reasons
std::size_t iteration = 0;

// if the first character is a '{' then read up to the '}' and return the index of it
std::optional<std::size_t> parse(std::string_view sv) {

   ++iteration;

   if (sv.front() == '{') {
      std::cout << "iteration: " << iteration << ", input string: " << std::quoted(sv) << std::endl;

      std::size_t idx = 0;
      while (sv[idx] != '}') {
         ++idx;
      }
      return idx;
   }
   return std::nullopt;
}


extern "C" int LLVMFuzzerTestOneInput([[maybe_unused]] const uint8_t* data, [[maybe_unused]] std::size_t count)
{
   // this is a safe use of reinterpret cast because we are allowed to observe anything that is
   // trivial via a pointer to char* or unsigned char* according to the C++ standard
   parse(std::string_view{reinterpret_cast<const char *>(data), count});

   return 0;
}
