#include "file_logger.h"

#include <sec21/file_loader.h>

#include <nlohmann/json.hpp>

#ifdef __cpp_lib_format
   #include <format>
   using std::format;
#else
   #include <fmt/format.h>
   using fmt::format;
#endif
#include <filesystem>

namespace sec21::log
{
   auto logfilename(int id) noexcept
   {
      return std::filesystem::temp_directory_path() / format("sec21_{}.log", id);
   }
   auto logfilename(entry const& e) noexcept { return logfilename(e.process_id); }

   std::vector<entry> messages_from_process(int pid)
   {
      return read_from_json<std::vector<entry>>(logfilename(pid));
   }

   void file_logger::log(entry const& e)
   {
      auto log_entries = messages_from_process(e.process_id);
      log_entries.push_back(e);

      std::ofstream ofs{logfilename(e)};
      nlohmann::json j = log_entries;
      ofs << j;
   }
}
