#include <std_Xt.h>

#include <CXNamedEvent.h>
#include <CImageLib.h>
#include <CAlignType.h>
#include <CStrUtil.h>
#include <CFmtString.h>
#include <CGlob.h>
#include <CSideType.h>
#include <CXImage.h>

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

struct CDeletePointer {
  template<class T>
  void operator()(const T *p) const {
    delete p;
  }
};

#include "CwmLib.h"
#include "CwmToolBar.h"

#include "CwmDesk.h"
