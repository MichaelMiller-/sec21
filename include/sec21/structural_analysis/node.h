#pragma once
// sec21
#include "support.h"
#include "force.h"
// c++
#include <vector>
// boost
#include <boost/config.hpp>
#include <boost/optional/optional.hpp>
// #include <boost/graph/properties.hpp>
#include <boost/qvm/vec.hpp>

namespace sec21 {
namespace structural_analysis {
   //! \todo mircosoft.bond struct
   struct node
   {
      //! \todo node must have a name; find_by_name() -> node
      boost::qvm::vec<float, 2>     position;
      boost::optional<support>      support;
      std::vector<force>            forces;
   };

}
}
