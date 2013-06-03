#ifndef CWM_RESIZE_WINDOW_H
#define CWM_RESIZE_WINDOW_H

#define CwmResizeWindowInst CwmResizeWindow::getInstance()

class CwmResizeWindow {
 private:
  int resize_tolerance_;

 public:
  static CwmResizeWindow *getInstance();

 ~CwmResizeWindow();

  void resizeDelayed(CwmWMWindow *window, int resize_type);
  void resize(CwmWMWindow *window, int resize_type);

 private:
  CwmResizeWindow();
};

#endif
