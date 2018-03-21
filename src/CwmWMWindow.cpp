#include <CwmI.h>
#include <CwmAdvHints.h>
#include <CXAtom.h>

struct CwmWindowEventDef {
  std::string event;
  std::string function;
};

struct CwmWindowEventFunction {
  CwmWMWindow    *window;
  CGlob          *compile;
  int             area;
  CXNamedEvent   *event;
  CwmFunctionDef *function;
  CwmData         data;

  CwmWindowEventFunction(CwmWMWindow *window1, int area1, CXNamedEvent *event1,
                         CwmFunctionDef *function1, CwmData data1) :
   window(window1), compile(0), area(area1), event(event1),
   function(function1), data(data1) {
  }

  CwmWindowEventFunction(const std::string &pattern1, int area1, CXNamedEvent *event1,
                         CwmFunctionDef *function1, CwmData data1) :
   window(0), compile(0), area(area1), event(event1),
   function(function1), data(data1) {
    compile = new CGlob(pattern1);
  }

 ~CwmWindowEventFunction() {
    delete compile;
    delete event;
    delete function;
  }
};

CwmWindowEventDef
window_event_defs[] = {
  { "Alt<Key>F2" , "f.raise"   , },
  { "Alt<Key>F3" , "f.lower"   , },
  { "Alt<Key>F4" , "f.close"   , },
  { "Alt<Key>F5" , "f.restore" , },
  { "Alt<Key>F7" , "f.move"    , },
  { "Alt<Key>F8" , "f.resize"  , },
  { "Alt<Key>F9" , "f.iconise" , },
  { "Alt<Key>F10", "f.maximize", },
  { "Alt<Key>F11", "f.minimize", },
  { ""           , ""          , },
};

//-------------------------------

CwmWindowEventFunctionMgr *
CwmWindowEventFunctionMgr::
getInstance()
{
  static CwmWindowEventFunctionMgr *instance;

  if (! instance)
    instance = new CwmWindowEventFunctionMgr();

  return instance;
}

CwmWindowEventFunctionMgr::
CwmWindowEventFunctionMgr()
{
}

void
CwmWindowEventFunctionMgr::
add(CwmWindowEventFunction *event_function)
{
  event_functions_.push_back(event_function);
}

void
CwmWindowEventFunctionMgr::
remove(CwmWindowEventFunction *event_function)
{
  event_functions_.remove(event_function);
}

void
CwmWindowEventFunctionMgr::
addEventFunction(CwmWMWindow *window, int area, CXNamedEvent *event,
                 CwmFunctionDef *function, CwmData data)
{
  CwmWindowEventFunction *event_function =
    new CwmWindowEventFunction(window, area, event, function, data);

  CwmWindowEventFunctionMgrInst->add(event_function);
}

void
CwmWindowEventFunctionMgr::
addPatternEventFunction(const std::string &pattern, int area, CXNamedEvent *event,
                        CwmFunctionDef *function, CwmData data)
{
  CwmWindowEventFunction *event_function =
    new CwmWindowEventFunction(pattern, area, event, function, data);

  CwmWindowEventFunctionMgrInst->add(event_function);
}

void
CwmWindowEventFunctionMgr::
processEventFunction(CwmWMWindow *window, int area, XEvent *event)
{
  std::string res_name  = window->getResName ();
  std::string res_class = window->getResClass();

  for (auto &event_function : event_functions_) {
    if (! (event_function->area & area))
      continue;

    if      (event_function->window != 0) {
      if (event_function->window != window)
        continue;
    }
    else if (event_function->compile != 0) {
      if (! event_function->compile->compare(res_name) &&
          ! event_function->compile->compare(res_class))
        continue;
    }

    if (event_function->event->matchEvent(event)) {
      event_function->function->processWindow(window, event_function->data);

      return;
    }
  }
}

void
CwmWindowEventFunctionMgr::
removeEventFunctions(CwmWMWindow *window)
{
  WindowEventFunctionList::const_iterator pef1 = event_functions_.begin();
  WindowEventFunctionList::const_iterator pef2 = event_functions_.end  ();

  while (pef1 != pef2) {
    if ((*pef1)->window == window) {
      event_functions_.remove(*pef1);

      delete *pef1;

      pef1 = event_functions_.begin();
      pef2 = event_functions_.end  ();
    }
    else
      ++pef1;
  }
}

void
CwmWindowEventFunctionMgr::
removeAllEventFunctions()
{
  for (auto &event_function : event_functions_)
    delete event_function;

  event_functions_.clear();
}

void
CwmWindowEventFunctionMgr::
grabEventKeys(CwmWMWindow *window)
{
  for (auto &event_function : event_functions_) {
    if (event_function->window != window)
      continue;

    if (! (event_function->area & CWM_WINDOW_USER_AREA))
      continue;

    XKeyPressedEvent *event = event_function->event->getKeyPressedEvent();

    if (event)
      CwmMachineInst->grabKey(window->getFrame()->getXWindow()->getXWin(),
                              event->keycode, event->state);
  }
}

void
CwmWindowEventFunctionMgr::
ungrabEventKeys(CwmWMWindow *window)
{
  for (auto &event_function : event_functions_) {
    if (event_function->window != window)
      continue;

    if (! (event_function->area & CWM_WINDOW_USER_AREA))
      continue;

    XKeyPressedEvent *event = event_function->event->getKeyPressedEvent();

    if (event)
      CwmMachineInst->ungrabKey(window->getFrame()->getXWindow()->getXWin(),
                                event->keycode, event->state);
  }
}

//-------------------------------

CwmWindowGlobalNotifyMgr *
CwmWindowGlobalNotifyMgr::
getInstance()
{
  static CwmWindowGlobalNotifyMgr *instance;

  if (! instance)
    instance = new CwmWindowGlobalNotifyMgr();

  return instance;
}

CwmWindowGlobalNotifyMgr::
CwmWindowGlobalNotifyMgr()
{
}

CwmWindowGlobalNotifyMgr::
~CwmWindowGlobalNotifyMgr()
{
}

void
CwmWindowGlobalNotifyMgr::
add(CwmWindowNotifyData *notify_proc)
{
  notify_procs_.push_back(notify_proc);
}

void
CwmWindowGlobalNotifyMgr::
remove(CwmWindowNotifyData *notify_proc)
{
  notify_procs_.remove(notify_proc);
}

void
CwmWindowGlobalNotifyMgr::
clear()
{
  notify_procs_.clear();
}

void
CwmWindowGlobalNotifyMgr::
addProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc, void *data)
{
  CwmWindowNotifyData *notify_proc = new CwmWindowNotifyData(type, proc, data);

  add(notify_proc);
}

void
CwmWindowGlobalNotifyMgr::
removeProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc, void *data)
{
  WindowNotifyDataList::const_iterator pnp1 = notify_procs_.begin();
  WindowNotifyDataList::const_iterator pnp2 = notify_procs_.end  ();

  while (pnp1 != pnp2) {
    if ((*pnp1)->match(type, proc, data)) {
      remove(*pnp1);

      delete *pnp1;

      pnp1 = notify_procs_.begin();
      pnp2 = notify_procs_.end  ();
    }
    else
      ++pnp1;
  }
}

void
CwmWindowGlobalNotifyMgr::
callProcs(CwmWMWindow *window, CwmWindowNotifyType type)
{
  WindowNotifyDataList::const_iterator pnp1 = notify_procs_.begin();
  WindowNotifyDataList::const_iterator pnp2 = notify_procs_.end  ();

  for ( ; pnp1 != pnp2; ++pnp1)
    (*pnp1)->setCalled(false);

  pnp1 = notify_procs_.begin();

  while (pnp1 != pnp2) {
    if ((*pnp1)->isType(type) && ! (*pnp1)->getCalled()) {
      (*pnp1)->setCalled(true);

      (*pnp1)->call(window);

      pnp1 = notify_procs_.begin();
      pnp2 = notify_procs_.end  ();
    }
    else
      ++pnp1;
  }
}

//-------------------------------

CwmWMWindowMgr::
CwmWMWindowMgr()
{
}

CwmWMWindowMgr::
~CwmWMWindowMgr()
{
  term();
}

void
CwmWMWindowMgr::
term()
{
  for (auto &window : window_list_)
    delete window;

  window_list_.clear();
}

CwmWMWindow *
CwmWMWindowMgr::
addWindow(CwmScreen &screen, Window xwin)
{
  CwmWMWindow *window = new CwmWMWindow(screen, xwin);

  window_list_.push_back(window);

  return window;
}

void
CwmWMWindowMgr::
removeWindow(CwmWMWindow *window)
{
  window_list_.remove(window);
}

CwmWMWindow *
CwmWMWindowMgr::
lookupFromWindow(Window xwin)
{
  if (! CwmMachineInst->isValidWindow(xwin)) {
    CwmWMWindow *window = lookupFromUserWindow(xwin);

    if (window != 0) {
      CwmUserWindow *user = window->getUser();

      if (user != 0)
        user->setXWindow(None);

      window->remove();
    }

    return 0;
  }

  Window parent = CwmMachineInst->getWindowTop(xwin);

  CwmWindow *parent_xwindow = CwmMachineInst->getWindowCwmXWindow(parent);

  if (! parent_xwindow)
    return 0;

  WindowList::iterator p1 = window_list_.begin();
  WindowList::iterator p2 = window_list_.end  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->isFrame(parent_xwindow))
      return *p1;

  return 0;
}

CwmWMWindow *
CwmWMWindowMgr::
lookupFromUserWindow(Window xwin)
{
  WindowList::iterator p1 = window_list_.begin();
  WindowList::iterator p2 = window_list_.end  ();

  for ( ; p1 != p2; ++p1) {
    CwmUserWindow *user = (*p1)->getUser();

    if (user != 0 && user->getXWin() == xwin)
      return *p1;
  }

  return 0;
}

//-------------------------------

void
CwmWMWindow::
processEvents()
{
  while (true) {
    XEvent *event = CwmEventMgrInst->processNextEvent();

    processEvent(event);
  }
}

void
CwmWMWindow::
processPendingEvents()
{
  XEvent event;

  while (CwmMachineInst->eventPending()) {
    CwmMachineInst->nextEvent(&event);

    processEvent(&event);
  }
}

void
CwmWMWindow::
processEvent(XEvent *event)
{
  switch (event->type) {
    case ButtonPress: {
      if (CwmResourceDataInst->getWindowPressRaise()) {
        Window window = CwmMachineInst->getEventWindow(event);

        CwmWMWindow *cwm_window = CwmMachineInst->getWindowWMWindow(window);

        if (cwm_window != 0) {
          CwmScreen &screen = cwm_window->getScreen();

          if (! screen.isTop(cwm_window)) {
            CwmWindow *xwindow = cwm_window->getScreen().getPointerWindow();

            if (! xwindow || cwm_window->isUser(xwindow)) {
              CwmWindow *xwindow = cwm_window->getUserWindow();

              if (xwindow != 0) {
                CwmMachineInst->focusLeave(cwm_window);

                cwm_window->raise();

                CwmMachineInst->flushEvents();

                xwindow->replayPointerEvents();
              }

              break;
            }
          }
        }
      }

      processButtonPress((XButtonPressedEvent *) event);

      break;
    }
    case ButtonRelease:
      processButtonRelease((XButtonReleasedEvent *) event);
      break;
    case KeyPress:
      processKeyPress((XKeyPressedEvent *) event);
      break;
    case KeyRelease:
      processKeyRelease((XKeyReleasedEvent *) event);
      break;
    case MotionNotify:
      break;
    case EnterNotify:
      processEnterNotify((XEnterWindowEvent *) event);
      break;
    case LeaveNotify:
      processLeaveNotify((XLeaveWindowEvent *) event);
      break;
    case ClientMessage:
      processClientMessage((XClientMessageEvent *) event);
      break;
    default:
      CwmMachineInst->logf("Unhandled Event Type %d\n", event->type);
      break;
  }
}

void
CwmWMWindow::
sendToUser(XEvent *event)
{
  Window window = CwmMachineInst->getEventWindow(event);

  CwmWMWindow *cwm_window = CwmMachineInst->getWindowWMWindow(window);

  if (! cwm_window)
    return;

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(window);

  if (cwm_window->isUser(xwindow)) {
    CwmMachineInst->focusLeave(cwm_window);

    xwindow->sendEvent((XEvent *) event, 0xFFFF);

    CwmMachineInst->focusEnter(cwm_window);
  }
}

void
CwmWMWindow::
processButtonPress(XButtonPressedEvent *event)
{
  CwmScreen &screen = CwmMachineInst->getWindowScreen(event->window);

  if (event->window == screen.getRoot()->getXWin()) {
    CwmRootEventFunctionMgrInst->process(screen, (XEvent *) event);

    return;
  }

  //------

  CwmToolBar *toolbar = screen.getCurrentDesk()->getToolBar();

  if (toolbar != 0)
    toolbar->processButtonPress(event->window, event->button);

  //------

  CwmCmdIcon *cmdicon = CwmCmdIconMgrInst->lookup(event->window);

  if (cmdicon != 0) {
    if (event->button == 2)
      cmdicon->move();

    return;
  }

  //------

  CwmDeskMgr *desk_mgr = screen.getDeskMgr();

  CwmDesk *desk = desk_mgr->getCurrentDesk();

  CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

  if (desk_icon_mgr != 0) {
    CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(event->window);

    if (desk_icon != 0) {
      if      (event->button == 1)
        desk_icon->raise();
      else if (event->button == 2)
        desk_icon->move();
      else if (event->button == 3)
        desk_icon->processMenu();
    }
  }

  //------

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (! window)
    return;

  //------

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if      (event->button == 1) {
    if (CwmResourceDataInst->getClickToFocus())
      CwmMachineInst->setFocusWindow(window);
  }

  if (window->decoration_->processButtonPress(event)) {
    return;
  }

  if      (event->button == 1) {
    if (window->isFrame(xwindow))
      window->raise();
  }
  else if (event->button == 2) {
    if (window->isFrame(xwindow))
      CwmMoveWindowInst->moveDelayed(window);
  }
  else if (event->button == 3) {
    window->processWindowMenu();
  }
}

void
CwmWMWindow::
processButtonRelease(XButtonReleasedEvent *)
{
}

void
CwmWMWindow::
processKeyPress(XKeyPressedEvent *event)
{
  CwmScreen &screen = CwmMachineInst->getWindowScreen(event->window);

  if (event->window == screen.getRoot()->getXWin()) {
    CwmRootEventFunctionMgrInst->process(screen, (XEvent *) event);

    return;
  }

  CwmToolBar *toolbar = screen.getCurrentDesk()->getToolBar();

  if (toolbar != 0) {
    CwmToolBarIcon *tool_icon = toolbar->getToolIcon(event->window);

    if (tool_icon != 0) {
      tool_icon->processKeyPress(event);

      return;
    }
  }

  CwmDeskMgr *desk_mgr = screen.getDeskMgr();

  CwmDesk *desk = desk_mgr->getCurrentDesk();

  CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

  if (desk_icon_mgr != 0) {
    CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(event->window);

    if (desk_icon != 0) {
      desk_icon->processKeyPress(event);

      return;
    }
  }

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (window != 0)
    CwmWindowEventFunctionMgrInst->
      processEventFunction(window, CWM_WINDOW_ANY_AREA, (XEvent *) event);
}

void
CwmWMWindow::
processKeyRelease(XKeyReleasedEvent *event)
{
  CwmScreen &screen = CwmMachineInst->getWindowScreen(event->window);

  if (event->window == screen.getRoot()->getXWin()) {
    CwmRootEventFunctionMgrInst->process(screen, (XEvent *) event);

    return;
  }

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (window != 0)
    CwmWindowEventFunctionMgrInst->
      processEventFunction(window, CWM_WINDOW_ANY_AREA, (XEvent *) event);
}

void
CwmWMWindow::
processEnterNotify(XEnterWindowEvent *event)
{
  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (! xwindow || ! xwindow->isValid())
    return;

  if (! CwmResourceDataInst->getClickToFocus()) {
    CwmScreen &screen = xwindow->getScreen();

    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(xwindow);

    if (window != 0) {
      CwmMachineInst->setFocusWindow(window);

      goto done;
    }

    CwmToolBar *toolbar = screen.getCurrentDesk()->getToolBar();

    if (toolbar != 0) {
      CwmToolBarIcon *tool_icon = toolbar->getToolIcon(xwindow);

      if (tool_icon != 0 && ! tool_icon->getWindow()->isIconised()) {
        CwmMachineInst->setFocusWindow(tool_icon->getWindow());

        goto done;
      }
    }

    CwmMachineInst->setFocusWindow(0);
  }

 done:
  xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (xwindow != 0 && xwindow->isValid())
    xwindow->callCallbacks(CWM_CALLBACK_ENTER, 0);
}

void
CwmWMWindow::
processLeaveNotify(XLeaveWindowEvent *event)
{
  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (! xwindow || ! xwindow->isValid())
    return;

  xwindow->callCallbacks(CWM_CALLBACK_LEAVE, 0);
}

void
CwmWMWindow::
processClientMessage(XClientMessageEvent *)
{
}

CwmWMWindow::
CwmWMWindow(CwmScreen &screen, Window xwin) :
 screen_(screen)
{
  cmap_ = CwmMachineInst->getWindowColormap(xwin);

  //------

  CwmColor *color = CwmResourceDataInst->getColor(screen);
  CwmFont  *font  = CwmResourceDataInst->getFont (screen);

  normal_graphics_ = new CwmGraphics(screen, font, color);

  //------

  color = CwmResourceDataInst->getFocusColor(screen);
  font  = CwmResourceDataInst->getFocusFont (screen);

  focus_graphics_ = new CwmGraphics(screen, font, color);

  //------

  graphics_ = normal_graphics_;

  //------

  rolled_up_ = false;

  //------

  int user_x, user_y, user_width, user_height, user_border;

  CwmMachineInst->getWindowGeometry(xwin, &user_x, &user_y, &user_width, &user_height,
                                    &user_border);

  if (CwmMachineInst->isValidWindow(xwin))
    user_ = new CwmUserWindow(this, xwin, user_x, user_y, user_width, user_height, user_border);
  else
    user_ = 0;

  frame_ = new CwmFrameWindow(this);

  //------

  hints_ = new CwmHints(this);

  //------

  decoration_ = new CwmDecoration(this);

  //------

  state_ = CWM_WINDOW_STATE_NORMAL;

  //------

  if (user_ != 0) {
    normal_x_      = user_->getX();
    normal_y_      = user_->getY();
    normal_width_  = user_->getWidth();
    normal_height_ = user_->getHeight();
  }
  else {
    normal_x_      = 0;
    normal_y_      = 0;
    normal_width_  = 1;
    normal_height_ = 1;
  }

  //------

  parent_ = 0;

  Window transient_for = getTransientForHint();

  if (transient_for != None)
    parent_ = CwmMachineInst->getWindowWMWindow(transient_for);

  if (parent_ != 0)
    parent_->children_.push_back(this);

  //------

  image_list_ = new CwmWindowImageList(this);

  //------

  focus_auto_raise_ = CwmResourceDataInst->getFocusAutoRaise();

  if (! CwmCustomHintMgrInst->getCustomHintValue
         (getResName(), CwmNfocusAutoRaise, (CwmData) &focus_auto_raise_))
    CwmCustomHintMgrInst->getCustomHintValue
     (getResClass(), CwmNfocusAutoRaise, (CwmData) &focus_auto_raise_);

  //------

  circulate_skip_ = false;

  if (! CwmCustomHintMgrInst->getCustomHintValue
         (getResName(), CwmNcirculateSkip, (CwmData) &circulate_skip_))
    CwmCustomHintMgrInst->getCustomHintValue
     (getResClass(), CwmNcirculateSkip, (CwmData) &circulate_skip_);

  //------

  toolbar_skip_ = false;

  if (! CwmCustomHintMgrInst->getCustomHintValue
         (getResName(), CwmNtoolbarSkip, (CwmData) &toolbar_skip_))
    CwmCustomHintMgrInst->getCustomHintValue
     (getResClass(), CwmNtoolbarSkip, (CwmData) &toolbar_skip_);

  //------

  window_menu_ = new CwmWindowMenu(this);

  //------

  decorate();

  //------

  int desk_num = getDeskProperty();

  CwmDesk *desk = screen.getDesk(desk_num);

  desk->addWMWindow(this);

  setDeskProperty();

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_CREATE);

  //------

  if      (isWMStateIconic())
    iconise();
  else if (getInitialStateHint() == IconicState)
    iconise();
  else
    setStateProperty();

  //------

  initUserWindow();

  //------

  addEvents();

  //------

  place();

  //------

  if (CwmMailMgrInst->isMailWindow(this))
    CwmMailMgrInst->addMailWindow(this);
}

CwmWMWindow::
~CwmWMWindow()
{
  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_DESTROY);

  callNotifyProcs(CWM_WINDOW_NOTIFY_DESTROY);

  //------

  if (CwmMachineInst->isFocus(this))
    CwmMachineInst->focusLeave(this);

  CwmMailMgrInst->deleteMailWindow(this);

  //------

  CwmWindow *user_xwindow = getUserWindow();

  if (user_xwindow != 0 && user_xwindow->getXWin() != None) {
    int x      = 0;
    int y      = 0;
    int border = 0;

    if (user_ != 0) {
      x      = user_->getX();
      y      = user_->getY();
      border = user_->getBorder();
    }

    if (CwmResourceDataInst->getPositionIsFrame()) {
      x -= decoration_->getLeftBorder();
      y -= decoration_->getTitleHeight() +
           decoration_->getTopBorder();
    }

    user_xwindow->reparent(screen_.getRoot(), x, y);

    user_xwindow->setBorderWidth(border);

    user_xwindow->removeFromSaveSet();
  }

  //------

  delete normal_graphics_;
  delete focus_graphics_;

  //------

  delete user_;
  delete frame_;

  //------

  delete hints_;

  delete window_menu_;

  CwmWindowEventFunctionMgrInst->removeEventFunctions(this);

  //------

  delete image_list_;

  //------

  for (auto &child : children_)
    child->parent_ = nullptr;
}

CwmFont *
CwmWMWindow::
getFont() const
{
  return normal_graphics_->getFont();
}

CwmColor *
CwmWMWindow::
getColor() const
{
  return normal_graphics_->getColor();
}

GC
CwmWMWindow::
getGC() const
{
  return normal_graphics_->getGC();
}

int
CwmWMWindow::
getFontHeight() const
{
  return normal_graphics_->getFontHeight();
}

void
CwmWMWindow::
place()
{
  if (! CwmResourceDataInst->getInteractivePlacement())
    return;

  if (state_ != CWM_WINDOW_STATE_NORMAL)
    return;

  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  if (user_xwindow->isViewable())
    return;

  if (CwmResourceDataInst->getMoveOpaque()) {
    int x, y;

    screen_.getRoot()->getPointerPosition(&x, &y);

    CwmWindow *xwindow = frame_->getXWindow();

    xwindow->move(x, y);

    map();
  }

  CwmMoveWindowInst->movePlaced(this);

  if (! CwmResourceDataInst->getMoveOpaque())
    map();
}

void
CwmWMWindow::
remove()
{
  if (CwmMachineInst->isFocusWindow(this))
    CwmMachineInst->setFocusWindow(0);

  //------

  CwmDesk *desk = screen_.getDesk(this);

  desk->removeWMWindow(this);

  //------

  if (parent_ != 0)
    parent_->children_.remove(this);

  screen_.removeWMWindow(this);

  CwmMachineInst->removeWMWindow(this);

  //------

  CwmScreen &screen = screen_;

  delete this;

  //------

  CwmAdvHintsInst->setStackedClientList(screen);
}

void
CwmWMWindow::
initUserWindow()
{
  uint attr_mask = CWEventMask | CWDontPropagate;

  XSetWindowAttributes attr;

  attr.event_mask = EnterWindowMask    | LeaveWindowMask      |
                    KeyPressMask       | KeyReleaseMask       |
                    FocusChangeMask    | VisibilityChangeMask |
                    PropertyChangeMask | StructureNotifyMask  |
                    PointerMotionMask  | ColormapChangeMask;

  attr.do_not_propagate_mask = ButtonPressMask | ButtonReleaseMask |
                               KeyPressMask    | KeyReleaseMask;

  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  CwmMachineInst->changeWindowAtributes(user_xwindow->getXWin(), attr_mask, &attr);

  user_xwindow->setBorderWidth(0);

  user_xwindow->addToSaveSet();

  if (CwmResourceDataInst->getClickToFocus())
    CwmMachineInst->setFocusWindow(this);
}

void
CwmWMWindow::
processWindowMenu()
{
  window_menu_->process();
}

void
CwmWMWindow::
processWindowMenuAtPosition(int x, int y)
{
  window_menu_->processAtPosition(frame_->getXWindow(), x, y);
}

void
CwmWMWindow::
processWindowMenu(CwmWMWindow *window, CwmData)
{
  window->processWindowMenuAtPosition(0, window->frame_->getHeight());
}

bool
CwmWMWindow::
isFrame(CwmWindow *xwindow)
{
  return (frame_ != 0 && xwindow == frame_->getXWindow());
}

bool
CwmWMWindow::
isUser(CwmWindow *xwindow)
{
  CwmWindow *user_xwindow = getUserWindow();

  return (xwindow == user_xwindow);
}

std::string
CwmWMWindow::
getName()
{
  std::string name = hints_->getName();

  if (name == "")
    name = hints_->getResName();

  if (name == "")
    name = "window";

  return name;
}

std::string
CwmWMWindow::
getResName()
{
  std::string name = hints_->getResName();

  if (name == "")
    name = "window";

  return name;
}

std::string
CwmWMWindow::
getResClass()
{
  std::string name = hints_->getResClass();

  if (name == "")
    name = hints_->getResName();

  if (name == "")
    name = "cwm";

  return name;
}

std::string
CwmWMWindow::
getIconName()
{
  std::string name = hints_->getIconName();

  if (name == "")
    name = hints_->getName();

  if (name == "")
    name = hints_->getResName();

  if (name == "")
    name = "icon";

  return name;
}

void
CwmWMWindow::
addEvents()
{
  for (int i = 0; window_event_defs[i].event != ""; ++i) {
    CXNamedEvent *event = new CXNamedEvent(window_event_defs[i].event);

    CwmFunctionDef *function = new CwmFunctionDef(window_event_defs[i].function);

    CwmWindowEventFunctionMgrInst->
      addEventFunction(this, CWM_WINDOW_ANY_AREA, event, function, 0);
  }
}

void
CwmWMWindow::
print()
{
  CwmMachineInst->logf("Window %s\n", getName().c_str());
  CwmMachineInst->logf(" Frame %x\n", (frame_ ? frame_->getXWindow() : 0));

  decoration_->print();

  CwmWindow *user_xwindow = getUserWindow();

  CwmMachineInst->logf("  User      %x\n", user_xwindow);
}

bool
CwmWMWindow::
onCurrentDesk()
{
  int desk_num = getDeskNum();

  return (desk_num == screen_.getCurrentDeskNum());
}

int
CwmWMWindow::
getDeskNum()
{
  return screen_.getDeskNum(this);
}

CwmDesk *
CwmWMWindow::
getDesk()
{
  return screen_.getDesk(this);
}

void
CwmWMWindow::
raise()
{
  screen_.raise(this);

  CwmMessageInst->updateChangeCount();
}

void
CwmWMWindow::
lower()
{
  screen_.lower(this);

  CwmMessageInst->updateChangeCount();
}

void
CwmWMWindow::
iconise()
{
  if (isIconised())
    return;

  setIconised();

  //------

  CwmWindowGroup window_group(this);

  window_group.unmap();

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_ICONISE);

  callNotifyProcs(CWM_WINDOW_NOTIFY_ICONISE);

  //------

  setStateProperty();
}

void
CwmWMWindow::
restore()
{
  CwmWindowGroup window_group(this);

  window_group.map();

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_RESTORE);

  callNotifyProcs(CWM_WINDOW_NOTIFY_RESTORE);

  //------

  setNormal();

  setStateProperty();

  //------

  raise();
}

void
CwmWMWindow::
minimize()
{
  if (isIconised())
    return;

  if (! isMinimized()) {
    if (isNormal()) {
      normal_x_      = frame_->getX();
      normal_y_      = frame_->getY();
      normal_width_  = frame_->getWidth();
      normal_height_ = frame_->getHeight();
    }

    frame_->minimize();

    setMinimised();
  }
  else {
    moveResize(normal_x_, normal_y_, normal_width_, normal_height_);

    redecorate();

    setNormal();
  }
}

void
CwmWMWindow::
maximize()
{
  if (isIconised())
    return;

  if (! isMaximized()) {
    if (isNormal()) {
      normal_x_      = frame_->getX();
      normal_y_      = frame_->getY();
      normal_width_  = frame_->getWidth();
      normal_height_ = frame_->getHeight();
    }

    frame_->maximize();

    setMaximised();
  }
  else {
    moveResize(normal_x_, normal_y_, normal_width_, normal_height_);

    redecorate();

    setNormal();
  }
}

void
CwmWMWindow::
restoreSize()
{
  if (isNormal())
    return;

  if (isIconised()) {
    restore();

    return;
  }

  moveResize(normal_x_, normal_y_, normal_width_, normal_height_);

  redecorate();

  setNormal();
}

void
CwmWMWindow::
close()
{
  // update happens in process of Unmap event

  if (getNotifyDeleteWindowHint())
    notifyDeleteWindow();
  else {
    CwmWindow *user_xwindow = getUserWindow();

    if (user_xwindow != 0)
      CwmMachineInst->killClient(user_xwindow->getXWin());
  }

  CwmMachineInst->flushEvents();

  closeInvalid();
}

void
CwmWMWindow::
closeInvalid()
{
  CwmWindow *user_xwindow = getUserWindow();

  if (user_xwindow != 0 && ! user_xwindow->isValid()) {
    unmap();

    if (user_ != 0)
      user_->setXWindow(None);

    remove();
  }
}

void
CwmWMWindow::
notifyDeleteWindow()
{
  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  XClientMessageEvent event;

  event.type         = ClientMessage;
  event.window       = user_xwindow->getXWin();
  event.message_type = CwmMachineInst->getWMProtocolsAtom().getXAtom();
  event.format       = 32;

  event.data.l[0] = CwmMachineInst->getWMDeleteWindowAtom().getXAtom();
  event.data.l[1] = CurrentTime;

  user_xwindow->sendEvent((XEvent *) &event, 0);
}

void
CwmWMWindow::
focusIn()
{
  graphics_ = focus_graphics_;

  decoration_->focusIn(graphics_);

  redraw();

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_FOCUS_IN);

  callNotifyProcs(CWM_WINDOW_NOTIFY_FOCUS_IN);
}

void
CwmWMWindow::
focusOut()
{
  graphics_ = normal_graphics_;

  decoration_->focusOut(graphics_);

  redraw();

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_FOCUS_OUT);

  callNotifyProcs(CWM_WINDOW_NOTIFY_FOCUS_OUT);
}

void
CwmWMWindow::
focus()
{
  CwmMachineInst->setFocusWindow(this);
}

void
CwmWMWindow::
rollUp()
{
  rolled_up_ = true;

  decoration_->rollUp();

  if (user_ != 0)
    user_->unmap();

  CwmMachineInst->flushEvents();

  redecorate();
}

void
CwmWMWindow::
rollDown()
{
  rolled_up_ = false;

  if (user_ != 0)
    user_->map();

  redecorate();

  decoration_->rollDown();

  CwmMachineInst->flushEvents();

  redecorate();
}

void
CwmWMWindow::
map()
{
  CwmWindow *xwindow = frame_->getXWindow();

  xwindow->map();
}

void
CwmWMWindow::
unmap()
{
  CwmWindow *xwindow = frame_->getXWindow();

  xwindow->unmap();
}

void
CwmWMWindow::
decorate()
{
  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  int x = getXHint();
  int y = getYHint();

  bool already_mapped = user_xwindow->isViewable();

  if (already_mapped || ! CwmResourceDataInst->getClientAutoPlace())
    user_xwindow->getPosition(&x, &y);

  int user_x = x;
  int user_y = y;

  if (x != -1 && y != -1) {
    if (CwmResourceDataInst->getPositionIsFrame()) {
      user_x += decoration_->getLeftBorder();
      user_y += decoration_->getTitleHeight() + decoration_->getTopBorder();
    }
  }
  else
    user_xwindow->getPosition(&user_x, &user_y);

  if (user_ != 0) {
    int user_width  = user_->getWidth ();
    int user_height = user_->getHeight();

    user_->moveResize(user_x, user_y, user_width, user_height);
  }

  //------

  user_xwindow->unmap();

  decoration_->decorate();

  //------

  frame_->position();

  //------

  decoration_->reparent(user_xwindow);

  user_xwindow->map();

  if (already_mapped || ! CwmResourceDataInst->getInteractivePlacement())
    map();

  redecorate();
}

void
CwmWMWindow::
redecorate()
{
  decoration_->redecorate();

  if (! rolled_up_) {
    frame_->position();

    CwmWindow *user_xwindow = getUserWindow();

    if (user_xwindow != 0)
      user_xwindow->move(0, 0);
  }
  else
    frame_->position();
}

void
CwmWMWindow::
redraw()
{
  decoration_->redraw();

  if (! getResizeHandleDecorationHint() && getBorderDecorationHint())
    frame_->draw();
}

CwmWindow *
CwmWMWindow::
getFrameWindow() const
{
  return frame_->getXWindow();
}

CwmWindow *
CwmWMWindow::
getUserWindow() const
{
  if (user_ != 0)
    return user_->getXWindow();
  else
    return 0;
}

Window
CwmWMWindow::
getXWin() const
{
  CwmWindow *user_window = getUserWindow();

  if (user_window != 0)
    return user_window->getXWin();
  else
    return None;
}

CwmDecoration *
CwmWMWindow::
getDecoration() const
{
  return decoration_;
}

void
CwmWMWindow::
move()
{
  CwmMoveWindowInst->move(this);
}

void
CwmWMWindow::
move(int x, int y)
{
  if (CwmResourceDataInst->getPositionOnScreen()) {
    if (x < 0)
      x = 0;

    if (y < 0)
      y = 0;

    if (x + frame_->getWidth() > screen_.getWidth())
      x = screen_.getWidth()  - frame_->getWidth();

    if (y + frame_->getHeight() > screen_.getHeight())
      y = screen_.getHeight() - frame_->getHeight();
  }

  frame_->move(x, y);

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_MOVE);

  callNotifyProcs(CWM_WINDOW_NOTIFY_MOVE);
}

void
CwmWMWindow::
moveResize(int x, int y, int width, int height)
{
  if (CwmResourceDataInst->getPositionOnScreen()) {
    if (x < 0)
      x = 0;

    if (y < 0)
      y = 0;

    if (width > screen_.getWidth())
      width = screen_.getWidth();

    if (height > screen_.getHeight())
      height = screen_.getHeight();

    if (x + width > screen_.getWidth())
      x = screen_.getWidth()  - width;

    if (y + height > screen_.getHeight())
      y = screen_.getHeight() - height;
  }

  frame_->moveResize(x, y, width, height);

  //------

  CwmWindowGlobalNotifyMgrInst->callProcs(this, CWM_WINDOW_NOTIFY_MOVE);

  callNotifyProcs(CWM_WINDOW_NOTIFY_MOVE);
}

void
CwmWMWindow::
resize()
{
  CwmResizeWindowInst->resize(this, CWM_RESIZE_FREE);
}

void
CwmWMWindow::
resizeUser(int width, int height)
{
  int x1 = frame_->getX();
  int y1 = frame_->getY();

  int user_width  = 1;
  int user_height = 1;

  if (user_ != 0) {
    user_width  = user_->getWidth ();
    user_height = user_->getHeight();
  }

  int width1  = frame_->getWidth () - (user_width  - width );
  int height1 = frame_->getHeight() - (user_height - height);

  if (CwmResourceDataInst->getPositionOnScreen()) {
    if (width1 > screen_.getWidth())
      width1 = screen_.getWidth();

    if (height1 > screen_.getHeight())
      height1 = screen_.getHeight();

    if (x1 + width1 > screen_.getWidth())
      x1 = screen_.getWidth()  - width1;

    if (y1 + height1 > screen_.getHeight())
      y1 = screen_.getHeight() - height1;
  }

  moveResize(x1, y1, width1, height1);

  redecorate();
}

void
CwmWMWindow::
setStateProperty()
{
  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  if (isIconised())
    CwmMachineInst->setWMStateNormal(user_xwindow->getXWin());
  else {
    CwmDeskMgr *desk_mgr = screen_.getDeskMgr();

    CwmDesk *desk = desk_mgr->getCurrentDesk();

    CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

    if (desk_icon_mgr != 0) {
      CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(this);

      if (desk_icon != 0)
        CwmMachineInst->setWMStateIconic(user_xwindow->getXWin(),
                                         desk_icon->getXWindow()->getXWin());
      else
        CwmMachineInst->setWMStateIconic(user_xwindow->getXWin(), None);
    }
  }
}

bool
CwmWMWindow::
isWMStateIconic()
{
  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return false;

  return CwmMachineInst->isWMStateIconic(user_xwindow->getXWin());
}

void
CwmWMWindow::
setDeskProperty()
{
  int desk_num = screen_.getDeskNum(this);

  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return;

  const CXAtom &atom = CwmMachineInst->getCwmDesktopAtom();

  CwmMachineInst->setIntegerProperty(user_xwindow->getXWin(), atom, desk_num + 1);
}

int
CwmWMWindow::
getDeskProperty()
{
  int desk_num;

  CwmWindow *user_xwindow = getUserWindow();

  if (! user_xwindow)
    return 0;

  const CXAtom &atom = CwmMachineInst->getCwmDesktopAtom();

  if (! CwmMachineInst->getIntegerProperty(user_xwindow->getXWin(), atom, &desk_num))
    return screen_.getCurrentDeskNum();

  return (desk_num - 1);
}

CwmImage *
CwmWMWindow::
getImage(int width, int height)
{
  return image_list_->getImage(width, height);
}

CwmImage *
CwmWMWindow::
getImage()
{
  return image_list_->getImage();
}

void
CwmWMWindow::
addNotifyProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc, void *data)
{
  CwmWindowNotifyData *notify_proc = new CwmWindowNotifyData(type, proc, data);

  notify_procs_.push_back(notify_proc);
}

void
CwmWMWindow::
removeNotifyProc(CwmWindowNotifyType type, CwmWindowNotifyProc proc, void *data)
{
  WindowNotifyDataList::const_iterator pnp1 = notify_procs_.begin();
  WindowNotifyDataList::const_iterator pnp2 = notify_procs_.end  ();

  while (pnp1 != pnp2) {
    if ((*pnp1)->match(type, proc, data)) {
      notify_procs_.remove(*pnp1);

      delete *pnp1;

      pnp1 = notify_procs_.begin();
      pnp2 = notify_procs_.end  ();
    }
    else
      ++pnp1;
  }
}

void
CwmWMWindow::
callNotifyProcs(CwmWindowNotifyType type)
{
  WindowNotifyDataList::const_iterator pnp1 = notify_procs_.begin();
  WindowNotifyDataList::const_iterator pnp2 = notify_procs_.end  ();

  for ( ; pnp1 != pnp2; ++pnp1)
    (*pnp1)->setCalled(false);

  pnp1 = notify_procs_.begin();

  while (pnp1 != pnp2) {
    if ((*pnp1)->isType(type) && ! (*pnp1)->getCalled()) {
      (*pnp1)->setCalled(true);

      (*pnp1)->call(this);

      pnp1 = notify_procs_.begin();
      pnp2 = notify_procs_.end  ();
    }
    else
      ++pnp1;
  }
}

Window
CwmWMWindow::
getWMIconWindowHint()
{
  CwmUserWindow *user = getUser();

  if (user != 0) {
    Window xwin = user->getXWin();

    return CwmMachineInst->getWMIconWindowHint(xwin);
  }
  else
    return None;
}
