class CwmCustomDeskIcon;

class CwmDeskIconMgr {
  typedef std::map<Window,CwmDeskIcon *> CwmDeskIconMap;

 private:
  CwmDesk &desk_;

  CwmDeskIconMap desk_icon_map_;

  CXNamedEvent *move_event_;
  CXNamedEvent *lower_event_;
  CXNamedEvent *raise_event_;
  CXNamedEvent *close_event_;
  CXNamedEvent *restore_event_;

 public:
  CwmDeskIconMgr(CwmDesk &desk);
 ~CwmDeskIconMgr();

  CwmDesk &getDesk() { return desk_; }

  bool isCurrentDesk(CwmDeskMgr *desk_mgr);
  bool isWindowOnDesk(CwmWMWindow *window);

  void addDeskIcon(CwmDeskIcon *icon);
  void removeDeskIcon(CwmDeskIcon *icon);

  void mapIcons();
  void unmapIcons();

  CwmDeskIcon *lookup(CwmWMWindow *window);
  CwmDeskIcon *lookup(CwmWindow *xwindow);
  CwmDeskIcon *lookup(Window xwin);

  CXNamedEvent *getMoveEvent();
  CXNamedEvent *getLowerEvent();
  CXNamedEvent *getRaiseEvent();
  CXNamedEvent *getRestoreEvent();
  CXNamedEvent *getCloseEvent();

 private:
  static void createNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                               CwmData data);
  static void destroyNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                CwmData data);
  static void iconiseNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                CwmData data);
  static void restoreNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                CwmData data);
  static void deskChangeStartNotifyProc(CwmDeskMgr *desk_mgr,
                                        CwmDeskMgrNotifyType type,
                                        CwmData data);
  static void deskChangeEndNotifyProc(CwmDeskMgr *desk_mgr,
                                      CwmDeskMgrNotifyType type,
                                      CwmData data);
};

class CwmDeskIcon {
 private:
  CwmDeskIconMgr &mgr_;
  CwmDesk        &desk_;
  CwmScreen      &screen_;
  CwmWMWindow    *window_;
  CwmWindow      *xwindow_;
  CwmImage       *image_;
  CFmtString     *fmt_string_;
  CwmGraphics    *graphics_;
  CwmXPixmap     *pixmap_mask_;
  CwmMask        *label_mask_;
  bool            mask_created_;
  int             icon_x_;
  int             icon_y_;
  int             icon_width_;
  int             icon_height_;
  int             pixmap_dx_;
  int             pixmap_dy_;
  int             pixmap_offset_;
  int             pixmap_width_;
  int             pixmap_height_;
  int             label_dx_;
  int             label_dy_;
  int             label_offset_;
  int             label_width_;
  int             label_height_;
  CwmMenuDef     *menu_def_;

 public:
  CwmDeskIcon(CwmDeskIconMgr &mgr, CwmWMWindow *window);
 ~CwmDeskIcon();

  CwmScreen   &getScreen () const { return screen_ ; }
  CwmWMWindow *getWindow () const { return window_ ; }
  CwmWindow   *getXWindow() const { return xwindow_; }

  Window getXWin() const;

  void move();
  void raise();
  void lower();
  void restore();
  void close();
  void redraw();
  void map();
  void unmap();

  bool getMapped() const;

  void setBackground(CwmGraphics *graphics);

  bool isWindow(CwmWindow *xwindow);
  bool isWindow(Window xwin);

  void processKeyPress(XKeyPressedEvent *);

  void focusIn();
  void focusOut();

  void processMenu();

  static void restoreProc(CwmDeskIcon *icon, CwmData data);
  static void moveProc(CwmDeskIcon *icon, CwmData data);
  static void raiseProc(CwmDeskIcon *icon, CwmData data);
  static void lowerProc(CwmDeskIcon *icon, CwmData data);
  static void closeProc(CwmDeskIcon *icon, CwmData data);
  static void circulateUpProc(CwmDeskIcon *icon, CwmData data);
  static void circulateDownProc(CwmDeskIcon *icon, CwmData data);
  static void focusNextProc(CwmDeskIcon *icon, CwmData data);
  static void focusPrevProc(CwmDeskIcon *icon, CwmData data);
  static void namedMenuProc(CwmDeskIcon *icon, CwmData name);

  static void focusInNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                CwmData data);
  static void focusOutNotifyProc(CwmWMWindow *window, CwmWindowNotifyType type,
                                 CwmData data);

 private:
  void createMask();

  void addToGrid();
  void removeFromGrid();

  void calcSize();

  void getLabelSize(int *width, int *height);

  void createMenuDef();

  static void doubleClickProc(CwmWindow *xwindow,
                              CwmData data, CwmData detail);
};

#define CwmCustomDeskIconMgrInst CwmCustomDeskIconMgr::getInstance()

class CwmCustomDeskIconMgr {
  typedef vector<CwmCustomDeskIcon *> CustomDeskIconList;

 private:
  CustomDeskIconList icons_;

 public:
  static CwmCustomDeskIconMgr *getInstance();

  void        setIcon(const std::string &pattern, const std::string &icon);
  void        setIconSmall(const std::string &pattern, const std::string &icon);
  std::string getIcon(CwmWMWindow *window);
  std::string getIconSmall(CwmWMWindow *window);
  void        deleteAll();

 private:
  CwmCustomDeskIcon *lookup(const std::string &pattern);
};

class CwmCustomDeskIcon {
 private:
  std::string  pattern_;
  CGlob       *compile_;
  std::string  icon_;
  std::string  icon_small_;

 public:
  CwmCustomDeskIcon(const std::string &pattern);
 ~CwmCustomDeskIcon();

  void setIcon(const std::string &icon) { icon_ = icon; }
  void setIconSmall(const std::string &icon) { icon_small_ = icon; }

  std::string getIcon() const { return icon_; }
  std::string getIconSmall() const { return icon_small_; }

  bool isPattern(const std::string &pattern) { return (pattern_ == pattern); }

  bool compare(const std::string &name);
};
