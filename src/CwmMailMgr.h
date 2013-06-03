#define CwmMailMgrInst CwmMailMgr::getInstance()

class CwmMailMgrTimer;

class CwmMailMgr {
 private:
  typedef std::list<CwmWMWindow *> WMWindowList;

  bool             initialized_;
  std::string      dir_;
  struct stat      last_stat_;
  int              last_error_;
  CwmMailMgrTimer *timer_;
  WMWindowList     mail_windows_;

 public:
  static CwmMailMgr *getInstance();

 ~CwmMailMgr();

  bool isMailWindow(CwmWMWindow *window);

  void addMailWindow(CwmWMWindow *window);
  void deleteMailWindow(CwmWMWindow *window);

  void timeOut();

 private:
  CwmMailMgr();

  void init();
};
