#include <CwmI.h>
#include <CXtTimer.h>

class CwmIdleTimer : public CXtTimer {
 private:
  CwmIdle *idle_;

 public:
  CwmIdleTimer(CwmIdle *idle);

  void timeOut();
};

static const int CWM_IDLE_TIMEOUT = 1000;

CwmIdle::
CwmIdle()
{
  enabled_   = true;
  timer_     = 0;
  idle_xwin_ = None;
}

CwmIdle::
~CwmIdle()
{
  delete timer_;
}

CwmIdle *
CwmIdle::
getInstance()
{
  static CwmIdle *instance;

  if (! instance)
    instance = new CwmIdle();

  return instance;
}

void
CwmIdle::
enable()
{
  enabled_ = true;
}

void
CwmIdle::
disable()
{
  enabled_ = false;
}

void
CwmIdle::
start()
{
  if (enabled_ && idle_xwin_ != None) {
    CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(idle_xwin_);

    if (xwindow != 0) {
      CwmIdleState idle_state(false);

      xwindow->callCallbacks(CWM_CALLBACK_IDLE, &idle_state);
    }
  }

  idle_xwin_ = None;

  delete timer_;

  timer_ = new CwmIdleTimer(this);
}

void
CwmIdle::
timeOut()
{
  if (! enabled_)
    return;

  CwmScreen *pointer_screen = 0;

  int num_screens = CwmMachineInst->getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = CwmMachineInst->getScreen(i);

    if (screen.hasPointer()) {
      pointer_screen = &screen;

      break;
    }
  }

  if (pointer_screen != 0) {
    CwmWindow *xwindow = pointer_screen->getPointerWindow();

    if (xwindow != 0) {
      if (xwindow->getXWin() != idle_xwin_) {
        idle_xwin_ = xwindow->getXWin();

        CwmIdleState idle_state(true);

        xwindow->callCallbacks(CWM_CALLBACK_IDLE, &idle_state);

        CwmMachineInst->flushEvents(true);
      }
    }
    else
      idle_xwin_ = None;
  }
}

CwmIdleTimer::
CwmIdleTimer(CwmIdle *idle) :
 CXtTimer(CWM_IDLE_TIMEOUT, CTIMER_FLAGS_REPEAT), idle_(idle)
{
}

void
CwmIdleTimer::
timeOut()
{
  idle_->timeOut();
}
