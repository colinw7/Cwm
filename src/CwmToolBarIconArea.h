class CwmToolBarIcon;

class CwmToolBarIconArea {
 public:
  CwmToolBarIconArea(CwmToolBar &toolbar, int x, int width);
 ~CwmToolBarIconArea();

  void addIcon(CwmWMWindow *window);
  void removeIcon(CwmWMWindow *window);

  CwmToolBarIcon *getIcon(CwmWindow *xwindow);
  CwmToolBarIcon *getIcon(Window xwin);
  CwmToolBarIcon *getIcon(CwmWMWindow *window);

  void redraw();

  void sortIcons();
  void resizeIcons();

  int getX() const;
  int getWidth() const;

  CwmToolBar &getToolBar() const { return toolbar_; }
  CwmWindow *getXWindow() const { return xwindow_; }

  int getIconWidth() const { return icon_width_; }

  CXNamedEvent *getRestoreEvent();
  CXNamedEvent *getCloseEvent();

 private:
  typedef std::list<CwmToolBarIcon *> ToolBarIconList;

  CwmToolBar  &toolbar_;
  CwmWindow   *xwindow_;
  CwmGraphics *graphics_;
  int          icon_width_;
  int          def_icon_width_;

  CXNamedEvent *restore_event_;
  CXNamedEvent *close_event_;

  ToolBarIconList icons_;
};

class CwmToolBarIcon {
 public:
  class CwmToolIconCmp {
   public:
    int operator()(CwmToolBarIcon *icon1, CwmToolBarIcon *icon2);
  };

  CwmToolBarIcon(CwmToolBarIconArea &icon_area, CwmWMWindow *window);
 ~CwmToolBarIcon();

  void redraw();

  void moveResize(int x, int y, int width, int height, bool force=false);
  void move(int x, int y);

  bool onCurrentDesk();

  void processKeyPress(XKeyPressedEvent *event);

  void focusIn();
  void focusOut();

  void processMenu();

  CwmToolBar &getToolBar() const { return toolbar_; }
  CwmWMWindow *getWindow() const { return window_; }
  CwmWindow *getXWindow() const { return xwindow_; }

 private:
  void setImage();

  void createMenuDef();

  static void doubleClickProc(CwmWindow *xwindow, CwmData data, CwmData detail);
  static void idleProc(CwmWindow *xwindow, CwmData data, CwmData detail);

  static void restoreProc(CwmToolBarIcon *icon, CwmData data);
  static void closeProc(CwmToolBarIcon *icon, CwmData data);

  static void moveNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);
  static void iconiseNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);
  static void restoreNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);
  static void destroyNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);
  static void focusInNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);
  static void focusOutNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);

 private:
  CwmToolBarIconArea &icon_area_;
  CwmToolBar         &toolbar_;
  CwmWMWindow        *window_;
  CwmWindow          *xwindow_;
  CwmGraphics        *graphics_;
  CwmGraphics        *normal_graphics_;
  CwmGraphics        *focus_graphics_;
  int                 border_;
  bool                positioned_;
  CwmImage           *image_;
  CwmMenuDef         *menu_def_;
};
