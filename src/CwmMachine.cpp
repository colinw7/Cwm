#include "CwmI.h"
#include "CXMachine.h"
#include "CXtTimer.h"
#include "X11/XKBlib.h"

CwmMachine *
CwmMachine::
getInstance()
{
  static CwmMachine *instance;

  if (instance == NULL)
    instance = new CwmMachine();

  return instance;
}

CwmMachine::
CwmMachine()
{
  init();
}

CwmMachine::
~CwmMachine()
{
  delete focus_mgr_;
  delete window_mgr_;
  delete screen_mgr_;

  delete debug_;
  delete log_;
}

void
CwmMachine::
init()
{
  grab_server_count_  = 0;
  grab_pointer_xwin_  = None;
  grab_keyboard_xwin_ = None;

  log_   = new CwmLog();
  debug_ = new CwmDebug();

  screen_mgr_ = new CwmScreenMgr();
  window_mgr_ = new CwmWMWindowMgr();

  CXMachineInst->setXErrorProc(&CwmMachine::logXError);

  focus_mgr_ = new CwmFocusMgr();
}

bool
CwmMachine::
init(const string &display_name)
{
  Display *display;

#ifdef CWM_USE_XT
  display = openXtDisplay(display_name);
#else
  display = CXMachineInst->openDisplay(display_name);
#endif

  if (display == NULL)
    return false;

  return true;
}

Display *
CwmMachine::
openXtDisplay(const string &display_name)
{
  int    i;
  int    argc;
  char **argv;

  CwmInst->getArgs(&argc, &argv);

  char **argv1 = new char * [argc + 1];

  for (i = 0; i < argc; ++i)
    argv1[i] = strdup(argv[i]);
  argv1[i] = NULL;

  Display *display =
    CXMachineInst->openXtDisplay(display_name, "Cwm", &argc, argv1);

  for (i = 0; i < argc; ++i)
    free((char *) argv1[i]);

  delete [] argv1;

  return display;
}

void
CwmMachine::
term()
{
  CwmGrabServer grab;

  window_mgr_->term();
  screen_mgr_->term();
}

string
CwmMachine::
getDisplayName() const
{
  return CXMachineInst->getDisplayName();
}

int
CwmMachine::
getNumScreens() const
{
  return CXMachineInst->getNumScreens();
}

CwmScreen &
CwmMachine::
getScreen(int screen_num)
{
  return screen_mgr_->getScreenByNum(screen_num);
}

CwmScreen &
CwmMachine::
getWindowScreen(Window xwin)
{
  return screen_mgr_->getScreenByWindow(xwin);
}

CwmWMWindow *
CwmMachine::
addWMWindow(CwmScreen &screen, Window xwin)
{
  return window_mgr_->addWindow(screen, xwin);
}

void
CwmMachine::
removeWMWindow(CwmWMWindow *window)
{
  window_mgr_->removeWindow(window);
}

CwmWMWindow *
CwmMachine::
getWindowWMWindow(CwmWindow *xwindow)
{
  Window xwin = xwindow->getXWin();

  return getWindowWMWindow(xwin);
}

CwmWMWindow *
CwmMachine::
getWindowWMWindow(Window xwin)
{
  return window_mgr_->lookupFromWindow(xwin);
}

CwmWMWindow *
CwmMachine::
getUserWindowWMWindow(CwmWindow *xwindow)
{
  Window xwin = xwindow->getXWin();

  return getUserWindowWMWindow(xwin);
}

CwmWMWindow *
CwmMachine::
getUserWindowWMWindow(Window xwin)
{
  return window_mgr_->lookupFromUserWindow(xwin);
}

void
CwmMachine::
addXWindow(CwmWindow *xwindow)
{
  cwm_xwindows_.push_back(xwindow);
}

void
CwmMachine::
removeXWindow(CwmWindow *xwindow)
{
  cwm_xwindows_.remove(xwindow);
}

CwmWindow *
CwmMachine::
getWindowCwmXWindow(Window xwin)
{
  XWindowList::const_iterator p1 = cwm_xwindows_.begin();
  XWindowList::const_iterator p2 = cwm_xwindows_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->getXWin() == xwin)
      return *p1;

  return NULL;
}

bool
CwmMachine::
controlScreens()
{
  int num_screens = getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = getScreen(i);

    if (! screen.selectWMInput()) {
      log("Another WM is already running\n");
      return false;
    }
  }

  return true;
}

bool
CwmMachine::
isValidWindow(Window xwin)
{
  return CXMachineInst->isValidWindow(xwin);
}

Colormap
CwmMachine::
getColormap(int screen_num)
{
  return CXMachineInst->getColormap(screen_num);
}

int
CwmMachine::
getWidth(int screen_num)
{
  return CXMachineInst->getWidth(screen_num);
}

int
CwmMachine::
getHeight(int screen_num)
{
  return CXMachineInst->getHeight(screen_num);
}

int
CwmMachine::
getDepth(int screen_num)
{
  return CXMachineInst->getDepth(screen_num);
}

Pixel
CwmMachine::
getBlackPixel(int screen_num)
{
  return CXMachineInst->getBlackPixel(screen_num);
}

Pixel
CwmMachine::
getWhitePixel(int screen_num)
{
  return CXMachineInst->getWhitePixel(screen_num);
}

Window
CwmMachine::
getRoot()
{
  return CXMachineInst->getRoot();
}

Window
CwmMachine::
getRoot(int screen_num)
{
  return CXMachineInst->getRoot(screen_num);
}

Window
CwmMachine::
getWindowRoot(Window xwin)
{
  return CXMachineInst->getWindowRoot(xwin);
}

Window
CwmMachine::
getWindowTop(Window xwin)
{
  return CXMachineInst->getWindowTop(xwin);
}

Window
CwmMachine::
getWindowParent(Window xwin)
{
  return CXMachineInst->getWindowParent(xwin);
}

bool
CwmMachine::
getWindowChildren(Window xwin, Window **children, int *num_children)
{
  return CXMachineInst->getWindowChildren(xwin, children, num_children);
}

void
CwmMachine::
synchronize()
{
  CXMachineInst->synchronize();
}

void
CwmMachine::
grabServer()
{
  if (grab_server_count_ == 0) {
    screen_mgr_->disableScreens();

    CXMachineInst->grabServer();
  }

  ++grab_server_count_;
}

void
CwmMachine::
ungrabServer()
{
  --grab_server_count_;

  if (grab_server_count_ == 0) {
    CXMachineInst->ungrabServer();

    screen_mgr_->enableScreens();
  }
}

bool
CwmMachine::
eventPending()
{
  return CXMachineInst->eventPending();
}

void
CwmMachine::
nextEvent(XEvent *event)
{
  CXMachineInst->nextEvent();

  memcpy(event, CXMachineInst->getEvent(), sizeof(XEvent));
}

bool
CwmMachine::
flushEvents(bool sync)
{
  return CXMachineInst->flushEvents(sync);
}

bool
CwmMachine::
flushWindowEvent(Window xwin, int mask)
{
  return CXMachineInst->flushWindowEvent(xwin, mask);
}

int
CwmMachine::
flushWindowEvents(Window xwin, int mask)
{
  return CXMachineInst->flushWindowEvents(xwin, mask);
}

bool
CwmMachine::
checkWindowEvent(Window xwin, int mask, XEvent *event)
{
  return CXMachineInst->checkWindowEvent(xwin, mask, event);
}

bool
CwmMachine::
checkTypedEvent(int type, XEvent *event)
{
  return CXMachineInst->checkTypedEvent(type, event);
}

bool
CwmMachine::
checkWindowTypedEvent(Window xwin, int type, XEvent *event)
{
  return CXMachineInst->checkWindowTypedEvent(xwin, type, event);
}

void
CwmMachine::
maskEvent(uint mask, XEvent *event)
{
  CXMachineInst->maskEvent(mask, event);
}

void
CwmMachine::
allowEvents(int mode)
{
  CXMachineInst->allowEvents(mode);
}

Colormap
CwmMachine::
getWindowColormap(Window xwin)
{
  trapStart();

  Colormap cmap = CXMachineInst->getWindowColormap(xwin);

  if (! trapEnd())
    cmap = 0;

  return cmap;
}

int
CwmMachine::
getWindowEventMask(Window xwin)
{
  trapStart();

  int event_mask = CXMachineInst->getWindowEventMask(xwin);

  if (! trapEnd())
    event_mask = 0;

  return event_mask;
}

bool
CwmMachine::
getWindowViewable(Window xwin)
{
  return CXMachineInst->getWindowViewable(xwin);
}

void
CwmMachine::
getWindowGeometry(Window xwin, int *x, int *y,
                  int *width, int *height, int *border)
{
  trapStart();

  CXMachineInst->getWindowGeometry(xwin, x, y, width, height, border);

  if (! trapEnd()) {
    *x      = 0;
    *y      = 0;
    *width  = 1;
    *height = 1;
    *border = 1;
  }
}

int
CwmMachine::
getMulticlickTime()
{
  return CXMachineInst->getMulticlickTime();
}

void
CwmMachine::
setFocusWindow(CwmWMWindow *window)
{
  focus_mgr_->setFocusWindow(window);
}

void
CwmMachine::
resetFocusWindow()
{
  focus_mgr_->resetFocusWindow();
}

bool
CwmMachine::
isFocusWindow(CwmWMWindow *window)
{
  return focus_mgr_->isFocusWindow(window);
}

bool
CwmMachine::
isFocus(CwmWMWindow *window)
{
  return focus_mgr_->isUserWindow(window);
}

void
CwmMachine::
focusNext(CwmScreen &screen)
{
  focus_mgr_->focusNext(screen);
}

void
CwmMachine::
focusPrev(CwmScreen &screen)
{
  focus_mgr_->focusPrev(screen);
}

void
CwmMachine::
focusEnter(CwmWMWindow *window)
{
  focus_mgr_->enterUserWindow(window);
}

void
CwmMachine::
focusLeave(CwmWMWindow *window)
{
  focus_mgr_->leaveUserWindow(window);
}

void
CwmMachine::
setInputFocus(Window xwin)
{
  CXMachineInst->setInputFocus(xwin);

  if (xwin != 0) {
    int event_mask = EnterWindowMask | LeaveWindowMask |
                     ExposureMask    | FocusChangeMask;

    flushWindowEvents(xwin, event_mask);
  }
  else
    flushEvents();
}

void
CwmMachine::
resetInputFocus()
{
  setInputFocus(None);
}

bool
CwmMachine::
translateCoords(Window src_w, Window dest_w, int src_x, int src_y,
                int *dest_x, int *dest_y, Window *child)
{
  return CXMachineInst->translateCoords(src_w, dest_w, src_x, src_y,
                                        dest_x, dest_y, child);
}

bool
CwmMachine::
queryPointer(Window xwin, int *x, int *y)
{
  return CXMachineInst->queryPointer(xwin, x, y);
}

void
CwmMachine::
destroyWindow(Window xwin)
{
  CXMachineInst->destroyWindow(xwin);
}

void
CwmMachine::
mapWindow(Window xwin)
{
  CXMachineInst->mapWindow(xwin);
}

void
CwmMachine::
mapWindowRaised(Window xwin)
{
  CXMachineInst->mapWindowRaised(xwin);
}

void
CwmMachine::
mapWindowChildren(Window xwin)
{
  CXMachineInst->mapWindowChildren(xwin);
}

void
CwmMachine::
unmapWindow(Window xwin)
{
  CXMachineInst->unmapWindow(xwin);
}

void
CwmMachine::
reparentWindow(Window xwin, Window parent_xwin, int x, int y)
{
  CXMachineInst->reparentWindow(xwin, parent_xwin, x, y);
}

void
CwmMachine::
lowerWindow(Window xwin)
{
  CXMachineInst->lowerWindow(xwin);
}

void
CwmMachine::
raiseWindow(Window xwin)
{
  CXMachineInst->raiseWindow(xwin);
}

void
CwmMachine::
moveWindow(Window xwin, int x, int y)
{
  CXMachineInst->moveWindow(xwin, x, y);
}

void
CwmMachine::
resizeWindow(Window xwin, int width, int height)
{
  CXMachineInst->resizeWindow(xwin, width, height);
}

void
CwmMachine::
moveResizeWindow(Window xwin, int x, int y, int width, int height)
{
  CXMachineInst->moveResizeWindow(xwin, x, y, width, height);
}

void
CwmMachine::
clearWindow(Window xwin)
{
  CXMachineInst->clearWindow(xwin);
}

void
CwmMachine::
configureWindow(Window xwin, uint mask, XWindowChanges *xwc)
{
  CXMachineInst->configureWindow(xwin, mask, xwc);
}

void
CwmMachine::
restackWindows(Window *windows, int num_windows)
{
  CXMachineInst->restackWindows(windows, num_windows);
}

void
CwmMachine::
shapeCombineMask(Window xwin, Window mask, int x, int y, int op)
{
  CXMachineInst->shapeCombineMask(xwin, mask, x, y, op);
}

bool
CwmMachine::
grabPointer(Window xwin, uint event_mask, Cursor cursor)
{
  resetPointerGrabs();

  if (! CXMachineInst->grabPointer(xwin, event_mask, cursor)) {
    log("Grab Pointer Failed\n");
    return false;
  }

  grab_pointer_xwin_ = xwin;

  return true;
}

void
CwmMachine::
ungrabPointer()
{
  if (grab_pointer_xwin_ == None)
    log("No pointer grab in progress\n");

  CXMachineInst->ungrabPointer();

  grab_pointer_xwin_ = None;
}

bool
CwmMachine::
grabKeyboard(Window xwin)
{
  resetKeyboardGrabs();

  if (! CXMachineInst->grabKeyboard(xwin)) {
    log("Grab Keyboard Failed\n");
    return false;
  }

  grab_keyboard_xwin_ = xwin;

  return true;
}

void
CwmMachine::
ungrabKeyboard()
{
  if (grab_keyboard_xwin_ == None)
    log("No keyboard grab in progress\n");

  CXMachineInst->ungrabKeyboard();

  grab_keyboard_xwin_ = None;
}

void
CwmMachine::
grabKey(Window xwin, int keycode, int state)
{
  WindowKeyCodeStateTriplet triplet(xwin, keycode, state);

  if (grab_key_map_.find(triplet) != grab_key_map_.end()) {
    log("Key grab already in progress\n");
    ungrabKey(xwin, keycode, state);
  }

  CXMachineInst->grabKey(xwin, keycode, state);

  grab_key_map_[triplet] = 1;
}

void
CwmMachine::
ungrabKey(Window xwin, int keycode, int state)
{
  WindowKeyCodeStateTriplet triplet(xwin, keycode, state);

  if (grab_key_map_.find(triplet) == grab_key_map_.end()) {
    log("No key grab in progress\n");
    return;
  }

  CXMachineInst->ungrabKey(xwin, keycode, state);

  grab_key_map_.erase(grab_key_map_.find(triplet));
}

void
CwmMachine::
grabButton(Window xwin, int button, int modifiers,
           int event_mask, Cursor cursor)
{
  WindowButtonModifiersTriplet triplet(xwin, button, modifiers);

  if (grab_button_map_.find(triplet) != grab_button_map_.end()) {
    log("Button grab already in progress\n");
    ungrabButton(xwin, button, modifiers);
  }

  CXMachineInst->grabButton(xwin, button, modifiers, event_mask, cursor);

  grab_button_map_[triplet] = 1;
}

void
CwmMachine::
ungrabButton(Window xwin, int button, int modifiers)
{
  WindowButtonModifiersTriplet triplet(xwin, button, modifiers);

  if (grab_button_map_.find(triplet) == grab_button_map_.end()) {
    log("No button grab in progress\n");
    return;
  }

  CXMachineInst->ungrabButton(xwin, button, modifiers);

  grab_button_map_.erase(grab_button_map_.find(triplet));
}

Window
CwmMachine::
createWindow(int x, int y, int width, int height, int border,
             uint attr_mask, XSetWindowAttributes *attr)
{
  return CXMachineInst->createWindow(x, y, width, height, border,
                                     attr_mask, attr);
}

Window
CwmMachine::
createWindow(Window parent_xwin, int x, int y, int width, int height,
             int border, uint attr_mask, XSetWindowAttributes *attr)
{
  return CXMachineInst->createWindow(parent_xwin, x, y, width, height,
                                     border, attr_mask, attr);
}

Pixmap
CwmMachine::
createPixmap(uint width, uint height, uint depth)
{
  return CXMachineInst->createXPixmap(width, height, depth);
}

Pixmap
CwmMachine::
createPixmap(Window parent_xwin, uint width, uint height, uint depth)
{
  return CXMachineInst->createXPixmap(parent_xwin, width, height, depth);
}

void
CwmMachine::
freePixmap(Pixmap xpixmap)
{
  CXMachineInst->freeXPixmap(xpixmap);
}

void
CwmMachine::
setKeyAutoRepeatRate(uint timeout, uint interval)
{
  CXMachineInst->setKeyAutoRepeatRate(timeout, interval);
}

void
CwmMachine::
getKeyAutoRepeatRate(uint *timeout, uint *interval)
{
  CXMachineInst->getKeyAutoRepeatRate(timeout, interval);
}

void
CwmMachine::
installColormap(Colormap cmap)
{
  CXMachineInst->installColormap(cmap);
}

void
CwmMachine::
changeWindowAtributes(Window xwin, uint attr_mask,
                      XSetWindowAttributes *attr)
{
  CXMachineInst->changeWindowAtributes(xwin, attr_mask, attr);
}

void
CwmMachine::
killClient(Window xwin)
{
  CXMachineInst->killClient(xwin);
}

void
CwmMachine::
setIntegerProperty(Window xwin, const CXAtom &name, int value)
{
  CXMachineInst->setIntegerProperty(xwin, name, value);
}

void
CwmMachine::
setStringProperty(Window xwin, const CXAtom &name, const string &value)
{
  CXMachineInst->setStringProperty(xwin, name, value);
}

void
CwmMachine::
setWindowProperty(const CXAtom &name, Window value)
{
  CXMachineInst->setWindowProperty(getRoot(), name, value);
}

void
CwmMachine::
setWindowProperty(Window xwin, const CXAtom &name, Window value)
{
  CXMachineInst->setWindowProperty(xwin, name, value);
}

void
CwmMachine::
setWindowArrayProperty(Window xwin, const CXAtom &name,
                       Window *xwins, int num_xwins)
{
  CXMachineInst->setWindowArrayProperty(xwin, name, xwins, num_xwins);
}

void
CwmMachine::
setAtomArrayProperty(Window xwin, const CXAtom &name,
                     const CXAtom **atoms, int num_atoms)
{
  CXMachineInst->setAtomArrayProperty(xwin, name, atoms, num_atoms);
}

void
CwmMachine::
setStringListProperty(Window xwin, const CXAtom &name,
                      char **strs, int num_strs)
{
  CXMachineInst->setStringListProperty(xwin, name, strs, num_strs);
}

bool
CwmMachine::
getIntegerProperty(const CXAtom &name, int *value)
{
  return CXMachineInst->getIntegerProperty(name, value);
}

bool
CwmMachine::
getIntegerProperty(Window xwin, const CXAtom &name, int *value)
{
  return CXMachineInst->getIntegerProperty(xwin, name, value);
}

bool
CwmMachine::
getStringProperty(const CXAtom &name, string &value)
{
  return CXMachineInst->getStringProperty(name, value);
}

bool
CwmMachine::
getStringProperty(Window xwin, const CXAtom &name, string &value)
{
  return CXMachineInst->getStringProperty(xwin, name, value);
}

bool
CwmMachine::
getPixmapProperty(Window xwin, const CXAtom &name, Pixmap *value)
{
  return CXMachineInst->getPixmapProperty(xwin, name, value);
}

void
CwmMachine::
deleteProperty(Window xwin, const CXAtom &name)
{
  CXMachineInst->deleteProperty(xwin, name);
}

bool
CwmMachine::
isWMWindow(Window xwin)
{
  return CXMachineInst->isWMWindow(xwin);
}

void
CwmMachine::
setWMStateNormal(Window xwin)
{
  CXMachineInst->setWMStateNormal(xwin);
}

void
CwmMachine::
setWMStateIconic(Window xwin, Window icon_xwin)
{
  CXMachineInst->setWMStateIconic(xwin, icon_xwin);
}

bool
CwmMachine::
isWMStateNormal(Window xwin)
{
  return CXMachineInst->isWMStateNormal(xwin);
}

bool
CwmMachine::
isWMStateIconic(Window xwin)
{
  return CXMachineInst->isWMStateIconic(xwin);
}

void
CwmMachine::
getWMName(Window xwin, string &name)
{
  CXMachineInst->getWMName(xwin, name);
}

void
CwmMachine::
getWMIconName(Window xwin, string &name)
{
  CXMachineInst->getWMIconName(xwin, name);
}

void
CwmMachine::
getWMNormalHints(Window xwin, XSizeHints **size_hints, int *supplied)
{
  CXMachineInst->getWMNormalHints(xwin, size_hints, supplied);
}

void
CwmMachine::
getWMHints(Window xwin, XWMHints **wm_hints)
{
  CXMachineInst->getWMHints(xwin, wm_hints);
}

void
CwmMachine::
getWMTransientFor(Window xwin, Window *transient_xwin)
{
  CXMachineInst->getWMTransientFor(xwin, transient_xwin);
}

void
CwmMachine::
getWMClassHint(Window xwin, XClassHint **class_hint)
{
  CXMachineInst->getWMClassHint(xwin, class_hint);
}

void
CwmMachine::
getWMClientMachine(Window xwin, string &client_machine)
{
  CXMachineInst->getWMClientMachine(xwin, client_machine);
}

void
CwmMachine::
getWMCommand(Window xwin, int *argc, char ***argv)
{
  CXMachineInst->getWMCommand(xwin, argc, argv);
}

void
CwmMachine::
getWMColormapWindows(Window xwin, Window **cmap_windows,
                     int *num_cmap_windows)
{
  CXMachineInst->getWMColormapWindows(xwin, cmap_windows, num_cmap_windows);
}

void
CwmMachine::
getWMProtocols(Window xwin, const CXAtom ***protocols, int *num_protocols)
{
  CXMachineInst->getWMProtocols(xwin, protocols, num_protocols);
}

Window
CwmMachine::
getWMIconWindowHint(Window xwin)
{
  return CXMachineInst->getWMIconWindowHint(xwin);
}

bool
CwmMachine::
getWMMwmHints(Window xwin, MotifWmHints **mwm_hints)
{
  return CXMachineInst->getWMMwmHints(xwin, mwm_hints);
}

const CXAtom &
CwmMachine::
getWMProtocolsAtom()
{
  return CXMachineInst->getWMProtocolsAtom();
}

const CXAtom &
CwmMachine::
getWMDeleteWindowAtom()
{
  return CXMachineInst->getWMDeleteWindowAtom();
}

const CXAtom &
CwmMachine::
getXSetRootIdAtom()
{
  return CXMachineInst->getXSetRootIdAtom();
}

const CXAtom &
CwmMachine::
getCwmDesktopAtom()
{
  return CXMachineInst->getCwmDesktopAtom();
}

bool
CwmMachine::
isWMChangeStateAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMChangeStateAtom(atom);
}

bool
CwmMachine::
isWMClassAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMClassAtom(atom);
}

bool
CwmMachine::
isWMClientMachineAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMClientMachineAtom(atom);
}

bool
CwmMachine::
isWMCommandAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMCommandAtom(atom);
}

bool
CwmMachine::
isWMDeleteWindowAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMDeleteWindowAtom(atom);
}

bool
CwmMachine::
isWMHintsAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMHintsAtom(atom);
}

bool
CwmMachine::
isWMIconNameAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMIconNameAtom(atom);
}

bool
CwmMachine::
isWMNameAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMNameAtom(atom);
}

bool
CwmMachine::
isWMNormalHintsAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMNormalHintsAtom(atom);
}

bool
CwmMachine::
isWMProtocolsAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMProtocolsAtom(atom);
}

bool
CwmMachine::
isWMSaveYourselfAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMSaveYourselfAtom(atom);
}

bool
CwmMachine::
isWMSizeHintsAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMSizeHintsAtom(atom);
}

bool
CwmMachine::
isWMStateAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMStateAtom(atom);
}

bool
CwmMachine::
isWMTakeFocusAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMTakeFocusAtom(atom);
}

bool
CwmMachine::
isWMTransientForAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMTransientForAtom(atom);
}

bool
CwmMachine::
isWMZoomHintsAtom(const CXAtom &atom)
{
  return CXMachineInst->isWMZoomHintsAtom(atom);
}

bool
CwmMachine::
sendKeyPressedEvent(Window xwin, int x, int y, int keycode)
{
  return CXMachineInst->sendKeyPressedEvent(xwin, x, y, keycode);
}

bool
CwmMachine::
sendKeyReleasedEvent(Window xwin, int x, int y, int keycode)
{
  return CXMachineInst->sendKeyReleasedEvent(xwin, x, y, keycode);
}

bool
CwmMachine::
sendEvent(Window xwin, int propagate, XEvent *event, int event_mask)
{
  if (! CXMachineInst->sendEvent(xwin, propagate, event, event_mask)) {
    log("Send Event Failed\n");
    return false;
  }

  return true;
}

bool
CwmMachine::
sendConfigureNotifyEvent(Window xwin, int x, int y, int width, int height,
                         int border, Window above_xwin)
{
  return CXMachineInst->sendConfigureNotifyEvent(xwin, x, y, width, height,
                                                 border, above_xwin);
}

void
CwmMachine::
selectInput(Window xwin, int event_mask)
{
  CXMachineInst->selectInput(xwin, event_mask);
}

void
CwmMachine::
setWindowBackgroundColor(Window xwin, Pixel bg)
{
  CXMachineInst->setWindowBackgroundColor(xwin, bg);
}

void
CwmMachine::
setWindowBackgroundPixmap(Window xwin, Pixmap pixmap)
{
  CXMachineInst->setWindowBackgroundPixmap(xwin, pixmap);
}

void
CwmMachine::
setWindowBorderWidth(Window xwin, int width)
{
  CXMachineInst->setWindowBorderWidth(xwin, width);
}

void
CwmMachine::
warpPointer(Window xwin, int x, int y)
{
  CXMachineInst->warpPointer(xwin, x, y);
}

void
CwmMachine::
addToSaveSet(Window xwin)
{
  CXMachineInst->addToSaveSet(xwin);
}

void
CwmMachine::
removeFromSaveSet(Window xwin)
{
  CXMachineInst->removeFromSaveSet(xwin);
}

GC
CwmMachine::
createGC(Pixel fg, Pixel bg)
{
  return CXMachineInst->createGC(fg, bg);
}

GC
CwmMachine::
createGC(Window xwin, Pixel fg, Pixel bg)
{
  return CXMachineInst->createGC(xwin, fg, bg);
}

GC
CwmMachine::
createXorGC()
{
  return CXMachineInst->createXorGC();
}

GC
CwmMachine::
createXorGC(Window xwin)
{
  return CXMachineInst->createXorGC(xwin);
}

void
CwmMachine::
freeGC(GC gc)
{
  return CXMachineInst->freeGC(gc);
}

void
CwmMachine::
setFillStipple(GC gc, Pixmap pixmap)
{
  return CXMachineInst->setFillStipple(gc, pixmap);
}

void
CwmMachine::
setForeground(GC gc, Pixel pixel)
{
  CXMachineInst->setForeground(gc, pixel);
}

void
CwmMachine::
setBackground(GC gc, Pixel pixel)
{
  CXMachineInst->setBackground(gc, pixel);
}

void
CwmMachine::
startRectClip(GC gc, int x, int y, int width, int height)
{
  CXMachineInst->startRectClip(gc, x, y, width, height);
}

void
CwmMachine::
startPixmapClip(GC gc, Pixmap xpixmap, int dx, int dy)
{
  CXMachineInst->startPixmapClip(gc, xpixmap, dx, dy);
}

void
CwmMachine::
endPixmapClip(GC gc)
{
  CXMachineInst->endPixmapClip(gc);
}

void
CwmMachine::
drawLine(Window xwin, GC gc, int x1, int y1, int x2, int y2)
{
  CXMachineInst->drawLine(xwin, gc, x1, y1, x2, y2);
}

void
CwmMachine::
drawRectangle(Window xwin, GC gc, int x, int y, int width, int height)
{
  CXMachineInst->drawRectangle(xwin, gc, x, y, width, height);
}

void
CwmMachine::
fillRectangle(Window xwin, GC gc, int x, int y, int width, int height)
{
  CXMachineInst->fillRectangle(xwin, gc, x, y, width, height);
}

void
CwmMachine::
drawPoint(Window xwin, GC gc, int x, int y)
{
  CXMachineInst->drawPoint(xwin, gc, x, y);
}

void
CwmMachine::
copyArea(Window src_xwin, Window dest_xwin, GC gc, int src_x, int src_y,
         int src_width, int src_height, int desy_x, int desy_y)
{
  CXMachineInst->copyArea(src_xwin, dest_xwin, gc, src_x, src_y,
                          src_width, src_height, desy_x, desy_y);
}

void
CwmMachine::
copyPlanes(Window src_xwin, int src_depth, Window dest_xwin, int dest_depth,
           GC gc, int src_x, int src_y, int src_width, int src_height,
           int desy_x, int desy_y)
{
  CXMachineInst->copyPlanes(src_xwin, src_depth, dest_xwin, dest_depth,
                            gc, src_x, src_y, src_width, src_height,
                            desy_x, desy_y);
}

Pixmap
CwmMachine::
createStipplePixmap()
{
  return CXMachineInst->createStipplePixmap();
}

CwmXPixmap *
CwmMachine::
createStippleCwmXPixmap(CwmScreen &screen)
{
  Pixmap bitmap = createStipplePixmap();

  CwmXPixmap *xpixmap = new CwmXPixmap(screen, bitmap, 2, 2);

  return xpixmap;
}

const CXAtom &
CwmMachine::
getAtom(const string &name)
{
  return CXMachineInst->getAtom(name);
}

const CXAtom &
CwmMachine::
getAtom(Atom atom)
{
  return CXMachineInst->getAtom(atom);
}

uint
CwmMachine::
keysymToKeycode(KeySym keysym)
{
  return CXMachineInst->keysymToKeycode(keysym);
}

Window
CwmMachine::
getEventWindow(XEvent *event)
{
  return CXMachineInst->getEventWindow(event);
}

string
CwmMachine::
getEventName(XEvent *event)
{
  return CXMachineInst->getEventName(event);
}

Time
CwmMachine::
getEventTime(XEvent *event)
{
  return CXMachineInst->getEventTime(event);
}

bool
CwmMachine::
sendIntegerServerMessage(Window client_xwin, Window server_xwin,
                         int value)
{
  return sendStringServerMessage(client_xwin, server_xwin,
                                 CStrUtil::toString(value));
}

bool
CwmMachine::
sendStringServerMessage(Window client_xwin, Window server_xwin,
                        const string &value)
{
  return CXMachineInst->sendStringServerMessage
          (client_xwin, server_xwin, value);
}

bool
CwmMachine::
readStringClientMessage(Window server_xwin, Window *client_xwin,
                        string &value)
{
  return CXMachineInst->readStringClientMessage
          (server_xwin, client_xwin, value);
}

void
CwmMachine::
trapStart()
{
  CXMachineInst->trapStart();
}

bool
CwmMachine::
trapEnd()
{
  return CXMachineInst->trapEnd();
}

void
CwmMachine::
logXError(const string &msg)
{
  CwmMachineInst->logf("%s\n", msg.c_str());
}

void
CwmMachine::
log(const string &str)
{
  log_->print(str);
}

void
CwmMachine::
logf(const char *format, ...)
{
  va_list vargs;

  va_start(vargs, format);

  log_->print(CStrUtil::vstrprintf(format, &vargs));

  va_end(vargs);
}

void
CwmMachine::
setDebug(bool flag)
{
  debug_->setDebug(flag);
}

bool
CwmMachine::
getDebug()
{
  return debug_->getDebug();
}

void
CwmMachine::
debug(const string &str)
{
  debug_->print(str);
}

void
CwmMachine::
debugf(const char *format, ...)
{
  va_list vargs;

  va_start(vargs, format);

  debug_->print(CStrUtil::vstrprintf(format, &vargs));

  va_end(vargs);
}

void
CwmMachine::
resetGrabs()
{
  resetPointerGrabs();

  resetKeyboardGrabs();
}

void
CwmMachine::
resetPointerGrabs()
{
  if (grab_pointer_xwin_ != None) {
    log("Pointer Grab already in progress\n");
    ungrabPointer();
  }
}

void
CwmMachine::
resetKeyboardGrabs()
{
  if (grab_keyboard_xwin_ != None) {
    log("Keyboard grab already in progress\n");
    ungrabKeyboard();
  }
}
