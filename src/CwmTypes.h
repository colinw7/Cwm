#ifndef CWM_TYPES_H
#define CWM_TYPES_H

enum CwmCursorType {
  CWM_CURSOR_NONE,
  CWM_CURSOR_TITLE,
  CWM_CURSOR_N,
  CWM_CURSOR_S,
  CWM_CURSOR_W,
  CWM_CURSOR_E,
  CWM_CURSOR_NW,
  CWM_CURSOR_NE,
  CWM_CURSOR_SW,
  CWM_CURSOR_SE,
  CWM_CURSOR_MOVE,
  CWM_CURSOR_MENU,
  CWM_CURSOR_BUSY,
  CWM_CURSOR_SELECT,
  CWM_CURSOR_QUERY,
  CWM_CURSOR_PLACE,
};

enum CwmWindowNotifyType {
  CWM_WINDOW_NOTIFY_CREATE,
  CWM_WINDOW_NOTIFY_DESTROY,
  CWM_WINDOW_NOTIFY_MOVE,
  CWM_WINDOW_NOTIFY_RESIZE,
  CWM_WINDOW_NOTIFY_ICONISE,
  CWM_WINDOW_NOTIFY_RESTORE,
  CWM_WINDOW_NOTIFY_FOCUS_IN,
  CWM_WINDOW_NOTIFY_FOCUS_OUT,
};

enum CwmXWindowCallType {
  CWM_CALLBACK_ENTER,
  CWM_CALLBACK_LEAVE,
  CWM_CALLBACK_IDLE,
  CWM_CALLBACK_BUTTON_PRESS,
  CWM_CALLBACK_BUTTON_RELEASE,
  CWM_CALLBACK_SINGLE_CLICK_1,
  CWM_CALLBACK_SINGLE_CLICK_2,
  CWM_CALLBACK_SINGLE_CLICK_3,
  CWM_CALLBACK_DOUBLE_CLICK_1,
  CWM_CALLBACK_DOUBLE_CLICK_2,
  CWM_CALLBACK_DOUBLE_CLICK_3,
};

enum CwmButtonType {
  CWM_LEFT_BUTTON   = (1<<0),
  CWM_MIDDLE_BUTTON = (1<<1),
  CWM_RIGHT_BUTTON  = (1<<2),
};

enum CwmResizeType {
  CWM_RESIZE_FREE   = 0,
  CWM_RESIZE_LEFT   = (1<<0),
  CWM_RESIZE_RIGHT  = (1<<1),
  CWM_RESIZE_TOP    = (1<<2),
  CWM_RESIZE_BOTTOM = (1<<3),

  CWM_RESIZE_TOP_LEFT     = (CWM_RESIZE_TOP    | CWM_RESIZE_LEFT ),
  CWM_RESIZE_TOP_RIGHT    = (CWM_RESIZE_TOP    | CWM_RESIZE_RIGHT),
  CWM_RESIZE_BOTTOM_LEFT  = (CWM_RESIZE_BOTTOM | CWM_RESIZE_LEFT ),
  CWM_RESIZE_BOTTOM_RIGHT = (CWM_RESIZE_BOTTOM | CWM_RESIZE_RIGHT),
};

enum CwmFocusHighlightType {
  CWM_FOCUS_HIGHLIGHT_NONE,
  CWM_FOCUS_HIGHLIGHT_TITLE,
  CWM_FOCUS_HIGHLIGHT_ALL,
};

enum CwmTitlePatternType {
  CWM_TITLE_PATTERN_SOLID,
  CWM_TITLE_PATTERN_GRADIENT
};

enum CwmDeskMgrNotifyType {
  CWM_DESK_MGR_NOTIFY_CHANGE_END,
  CWM_DESK_MGR_NOTIFY_CHANGE_START,
};

enum CwmWindowType {
  CWM_WINDOW_TYPE_NORMAL   = (1<<0),
  CWM_WINDOW_TYPE_ICONISED = (1<<1),
  CWM_WINDOW_TYPE_ANY      = (0xFFFF),
};

class CwmCirculateWindowStack;
class CwmCmdIcon;
class CwmColor;
class CwmColormapMgr;
class CwmColorMgr;
class CwmConfig;
class CwmContainerChild;
class CwmCursorMgr;
class CwmCustomDeskIcon;
class CwmCustomDeskIconMgr;
class CwmCustomHint;
class CwmCustomIcon;
class CwmDebug;
class CwmDecoration;
class CwmDefIcon;
class CwmDesk;
class CwmDeskIcon;
class CwmDeskIconMgr;
class CwmDeskMgr;
class CwmESideChild;
class CwmFocusMgr;
class CwmFont;
class CwmFontMgr;
class CwmFrameWindow;
class CwmFunctionDef;
class CwmGraphics;
class CwmGrid;
class CwmHints;
class CwmIdle;
class CwmImage;
class CwmImageSpec;
class CwmInfo;
class CwmLog;
class CwmMask;
class CwmMaximizeChild;
class CwmMenu;
class CwmMenuChild;
class CwmMenuDef;
class CwmMenuEntry;
class CwmMenuItem;
class CwmMinimizeChild;
class CwmMoveFeedback;
class CwmNamedColor;
class CwmNECornerChild;
class CwmNSideChild;
class CwmNWCornerChild;
class CwmResizeFeedback;
class CwmRootImage;
class CwmScreen;
class CwmScreenMgr;
class CwmSECornerChild;
class CwmSSideChild;
class CwmSWCornerChild;
class CwmTabWindow;
class CwmTitleChild;
class CwmToolBar;
class CwmToolBarClock;
class CwmToolBarCmd;
class CwmToolBarIcon;
class CwmToolBarIconArea;
class CwmToolBarIconDef;
class CwmToolBarMenu;
class CwmToolBarMgr;
class CwmUnixCmd;
class CwmUserWindow;
class CwmWindow;
class CwmWindowChild;
class CwmWindowGroup;
class CwmWindowImageList;
class CwmWindowMenu;
class CwmWindowStack;
class CwmWMWindow;
class CwmWMWindowMgr;
class CwmWSideChild;
class CwmXEvent;
class CwmXPixmap;

class CXNamedEvent;
class CXScreen;

typedef void *CwmData;

typedef void (*CwmXWindowCallProc)
                (CwmWindow *xwindow, CwmData data, CwmData detail);
typedef void (*CwmRootProc)
                (CwmScreen &screen, CwmData data);
typedef void (*CwmWindowProc)
                (CwmWMWindow *window, CwmData data);
typedef void (*CwmIconProc)
                (CwmDeskIcon *icons, CwmData data);
typedef void (*CwmXWindowProc)
                (CwmWindow *xwindow, CwmData data);
typedef void (*CwmMenuProc)
                (CwmData data1, CwmData data2, CwmData data3,
                 CwmData data4, CwmData data5, CwmData data6);
typedef void (*CwmSigHandler)
                (int signum);

#include <map>
#include <vector>
#include <list>

typedef std::vector<std::string> StringVectorT;

#endif
