#include <CwmI.h>

CwmWindow::
CwmWindow(CwmScreen &screen, CwmWindow *parent, int x, int y,
          int width, int height, uint event_mask, CwmCursorType cursor) :
 screen_(screen), x_(x), y_(y), width_(width), height_(height)
{
  mapped_ = false;

  create(screen_, parent->xwin_, event_mask, cursor);

  CwmMachineInst->addXWindow(this);
}

CwmWindow::
CwmWindow(CwmScreen &screen, Window parent, int x, int y,
          int width, int height, uint event_mask, CwmCursorType cursor) :
 screen_(screen), x_(x), y_(y), width_(width), height_(height)
{
  mapped_ = false;

  create(screen_, parent, event_mask, cursor);

  CwmMachineInst->addXWindow(this);
}

CwmWindow::
CwmWindow(CwmScreen &screen, Window xwin, int x, int y, int width, int height, bool mapped) :
 screen_(screen), xwin_(xwin), x_(x), y_(y), width_(width), height_(height), mapped_(mapped)
{
  owner_ = false;

  CwmMachineInst->addXWindow(this);
}

void
CwmWindow::
create(CwmScreen &screen, Window parent, uint event_mask, CwmCursorType cursor)
{
  XSetWindowAttributes attr;

  attr.event_mask        = event_mask;
  attr.cursor            = screen.getCursor(cursor);
  attr.override_redirect = True;

  uint attr_mask = CWEventMask | CWCursor | CWOverrideRedirect;

  xwin_ = CwmMachineInst->createWindow(parent, x_, y_, width_, height_, 0, attr_mask, &attr);

  owner_ = true;
}

CwmWindow::
~CwmWindow()
{
  std::for_each(callbacks_.begin(), callbacks_.end(), CDeletePointer());

  callbacks_.clear();

  CwmMachineInst->removeXWindow(this);

  if (owner_ && CwmMachineInst->isValidWindow(xwin_)) {
    CwmMachineInst->destroyWindow(xwin_);

    int event_mask = ButtonPressMask   | ButtonReleaseMask        |
                     EnterWindowMask   | LeaveWindowMask          |
                     KeyPressMask      | ExposureMask             |
                     FocusChangeMask   | PropertyChangeMask       |
                     PointerMotionMask | SubstructureRedirectMask |
                     ColormapChangeMask;

    CwmMachineInst->flushWindowEvents(xwin_, event_mask);
  }
}

void
CwmWindow::
setBackground(CwmGraphics *graphics)
{
  CwmColor *color = graphics->getColor();

  CwmMachineInst->setWindowBackgroundColor(xwin_, color->getBg());
}

void
CwmWindow::
setBackgroundPixmap(CwmXPixmap *pixmap)
{
  CwmMachineInst->setWindowBackgroundPixmap(xwin_, pixmap->getXPixmap());
}

void
CwmWindow::
setSaveUnder(bool flag)
{
  XSetWindowAttributes attr;

  attr.save_under = flag;

  CwmMachineInst->changeWindowAtributes(xwin_, CWSaveUnder, &attr);
}

void
CwmWindow::
map()
{
  if (mapped_)
    return;

  CwmMachineInst->mapWindow(xwin_);

  CwmMachineInst->flushWindowEvent(xwin_, MapNotify);

  mapped_ = true;
}

void
CwmWindow::
mapRaised()
{
  if (mapped_)
    return;

  CwmMachineInst->mapWindowRaised(xwin_);

  CwmMachineInst->flushWindowEvent(xwin_, MapNotify);

  mapped_ = true;
}

void
CwmWindow::
mapSubwindows()
{
  CwmMachineInst->mapWindowChildren(xwin_);
}

void
CwmWindow::
unmap()
{
  if (! mapped_)
    return;

  CwmMachineInst->unmapWindow(xwin_);

  CwmMachineInst->flushWindowEvent(xwin_, UnmapNotify);

  mapped_ = false;
}

void
CwmWindow::
lower()
{
  CwmMachineInst->lowerWindow(xwin_);
}

void
CwmWindow::
raise()
{
  CwmMachineInst->raiseWindow(xwin_);
}

void
CwmWindow::
move(int x, int y)
{
  if (x == x_ && y == y_)
    return;

  x_ = x;
  y_ = y;

  CwmMachineInst->moveWindow(xwin_, x_, y_);
}

void
CwmWindow::
resize(int width, int height)
{
  if (width == width_ && height == height_)
    return;

  width_  = width;
  height_ = height;

  CwmMachineInst->resizeWindow(xwin_, width_, height_);
}

void
CwmWindow::
moveResize(int x, int y, int width, int height)
{
  if (x_ == x && y_ == y && width_ == width && height_ == height)
    return;

  x_      = x;
  y_      = y;
  width_  = width;
  height_ = height;

  CwmMachineInst->moveResizeWindow(xwin_, x_, y_, width_, height_);
}

void
CwmWindow::
clear()
{
  CwmMachineInst->clearWindow(xwin_);
}

void
CwmWindow::
setStackMode(int stack_mode, CwmWMWindow *sibling)
{
  XWindowChanges xwc;

  int mask = CWStackMode;

  xwc.stack_mode = stack_mode;

  if (sibling != 0) {
    xwc.sibling = sibling->getXWin();

    mask |= CWSibling;
  }

  CwmMachineInst->debugf("Stack Mode = %d\n", xwc.stack_mode);

  if (sibling != 0)
    CwmMachineInst->debugf("Sibling %s\n", sibling->getName().c_str());

  CwmMachineInst->configureWindow(xwin_, mask, &xwc);
}

void
CwmWindow::
configure(uint mask, XWindowChanges *xwc)
{
  CwmMachineInst->configureWindow(xwin_, mask, xwc);
}

bool
CwmWindow::
isValid()
{
  return CwmMachineInst->isValidWindow(xwin_);
}

void
CwmWindow::
warpPointer(int x, int y)
{
  CwmMachineInst->warpPointer(xwin_, x, y);
}

bool
CwmWindow::
selectInput(uint event_mask)
{
  CwmMachineInst->trapStart();

  CwmMachineInst->selectInput(xwin_, event_mask);

  if (! CwmMachineInst->trapEnd())
    return false;

  return true;
}

bool
CwmWindow::
addInput(uint event_mask)
{
  int win_event_mask = CwmMachineInst->getWindowEventMask(xwin_);

  if (! (win_event_mask & event_mask)) {
    if (! selectInput(win_event_mask | event_mask))
      return false;
  }

  return true;
}

bool
CwmWindow::
getChildWindows(Window **windows, int *num_windows)
{
  CwmMachineInst->trapStart();

  bool flag = CwmMachineInst->getWindowChildren(xwin_, windows, num_windows);

  if (! CwmMachineInst->trapEnd())
    return false;

  return flag;
}

void
CwmWindow::
grabButtonPress()
{
  Cursor cursor = screen_.getCursor(CWM_CURSOR_TITLE);

  CwmMachineInst->grabButton(xwin_, AnyButton, AnyModifier, ButtonPressMask, cursor);
}

void
CwmWindow::
grabButton1Press()
{
  Cursor cursor = screen_.getCursor(CWM_CURSOR_TITLE);

  CwmMachineInst->grabButton(xwin_, Button1, AnyModifier, ButtonPressMask, cursor);
}

void
CwmWindow::
ungrabButtonPress()
{
  CwmMachineInst->ungrabButton(xwin_, AnyButton, AnyModifier);
}

void
CwmWindow::
ungrabButton1Press()
{
  CwmMachineInst->ungrabButton(xwin_, Button1, AnyModifier);
}

bool
CwmWindow::
grab(uint event_mask, CwmCursorType cursor_type)
{
  CwmIdleInst->disable();

  Cursor cursor = screen_.getCursor(cursor_type);

  if (! CwmMachineInst->grabPointer(xwin_, event_mask, cursor))
    return false;

  if (! CwmMachineInst->grabKeyboard(xwin_)) {
    CwmMachineInst->ungrabPointer();
    return false;
  }

  return true;
}

bool
CwmWindow::
ungrab()
{
  CwmMachineInst->ungrabKeyboard();

  CwmMachineInst->ungrabPointer();

  CwmIdleInst->enable();

  return true;
}

bool
CwmWindow::
grabPointer(uint event_mask, CwmCursorType cursor_type)
{
  CwmIdleInst->disable();

  Cursor cursor = screen_.getCursor(cursor_type);

  if (! CwmMachineInst->grabPointer(xwin_, event_mask, cursor))
    return false;

  return true;
}

bool
CwmWindow::
ungrabPointer()
{
  CwmMachineInst->ungrabPointer();

  CwmIdleInst->enable();

  return true;
}

bool
CwmWindow::
getPointerPosition(int *x, int *y)
{
  return CwmMachineInst->queryPointer(xwin_, x, y);
}

void
CwmWindow::
coordsToRoot(int x, int y, int *root_x, int *root_y)
{
  Window child;

  CwmMachineInst->translateCoords(xwin_, screen_.getRoot()->xwin_, x, y, root_x, root_y, &child);
}

void
CwmWindow::
setBorderWidth(int width)
{
  CwmMachineInst->setWindowBorderWidth(xwin_, width);
}

void
CwmWindow::
addToSaveSet()
{
  CwmMachineInst->addToSaveSet(xwin_);
}

void
CwmWindow::
removeFromSaveSet()
{
  CwmMachineInst->removeFromSaveSet(xwin_);
}

void
CwmWindow::
sendEvent(XEvent *event, int event_mask)
{
  CwmMachineInst->sendEvent(xwin_, False, event, event_mask);
}

void
CwmWindow::
sendPointerEvent(XEvent *event)
{
  CwmMachineInst->sendEvent(PointerWindow, True, event, 0);
}

void
CwmWindow::
allowPointerEvents()
{
  CwmMachineInst->allowEvents(SyncPointer);

  CwmMachineInst->flushEvents();
}

void
CwmWindow::
replayPointerEvents()
{
  CwmMachineInst->allowEvents(ReplayPointer);

  CwmMachineInst->flushEvents();
}

void
CwmWindow::
addCallback(CwmXWindowCallType type, CwmXWindowCallProc proc, CwmData data)
{
  CwmXWindowCallback *callback = new CwmXWindowCallback(this, type, proc, data);

  callbacks_.push_back(callback);
}

void
CwmWindow::
callCallbacks(CwmXWindowCallType type, CwmData detail)
{
  XWindowCallbackList callbacks = callbacks_;

  XWindowCallbackList::const_iterator pcallback1 = callbacks.begin();
  XWindowCallbackList::const_iterator pcallback2 = callbacks.end  ();

  for ( ; pcallback1 != pcallback2; ++pcallback1)
    (*pcallback1)->invokeIfType(type, detail);
}

void
CwmWindow::
deleteCallback(CwmXWindowCallType type, CwmXWindowCallProc proc, CwmData data)
{
  XWindowCallbackList::const_iterator pcallback1 = callbacks_.begin();
  XWindowCallbackList::const_iterator pcallback2 = callbacks_.end  ();

  while (pcallback1 != pcallback2) {
    if ((*pcallback1)->match(type, proc, data)) {
      callbacks_.remove(*pcallback1);

      delete *pcallback1;

      pcallback1 = callbacks_.begin();
      pcallback2 = callbacks_.end  ();
    }
    else
      ++pcallback1;
  }
}

Pixmap
CwmWindow::
toMask()
{
  CImageNameSrc src(std::string("mask_") + CStrUtil::toString(xwin_) +
                    "?width="  + CStrUtil::toString(width_) +
                    "?height=" + CStrUtil::toString(height_));

  CImagePtr image = CImageMgrInst->createImage(src);

  image.cast<CXImage>()->getXImage(xwin_, 0, 0, width_, height_);

  Pixmap mask_pixmap = image.cast<CXImage>()->getXMask();

  return mask_pixmap;
}

void
CwmWindow::
reparent(CwmWindow *parent, int x, int y)
{
  CwmMachineInst->reparentWindow(xwin_, parent->xwin_, x, y);

  CwmMachineInst->flushWindowEvent(xwin_, MapNotify);
  CwmMachineInst->flushWindowEvent(xwin_, ReparentNotify);
  CwmMachineInst->flushWindowEvent(xwin_, UnmapNotify);
}

bool
CwmWindow::
isViewable()
{
  return CwmMachineInst->getWindowViewable(xwin_);
}

void
CwmWindow::
getPosition(int *x, int *y)
{
  CwmMachineInst->getWindowGeometry(xwin_, x, y, 0, 0, 0);
}

void
CwmWindow::
getSize(int *width, int *height)
{
  CwmMachineInst->getWindowGeometry(xwin_, 0, 0, width, height, 0);
}

void
CwmWindow::
getGeometry(int *x, int *y, int *width, int *height, int *border)
{
  CwmMachineInst->getWindowGeometry(xwin_, x, y, width, height, border);
}

bool
CwmWindow::
translateCoords(CwmWindow *dest, int src_x, int src_y, int *dest_x, int *dest_y)
{
  Window child;

  if (! CwmMachineInst->translateCoords(xwin_, dest->xwin_, src_x, src_y, dest_x, dest_y, &child))
    return false;

  return true;
}

CwmXWindowCallback::
CwmXWindowCallback(CwmWindow *xwindow, CwmXWindowCallType type,
                   CwmXWindowCallProc proc, CwmData data) :
 xwindow_(xwindow), type_(type), proc_(proc), data_(data)
{
}

CwmXWindowCallback::
~CwmXWindowCallback()
{
}

void
CwmXWindowCallback::
invokeIfType(CwmXWindowCallType type, CwmData detail)
{
  if (type_ == type)
    proc_(xwindow_, data_, detail);
}

void
CwmXWindowCallback::
invoke(CwmData detail)
{
  proc_(xwindow_, data_, detail);
}

bool
CwmXWindowCallback::
match(CwmXWindowCallType type, CwmXWindowCallProc proc, CwmData data)
{
  return (type == type_) && (proc == proc_) && (data == data_);
}
