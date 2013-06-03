class CwmMask {
 private:
  CwmScreen   &screen_;
  CwmGraphics *graphics_;
  int          width_;
  int          height_;
  CwmXPixmap  *xpixmap_;

 public:
  CwmMask(CwmScreen &screen, CwmGraphics *graphics, int width, int height);
 ~CwmMask();

  void drawString(int x, int y, const std::string &str);

  void combine(CwmWindow *xwindow, int x, int y, int op);
};
