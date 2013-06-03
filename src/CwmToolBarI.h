#include <std_Xt.h>

#include "CGlob.h"
#include "CXNamedEvent.h"
#include "CFmtString.h"
#include "CSideType.h"
#include "CXImage.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>

typedef std::vector<std::string>  StringVectorT;

using std::string;
using std::map;
using std::vector;
using std::list;

template<class T>
class CDeleteMapSecond {
 public:
  void operator()(typename T::value_type &p) const {
    delete p.second;
  }
};

#include "CwmLib.h"
#include "CwmDesk.h"

#include "CwmToolBarLib.h"
