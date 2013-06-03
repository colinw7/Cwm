#include "CwmI.h"

CwmResizeWindow::
CwmResizeWindow()
{
  resize_tolerance_ = 0;
}

CwmResizeWindow::
~CwmResizeWindow()
{
}

CwmResizeWindow *
CwmResizeWindow::
getInstance()
{
  static CwmResizeWindow *instance;

  if (instance == NULL)
    instance = new CwmResizeWindow();

  return instance;
}

void
CwmResizeWindow::
resizeDelayed(CwmWMWindow *window, int resize_type)
{
  resize_tolerance_ = 4;

  resize(window, resize_type);

  resize_tolerance_ = 0;
}

void
CwmResizeWindow::
resize(CwmWMWindow *window, int resize_type)
{
  CwmScreen &screen = window->getScreen();

  CwmFrameWindow *frame = window->getFrame();
  CwmUserWindow  *user  = window->getUser ();

  if (user == NULL)
    return;

  int last_event_type = CwmEventMgrInst->getLastEventType();

  if (last_event_type == KeyPress && CwmResourceDataInst->getEnableWarp())
    screen.getRoot()->warpPointer(frame->getX() + frame->getWidth ()/2,
                                  frame->getY() + frame->getHeight()/2);

  bool resize_started = false;

  bool escape_pressed = false;

  CwmCursorType cursor_type = CWM_CURSOR_MOVE;

  if      (resize_type == CWM_RESIZE_TOP_LEFT)
    cursor_type = CWM_CURSOR_NW;
  else if (resize_type == CWM_RESIZE_BOTTOM_LEFT)
    cursor_type = CWM_CURSOR_SW;
  else if (resize_type == CWM_RESIZE_TOP_RIGHT)
    cursor_type = CWM_CURSOR_NE;
  else if (resize_type == CWM_RESIZE_BOTTOM_RIGHT)
    cursor_type = CWM_CURSOR_SE;
  else if (resize_type == CWM_RESIZE_LEFT)
    cursor_type = CWM_CURSOR_W;
  else if (resize_type == CWM_RESIZE_RIGHT)
    cursor_type = CWM_CURSOR_E;
  else if (resize_type == CWM_RESIZE_TOP)
    cursor_type = CWM_CURSOR_N;
  else if (resize_type == CWM_RESIZE_BOTTOM)
    cursor_type = CWM_CURSOR_S;

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    PointerMotionMask;

  if (! screen.getRoot()->grab(event_mask, cursor_type))
    return;

  int start_x;
  int start_y;

  screen.getRoot()->getPointerPosition(&start_x, &start_y);

  int x11 = frame->getX();
  int y11 = frame->getY();
  int x12 = frame->getX() + frame->getWidth () - 1;
  int y12 = frame->getY() + frame->getHeight() - 1;

  int x21 = x11;
  int y21 = y11;
  int x22 = x12;
  int y22 = y12;

  CwmGrabServer grab;

  int dx1 = x21 - x11;
  int dy1 = y21 - y11;
  int dx2 = x22 - x12;
  int dy2 = y22 - y12;

  while (true) {
    XEvent *event = CwmEventMgrInst->processNextEvent();

    if      (event->type == ButtonPress)
      break;
    else if (event->type == ButtonRelease)
      break;
    else if (event->type == MotionNotify) {
      if (resize_started)
        screen.drawOutline(x21, y21, x22 - x21, y22 - y21);

      int end_x = event->xmotion.x_root;
      int end_y = event->xmotion.y_root;

      if (! (resize_type & CWM_RESIZE_LEFT ) &&
          ! (resize_type & CWM_RESIZE_RIGHT)) {
        start_x = end_x;

        if      (end_x < x21)
          resize_type |= CWM_RESIZE_LEFT;
        else if (end_x > x22)
          resize_type |= CWM_RESIZE_RIGHT;
      }

      if (! (resize_type & CWM_RESIZE_TOP   ) &&
          ! (resize_type & CWM_RESIZE_BOTTOM)) {
        start_y = end_y;

        if      (end_y < y21)
          resize_type |= CWM_RESIZE_TOP;
        else if (end_y > y22)
          resize_type |= CWM_RESIZE_BOTTOM;
      }

      if (! resize_started &&
          (abs(end_x - start_x) >= resize_tolerance_ ||
           abs(end_y - start_y) >= resize_tolerance_))
        resize_started = true;

      if (resize_type & CWM_RESIZE_LEFT  ) x21 = x11 + (end_x - start_x);
      if (resize_type & CWM_RESIZE_TOP   ) y21 = y11 + (end_y - start_y);
      if (resize_type & CWM_RESIZE_RIGHT ) x22 = x12 + (end_x - start_x);
      if (resize_type & CWM_RESIZE_BOTTOM) y22 = y12 + (end_y - start_y);

      dx1 = x21 - x11;
      dy1 = y21 - y11;
      dx2 = x22 - x12;
      dy2 = y22 - y12;

      if (user->getWidth() + (dx2 - dx1) < window->getMinWidthHint()) {
        if (resize_type & CWM_RESIZE_LEFT)
          x21 = user->getWidth() - window->getMinWidthHint() + dx2 + x11;

        if (resize_type & CWM_RESIZE_RIGHT)
          x22 = window->getMinWidthHint() - user->getWidth() + dx1 + x12;

        dx1 = x21 - x11;
        dx2 = x22 - x12;
      }

      if (user->getHeight() + (dy2 - dy1) < window->getMinHeightHint()) {
        if (resize_type & CWM_RESIZE_TOP)
          y21 = user->getHeight() - window->getMinHeightHint() + dy2 + y11;

        if (resize_type & CWM_RESIZE_BOTTOM)
          y22 = window->getMinHeightHint() - user->getHeight() + dy1 + y12;

        dy1 = y21 - y11;
        dy2 = y22 - y12;
      }

      if (CwmResourceDataInst->getFeedbackResize() && resize_started)
        screen.showResizeFeedback();

      screen.updateResizeFeedback(window, dx2 - dx1, dy2 - dy1);

      if (resize_started)
        screen.drawOutline(x21, y21, x22 - x21, y22 - y21);
    }
    else if (event->type == KeyPress) {
      if (CwmXEventInst->isEscapeKeyEvent(&event->xkey)) {
        escape_pressed = true;

        break;
      }
    }
    else if (event->type == KeyRelease) {
    }
    else if (event->type == EnterNotify) {
    }
    else if (event->type == LeaveNotify) {
    }
    else
      CwmMachineInst->logf("ResizeWindow: Unexpected event %d\n", event->type);
  }

  if (resize_started)
    screen.drawOutline(x21, y21, x22 - x21, y22 - y21);

  screen.hideResizeFeedback();

  screen.getRoot()->ungrab();

  if (! resize_started) {
    x21 = x11;
    y21 = y11;
    x22 = x12;
    y22 = y12;
  }

  if (escape_pressed) {
    if (! screen.getRoot()->grabPointer(event_mask, CWM_CURSOR_TITLE)) {
      CwmMachineInst->log("Grab Failed for Resize\n");
      return;
    }

    while (true) {
      XEvent *event = CwmEventMgrInst->processNextEvent();

      if (event->type == ButtonRelease)
        break;
    }

    x21 = x11;
    y21 = y11;
    x22 = x12;
    y22 = y12;

    screen.getRoot()->ungrabPointer();
  }

  if (x11 == x21 && y11 == y21 && x12 == x22 && y12 == y22)
    return;

  if (frame->getX() + dx1 < 0)
    dx1 = -frame->getX();

  if (frame->getY() + dy1 < 0)
    dy1 = -frame->getY();

  if (frame->getX() + frame->getWidth () + dx2 > screen.getWidth())
    dx2 = screen.getWidth()  - frame->getX() - frame->getWidth ();

  if (frame->getY() + frame->getHeight() + dy2 > screen.getHeight())
    dy2 = screen.getHeight() - frame->getY() - frame->getHeight();

  window->moveResize(frame->getX() + dx1,
                     frame->getY() + dy1,
                     frame->getWidth () + (dx2 - dx1),
                     frame->getHeight() + (dy2 - dy1));

  window->setNormal();

  window->redecorate();
}
