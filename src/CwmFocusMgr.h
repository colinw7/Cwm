#ifndef CWM_FOCUS_MGR_H
#define CWM_FOCUS_MGR_H

#include <CwmTypes.h>
#include <std_Xt.h>

class CwmFocusMgrTimer;

class CwmFocusMgr {
 public:
  CwmFocusMgr();
 ~CwmFocusMgr();

  // Focus Window

  void focusNext(CwmScreen &screen);
  void focusPrev(CwmScreen &screen);
  void setFocusWindow(CwmWMWindow *window);
  void resetFocusWindow();
  bool isFocusWindow(CwmWMWindow *window);

  // Grab Focus on Button Press Window

  void enterUserWindow(CwmWMWindow *window);
  void leaveUserWindow(CwmWMWindow *window);
  bool isUserWindow(CwmWMWindow *window);

  void grabButtons(CwmWMWindow *window);
  void ungrabButtons(CwmWMWindow *window);
  void ungrabButtonsIfWindow(CwmWMWindow *window);

 private:
  void grabButton1(CwmWMWindow *window);
  void ungrabButton1(CwmWMWindow *window);

  void setInputFocus(CwmWindow *xwindow);
  void resetInputFocus();

  bool isValidWindow(CwmWMWindow *window, Window xwin);

  static void autoRaiseProc(CwmData data, XtIntervalId *id);

 private:
  CwmWMWindow      *focus_window_ { nullptr };
  CwmFocusMgrTimer *auto_raise_timer_ { nullptr };
  CwmWMWindow      *user_window_ { nullptr };
  CwmWMWindow      *top_window_ { nullptr };
  CwmWMWindow      *grab_buttons_window_ { nullptr };
};

#endif
