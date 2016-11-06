#include "std_c++.h"
#include "CStrUtil/CStrUtil.h"
#include "CTclApp/CTclApp.h"
#include "CXLib/CXLib.h"
#include "CwmTcl.h"
#include "CwmTclCanvas.h"

#include "CwmTcl.k"

#define CwmTclAppCommand(N) \
class N : public CTclAppCommand { \
 private: \
  CwmTcl *cwm_; \
\
 public: \
  N(CwmTcl *cwm) : \
    CTclAppCommand(cwm, #N), cwm_(cwm) { \
  } \
\
 protected: \
  bool proc(int argc, const char **argv); \
};

CwmTclAppCommand(CwmTclCheckUpdate)
CwmTclAppCommand(CwmTclExit)

CwmTclAppCommand(cwm_help)
CwmTclAppCommand(cwm_get_windows)
CwmTclAppCommand(cwm_circulate_up)
CwmTclAppCommand(cwm_circulate_down)
CwmTclAppCommand(cwm_raise_window)
CwmTclAppCommand(cwm_lower_window)
CwmTclAppCommand(cwm_close_window)
CwmTclAppCommand(cwm_restore_window)
CwmTclAppCommand(cwm_move_window)
CwmTclAppCommand(cwm_resize_window)
CwmTclAppCommand(cwm_iconise_window)
CwmTclAppCommand(cwm_maximize_window)
CwmTclAppCommand(cwm_minimize_window)
CwmTclAppCommand(cwm_roll_up_window)
CwmTclAppCommand(cwm_roll_down_window)
CwmTclAppCommand(cwm_focus_window)
CwmTclAppCommand(cwm_get_screen_value)
CwmTclAppCommand(cwm_get_window_value)

int
main(int argc, char **argv)
{
  CwmTcl app(argc, argv);
}

CwmTcl::
CwmTcl(int argc, char **argv) :
 CTclApp(argc, argv), server_xwin_(0), client_xwin_(0), canvas_(NULL),
 change_count_(0)
{
  bool interactive = true;

  for (int i = 1; i < argc; ++i)
    if (strcmp(argv[i], "-x") == 0)
      interactive = false;

  setInteractive(interactive);

  init();
}

CwmTcl::
~CwmTcl()
{
}

void
CwmTcl::
startup()
{
  CXMachineInst->setDisplay(getDisplay());

  const CXAtom &atom = CXMachineInst->getAtom("CWM_SERVER_WINDOW");

  CXMachineInst->getWindowProperty(atom, &server_xwin_);

  createWindow();
}

void
CwmTcl::
createWindow()
{
  XSetWindowAttributes attr;

  attr.event_mask        = PropertyChangeMask;
  attr.cursor            = None;
  attr.override_redirect = True;

  unsigned int attr_mask = CWEventMask | CWCursor | CWOverrideRedirect;

  client_xwin_ = CXMachineInst->createWindow(0, 0, 1, 1, 0, attr_mask, &attr);
}

void
CwmTcl::
addCommands()
{
  new CwmTclCanvasCmd(this);

  new CwmTclCheckUpdate(this);
  new CwmTclExit(this);

  new cwm_help(this);
  new cwm_get_windows(this);
  new cwm_circulate_up(this);
  new cwm_circulate_down(this);
  new cwm_raise_window(this);
  new cwm_lower_window(this);
  new cwm_close_window(this);
  new cwm_restore_window(this);
  new cwm_move_window(this);
  new cwm_resize_window(this);
  new cwm_iconise_window(this);
  new cwm_maximize_window(this);
  new cwm_minimize_window(this);
  new cwm_roll_up_window(this);
  new cwm_roll_down_window(this);
  new cwm_focus_window(this);
  new cwm_get_screen_value(this);
  new cwm_get_window_value(this);
}

string
CwmTcl::
getTclStr()
{
  return CwmTcl_TK;
}

void
CwmTcl::
addCanvas(CwmTclCanvas *canvas)
{
  canvas_ = canvas;
}

string
CwmTcl::
sendClientMessage(const string &name, int argc, const char **argv)
{
  string msg = name;

  for (int i = 1; i < argc; ++i)
    msg += " " + string(argv[i]);

  return sendClientMessage(msg);
}

string
CwmTcl::
sendClientMessage(const string &str)
{
  CXMachineInst->sendStringClientMessage(server_xwin_, client_xwin_, str);

  CXMachineInst->waitForClientMessage(client_xwin_);

  string str1;

  CXMachineInst->readStringServerMessage(client_xwin_, &server_xwin_, str1);

  return str1;
}

void
CwmTcl::
redraw()
{
  if (canvas_ == NULL)
    return;

  eval("CwmTclRedraw");
}

void
CwmTcl::
mousePress(int button, int x, int y)
{
  eval("CwmTclMousePress %d %d %d", button, x, y);
}

void
CwmTcl::
keyPress(const string &key)
{
  eval("CwmTclKeyPress {%s}", key.c_str());
}

void
CwmTcl::
checkUpdate()
{
  const CXAtom &atom = CXMachineInst->getAtom("CHANGE_COUNT");

  int change_count;

  if (! CXMachineInst->getIntegerProperty(server_xwin_, atom, &change_count))
    return;

  if (change_count != change_count_)
    eval("CwmTclRedraw");

  change_count_ = change_count;
}

bool
CwmTclCheckUpdate::
proc(int, const char **)
{
  cwm_->checkUpdate();

  return true;
}

bool
CwmTclExit::
proc(int, const char **)
{
  exit(0);
}

bool
cwm_help::
proc(int, const char **)
{
  cout << "cwm_get_windows" << endl;
  cout << "cwm_circulate_up" << endl;
  cout << "cwm_circulate_down" << endl;
  cout << "cwm_raise_window <window>" << endl;
  cout << "cwm_lower_window <window>" << endl;
  cout << "cwm_close_window <window>" << endl;
  cout << "cwm_restore_window <window>" << endl;
  cout << "cwm_move_window <window>" << endl;
  cout << "cwm_resize_window <window>" << endl;
  cout << "cwm_iconise_window <window>" << endl;
  cout << "cwm_maximize_window <window>" << endl;
  cout << "cwm_minimize_window <window>" << endl;
  cout << "cwm_roll_up_window <window>" << endl;
  cout << "cwm_roll_down_window <window>" << endl;
  cout << "cwm_focus_window <window>" << endl;
  cout << "cwm_get_screen_value <name>" << endl;
  cout << "cwm_get_window_value <window> <name>" << endl;

  return true;
}

bool
cwm_get_windows::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("get_windows", argc, argv);

  vector<int> integers;

  CStrUtil::toIntegers(str, integers);

  setIntegerArrayResult(integers);

  return true;
}

bool
cwm_circulate_up::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("circulate_up", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_circulate_down::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("circulate_down", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_raise_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("raise_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_lower_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("lower_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_close_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("close_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_restore_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("restore_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_move_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("move_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_resize_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("resize_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_iconise_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("iconise_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_maximize_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("maximize_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_minimize_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("minimize_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_roll_up_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("roll_up_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_roll_down_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("roll_down_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_focus_window::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("focus_window", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_get_screen_value::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("get_screen_value", argc, argv);

  setStringResult(str);

  return true;
}

bool
cwm_get_window_value::
proc(int argc, const char **argv)
{
  string str = cwm_->sendClientMessage("get_window_value", argc, argv);

  setStringResult(str);

  return true;
}
