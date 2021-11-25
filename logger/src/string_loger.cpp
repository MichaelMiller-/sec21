#include "string_loger.h"

using namespace sec21::log;

void string_logger::log(entry const& e)
{
   f.insert(e.process_id, e);
}
