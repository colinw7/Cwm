class CwmTabWindow {
 private:
  CwmScreen   &screen_;
  CwmWindow   *xwindow_;
  CwmGraphics *graphics_;
  int          x_;
  int          y_;
  int          width_;
  int          height_;
  std::string  str_;

 public:
  CwmTabWindow(CwmScreen &screen1);
 ~CwmTabWindow();

  void show(const std::string &str);
  void hide();

 private:
  void init();
};
