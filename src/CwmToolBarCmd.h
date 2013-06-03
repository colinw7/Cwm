class CwmToolBarIconDef;

class CwmToolBarCmd {
 private:
  CwmToolBar  &toolbar_;
  CwmScreen   &screen_;
  CwmWindow   *xwindow_;
  CwmImage    *image_;
  CwmGraphics *graphics_;
  std::string  label_;
  std::string  command_;
  bool         add_display_;
  bool         active_;
  bool         pressed_;

 public:
  CwmToolBarCmd(CwmToolBar &toolbar, CwmToolBarIconDef *icon_def, int x);
 ~CwmToolBarCmd();

  void redraw();
  void invoke();

  int getWidth() const;

 private:
  static void invokeProc (CwmWindow *xwindow, CwmData data, CwmData detail);
  static void enterProc  (CwmWindow *xwindow, CwmData data, CwmData detail);
  static void leaveProc  (CwmWindow *xwindow, CwmData data, CwmData detail);
  static void pressProc  (CwmWindow *xwindow, CwmData data, CwmData detail);
  static void releaseProc(CwmWindow *xwindow, CwmData data, CwmData detail);
  static void idleProc   (CwmWindow *xwindow, CwmData data, CwmData detail);
};
