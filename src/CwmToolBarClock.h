class CwmToolBarClockTimer;

class CwmToolBarClock {
 private:
  CwmToolBar           &toolbar_;
  CwmGraphics          *graphics_;
  std::string           format_;
  CwmWindow            *xwindow_;
  CwmToolBarClockTimer *timer_;

 public:
  CwmToolBarClock(CwmToolBar &toolbar);
 ~CwmToolBarClock();

  void redraw();

  void enable();
  void disable();

  int getWidth() const;
  int getHeight() const;

  static void idleProc(CwmWindow *xwindow, CwmData data, CwmData detail);
};
