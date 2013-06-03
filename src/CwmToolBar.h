class CwmToolBar;
class CwmToolBarIconArea;
class CwmToolBarClock;
class CwmToolBarMenu;
class CwmToolBarCmd;
class CwmToolBarIcon;
class CwmToolBarIconDef;

class CwmToolBarMgr {
 private:
  CwmDesk    &desk_;
  CwmToolBar *toolbar_;

 public:
  CwmToolBarMgr(CwmDesk &desk);
 ~CwmToolBarMgr();

  CwmDesk    &getDesk   () const { return desk_;    }
  CwmToolBar *getToolBar() const { return toolbar_; }

  void enable();
  void disable();

 private:
  static void deskChangeStartNotifyProc(CwmDeskMgr *desk_mgr,
                                        CwmDeskMgrNotifyType type,
                                        CwmData data);
  static void deskChangeEndNotifyProc(CwmDeskMgr *desk_mgr,
                                      CwmDeskMgrNotifyType type,
                                      CwmData data);
};

class CwmToolBar {
 private:
  typedef list<CwmToolBarCmd *> ToolBarCmdList;

  CwmToolBarMgr      &mgr_;
  int                 border_;
  CwmWindow          *xwindow_;
  CwmGraphics        *graphics_;
  CwmToolBarIconArea *icon_area_;
  CwmToolBarClock    *clock_;
  CwmToolBarMenu     *menu_;
  bool                add_icons_;
  bool                add_windows_;
  bool                show_clock_;
  std::string         time_format_;
  int                 icon_area_x1_;
  int                 icon_area_x2_;
  ToolBarCmdList      cmd_icons_;

 public:
  CwmToolBar(CwmToolBarMgr &mgr);
 ~CwmToolBar();

  void redraw();

  bool isWindow(CwmWindow *xwindow);
  bool isWindow(Window xwin);

  bool isMenuWindow(CwmWindow *xwindow);
  bool isMenuWindow(Window xwin);

  bool isToolIcon(CwmWMWindow *window);
  bool isToolIcon(CwmWindow *xwindow);
  bool isToolIcon(Window xwin);

  CwmToolBarIcon *getToolIcon(CwmWMWindow *window);
  CwmToolBarIcon *getToolIcon(CwmWindow *xwindow);
  CwmToolBarIcon *getToolIcon(Window xwin);

  void processMenu();
  void processMenuMenu();

  void map();
  void unmap();

  void enable();
  void disable();

  int getWidth() const;
  int getHeight() const;

  void addCmdIcon(CwmToolBarIconDef *icon_def);

  void addIcon(CwmWMWindow *window);
  void removeIcon(CwmWMWindow *window);

  CwmWindow *getXWindow() const { return xwindow_; }

  int         getBorder    () const { return border_     ; }
  bool        getAddIcons  () const { return add_icons_  ; }
  bool        getAddWindows() const { return add_windows_; }
  std::string getTimeFormat() const { return time_format_; }

  void setAddIcons  (bool flag) { add_icons_   = flag; }
  void setAddWindows(bool flag) { add_windows_ = flag; }

  CwmScreen &getScreen() const;

  static void raiseWindowProc(CwmWMWindow *window, CwmData data);

  bool processExpose(CwmWindow *xwindow);

  bool processButtonPress(Window xwin, int button);

 private:
  CwmMenuDef *buildMenu(CwmCirculateWindowStack &window_stack);
  CwmMenuDef *buildMenuCascade(CwmWindowGroup &window_group);

  static void createNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                               CwmData data);
  static void iconiseNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                CwmData data);
};
