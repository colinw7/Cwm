#include <std_Xt.h>

#include <CXNamedEvent.h>
#include <CImageLib.h>
#include <CAlignType.h>
#include <CGlob.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CFmtString.h>
#include <CRGBA.h>
#include <CSideType.h>
#include <CXImage.h>
#include <CXMachine.h>

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

template<class T>
class CDeleteMapSecond {
 public:
  void operator()(typename T::value_type &p) const {
    delete p.second;
  }
};

#include <CwmLib.h>

#include <CwmTypes.h>

#include <CwmCmdIcon.h>
#include <CwmColormapMgr.h>
#include <CwmConfig.h>
#include <CwmCursorMgr.h>
#include <CwmCustomIcon.h>
#include <CwmDebug.h>
#include <CwmFontMgr.h>
#include <CwmInfo.h>
#include <CwmLog.h>
#include <CwmMailMgr.h>
#include <CwmMessage.h>
#include <CwmMoveFeedback.h>
#include <CwmPaste.h>
#include <CwmResizeFeedback.h>
#include <CwmScreenMenu.h>
#include <CwmTabWindow.h>

#include <CwmWindowImageList.h>
#include <CwmWindowMenu.h>

#include <CwmDeskLib.h>
#include <CwmDecorationLib.h>
#include <CwmToolBarLib.h>
