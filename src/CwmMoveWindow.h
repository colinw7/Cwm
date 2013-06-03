#ifndef CWM_MOVE_WINDOW_H
#define CWM_MOVE_WINDOW_H

#define CwmMoveWindowInst CwmMoveWindow::getInstance()

class CwmMoveWindow {
 private:
  int           move_threshold_;
  CwmCursorType move_cursor_type_;
  int           position_on_screen_;

 public:
  static CwmMoveWindow *getInstance();

 ~CwmMoveWindow();

  void moveDelayed(CwmWMWindow *window);
  void movePlaced(CwmWMWindow *window);
  void move(CwmWMWindow *window);
  void moveInteractive(CwmScreen &screen, CwmWindow *xwindow,
                       int, int, int, int, int *, int *);

 private:
  CwmMoveWindow();
};

#endif
