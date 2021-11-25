#include "console_logger.h"

#include <spdlog/spdlog.h>

using namespace sec21::log;

console_logger::console_logger()
{
#ifndef NDEBUG
   spdlog::set_level(spdlog::level::debug);
#endif
}
void console_logger::log(entry const& e)
{
   switch (e.kind) {
   case Kind::Info:
      spdlog::info("log: {} message: {}", e.process_id, e.message);
      break;
   case Kind::Error:
      spdlog::error("log: {} message: {}", e.process_id, e.message);
      break;
   case Kind::Warning:
      spdlog::warn("log: {} message: {}", e.process_id, e.message);
      break;
   case Kind::Debug:
#ifndef NDEBUG
      spdlog::debug("log: {} message: {}", e.process_id, e.message);
#endif
      break;
   case Kind::Metric:
      assert(0);
      break;
   default:
      assert(0);
   }
}
