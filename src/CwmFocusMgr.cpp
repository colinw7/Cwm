#include <CwmFocusMgr.h>
#include <CwmMachine.h>
#include <CwmScreen.h>
#include <CwmWindow.h>
#include <CwmWMWindow.h>
#include <CwmAdvHints.h>
#include <CwmResourceData.h>
#include <CwmDeskIcon.h>
#include <CwmDeskMgr.h>
#include <CXtTimer.h>

class CwmFocusMgrTimer : CXtTimer {
 private:
  CwmFocusMgr *mgr_;
  CwmWMWindow *window_;

 public:
  CwmFocusMgrTimer(CwmFocusMgr *mgr, CwmWMWindow *window);

  CwmFocusMgr *mgr() const { return mgr_; }

  void timeOut();
};

CwmFocusMgr::
CwmFocusMgr()
{
}

CwmFocusMgr::
~CwmFocusMgr()
{
  delete auto_raise_timer_;
}

void
CwmFocusMgr::
focusNext(CwmScreen &screen)
{
  if (! CwmResourceDataInst->getClickToFocus())
    return;

  Window *windows;
  int     num_windows;

  if (! screen.getWindows(&windows, &num_windows))
    return;

  int i;

  CwmWMWindow *window = nullptr;

  for (i = 0; i < num_windows; ++i) {
    window = CwmMachineInst->getWindowWMWindow(windows[i]);

    if (! window)
      continue;

    if (isFocusWindow(window))
      break;
  }

  ++i;

  for ( ; i < num_windows; ++i) {
    window = CwmMachineInst->getWindowWMWindow(windows[i]);

    if (isValidWindow(window, windows[i]))
      break;
  }

  if (i >= num_windows) {
    for (i = 0; i < num_windows; ++i) {
      window = CwmMachineInst->getWindowWMWindow(windows[i]);

      if (isValidWindow(window, windows[i]))
        break;
    }

    if (i >= num_windows)
      return;
  }

  if (windows)
    XFree(windows);

  setFocusWindow(window);
}

void
CwmFocusMgr::
focusPrev(CwmScreen &screen)
{
  if (! CwmResourceDataInst->getClickToFocus())
    return;

  Window *windows;
  int     num_windows;

  if (! screen.getWindows(&windows, &num_windows))
    return;

  int i;

  CwmWMWindow *window = nullptr;

  for (i = num_windows - 1; i >= 0; i--) {
    window = CwmMachineInst->getWindowWMWindow(windows[i]);

    if (! window)
      continue;

    if (! isFocusWindow(window))
      break;
  }

  i--;

  for ( ; i >= 0; i--) {
    window = CwmMachineInst->getWindowWMWindow(windows[i]);

    if (isValidWindow(window, windows[i]))
      break;
  }

  if (i < 0) {
    for (i = num_windows - 1; i >= 0; i--) {
      window = CwmMachineInst->getWindowWMWindow(windows[i]);

      if (isValidWindow(window, windows[i]))
        break;
    }

    if (i < 0)
      return;
  }

  if (windows)
    XFree(windows);

  setFocusWindow(window);
}

void
CwmFocusMgr::
setFocusWindow(CwmWMWindow *window)
{
  if (isFocusWindow(window))
    return;

  resetFocusWindow();

  if (! window)
    return;

  focus_window_ = window;

  if (! window->isIconised()) {
    if (CwmResourceDataInst->getClickToFocus())
      ungrabButtons(window);

    if (CwmResourceDataInst->getFocusGrabKeys())
      CwmWindowEventFunctionMgrInst->grabEventKeys(window);

    CwmWindow *xwindow = window->getUserWindow();

    if (xwindow != 0) {
      setInputFocus(xwindow);

      CwmScreen &screen = window->getScreen();

      screen.installColormap(window);

      if (window->getFocusAutoRaise())
        auto_raise_timer_ = new CwmFocusMgrTimer(this, window);
    }
  }

  window->focusIn();

  CwmAdvHintsInst->setActiveWindow(window->getScreen(), window);
}

bool
CwmFocusMgr::
isFocusWindow(CwmWMWindow *window)
{
  return (window == focus_window_);
}

void
CwmFocusMgr::
resetFocusWindow()
{
  delete auto_raise_timer_;

  auto_raise_timer_ = nullptr;

  CwmWMWindow *window = focus_window_;

  focus_window_ = nullptr;

  if (! window) {
    resetInputFocus();
    return;
  }

  CwmScreen &screen = window->getScreen();

  if (! window->isIconised()) {
    window->focusOut();

    if (CwmResourceDataInst->getClickToFocus())
      grabButtons(window);

    if (CwmResourceDataInst->getFocusGrabKeys())
      CwmWindowEventFunctionMgrInst->ungrabEventKeys(window);

    screen.uninstallColormap();
  }

  CwmWindow *xwindow = screen.getRoot();

  setInputFocus(xwindow);

  CwmAdvHintsInst->setActiveWindow(screen, 0);
}

void
CwmFocusMgr::
grabButtons(CwmWMWindow *window)
{
  if (window == grab_buttons_window_) {
    CwmMachineInst->log("grabButtons on window with grab\n");
    return;
  }

  if (grab_buttons_window_ != 0)
    grab_buttons_window_->getFrameWindow()->ungrabButtonPress();

  grab_buttons_window_ = window;

  if (grab_buttons_window_ != 0)
    grab_buttons_window_->getFrameWindow()->grabButtonPress();
}

void
CwmFocusMgr::
ungrabButtons(CwmWMWindow *window)
{
  if (window != grab_buttons_window_) {
    CwmMachineInst->log("ungrabButtons on window without grab\n");
    return;
  }

  if (grab_buttons_window_ != 0)
    grab_buttons_window_->getFrameWindow()->ungrabButtonPress();

  grab_buttons_window_ = nullptr;
}

void
CwmFocusMgr::
ungrabButtonsIfWindow(CwmWMWindow *window)
{
  if (window == grab_buttons_window_)
    ungrabButtons(window);
}

void
CwmFocusMgr::
grabButton1(CwmWMWindow *window)
{
  window->getFrameWindow()->grabButton1Press();
}

void
CwmFocusMgr::
ungrabButton1(CwmWMWindow *window)
{
  window->getFrameWindow()->ungrabButton1Press();
}

bool
CwmFocusMgr::
isValidWindow(CwmWMWindow *window, Window xwin)
{
  if (! window)
    return false;

  if (! window->isIconised()) {
    if (xwin != window->getFrameWindow()->getXWin())
      return false;
  }
  else {
    if (CwmResourceDataInst->getNoIconFocus())
      return false;

    CwmScreen &screen = window->getScreen();

    CwmDeskMgr *desk_mgr = screen.getDeskMgr();

    CwmDesk *desk = desk_mgr->getCurrentDesk();

    CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

    if (! desk_icon_mgr)
      return false;

    CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(xwin);

    if (! desk_icon)
      return false;
  }

  return true;
}

void
CwmFocusMgr::
setInputFocus(CwmWindow *xwindow)
{
  if (! xwindow->getMapped()) {
    resetInputFocus();

    return;
  }

  CwmMachineInst->setInputFocus(xwindow->getXWin());
}

void
CwmFocusMgr::
resetInputFocus()
{
  CwmMachineInst->resetInputFocus();
}

void
CwmFocusMgr::
enterUserWindow(CwmWMWindow *window)
{
  if (window == user_window_)
    return;

  leaveUserWindow(window);

  if (window->getScreen().isTop(window)) {
    top_window_ = nullptr;
    return;
  }

  user_window_ = window;

  grabButtons(user_window_);

  top_window_ = nullptr;
}

void
CwmFocusMgr::
leaveUserWindow(CwmWMWindow *)
{
  if (! user_window_)
    return;

  ungrabButtons(user_window_);

  user_window_ = nullptr;
}

bool
CwmFocusMgr::
isUserWindow(CwmWMWindow *window)
{
  return (window == user_window_);
}

CwmFocusMgrTimer::
CwmFocusMgrTimer(CwmFocusMgr *mgr, CwmWMWindow *window) :
 CXtTimer(uint(CwmResourceDataInst->getAutoRaiseDelay()), CTIMER_FLAGS_NO_DELETE),
 mgr_(mgr), window_(window)
{
}

void
CwmFocusMgrTimer::
timeOut()
{
  window_->raise();
}
