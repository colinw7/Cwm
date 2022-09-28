#include <CwmI.h>

CwmMessage *
CwmMessage::
getInstance()
{
  static CwmMessage *instance;

  if (! instance)
    instance = new CwmMessage();

  return instance;
}

CwmMessage::
CwmMessage() :
 change_count_(0)
{
  XSetWindowAttributes attr;

  attr.event_mask        = PropertyChangeMask;
  attr.cursor            = None;
  attr.override_redirect = True;

  uint attr_mask = CWEventMask | CWCursor | CWOverrideRedirect;

  xwin_ = CwmMachineInst->createWindow(0, 0, 1, 1, 0, attr_mask, &attr);

  const CXAtom &atom = CwmMachineInst->getAtom("CWM_SERVER_WINDOW");

  CwmMachineInst->setWindowProperty(atom, xwin_);

  updateChangeCount();
}

CwmMessage::
~CwmMessage()
{
  CwmMachineInst->destroyWindow(xwin_);
}

void
CwmMessage::
init()
{
}

bool
CwmMessage::
processClientMessage(XClientMessageEvent *event)
{
  if (event->window != xwin_)
    return false;

  Window      client_xwin;
  std::string value;

  CwmMachineInst->readStringClientMessage(xwin_, &client_xwin, value);

  std::vector<std::string> words;

  CStrUtil::toWords(value, words);

  auto num_words = words.size();

  if (num_words == 0)
    goto fail;

  for (uint i = 0; i < num_words; ++i)
    CwmMachineInst->log(words[i] + '\n');

  if      (words[0] == "get_windows") {
    const CwmScreen &screen = CwmMachineInst->getScreen(0);

    CwmCirculateWindowStack &window_stack = screen.getCirculateWindowStack();

    std::string str;

    for (uint i = 0; i < uint(window_stack.size()); i++) {
      auto &window_group = window_stack[i];

      if (window_group.size() == 0)
        continue;

      auto num_windows = window_group.size();

      for (uint j = 0; j < num_windows; ++j) {
        CwmWMWindow &window = window_group[j];

        if (str != "")
          str += " ";

        str += CStrUtil::toString(window.getXWin());
      }
    }

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, str);
  }
  else if (words[0] == "circulate_up") {
    const CwmScreen &screen = CwmMachineInst->getScreen(0);

    screen.circulateUp();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "circulate_down") {
    const CwmScreen &screen = CwmMachineInst->getScreen(0);

    screen.circulateDown();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "raise_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->raise();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "lower_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->lower();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "close_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->close();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "restore_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->restore();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "move_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->move();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "resize_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->resize();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "iconise_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->iconise();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "maximize_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->maximize();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "minimize_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->minimize();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "roll_up_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->rollUp();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "roll_down_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->rollDown();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "focus_window") {
    if (num_words < 2)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    window->focus();

    CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "1");
  }
  else if (words[0] == "get_screen_value") {
    const CwmScreen &screen = CwmMachineInst->getScreen(0);

    if (num_words < 2)
      goto fail;

    if      (words[1] == "width") {
      int width = screen.getWidth();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, width);
    }
    else if (words[1] == "height") {
      int height = screen.getHeight();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, height);
    }
    else {
      CwmMachineInst->log("Invalid screen value " + words[2] + "\n");
    }
  }
  else if (words[0] == "get_window_value") {
    if (num_words < 3)
      goto fail;

    CwmWMWindow *window = stringToWindow(words[1]);

    if (! window)
      goto fail;

    if      (words[2] == "x") {
      int x = window->getFrame()->getX();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, x);
    }
    else if (words[2] == "y") {
      int y = window->getFrame()->getY();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, y);
    }
    else if (words[2] == "width") {
      int width = window->getFrame()->getWidth();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, width);
    }
    else if (words[2] == "height") {
      int height = window->getFrame()->getHeight();

      CwmMachineInst->sendIntegerServerMessage(client_xwin, xwin_, height);
    }
    else {
      CwmMachineInst->log("Invalid window value " + words[2] + "\n");
    }
  }
  else
    goto fail;

  return true;

 fail:
  CwmMachineInst->log("Invalid Command " + value + "\n");

  CwmMachineInst->sendStringServerMessage(client_xwin, xwin_, "0");

  return true;
}

CwmWMWindow *
CwmMessage::
stringToWindow(const std::string &str)
{
  Window xwin = Window(CStrUtil::toInteger(str));

  auto *window = CwmMachineInst->getWindowWMWindow(xwin);

  return window;
}

void
CwmMessage::
updateChangeCount()
{
  const auto &atom = CwmMachineInst->getAtom("CHANGE_COUNT");

  CwmMachineInst->setIntegerProperty(xwin_, atom, change_count_);

  ++change_count_;
}
