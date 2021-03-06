#define CwmToolBarIconDefMgrInst CwmToolBarIconDefMgr::getInstance()

class CwmToolBarIconDef;

class CwmToolBarIconDefMgr {
 public:
  static CwmToolBarIconDefMgr *getInstance();

  CwmToolBarIconDefMgr();
 ~CwmToolBarIconDefMgr();

  void addToolBarIcon(const std::string &icon, const std::string &label,
                      const std::string &command, bool add_display,
                      const std::string &bg, const std::string &fg);

  void addToolBarIcons(CwmToolBar &toolbar);

 private:
  typedef std::list<CwmToolBarIconDef *> ToolBarIconDefList;

  ToolBarIconDefList toolbar_icons_;
};

class CwmToolBarIconDef {
 public:
  CwmToolBarIconDef(const std::string &icon, const std::string &label,
                    const std::string &command, bool add_display,
                    const std::string &bg, const std::string &fg);
 ~CwmToolBarIconDef();

  std::string getIcon      () const { return icon_       ; }
  std::string getLabel     () const { return label_      ; }
  std::string getCommand   () const { return command_    ; }
  bool        getAddDisplay() const { return add_display_; }
  std::string getBg        () const { return bg_         ; }
  std::string getFg        () const { return fg_         ; }

 private:
  std::string icon_;
  std::string label_;
  std::string command_;
  bool        add_display_;
  std::string bg_;
  std::string fg_;
};
