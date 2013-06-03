class CwmToolBarMenu  {
 private:
  CwmToolBar  &toolbar_;
  CwmGraphics *graphics_;
  CwmWindow   *xwindow_;
  CwmImage    *image_;

  static const char *menu_image[];

 public:
  CwmToolBarMenu(CwmToolBar &toolbar);
 ~CwmToolBarMenu();

  void redraw();

  int getWidth () const;
  int getHeight() const;

  void processMenu();

  CwmWindow *getXWindow() const { return xwindow_; }
};
