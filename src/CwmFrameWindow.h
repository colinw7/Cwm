#ifndef CWM_FRAM_WINDOW_H
#define CWM_FRAM_WINDOW_H

class CwmFrameWindow {
 private:
  CwmWMWindow *window;
  CwmWindow   *xwindow;
  CwmGraphics *graphics;
  bool         positioned;

 public:
  CwmFrameWindow(CwmWMWindow *window);
 ~CwmFrameWindow();

  void map();
  void unmap();

  void position();
  void draw();

  void move(int x, int y);
  void moveResize(int x, int y, int width, int height);

  void minimize();
  void maximize();

  CwmWindow *getXWindow() const { return xwindow; }

  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;
};

#endif
