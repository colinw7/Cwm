#define CwmCmdIconMgrInst CwmCmdIconMgr::getInstance()

class CwmCmdIconMgr {
 public:
  static CwmCmdIconMgr *getInstance();

  CwmCmdIconMgr();
 ~CwmCmdIconMgr();

  void addCmdIcon(CwmScreen &screen, CwmCustomIcon &icon_def);

  CwmCmdIcon *lookup(const CwmWindow &xwindow);
  CwmCmdIcon *lookup(Window xwin);

  void deleteAll();

 private:
  typedef std::vector<CwmCmdIcon *>      CmdIconList;
  typedef std::map<Window, CwmCmdIcon *> CmdIconMap;

  CmdIconList command_icon_list_;
  CmdIconMap  command_icon_map_;
};

class CwmCmdIcon {
 public:
  CwmCmdIcon(CwmScreen &screen, CwmCustomIcon &icon_def);
 ~CwmCmdIcon();

  void redraw();
  void invoke();
  void move();

  CwmWindow *getXWindow() { return xwindow_; }

 private:
  void createMask();

  void reposition();

  static void invokeProc(CwmWindow *xwindow, CwmData data, CwmData detail);

 private:
  CwmScreen   &screen_;
  CwmWindow   *xwindow_;
  CwmImage    *image_;
  CwmGraphics *graphics_;
  CwmXPixmap  *pixmap_mask_;
  CwmMask     *label_mask_;
  int          icon_x_;
  int          icon_y_;
  int          icon_width_;
  int          icon_height_;
  int          pixmap_dx_;
  int          pixmap_width_;
  int          pixmap_height_;
  int          label_dx_;
  int          label_width_;
  int          label_height_;
  std::string  label_;
  std::string  command_;
};
