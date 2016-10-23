#include <CTriplet.h>

#define CwmMachineInst CwmMachine::getInstance()

class CXAtom;

class CwmMachine {
 public:
  static CwmMachine *getInstance();

 ~CwmMachine();

  bool init(const std::string &display_name);
  void term();

  Colormap getColormap(int num);
  int      getWidth(int num);
  int      getHeight(int num);
  int      getDepth(int num);
  Pixel    getBlackPixel(int num);
  Pixel    getWhitePixel(int num);

  Window getRoot();
  Window getRoot(int num);

  Window getWindowRoot(Window xwin);
  Window getWindowTop(Window xwin);
  Window getWindowParent(Window xwin);
  bool   getWindowChildren(Window xwin, Window **children, int *num_children);

  void synchronize();

  void grabServer();
  void ungrabServer();

  bool eventPending();
  void nextEvent(XEvent *event);

  std::string getDisplayName() const;
  int         getNumScreens() const;

  CwmScreen &getScreen(int screen_num);
  CwmScreen &getWindowScreen(Window xwin);

  CwmWMWindow *addWMWindow(CwmScreen &screen, Window xwin);
  void         removeWMWindow(CwmWMWindow *window);

  CwmWMWindow *getWindowWMWindow(CwmWindow *xwindow);
  CwmWMWindow *getWindowWMWindow(Window xwin);
  CwmWMWindow *getUserWindowWMWindow(CwmWindow *xwindow);
  CwmWMWindow *getUserWindowWMWindow(Window xwin);

  void       addXWindow(CwmWindow *xwindow);
  void       removeXWindow(CwmWindow *xwindow);
  CwmWindow *getWindowCwmXWindow(Window xwin);

  bool controlScreens();

  bool isValidWindow(Window xwin);

  Colormap getWindowColormap(Window xwin);

  int  getWindowEventMask(Window xwin);
  bool getWindowViewable(Window xwin);

  void getWindowGeometry(Window xwin, int *x, int *y, int *width, int *height, int *border);

  int getMulticlickTime();

  bool isFocusWindow(CwmWMWindow *window);
  void setFocusWindow(CwmWMWindow *window);
  void resetFocusWindow();

  void focusNext(CwmScreen &screen);
  void focusPrev(CwmScreen &screen);

  bool isFocus(CwmWMWindow *window);
  void focusEnter(CwmWMWindow *window);
  void focusLeave(CwmWMWindow *window);

  void setInputFocus(Window xwin);
  void resetInputFocus();

  bool translateCoords(Window src_w, Window dest_w, int src_x, int src_y,
                       int *dest_x, int *dest_y, Window *child);
  bool queryPointer(Window xwin, int *x, int *y);

  void destroyWindow(Window xwin);

  void mapWindow(Window xwin);
  void mapWindowRaised(Window xwin);
  void mapWindowChildren(Window xwin);
  void unmapWindow(Window xwin);

  void reparentWindow(Window xwin, Window parent_xwin, int x, int y);

  void lowerWindow(Window xwin);
  void raiseWindow(Window xwin);

  void moveWindow(Window xwin, int x, int y);
  void resizeWindow(Window xwin, int width, int height);
  void moveResizeWindow(Window xwin, int x, int y, int width, int height);

  void clearWindow(Window xwin);

  bool isWMWindow(Window xwin);

  void setWMStateNormal(Window xwin);
  void setWMStateIconic(Window xwin, Window icon_xwin);

  bool isWMStateNormal(Window xwin);
  bool isWMStateIconic(Window xwin);

  void configureWindow(Window xwin, uint mask, XWindowChanges *xwc);

  void maskEvent(uint mask, XEvent *event);

  void allowEvents(int mode);

  void restackWindows(Window *windows, int num_windows);

  void shapeCombineMask(Window xwin, Window mask, int x, int y, int op);

  bool grabPointer(Window xwin, uint event_mask, Cursor cursor);
  void ungrabPointer();

  bool grabKeyboard(Window xwin);
  void ungrabKeyboard();

  void grabKey(Window xwin, int keycode, int state);
  void ungrabKey(Window xwin, int keycode, int state);

  void grabButton(Window xwin, int button, int modifiers, int event_mask, Cursor cursor);
  void ungrabButton(Window xwin, int button, int modifiers);

  Window createWindow(int x, int y, int width, int height, int border,
                      uint attr_mask, XSetWindowAttributes *attr);
  Window createWindow(Window parent_xwin, int x, int y, int width, int height,
                      int border, uint attr_mask, XSetWindowAttributes *attr);

  Pixmap createPixmap(Window parent_xwin, uint width, uint height, uint depth);
  Pixmap createPixmap(uint width, uint height, uint depth);
  void   freePixmap(Pixmap xpixmap);

  void setKeyAutoRepeatRate(uint timeout, uint interval);
  void getKeyAutoRepeatRate(uint *timeout, uint *interval);

  void installColormap(Colormap cmap);

  void changeWindowAtributes(Window xwin, uint attr_mask, XSetWindowAttributes *attr);

  void killClient(Window xwin);

  void setIntegerProperty(Window xwin, const CXAtom &name, int value);
  void setStringProperty(Window xwin, const CXAtom &name, const std::string &value);
  void setWindowProperty(const CXAtom &name, Window value);
  void setWindowProperty(Window xwin, const CXAtom &name, Window value);
  void setWindowArrayProperty(Window xwin, const CXAtom &name, Window *xwins, int num_xwins);
  void setAtomArrayProperty(Window xwin, const CXAtom &name, const CXAtom **atoms, int num_atoms);
  void setStringListProperty(Window xwin, const CXAtom &name, char **strs, int num_strs);
  bool getIntegerProperty(Window xwin, const CXAtom &name, int *value);
  bool getIntegerProperty(const CXAtom &name, int *value);
  bool getStringProperty(Window xwin, const CXAtom &name, std::string &value);
  bool getStringProperty(const CXAtom &name, std::string &value);
  bool getPixmapProperty(Window xwin, const CXAtom &name, Pixmap *value);
  void deleteProperty(Window xwin, const CXAtom &name);

  void   getWMName(Window xwin, std::string &name);
  void   getWMIconName(Window xwin, std::string &name);
  void   getWMNormalHints(Window xwin, XSizeHints **size_hints, int *supplied);
  Window getWMIconWindowHint(Window xwin);
  void   getWMHints(Window xwin, XWMHints **wm_hints);
  void   getWMTransientFor(Window xwin, Window *transient_xwin);
  void   getWMClassHint(Window xwin, XClassHint **class_hint);
  void   getWMClientMachine(Window xwin, std::string &client_machine);
  void   getWMCommand(Window xwin, int *argc, char ***argv);
  void   getWMColormapWindows(Window xwin, Window **cmap_windows, int *num_cmap_windows);
  void   getWMProtocols(Window xwin, const CXAtom ***protocols, int *num_protocols);
  bool   getWMMwmHints(Window xwin, MotifWmHints &mwm_hints);

  const CXAtom &getWMProtocolsAtom();
  const CXAtom &getWMDeleteWindowAtom();
  const CXAtom &getXSetRootIdAtom();
  const CXAtom &getCwmDesktopAtom();

  bool isWMChangeStateAtom(const CXAtom &atom);
  bool isWMClassAtom(const CXAtom &atom);
  bool isWMClientMachineAtom(const CXAtom &atom);
  bool isWMCommandAtom(const CXAtom &atom);
  bool isWMDeleteWindowAtom(const CXAtom &atom);
  bool isWMHintsAtom(const CXAtom &atom);
  bool isWMIconNameAtom(const CXAtom &atom);
  bool isWMNameAtom(const CXAtom &atom);
  bool isWMNormalHintsAtom(const CXAtom &atom);
  bool isWMProtocolsAtom(const CXAtom &atom);
  bool isWMSaveYourselfAtom(const CXAtom &atom);
  bool isWMSizeHintsAtom(const CXAtom &atom);
  bool isWMStateAtom(const CXAtom &atom);
  bool isWMTakeFocusAtom(const CXAtom &atom);
  bool isWMTransientForAtom(const CXAtom &atom);
  bool isWMZoomHintsAtom(const CXAtom &atom);

  bool sendKeyPressedEvent(Window xwin, int x, int y, int keycode);
  bool sendKeyReleasedEvent(Window xwin, int x, int y, int keycode);
  bool sendEvent(Window xwin, int propagate, XEvent *event, int event_mask);
  bool sendConfigureNotifyEvent(Window xwin, int x, int y, int width,
                                int height, int border, Window above_xwin);

  std::string displayString();

  bool flushEvents(bool sync=true);
  bool flushWindowEvent(Window xwin, int mask);
  int  flushWindowEvents(Window xwin, int mask);

  bool checkWindowEvent(Window xwin, int event_mask, XEvent *event);
  bool checkTypedEvent(int type, XEvent *event);
  bool checkWindowTypedEvent(Window xwin, int event_type, XEvent *event);

  void selectInput(Window xwin, int event_mask);

  void setWindowBackgroundColor(Window xwin, Pixel bg);
  void setWindowBackgroundPixmap(Window xwin, Pixmap pixmap);
  void setWindowBorderWidth(Window xwin, int width);

  void warpPointer(Window xwin, int x, int y);

  void addToSaveSet(Window xwin);
  void removeFromSaveSet(Window xwin);

  GC   createGC(Pixel fg, Pixel bg);
  GC   createGC(Window xwin, Pixel fg, Pixel bg);
  GC   createXorGC();
  GC   createXorGC(Window xwin);
  void freeGC(GC gc);

  void setFillStipple(GC gc, Pixmap pixmap);

  void setForeground(GC gc, Pixel pixel);
  void setBackground(GC gc, Pixel pixel);

  void startRectClip(GC gc, int x, int y, int width, int height);
  void startPixmapClip(GC gc, Pixmap xpixmap, int dx, int dy);
  void endPixmapClip(GC gc);

  void drawLine(Window xwin, GC gc, int x1, int y1, int x2, int y2);
  void drawRectangle(Window xwin, GC gc, int x, int y, int width, int height);
  void fillRectangle(Window xwin, GC gc, int x, int y, int width, int height);
  void drawPoint(Window xwin, GC gc, int x, int y);

  void copyArea(Window src_xwin, Window dest_xwin, GC gc, int src_x, int src_y,
                int src_width, int src_height, int desy_x, int desy_y);
  void copyPlanes(Window src_xwin, int src_depth, Window dest_xwin,
                  int dest_depth, GC gc, int src_x, int src_y, int src_width,
                  int src_height, int desy_x, int desy_y);

  Pixmap      createStipplePixmap();
  CwmXPixmap *createStippleCwmXPixmap(CwmScreen &screen);

  const CXAtom &getAtom(const std::string &name);
  const CXAtom &getAtom(Atom atom);

  uint keysymToKeycode(KeySym keysym);

  Window      getEventWindow(XEvent *event);
  std::string getEventName(XEvent *event);
  Time        getEventTime(XEvent *event);

  bool sendIntegerServerMessage(Window client_xwin, Window server_xwin, int value);
  bool sendStringServerMessage(Window client_xwin, Window server_xwin, const std::string &value);
  bool readStringClientMessage(Window server_xwin, Window *client_xwin, std::string &value);

  void trapStart();
  bool trapEnd();

  static void logXError(const std::string &msg);

  void log(const std::string &str);
  void logf(const char *format, ...);

  void setDebug(bool flag);
  bool getDebug();

  void debug(const std::string &str);
  void debugf(const char *format, ...);

 private:
  CwmMachine();

  void init();

  Display *openXtDisplay(const std::string &display_name);

  void termWindows();
  void termScreens();

  void resetGrabs();

  void resetPointerGrabs();
  void resetKeyboardGrabs();

 private:
  typedef std::list<CwmWindow *> XWindowList;

  typedef CTriplet<Window,int,int> WindowKeyCodeStateTriplet;
  typedef CTriplet<Window,int,int> WindowButtonModifiersTriplet;

  typedef std::map<WindowKeyCodeStateTriplet,int>    GrabKeyMap;
  typedef std::map<WindowButtonModifiersTriplet,int> GrabButtonMap;

  CwmScreenMgr   *screen_mgr_;
  CwmWMWindowMgr *window_mgr_;
  XWindowList     cwm_xwindows_;
  CwmFocusMgr    *focus_mgr_;

  int    grab_server_count_;
  Window grab_pointer_xwin_;
  Window grab_keyboard_xwin_;
  Window grab_button_xwin_;
  int    grab_button_button_;
  int    grab_button_modifiers_;

  GrabKeyMap    grab_key_map_;
  GrabButtonMap grab_button_map_;

  CwmLog   *log_;
  CwmDebug *debug_;
};
