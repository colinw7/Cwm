class CwmInfoTimer;

class CwmInfo {
 private:
  CwmScreen    &screen;
  CwmWindow    *xwindow;
  CwmGraphics  *graphics;
  CwmInfoTimer *timer;

 public:
  CwmInfo(CwmScreen &screen1);
 ~CwmInfo();

  void show(const std::string &str);
  void hide();

  void timeOut();

 private:
  void init();
};
