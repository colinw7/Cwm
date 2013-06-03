#include <CwmI.h>
#include <CXtTimer.h>
#include <sys/stat.h>

class CwmMailMgrTimer : public CXtTimer {
 private:
  enum { MAIL_TIMEOUT = 30000 };

  CwmMailMgr *mgr_;

 public:
  CwmMailMgrTimer(CwmMailMgr *mgr);

  void timeOut();
};

CwmMailMgr::
CwmMailMgr()
{
  initialized_ = false;
  last_error_  = 0;
  timer_       = NULL;
}

CwmMailMgr::
~CwmMailMgr()
{
  delete timer_;
}

CwmMailMgr *
CwmMailMgr::
getInstance()
{
  static CwmMailMgr *instance;

  if (instance == NULL)
    instance = new CwmMailMgr();

  return instance;
}

bool
CwmMailMgr::
isMailWindow(CwmWMWindow *window)
{
  init();

  string res_name = window->getResName();

  return (res_name != "" && res_name.find("mail") != string::npos);
}

void
CwmMailMgr::
addMailWindow(CwmWMWindow *window)
{
  init();

  mail_windows_.push_back(window);
}

void
CwmMailMgr::
deleteMailWindow(CwmWMWindow *window)
{
  init();

  mail_windows_.remove(window);
}

void
CwmMailMgr::
init()
{
  if (initialized_)
    return;

  string value = (getenv("MAIL") ? getenv("MAIL") : "");

  if (! value.empty()) {
    dir_ = value;

    last_error_ = stat(dir_.c_str(), &last_stat_);

    timer_ = new CwmMailMgrTimer(this);

    initialized_ = true;
  }
}

void
CwmMailMgr::
timeOut()
{
  struct stat this_stat;

  int this_error = stat(dir_.c_str(), &this_stat);

  if (this_error == 0 && last_error_ == 0 &&
      this_stat.st_ctime != last_stat_.st_ctime &&
      this_stat.st_size  >  last_stat_.st_size) {
    WMWindowList::const_iterator pwindow1 = mail_windows_.begin();
    WMWindowList::const_iterator pwindow2 = mail_windows_.end  ();

    for ( ; pwindow1 != pwindow2; ++pwindow1) {
      (*pwindow1)->restore();

      (*pwindow1)->raise();
    }
  }

  memcpy(&last_stat_, &this_stat, sizeof(struct stat));

  last_error_ = this_error;
}

CwmMailMgrTimer::
CwmMailMgrTimer(CwmMailMgr *mgr) :
 CXtTimer(MAIL_TIMEOUT, CTIMER_FLAGS_REPEAT), mgr_(mgr)
{
}

void
CwmMailMgrTimer::
timeOut()
{
  mgr_->timeOut();
}
