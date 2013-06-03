class CwmResizeFeedback {
 private:
  CwmScreen   &screen;
  CwmWindow   *xwindow;
  CwmGraphics *graphics;
  bool         displayed;
  int          feedback_x;
  int          feedback_y;

 public:
  CwmResizeFeedback(CwmScreen &screen);
 ~CwmResizeFeedback();

  void show();
  void update(CwmWMWindow *window, int dx, int dy);
  void hide();

 private:
  void initGeometry();
};
