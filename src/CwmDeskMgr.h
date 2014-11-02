class CwmDesk;
class CwmDeskMgr;
class CwmToolBarMgr;
class CwmToolBar;

typedef void (*CwmDeskMgrNotifyProc)
                (CwmDeskMgr *desk_mgr, CwmDeskMgrNotifyType type, CwmData data);

class CwmDeskMgrNotifyData {
 public:
  CwmDeskMgrNotifyData(CwmDeskMgrNotifyType type, CwmDeskMgrNotifyProc proc, CwmData data) :
   type_(type), proc_(proc), data_(data) {
  }

 ~CwmDeskMgrNotifyData() { }

  bool isType(CwmDeskMgrNotifyType type) const {
    return (type_ == type);
  }

  void call(CwmDeskMgr *desk_mgr) {
    proc_(desk_mgr, type_, data_);
  }

  bool match(CwmDeskMgrNotifyType type, CwmDeskMgrNotifyProc proc, CwmData data) {
    return ((type_ == type) && (proc_ == proc) && (data_ == data));
  }

  void setCalled(bool flag) { called_ = flag; }
  bool getCalled() const { return called_; }

 private:
  CwmDeskMgrNotifyType type_;
  CwmDeskMgrNotifyProc proc_;
  CwmData              data_;
  bool                 called_;
};

#define CwmDeskResourceMgrInst CwmDeskResourceMgr::getInstance()

class CwmDeskResourceMgr {
 public:
  static CwmDeskResourceMgr *getInstance();

  CwmDeskResourceMgr();
 ~CwmDeskResourceMgr();

  std::string getDeskName(int desk_num);
  bool        getDeskDisplayIcons(int desk_num);
  bool        getDeskDisplayToolBar(int desk_num);
  std::string getDeskImage(int desk_num);

  void setDeskName(int desk_num, const std::string &name);
  void setDeskDisplayIcons(int desk_num, bool display_icons);
  void setDeskDisplayToolBar(int desk_num, bool display_toolbar);
  void setDeskImage(int desk_num, const std::string &filename);

 private:
  typedef std::map<int,std::string> DeskNameMap;
  typedef std::map<int,bool>        DeskDisplayIconsMap;
  typedef std::map<int,bool>        DeskDisplayToolbarMap;
  typedef std::map<int,std::string> DeskImageNameMap;

  DeskNameMap           desk_name_;
  DeskDisplayIconsMap   desk_display_icons_;
  DeskDisplayToolbarMap desk_display_toolbar_;
  DeskImageNameMap      desk_image_name_;
};

class CwmDeskMgr {
 public:
  CwmDeskMgr(CwmScreen &screen, int num_desks);
 ~CwmDeskMgr();

  CwmScreen &getScreen() const { return screen_; }

  void changeDesk(int desk_num);
  void changeDesk(CwmDesk *desk);

  void processMenu();

  int getNumDesks() const { return desks_.size(); }

  int      getDeskNum(CwmWMWindow *window);
  CwmDesk *getDesk(int num);
  CwmDesk *getDesk(CwmWMWindow *window);

  int      getCurrentDeskNum() const;
  CwmDesk *getCurrentDesk() const { return current_desk_; }

  void addNotifyProc(CwmDeskMgrNotifyType type, CwmDeskMgrNotifyProc proc, void *data);
  void removeNotifyProc(CwmDeskMgrNotifyType type, CwmDeskMgrNotifyProc proc, void *data);

  void enable();
  void disable();

  CwmMenuDef *getMenuDef();

  void callNotifyProcs(CwmDeskMgrNotifyType type);

  static void moveToDesk(CwmDeskMgr *desk_mgr, CwmDesk *desk);

 private:
  typedef std::list<CwmDeskMgrNotifyData *> DeskMgrNotifyDataList;
  typedef std::vector<CwmDesk *>            DeskList;

  CwmScreen             &screen_;
  DeskList               desks_;
  CwmDesk               *current_desk_;
  DeskMgrNotifyDataList  notify_procs_;
  CwmMenuDef            *menu_def_;
};

class CwmDesk {
 public:
  typedef std::list<CwmWMWindow *> WMWindowList;

 public:
  CwmDesk(CwmDeskMgr *mgr, int num);
 ~CwmDesk();

  void addWMWindow(CwmWMWindow *window);
  void removeWMWindow(CwmWMWindow *window);

  void hide();
  void show();

  void setRootImage(const std::string &spec);
  void drawRootImage();

  CwmDeskMgr     *getDeskMgr    () const { return mgr_        ; }
  int             getNum        () const { return num_        ; }
  std::string     getName       () const { return name_       ; }
  char            getMnemonic   () const { return mnemonic_   ; }
  CwmDeskIconMgr *getDeskIconMgr() const { return icon_mgr_   ; }
  CwmToolBarMgr  *getToolBarMgr () const { return toolbar_mgr_; }

  void setName(const std::string &name) { name_ = name; }
  void setMnemonic(char mnemonic) { mnemonic_ = mnemonic; }

  WMWindowList getWindows() const { return windows_; }

  CwmScreen  &getScreen () const;
  CwmToolBar *getToolBar() const;

  void enable();
  void disable();

 private:
  void initRootImage();

 private:
  CwmDeskMgr     *mgr_;
  int             num_;
  std::string     name_;
  char            mnemonic_;
  WMWindowList    windows_;
  std::string     root_image_spec_;
  CwmRootImage   *root_image_;
  CwmDeskIconMgr *icon_mgr_;
  CwmToolBarMgr  *toolbar_mgr_;
};
