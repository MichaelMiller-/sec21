#pragma once

#include <algorithm>
#include <cstring>
#include <string_view>
#include <tuple>

using fuzz_span_t = std::basic_string_view<std::uint8_t>;

template <typename T>
[[nodiscard]] std::pair<T, fuzz_span_t> make_fuzzed_object(fuzz_span_t input)
{
   T result{};

   auto bytes_used = std::min(input.size(), sizeof(T));

   std::memcpy(&result, input.data(), bytes_used);

   input.remove_prefix(bytes_used);

   return {result, input};
}

template <typename... Ts>
[[nodiscard]] std::pair<std::tuple<Ts...>, fuzz_span_t> make_fuzzed_objects(fuzz_span_t input)
{
   return make_fuzzed_object<std::tuple<Ts...>>(input);
}
