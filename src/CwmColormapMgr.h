class CwmColormapMgr {
 private:
  CwmScreen   &screen_;
  CwmWMWindow *window_;
  Colormap     cmap_;

 public:
  CwmColormapMgr(CwmScreen &screen);
 ~CwmColormapMgr();

  void install(CwmWMWindow *window);
  void uninstall();

 private:
  void install(CwmWMWindow *window, Colormap cmap);
};
