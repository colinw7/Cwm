#include "CwmI.h"

CwmMoveWindow::
CwmMoveWindow()
{
  move_threshold_     = 0;
  move_cursor_type_   = CWM_CURSOR_MOVE;
  position_on_screen_ = -1;
}

CwmMoveWindow::
~CwmMoveWindow()
{
}

CwmMoveWindow *
CwmMoveWindow::
getInstance()
{
  static CwmMoveWindow *instance;

  if (instance == NULL)
    instance = new CwmMoveWindow();

  return instance;
}

void
CwmMoveWindow::
moveDelayed(CwmWMWindow *window)
{
  move_threshold_ = CwmResourceDataInst->getMoveThreshold();

  move(window);

  move_threshold_ = 0;
}

void
CwmMoveWindow::
movePlaced(CwmWMWindow *window)
{
  move_cursor_type_   = CWM_CURSOR_PLACE;
  position_on_screen_ = false;

  move(window);

  move_cursor_type_   = CWM_CURSOR_MOVE;
  position_on_screen_ = CwmResourceDataInst->getPositionOnScreen();
}

void
CwmMoveWindow::
move(CwmWMWindow *window)
{
  CwmScreen &screen = window->getScreen();

  CwmFrameWindow *frame = window->getFrame();

  if (position_on_screen_ == -1)
    position_on_screen_ = CwmResourceDataInst->getPositionOnScreen();

  int last_event_type = CwmEventMgrInst->getLastEventType();

  if (last_event_type == KeyPress && CwmResourceDataInst->getEnableWarp())
    screen.getRoot()->warpPointer(frame->getX() + frame->getWidth ()/2,
                                  frame->getY() + frame->getHeight()/2);

  int dx;
  int dy;

  moveInteractive(screen,
                  frame->getXWindow(),
                  frame->getX(),
                  frame->getY(),
                  frame->getWidth(),
                  frame->getHeight(),
                  &dx, &dy);

  if (dx == 0 && dy == 0)
    return;

  if (! CwmResourceDataInst->getMoveOpaque())
    window->move(frame->getX() + dx, frame->getY() + dy);
  else
    window->move(frame->getX(), frame->getY());
}

void
CwmMoveWindow::
moveInteractive(CwmScreen &screen, CwmWindow *xwindow, int x, int y,
                int width, int height, int *dx, int *dy)
{
  int save_x = x;
  int save_y = y;

  bool move_started = false;

  *dx = 0;
  *dy = 0;

  bool escape_pressed = false;

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    PointerMotionMask;

  if (! screen.getRoot()->grab(event_mask, move_cursor_type_))
    return;

  int start_x, start_y;

  xwindow->getPointerPosition(&start_x, &start_y);

  int end_x = start_x;
  int end_y = start_y;

  if (! CwmResourceDataInst->getMoveOpaque())
    CwmMachineInst->grabServer();

  while (true) {
    XEvent *event = CwmEventMgrInst->processNextEvent();

    if      (event->type == ButtonPress)
      break;
    else if (event->type == ButtonRelease)
      break;
    else if (event->type == MotionNotify) {
      if (move_started) {
        if (! CwmResourceDataInst->getMoveOpaque())
          screen.drawOutline(x + *dx, y + *dy, width, height);
      }

      end_x = event->xmotion.x_root;
      end_y = event->xmotion.y_root;

      *dx = end_x - start_x;
      *dy = end_y - start_y;

      if (! move_started &&
          (abs(*dx) >= move_threshold_ || abs(*dy) >= move_threshold_))
        move_started = true;

      if (CwmResourceDataInst->getFeedbackMove() && move_started)
        screen.showMoveFeedback();

      if (position_on_screen_) {
        if (x + *dx < 0)
          *dx = -x;

        if (y + *dy < 0)
          *dy = -y;

        if (x + width + *dx >= screen.getWidth())
          *dx = screen.getWidth() - width - x;

        if (y + height + *dy >= screen.getHeight())
          *dy = screen.getHeight() - height - y;
      }

      screen.updateMoveFeedback(x + *dx, y + *dy);

      if (move_started) {
        if (! CwmResourceDataInst->getMoveOpaque())
          screen.drawOutline(x + *dx, y + *dy, width, height);
        else {
          x += *dx;
          y += *dy;

          xwindow->move(x, y);

          start_x = end_x;
          start_y = end_y;
        }
      }
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
      CwmMachineInst->logf("MoveWindow: Unexpected event %d\n", event->type);
  }

  if (move_started) {
    if (! CwmResourceDataInst->getMoveOpaque())
      screen.drawOutline(x + *dx, y + *dy, width, height);
  }

  screen.hideMoveFeedback();

  if (! CwmResourceDataInst->getMoveOpaque())
    CwmMachineInst->ungrabServer();

  screen.getRoot()->ungrab();

  if (! move_started) {
    *dx = 0;
    *dy = 0;

    return;
  }

  if (escape_pressed) {
    if (! screen.getRoot()->grabPointer(event_mask, CWM_CURSOR_TITLE)) {
      CwmMachineInst->log("Grab Pointer Failed for Move\n");
      return;
    }

    while (true) {
      XEvent *event = CwmEventMgrInst->processNextEvent();

      if (event->type == ButtonRelease)
        break;
    }

    *dx = 0;
    *dy = 0;

    screen.getRoot()->ungrabPointer();

    return;
  }

  if (CwmResourceDataInst->getMoveOpaque()) {
    *dx = x - save_x;
    *dy = y - save_y;
  }
}
