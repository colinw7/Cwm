#include <CwmI.h>
#include <CXAtom.h>

#include <cassert>
#include <sstream>

CwmEventMgr::
CwmEventMgr()
{
  last_event_time_   = 0;

  multiclick_window_ = None;
  multiclick_button_ = 0;
  multiclick_time_   = 0;
  multiclick_count_  = 0;

  save_event_window_ = None;
  save_event_mask_   = 0;
}

CwmEventMgr::
~CwmEventMgr()
{
}

CwmEventMgr *
CwmEventMgr::
getInstance()
{
  static CwmEventMgr *instance;

  if (! instance)
    instance = new CwmEventMgr();

  return instance;
}

void
CwmEventMgr::
flushEvents(bool sync)
{
  CwmMachineInst->flushEvents(sync);
}

void
CwmEventMgr::
processEvents()
{
  XEvent event;

  while (eventPending()) {
    CwmMachineInst->nextEvent(&event);

    processEvent(&event);
  }
}

bool
CwmEventMgr::
eventPending()
{
  return CwmMachineInst->eventPending();
}

XEvent *
CwmEventMgr::
processNextEvent()
{
  static XEvent event;

  bool event_found = false;

  CwmIdleInst->start();

  while (! event_found) {
    CwmMachineInst->nextEvent(&event);

    event_found = processEvent(&event);
  }

  CwmIdleInst->start();

  return &event;
}

bool
CwmEventMgr::
processEvent(XEvent *event)
{
  if (CwmMachineInst->getDebug())
    printEvent(event);

  memcpy(&last_event_, event, sizeof(XEvent));

  switch (event->type) {
    case MapRequest:
      return processMapRequest((XMapRequestEvent *) event);
    case MapNotify:
      return processMapNotify((XMapEvent *) event);
    case UnmapNotify:
      return processUnmapNotify((XUnmapEvent *) event);
    case ConfigureRequest:
      return processConfigureRequest((XConfigureRequestEvent *) event);
    case ConfigureNotify:
      return processConfigureNotify((XConfigureEvent *) event);
    case ReparentNotify:
      return processReparentNotify((XReparentEvent *) event);
    case EnterNotify:
      return processEnterNotify((XEnterWindowEvent *) event);
    case LeaveNotify:
      return processLeaveNotify((XLeaveWindowEvent *) event);
    case ButtonPress:
      return processButtonPress((XButtonPressedEvent *) event);
    case ButtonRelease:
      return processButtonRelease((XButtonReleasedEvent *) event);
    case KeyPress:
      return processKeyPress((XKeyPressedEvent *) event);
    case KeyRelease:
      return processKeyRelease((XKeyReleasedEvent *) event);
    case MotionNotify:
      return processMotionNotify((XMotionEvent *) event);
    case Expose:
      return processExpose((XExposeEvent *) event);
    case NoExpose:
      return processNoExpose((XNoExposeEvent *) event);
    case FocusIn:
      return processFocusIn((XFocusChangeEvent *) event);
    case FocusOut:
      return processFocusOut((XFocusChangeEvent *) event);
    case PropertyNotify:
      return processPropertyNotify((XPropertyEvent *) event);
    case VisibilityNotify:
      return processVisibilityNotify((XVisibilityEvent *) event);
    case DestroyNotify:
      return processDestroyNotify((XDestroyWindowEvent *) event);
    case ColormapNotify:
      return processColormapNotify((XColormapEvent *) event);
    case ClientMessage:
      return processClientMessage((XClientMessageEvent *) event);
    case MappingNotify:
      return processMappingNotify((XMappingEvent *) event);
    default:
      CwmMachineInst->logf("Unhandled Event %s\n", CwmMachineInst->getEventName(event).c_str());
      break;
  }

  return false;
}

void
CwmEventMgr::
printEvent(XEvent *event)
{
  if (event->type == MotionNotify)
    return;

  Window xwin = CwmMachineInst->getEventWindow(event);

  if (! CwmMachineInst->isValidWindow(xwin))
    CwmMachineInst->logf("printEvent: Invalid Window %x, Event %s\n", (uint) xwin,
                         CwmMachineInst->getEventName(event).c_str());

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(xwin);

  std::string name;

  if (window != 0)
    name = window->getName();
  else {
    std::ostringstream ost;

    ost << std::hex << (uint) xwin;

    name = ost.str();
  }

  std::string event_name = CwmMachineInst->getEventName(event);

  event_name += " " + name;

  if (event->type != MotionNotify)
    CwmMachineInst->debugf("%s\n", event_name.c_str());
}

int
CwmEventMgr::
getLastEventType()
{
  return last_event_.type;
}

Time
CwmEventMgr::
getLastEventTime()
{
  return last_event_time_;
}

bool
CwmEventMgr::
processMapRequest(XMapRequestEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (window != 0) {
    if (window->isIconised())
      window->restore();

    return false;
  }

  CwmScreen &screen = CwmMachineInst->getWindowScreen(event->window);

  CwmGrabServer grab;

  window = screen.addWMWindow(event->window);

  CwmUserWindow *user = window->getUser();

  if (user != 0)
    user->sendConfigureNotify();

  return false;
}

bool
CwmEventMgr::
processMapNotify(XMapEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (! window)
    return false;

  window->restore();

  return false;
}

bool
CwmEventMgr::
processUnmapNotify(XUnmapEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getUserWindowWMWindow(event->window);

  if (! window)
    return false;

  if (CwmMachineInst->getDebug())
    window->print();

  if (isDestroyPending(event->window)) {
    CwmUserWindow *user = window->getUser();

    if (user != 0) {
      CwmWindow *xwindow = user->getXWindow();

      if (event->window == xwindow->getXWin())
        user->setXWindow(None);
    }
  }

  if (! window->getRolledUp()) {
    CwmGrabServer grab;

    window->remove();
  }

  return false;
}

bool
CwmEventMgr::
processConfigureRequest(XConfigureRequestEvent *event)
{
  XEvent event1;

  if (CwmMachineInst->checkWindowTypedEvent(event->window, MapRequest, &event1))
    processMapRequest((XMapRequestEvent *) &event1);

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  XWindowChanges xwc;
  uint           mask;

  if (window != 0) {
    CwmMachineInst->debugf("ConfigureRequest %s\n", window->getName().c_str());

    if (event->value_mask & CWX)
      CwmMachineInst->debugf("X = %d\n", event->x);
    if (event->value_mask & CWY)
      CwmMachineInst->debugf("Y = %d\n", event->y);
    if (event->value_mask & CWWidth)
      CwmMachineInst->debugf("Width = %d\n", event->width);
    if (event->value_mask & CWHeight)
      CwmMachineInst->debugf("Height = %d\n", event->height);
    if (event->value_mask & CWBorderWidth)
      CwmMachineInst->debugf("Border = %d\n", event->border_width);

    CwmMachineInst->debugf("Mask %x\n", event->value_mask);

    CwmUserWindow *user = window->getUser();

    if (user != 0) {
      CwmWindow *xwindow = user->getXWindow();

      if (event->window != xwindow->getXWin()) {
        CwmMachineInst->log("Configure on non-user window\n");
        return false;
      }
    }

    mask = CWX | CWY | CWWidth | CWHeight;

    mask &= event->value_mask;

    CwmMachineInst->debugf("Mask %x\n", mask);

    xwc.x      = event->x;
    xwc.y      = event->y;
    xwc.width  = event->width;
    xwc.height = event->height;

    CwmFrameWindow *frame = window->getFrame();

    CwmDecoration *decoration = window->getDecoration();

    if (! (event->value_mask & CWX))
      xwc.x = frame->getX() + decoration->getLeftBorder();

    if (! (event->value_mask & CWY))
      xwc.y = frame->getY() + decoration->getTopBorder() +
              decoration->getTitleHeight();

    if (! (event->value_mask & CWWidth))
      xwc.width = user->getWidth();

    if (! (event->value_mask & CWHeight))
      xwc.height = user->getHeight();

    frame->moveResize(xwc.x - decoration->getLeftBorder (),
                      xwc.y - decoration->getTopBorder  () - decoration->getTitleHeight(),
                      xwc.width  + (frame->getWidth () - user->getWidth ()),
                      xwc.height + (frame->getHeight() - user->getHeight()));

    if (event->value_mask & CWStackMode) {
      if (window->isIconised())
        window->restore();

      CwmWMWindow *sibling = 0;

      if ((event->value_mask & CWSibling) && (event->above != None))
        sibling = CwmMachineInst->getWindowWMWindow(event->above);

      window->getFrameWindow()->setStackMode(event->detail, sibling);

      CwmWindowGroup window_group(window);

      window_group.restack();
    }
  }
  else {
    if (event->value_mask & CWX)
      CwmMachineInst->debugf("X = %d\n", event->x);
    if (event->value_mask & CWY)
      CwmMachineInst->debugf("Y = %d\n", event->y);
    if (event->value_mask & CWWidth)
      CwmMachineInst->debugf("Width = %d\n", event->width);
    if (event->value_mask & CWHeight)
      CwmMachineInst->debugf("Height = %d\n", event->height);
    if (event->value_mask & CWBorderWidth)
      CwmMachineInst->debugf("Border = %d\n", event->border_width);

    CwmMachineInst->debugf("Mask %x\n", event->value_mask);

    mask = CWX | CWY | CWWidth | CWHeight | CWBorderWidth;

    mask &= event->value_mask;

    CwmMachineInst->debugf("Mask %x\n", event->value_mask);

    xwc.x            = event->x;
    xwc.y            = event->y;
    xwc.width        = event->width;
    xwc.height       = event->height;
    xwc.border_width = event->border_width;

    if (mask != 0)
      CwmMachineInst->configureWindow(event->window, mask, &xwc);
  }

  return false;
}

bool
CwmEventMgr::
processConfigureNotify(XConfigureEvent *)
{
  return false;
}

bool
CwmEventMgr::
processReparentNotify(XReparentEvent *)
{
  return false;
}

bool
CwmEventMgr::
processEnterNotify(XEnterWindowEvent *event)
{
  last_event_time_ = event->time;

  Window xwin = event->window;

  XEvent event1;

  while (CwmMachineInst->checkWindowTypedEvent(xwin, EnterNotify, &event1))
    ;

  if (CwmMachineInst->checkWindowTypedEvent(xwin, LeaveNotify, &event1)) {
    while (CwmMachineInst->checkWindowTypedEvent(xwin, LeaveNotify, &event1))
      ;

    return false;
  }

  if (CwmResourceDataInst->getWindowPressRaise()) {
    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

    if (window != 0) {
      CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

      if (window->isUser(xwindow))
        CwmMachineInst->focusEnter(window);
    }
  }

  return true;
}

bool
CwmEventMgr::
processLeaveNotify(XLeaveWindowEvent *event)
{
  last_event_time_ = event->time;

  Window xwin = event->window;

  XEvent event1;

  while (CwmMachineInst->checkWindowTypedEvent(xwin, LeaveNotify, &event1))
    ;

  if (CwmMachineInst->checkWindowTypedEvent(xwin, EnterNotify, &event1)) {
    while (CwmMachineInst->checkWindowTypedEvent(xwin, EnterNotify, &event1))
      ;

    return false;
  }

  if (CwmResourceDataInst->getWindowPressRaise()) {
    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

    if (window != 0) {
      CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

      if (window->isUser(xwindow))
        CwmMachineInst->focusLeave(window);
    }
  }

  return true;
}

bool
CwmEventMgr::
processButtonPress(XButtonPressedEvent *event)
{
  last_event_time_ = event->time;

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (! xwindow)
    return false;

  xwindow->callCallbacks(CWM_CALLBACK_BUTTON_PRESS, event);

  if (event->window == multiclick_window_ && event->button == (uint) multiclick_button_ &&
      (event->time - multiclick_time_) <= (uint) CwmResourceDataInst->getDoubleClickTime()) {
    multiclick_time_ = event->time;

    multiclick_count_++;
  }
  else {
    multiclick_window_ = event->window;
    multiclick_button_ = event->button;
    multiclick_time_   = event->time;
    multiclick_count_  = 1;
  }

  return true;
}

bool
CwmEventMgr::
processButtonRelease(XButtonReleasedEvent *event)
{
  last_event_time_ = event->time;

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(multiclick_window_);

  if (! xwindow)
    return false;

  if      (multiclick_count_ == 1) {
    if      (multiclick_button_ == 1)
      xwindow->callCallbacks(CWM_CALLBACK_SINGLE_CLICK_1, 0);
    else if (multiclick_button_ == 2)
      xwindow->callCallbacks(CWM_CALLBACK_SINGLE_CLICK_2, 0);
    else if (multiclick_button_ == 3)
      xwindow->callCallbacks(CWM_CALLBACK_SINGLE_CLICK_3, 0);
    else
      CwmMachineInst->logf("Single Click of Button %d not supported\n", multiclick_button_);
  }
  else if (multiclick_count_ == 2) {
    if      (multiclick_button_ == 1)
      xwindow->callCallbacks(CWM_CALLBACK_DOUBLE_CLICK_1, 0);
    else if (multiclick_button_ == 2)
      xwindow->callCallbacks(CWM_CALLBACK_DOUBLE_CLICK_2, 0);
    else if (multiclick_button_ == 3)
      xwindow->callCallbacks(CWM_CALLBACK_DOUBLE_CLICK_3, 0);
    else
      CwmMachineInst->logf("Double Click of Button %d not supported\n", multiclick_button_);
  }

  xwindow->callCallbacks(CWM_CALLBACK_BUTTON_RELEASE, event);

  return true;
}

bool
CwmEventMgr::
processKeyPress(XKeyPressedEvent *event)
{
  last_event_time_ = event->time;

  return true;
}

bool
CwmEventMgr::
processKeyRelease(XKeyReleasedEvent *event)
{
  last_event_time_ = event->time;

  return true;
}

bool
CwmEventMgr::
processMotionNotify(XMotionEvent *event)
{
  last_event_time_ = event->time;

  if (CwmResourceDataInst->getWindowPressRaise()) {
    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

    if (window != 0) {
      CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

      if (window->isUser(xwindow))
        CwmMachineInst->focusEnter(window);
    }
  }

  return true;
}

bool
CwmEventMgr::
processExpose(XExposeEvent *event)
{
  if (event->count != 0)
    return false;

  CwmScreen &screen = CwmMachineInst->getWindowScreen(event->window);

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (! xwindow)
    return false;

  flushExposeEvents(xwindow);

  CwmToolBar *toolbar = screen.getCurrentDesk()->getToolBar();

  if (toolbar != 0) {
    if (toolbar->processExpose(xwindow))
      return false;
  }

  CwmCmdIcon *cmdicon = CwmCmdIconMgrInst->lookup(*xwindow);

  if (cmdicon != 0) {
    cmdicon->redraw();

    return false;
  }

  CwmDeskMgr *desk_mgr = screen.getDeskMgr();

  CwmDesk *desk = desk_mgr->getCurrentDesk();

  CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

  if (desk_icon_mgr != 0) {
    CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(xwindow);

    if (desk_icon != 0) {
      desk_icon->redraw();

      return false;
    }
  }

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(xwindow);

  if (! window)
    return false;

  CwmDecoration *decoration = window->getDecoration();

  if (decoration->processExpose(event))
    return false;

  if (window->isFrame(xwindow))
    window->getFrame()->draw();

  return false;
}

bool
CwmEventMgr::
processNoExpose(XNoExposeEvent *)
{
  return false;
}

bool
CwmEventMgr::
processFocusIn(XFocusChangeEvent *event)
{
  XEvent event1;

  if (CwmMachineInst->checkWindowTypedEvent(event->window, FocusOut, &event1))
    return false;

  return false;
}

bool
CwmEventMgr::
processFocusOut(XFocusChangeEvent *)
{
  return false;
}

bool
CwmEventMgr::
processPropertyNotify(XPropertyEvent *event)
{
  last_event_time_ = event->time;

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (! window)
    return false;

  const CXAtom &atom = CwmMachineInst->getAtom(event->atom);

  CwmMachineInst->debugf("Property %s\n", atom.getName().c_str());

  CwmHints *hints = window->getHints();

  if      (CwmMachineInst->isWMClassAtom(atom))
    hints->readClassHint();
  else if (CwmMachineInst->isWMClientMachineAtom(atom))
    hints->readSessionHints();
  else if (CwmMachineInst->isWMCommandAtom(atom))
    hints->readSessionHints();
  else if (CwmMachineInst->isWMHintsAtom(atom))
    hints->readWMHints();
  else if (CwmMachineInst->isWMIconNameAtom(atom))
    hints->readWMIconName();
  else if (CwmMachineInst->isWMNameAtom(atom))
    hints->readWMName();
  else if (CwmMachineInst->isWMNormalHintsAtom(atom))
    hints->readWMSizeHints();
  else if (CwmMachineInst->isWMSizeHintsAtom(atom))
    hints->readWMSizeHints();
  else if (CwmMachineInst->isWMTransientForAtom(atom))
    hints->readTransientHint();
  else if (CwmMachineInst->isWMZoomHintsAtom(atom))
    hints->readWMSizeHints();
  else if (CwmMachineInst->isWMProtocolsAtom(atom))
    hints->readWMProtocols();
  else if (CwmMachineInst->isWMStateAtom(atom))
    ;
  else
    CwmMachineInst->debugf("Unhandled property %s\n", atom.getName().c_str());

  window->redraw();

  CwmToolBar *toolbar = window->getDesk()->getToolBar();

  toolbar->redraw();

  return false;
}

bool
CwmEventMgr::
processVisibilityNotify(XVisibilityEvent *event)
{
#ifdef NEVER
  if      (event->state == VisibilityFullyObscured)
    CwmMachineInst->log("VisibilityFullyObscured\n");
  else if (event->state == VisibilityPartiallyObscured)
    CwmMachineInst->log("VisibilityPartiallyObscured\n");
  else if (event->state == VisibilityUnobscured)
    CwmMachineInst->log("VisibilityUnobscured\n");
#else
  assert(event);
#endif

  return false;
}

bool
CwmEventMgr::
processDestroyNotify(XDestroyWindowEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getUserWindowWMWindow(event->window);

  if (! window)
    return false;

  if (CwmMachineInst->getDebug())
    window->print();

  CwmGrabServer grab;

  window->remove();

  return false;
}

bool
CwmEventMgr::
processColormapNotify(XColormapEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getUserWindowWMWindow(event->window);

  if (! window)
    return false;

  window->setColormap(event->colormap);

  return false;
}

bool
CwmEventMgr::
processClientMessage(XClientMessageEvent *event)
{
  if (event->window == CwmMachineInst->getRoot())
    CwmGnomeInst->processRootClientMessage(event);

  if (CwmMessageInst->processClientMessage(event))
    return true;

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (! window)
    return false;

  const CXAtom &atom = CwmMachineInst->getAtom(event->message_type);

  if (CwmMachineInst->isWMChangeStateAtom(atom)) {
    long state = event->data.l[0];

    if (state == NormalState) {
      if (window->isIconised())
        window->restore();
    }
    else {
      if (! window->isIconised())
        window->iconise();
    }
  }
  else
    CwmMachineInst->logf("Unhandled Client Message - Type %s\n", atom.getName().c_str());

  return false;
}

bool
CwmEventMgr::
processMappingNotify(XMappingEvent *event)
{
  XRefreshKeyboardMapping(event);

  return false;
}

Window
CwmEventMgr::
waitForButtonClick(CwmWindow *xwindow, int buttons)
{
  XEvent event;

  if (! buttons)
    buttons = CWM_LEFT_BUTTON | CWM_MIDDLE_BUTTON | CWM_RIGHT_BUTTON;

  while (true) {
    while (CwmMachineInst->checkTypedEvent(ButtonRelease, &event))
      ;

    CwmMachineInst->maskEvent(ButtonPressMask | KeyPress, &event);

    if (event.type == ButtonPress) {
      if ((event.xbutton.button == 1 && ! (buttons & CWM_LEFT_BUTTON  )) ||
          (event.xbutton.button == 2 && ! (buttons & CWM_MIDDLE_BUTTON)) ||
          (event.xbutton.button == 3 && ! (buttons & CWM_RIGHT_BUTTON )))
        continue;

      while (CwmMachineInst->checkTypedEvent(ButtonPress, &event))
        ;
    }
    else {
      if (CwmXEventInst->isEscapeKeyEvent(&event.xkey))
        return None;
    }

    break;
  }

  CwmScreen &screen = xwindow->getScreen();

  Window press_window = screen.getCoordWindow(event.xbutton.x_root, event.xbutton.y_root);

  while (true) {
    while (CwmMachineInst->checkTypedEvent(ButtonPress, &event))
      ;

    CwmMachineInst->maskEvent(ButtonReleaseMask | KeyPress, &event);

    if (event.type == ButtonPress) {
      if ((event.xbutton.button == 1 && ! (buttons & CWM_LEFT_BUTTON  )) ||
          (event.xbutton.button == 2 && ! (buttons & CWM_MIDDLE_BUTTON)) ||
          (event.xbutton.button == 3 && ! (buttons & CWM_RIGHT_BUTTON )))
        continue;

      while (CwmMachineInst->checkTypedEvent(ButtonRelease, &event))
        ;
    }
    else {
      if (CwmXEventInst->isEscapeKeyEvent(&event.xkey))
        return None;
    }

    break;
  }

  return press_window;
}

Window
CwmEventMgr::
waitForButtonPress(CwmWindow *xwindow, int buttons)
{
  XEvent event;

  if (! buttons)
    buttons = CWM_LEFT_BUTTON | CWM_MIDDLE_BUTTON | CWM_RIGHT_BUTTON;

  while (true) {
    while (CwmMachineInst->checkTypedEvent(ButtonRelease, &event))
      ;

    CwmMachineInst->maskEvent(ButtonPressMask | KeyPress, &event);

    if (event.type == ButtonPress) {
      if ((event.xbutton.button == 1 && ! (buttons & CWM_LEFT_BUTTON  )) ||
          (event.xbutton.button == 2 && ! (buttons & CWM_MIDDLE_BUTTON)) ||
          (event.xbutton.button == 3 && ! (buttons & CWM_RIGHT_BUTTON )))
        continue;

      while (CwmMachineInst->checkTypedEvent(ButtonPress, &event))
        ;
    }
    else {
      if (CwmXEventInst->isEscapeKeyEvent(&event.xkey))
        return None;
    }

    break;
  }

  CwmScreen &screen = xwindow->getScreen();

  Window press_window = screen.getCoordWindow(event.xbutton.x_root, event.xbutton.y_root);

  return press_window;
}

Window
CwmEventMgr::
waitForButtonRelease(CwmWindow *xwindow, int buttons)
{
  XEvent event;

  if (! buttons)
    buttons = CWM_LEFT_BUTTON | CWM_MIDDLE_BUTTON | CWM_RIGHT_BUTTON;

  while (true) {
    while (CwmMachineInst->checkTypedEvent(ButtonPress, &event))
      ;

    CwmMachineInst->maskEvent(ButtonReleaseMask, &event);

    if ((event.xbutton.button == 1 && ! (buttons & CWM_LEFT_BUTTON  )) ||
        (event.xbutton.button == 2 && ! (buttons & CWM_MIDDLE_BUTTON)) ||
        (event.xbutton.button == 3 && ! (buttons & CWM_RIGHT_BUTTON )))
      continue;

    while (CwmMachineInst->checkTypedEvent(ButtonRelease, &event))
      ;

    break;
  }

  CwmScreen &screen = xwindow->getScreen();

  Window release_window = screen.getCoordWindow(event.xbutton.x_root, event.xbutton.y_root);

  return release_window;
}

Window
CwmEventMgr::
waitForKeyRelease(CwmWindow *xwindow)
{
  CwmScreen &screen = xwindow->getScreen();

  return waitForKeyRelease(screen);
}

Window
CwmEventMgr::
waitForKeyRelease(CwmScreen &screen)
{
  XEvent event;

  while (true) {
    while (CwmMachineInst->checkTypedEvent(KeyPress, &event))
      ;

    CwmMachineInst->maskEvent(KeyReleaseMask, &event);

    while (CwmMachineInst->checkTypedEvent(KeyRelease, &event))
      ;

    break;
  }

  Window release_window = screen.getCoordWindow(event.xkey.x_root, event.xkey.y_root);

  return release_window;
}

void
CwmEventMgr::
flushExposeEvents(CwmWindow *xwindow)
{
  Window xwin = xwindow->getXWin();

  CwmMachineInst->flushWindowEvents(xwin, Expose);
}

void
CwmEventMgr::
flushEnterLeaveEvents(CwmWindow *xwindow)
{
  Window xwin = xwindow->getXWin();

  int count = 0;

  do {
    count = CwmMachineInst->flushWindowEvents(xwin, EnterNotify) +
            CwmMachineInst->flushWindowEvents(xwin, LeaveNotify);
  }
  while (count > 0);
}

bool
CwmEventMgr::
isDestroyPending(Window xwin)
{
  if (CwmMachineInst->flushWindowEvents(xwin, DestroyNotify))
    return true;

  return false;
}

CwmWMWindow *
CwmEventMgr::
selectWindow(CwmScreen &screen)
{
  uint event_mask = ButtonPressMask | ButtonReleaseMask;

  if (! screen.getRoot()->grab(event_mask, CWM_CURSOR_QUERY))
    return 0;

  Window selected_window = waitForButtonClick(screen.getRoot(), CWM_LEFT_BUTTON);

  screen.getRoot()->ungrab();

  Window parent = CwmMachineInst->getWindowParent(selected_window);

  while (parent != None) {
    selected_window = parent;

    parent = CwmMachineInst->getWindowParent(selected_window);
  }

  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(selected_window);

  return window;
}
