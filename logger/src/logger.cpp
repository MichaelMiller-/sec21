#include "log/logger.h"
#include "base_logger.h"
#include "console_logger.h"
#include "file_logger.h"
#include "registry.h"

#include <sec21/timestamp.h>

namespace sec21::log
{
   using registry_t = registry<console_logger, file_logger>;

   void log(entry const& e) { registry_t::instance().log(e); }

   void info(std::string_view message, std::optional<boost::uuids::uuid> element)
   {
      log(entry{Kind::Info, timestamp<long>::now(), getpid(), std::string{message}, element});
   }

   void error(entry const& e)
   {
      log(e);
   }

   void error(std::string_view message, std::optional<boost::uuids::uuid> element)
   {
      log(entry{Kind::Error, timestamp<long>::now(), getpid(), std::string{message}, element});
   }

   void debug(std::string_view message)
   {
      log(entry{Kind::Debug, timestamp<long>::now(), getpid(), std::string{message}});
   }

   void warn(entry const& e)
   {
      log(e);
   }

   void warn(std::string_view message)
   {
      log(entry{Kind::Warning, timestamp<long>::now(), getpid(), std::string{message}});
   }

} // namespace sec21::log