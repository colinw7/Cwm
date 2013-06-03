class CwmMoveFeedback {
 private:
  CwmScreen   &screen;
  CwmWindow   *xwindow;
  CwmGraphics *graphics;
  bool         displayed;
  int          feedback_x;
  int          feedback_y;

 public:
  CwmMoveFeedback(CwmScreen &screen);
 ~CwmMoveFeedback();

  void show();
  void update(int x, int y);
  void hide();

 private:
  void initGeometry();
};
