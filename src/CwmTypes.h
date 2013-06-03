enum CwmWindowType {
  CWM_WINDOW_TYPE_NORMAL   = (1<<0),
  CWM_WINDOW_TYPE_ICONISED = (1<<1),
  CWM_WINDOW_TYPE_ANY      = (0xFFFF),
};

class CwmCmdIcon;
class CwmColorMgr;
class CwmConfig;
class CwmCustomDeskIcon;
class CwmCustomDeskIconMgr;
class CwmCustomIcon;
class CwmDefIcon;
class CwmESideChild;
class CwmMask;
class CwmMaximizeChild;
class CwmMenuChild;
class CwmMinimizeChild;
class CwmNECornerChild;
class CwmNSideChild;
class CwmNWCornerChild;
class CwmScreenMgr;
class CwmScreen;
class CwmSECornerChild;
class CwmSSideChild;
class CwmSWCornerChild;
class CwmTitleChild;
class CwmToolBar;
class CwmToolBarClock;
class CwmToolBarCmd;
class CwmToolBarIcon;
class CwmToolBarIconDef;
class CwmToolBarIconArea;
class CwmToolBarMenu;
class CwmToolBarMgr;
class CwmWMWindowMgr;
class CwmWindowStack;
class CwmWSideChild;

typedef void (*CwmSigHandler)
                (int signum);
