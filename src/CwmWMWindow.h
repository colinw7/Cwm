#ifndef CWM_WM_WINDOW_H
#define CWM_WM_WINDOW_H

enum CwmWindowArea {
  CWM_WINDOW_MENU_AREA     = (1<<0),
  CWM_WINDOW_TITLE_AREA    = (1<<1),
  CWM_WINDOW_MAXIMIZE_AREA = (1<<2),
  CWM_WINDOW_MINIMIZE_AREA = (1<<3),
  CWM_WINDOW_BORDER_AREA   = (1<<4),
  CWM_WINDOW_USER_AREA     = (1<<5),
  CWM_WINDOW_ANY_AREA      = (0xFFFF),
};

enum CwmWindowState {
  CWM_WINDOW_STATE_NORMAL,
  CWM_WINDOW_STATE_ICONISED,
  CWM_WINDOW_STATE_MINIMIZED,
  CWM_WINDOW_STATE_MAXIMIZED,
};

struct CwmWindowEventFunction;

typedef void (*CwmWindowNotifyProc)
                (CwmWMWindow *window, CwmWindowNotifyType type, CwmData data);

class CwmWindowNotifyData {
 private:
  CwmWindowNotifyType type;
  CwmWindowNotifyProc proc;
  CwmData             data;
  bool                called;

 public:
  CwmWindowNotifyData(CwmWindowNotifyType type1, CwmWindowNotifyProc proc1,
                      CwmData data1) : type(type1), proc(proc1), data(data1) {
  }

 ~CwmWindowNotifyData() { }

  bool isType(CwmWindowNotifyType type1) const {
    return (type == type1);
  }

  void call(CwmWMWindow *window) {
    proc(window, type, data);
  }

  bool match(CwmWindowNotifyType type1, CwmWindowNotifyProc proc1,
             CwmData data1) {
    return ((type == type1) && (proc == proc1) && (data == data1));
  }

  void setCalled(bool flag) { called = flag; }
  bool getCalled() const { return called; }
};

//-------------------------------

#define CwmWindowEventFunctionMgrInst CwmWindowEventFunctionMgr::getInstance()

class CwmWindowEventFunctionMgr {
  typedef list<CwmWindowEventFunction *> WindowEventFunctionList;

 private:
  WindowEventFunctionList event_functions_;

 public:
  static CwmWindowEventFunctionMgr *getInstance();

  CwmWindowEventFunctionMgr();

  void add(CwmWindowEventFunction *event_function);
  void remove(CwmWindowEventFunction *event_function);

  void addEventFunction(CwmWMWindow *window, int area,
                        CXNamedEvent *event,
                        CwmFunctionDef *function, CwmData data);

  void addPatternEventFunction(const std::string &pattern, int area,
                               CXNamedEvent *event,
                               CwmFunctionDef *function, CwmData data);

  void processEventFunction(CwmWMWindow *window, int area, XEvent *event);

  void removeEventFunctions(CwmWMWindow *window);

  void removeAllEventFunctions();

  void grabEventKeys(CwmWMWindow *window);
  void ungrabEventKeys(CwmWMWindow *window);
};

//-------------------------------

#define CwmWindowGlobalNotifyMgrInst CwmWindowGlobalNotifyMgr::getInstance()

class CwmWindowGlobalNotifyMgr {
  typedef list<CwmWindowNotifyData *> WindowNotifyDataList;

 private:
  WindowNotifyDataList notify_procs_;

 public:
  static CwmWindowGlobalNotifyMgr *getInstance();

  CwmWindowGlobalNotifyMgr();
 ~CwmWindowGlobalNotifyMgr();

  void add(CwmWindowNotifyData *notify_proc);
  void remove(CwmWindowNotifyData *notify_proc);

  void clear();

  void addProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc,
               CwmData data);
  void removeProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc,
                  CwmData data);

  void callProcs(CwmWMWindow *window, CwmWindowNotifyType type);
};

//-------------------------------

class CwmWMWindowMgr {
 private:
  typedef list<CwmWMWindow *> WindowList;

  WindowList window_list_;

 public:
  CwmWMWindowMgr();
 ~CwmWMWindowMgr();

  void term();

  CwmWMWindow *addWindow(CwmScreen &screen, Window xwin);

  void removeWindow(CwmWMWindow *window);

  CwmWMWindow *lookupFromWindow(Window xwin);
  CwmWMWindow *lookupFromUserWindow(Window xwin);
};

class CwmWMWindow {
 public:
  typedef list<CwmWMWindow *>         WMWindowList;
  typedef list<CwmWindowNotifyData *> WindowNotifyDataList;

 private:
  CwmScreen            &screen_;

  Colormap              cmap_;
  CwmGraphics          *normal_graphics_;
  CwmGraphics          *focus_graphics_;
  CwmGraphics          *graphics_;
  CwmUserWindow        *user_;
  CwmFrameWindow       *frame_;
  CwmDecoration        *decoration_;
  CwmWindowState        state_;
  int                   normal_x_;
  int                   normal_y_;
  int                   normal_width_;
  int                   normal_height_;
  CwmHints             *hints_;
  CwmWMWindow          *parent_;
  bool                  focus_auto_raise_;
  bool                  circulate_skip_;
  bool                  toolbar_skip_;
  bool                  rolled_up_;
  CwmWindowMenu        *window_menu_;
  WMWindowList          children_;
  CwmWindowImageList   *image_list_;
  WindowNotifyDataList  notify_procs_;

 public:
  CwmWMWindow(CwmScreen &screen, Window xwin);
 ~CwmWMWindow();

  CwmScreen &getScreen() const { return screen_; }

  Colormap getColormap() const { return cmap_; }

  void setColormap(Colormap cmap) { cmap_ = cmap; }

  CwmFont *getFont() const;

  CwmColor *getColor() const;

  GC getGC() const;

  CwmGraphics *getGraphics() const { return normal_graphics_; }

  CwmWMWindow *getParent() const { return parent_; }

  int getFontHeight() const;

  const WMWindowList &getChildren() const { return children_; }

  bool getCirculateSkip () const { return circulate_skip_  ; }
  bool getToolBarSkip   () const { return toolbar_skip_    ; }
  bool getFocusAutoRaise() const { return focus_auto_raise_; }
  bool getRolledUp      () const { return rolled_up_       ; }

  bool isNormal   () const { return state_ == CWM_WINDOW_STATE_NORMAL   ; }
  bool isIconised () const { return state_ == CWM_WINDOW_STATE_ICONISED ; }
  bool isMaximized() const { return state_ == CWM_WINDOW_STATE_MAXIMIZED; }
  bool isMinimized() const { return state_ == CWM_WINDOW_STATE_MINIMIZED; }

  void setNormal   () { state_ = CWM_WINDOW_STATE_NORMAL   ; }
  void setIconised () { state_ = CWM_WINDOW_STATE_ICONISED ; }
  void setMaximised() { state_ = CWM_WINDOW_STATE_MAXIMIZED; }
  void setMinimised() { state_ = CWM_WINDOW_STATE_MINIMIZED; }

  CwmHints *getHints() const { return hints_; }

  std::string getNameHint() const { return hints_->getName(); }

  std::string getIconNameHint() const { return hints_->getIconName(); }

  int    getXHint         () { return hints_->getX(); }
  int    getYHint         () { return hints_->getY(); }
  int    getWidthIncHint  () { return hints_->getWidthInc(); }
  int    getHeightIncHint () { return hints_->getHeightInc(); }
  int    getMinWidthHint  () { return hints_->getMinWidth(); }
  int    getMinHeightHint () { return hints_->getMinHeight(); }
  int    getMaxWidthHint  () { return hints_->getMaxWidth(); }
  int    getMaxHeightHint () { return hints_->getMaxHeight(); }
  int    getBaseWidthHint () { return hints_->getBaseWidth(); }
  int    getBaseHeightHint() { return hints_->getBaseHeight(); }
  double getMinAspectHint () { return hints_->getMinAspect(); }
  double getMaxAspectHint () { return hints_->getMaxAspect(); }
  int    getWinGravityHint() { return hints_->getWinGravity(); }

  bool   getInputHint       () { return hints_->getInput(); }
  int    getInitialStateHint() { return hints_->getInitialState(); }
  Window getWMIconWindowHint();
  Pixmap getIconPixmapHint  () { return hints_->getIconPixmap(); }
  Pixmap getIconMaskHint    () { return hints_->getIconMask(); }
  int    getIconDepthHint   () { return hints_->getIconDepth(); }
  int    getIconXHint       () { return hints_->getIconX(); }
  int    getIconYHint       () { return hints_->getIconY(); }
  Window getWindowGroupHint () { return hints_->getWindowGroup(); }

  Window getTransientForHint() { return hints_->getTransientFor(); }

  std::string getResNameHint () const { return hints_->getResName(); }
  std::string getResClassHint() const { return hints_->getResClass(); }

  std::string getClientMachineHint() const { return hints_->getClientMachine(); }

  StringVectorT getCommandArgVHint() const { return hints_->getCommandArgV(); }

  Window getColormapWindowHint() const { return hints_->getColormapWindow(); }

  bool getTakeFocusHint         () { return hints_->getTakeFocus(); }
  bool getSaveYourselfHint      () { return hints_->getSaveYourself(); }
  bool getNotifyDeleteWindowHint() { return hints_->getDeleteWindow(); }

  int getMwmFunctionsHint  () const { return hints_->getMwmFunctions(); }
  int getMwmDecorationsHint() const { return hints_->getMwmDecorations(); }
  int getMwmInputModeHint  () const { return hints_->getMwmInputMode(); }
  int getMwmStatusHint     () const { return hints_->getMwmStatus(); }

  bool getMoveFunctionHint() {
    return hints_->getMoveFunction();
  }

  bool getResizeFunctionHint() {
    return hints_->getResizeFunction();
  }

  bool getIconiseFunctionHint() {
    return hints_->getIconiseFunction();
  }

  bool getMaximizeFunctionHint() {
    return hints_->getMaximizeFunction();
  }

  bool getMinimizeFunctionHint() {
    return hints_->getMinimizeFunction();
  }

  bool getRestoreFunctionHint() {
    return hints_->getRestoreFunction();
  }

  bool getRaiseFunctionHint() {
    return true;
  }

  bool getLowerFunctionHint() {
    return true;
  }

  bool getRollUpFunctionHint() {
    return true;
  }

  bool getRollDownFunctionHint() {
    return true;
  }

  bool getCloseFunctionHint() {
    return hints_->getCloseFunction();
  }

  bool getTitleDecorationHint() {
    return hints_->getTitleDecoration();
  }

  bool getBorderDecorationHint() {
    return hints_->getBorderDecoration();
  }

  bool getResizeHandleDecorationHint() {
    return hints_->getResizeHandleDecoration();
  }

  bool getMaximizeDecorationHint() {
    return hints_->getMaximizeDecoration();
  }

  bool getMinimizeDecorationHint() {
    return hints_->getMinimizeDecoration();
  }

  bool getMenuDecorationHint() {
    return hints_->getMenuDecoration();
  }

  bool getCloseDecorationHint() {
    return hints_->getCloseDecoration();
  }

  CwmUserWindow  *getUser () const { return user_ ; }
  CwmFrameWindow *getFrame() const { return frame_; }

  CwmWindow *getFrameWindow() const;

  CwmWindow *getUserWindow() const;

  Window getXWin() const;

  CwmDecoration *getDecoration() const;

  void remove();

  bool isFrame(CwmWindow *xwindow);
  bool isUser (CwmWindow *xwindow);

  std::string getName();
  std::string getResName();
  std::string getResClass();
  std::string getIconName();

  void addEventFunction(int area, CXNamedEvent *event,
                        CwmFunctionDef *function, CwmData data);

  void print();

  int      getDeskNum();
  CwmDesk *getDesk();
  bool     onCurrentDesk();

  void raise();
  void lower();
  void iconise();
  void minimize();
  void maximize();
  void restoreSize();
  void restore();
  void close();
  void closeInvalid();

  void focusIn();
  void focusOut();

  void rollUp();
  void rollDown();

  void focus();

  void map();
  void unmap();

  void decorate();

  void redecorate();

  void redraw();

  void move();
  void move(int x, int y);
  void moveResize(int x, int y, int width, int height);
  void resize();
  void resizeUser(int width, int height);

  void setStateProperty();
  bool getStateProperty();

  bool isWMStateIconic();

  void setDeskProperty();
  int  getDeskProperty();

  CwmImage *getImage(int width, int height);
  CwmImage *getImage();

  void processWindowMenu(void);

  void processWindowMenuAtPosition(int x, int y);

  static void processEvents();
  static void processWindowMenu(CwmWMWindow *window, CwmData data);

  void addNotifyProc(CwmWindowNotifyType type,
                     CwmWindowNotifyProc proc, CwmData data);
  void removeNotifyProc(CwmWindowNotifyType type,
                        CwmWindowNotifyProc proc, CwmData data);

 private:
  static void processEvent(XEvent *event);

  static void sendToUser(XEvent *event);

  static void processButtonPress(XButtonPressedEvent *event);
  static void processButtonRelease(XButtonReleasedEvent *event);
  static void processKeyPress(XKeyPressedEvent *event);
  static void processKeyRelease(XKeyReleasedEvent *event);
  static void processEnterNotify(XEnterWindowEvent *event);
  static void processLeaveNotify(XLeaveWindowEvent *event);
  static void processClientMessage(XClientMessageEvent *event);

  void place();
  void initUserWindow();
  void addEvents();

  void notifyDeleteWindow();

  void callNotifyProcs(CwmWindowNotifyType type);

  static void processPendingEvents();
};

#endif
