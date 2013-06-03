#ifndef CWM_USER_WINDOW_H
#define CWM_USER_WINDOW_H

class CwmUserWindow {
 private:
  CwmWMWindow *window_;
  CwmWindow   *xwindow_;
  int          x_;
  int          y_;
  int          border_;

 public:
  CwmUserWindow(CwmWMWindow *window, Window xwin, int x, int y,
                int width, int height, int border);
 ~CwmUserWindow();

  void map();
  void unmap();

  CwmWindow *getXWindow() const { return xwindow_; }

  Window getXWin() const;

  void setXWindow(Window xwin);

  int getX() const { return x_; }
  int getY() const { return y_; }

  int getWidth () const;
  int getHeight() const;

  int getBorder() const { return border_; }

  void fixSize(int *width, int *height);

  void sizeToUser(int *width, int *height);

  void resize(int width, int height);
  void moveResize(int x, int y, int width, int height);

  void sendConfigureNotify();

  static void incrementRound(int *number, int increment);
};

#endif
