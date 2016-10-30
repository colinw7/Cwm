#include <CwmAdvHints.h>
#include <CwmScreen.h>
#include <CwmWindow.h>
#include <CwmMachine.h>
#include <CwmDesk.h>
#include <CwmWMWindow.h>
#include <CwmWindowStack.h>

#include <CXAtom.h>
#include <CXNamedEvent.h>

CwmAdvHints *
CwmAdvHints::
getInstance()
{
  static CwmAdvHints *instance;

  if (! instance)
    instance = new CwmAdvHints();

  return instance;
}

void
CwmAdvHints::
init(CwmScreen &screen)
{
  createAdvHintsWindow(screen);

  createSupportedProperties(screen);

  setWMName();

  setNumDesktops(screen);

  setDesktopGeometry(screen);
  setDesktopViewport(screen);
  setWorkArea(screen);

  setCurrentDesktop(screen);

  setDesktopNames(screen);

  setClientList(screen);
  setStackedClientList(screen);
}

void
CwmAdvHints::
createAdvHintsWindow(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_SUPPORTING_WM_CHECK");

  xwin_ = CwmMachineInst->createWindow(None, -200, -200, 5, 5, 0, 0, 0);

  CwmMachineInst->setWindowProperty(root_xwin, atom, xwin_);
  CwmMachineInst->setWindowProperty(xwin_    , atom, xwin_);
}

void
CwmAdvHints::
setWMName()
{
  const CXAtom &atom = CwmMachineInst->getAtom("_NET_WM_NAME");

  CwmMachineInst->setStringProperty(xwin_, atom, "Cwm");
}

void
CwmAdvHints::
createSupportedProperties(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_SUPPORTED");

  const CXAtom *atoms[20];

  atoms[ 0] = &CwmMachineInst->getAtom("_NET_WM_NAME"             );
  atoms[ 1] = &CwmMachineInst->getAtom("_NET_CLIENT_LIST"         );
  atoms[ 2] = &CwmMachineInst->getAtom("_NET_CLIENT_LIST_STACKING");
  atoms[ 3] = &CwmMachineInst->getAtom("_NET_NUMBER_OF_DESKTOPS"  );
  atoms[ 4] = &CwmMachineInst->getAtom("_NET_DESKTOP_GEOMETRY"    );
  atoms[ 5] = &CwmMachineInst->getAtom("_NET_DESKTOP_VIEWPORT"    );
  atoms[ 6] = &CwmMachineInst->getAtom("_NET_CURRENT_DESKTOP"     );
  atoms[ 7] = &CwmMachineInst->getAtom("_NET_DESKTOP_NAMES"       );
  atoms[ 8] = &CwmMachineInst->getAtom("_NET_ACTIVE_WINDOW"       );
  atoms[ 9] = &CwmMachineInst->getAtom("_NET_WORKAREA"            );
  atoms[10] = &CwmMachineInst->getAtom("_NET_LAYER"               );
  atoms[11] = &CwmMachineInst->getAtom("_NET_STATE"               );

  CwmMachineInst->setAtomArrayProperty(root_xwin, atom, atoms, 12);
}

void
CwmAdvHints::
setClientList(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_CLIENT_LIST");

  CwmDesk *desk = screen.getCurrentDesk();

  CwmDesk::WMWindowList windows = desk->getWindows();

  Window *xwins = new Window [windows.size()];

  CwmDesk::WMWindowList::const_iterator pwindow1 = windows.begin();
  CwmDesk::WMWindowList::const_iterator pwindow2 = windows.end  ();

  int num_xwins = 0;

  for ( ; pwindow1 != pwindow2; ++pwindow1)
    xwins[num_xwins++] = (*pwindow1)->getXWin();

  CwmMachineInst->setWindowArrayProperty(root_xwin, atom, xwins, num_xwins);

  delete [] xwins;
}

void
CwmAdvHints::
setStackedClientList(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_CLIENT_LIST_STACKING");

  CwmCirculateWindowStack &window_stack = screen.getCirculateWindowStack();

  typedef std::vector<Window> WindowIds;

  WindowIds windowIds;

  for (int i = 0; i < window_stack.size(); i++) {
    CwmWindowGroup &window_group = window_stack[i];

    if (window_group.size() == 0)
      continue;

    for (int i = 0; i < window_group.size(); i++) {
      CwmWMWindow &window = window_group[i];

      windowIds.push_back(window.getXWin());
    }
  }

  int n = windowIds.size();

  for (int i = 0; i < n/2; ++i)
    std::swap(windowIds[i], windowIds[n - i - 1]);

  CwmMachineInst->setWindowArrayProperty(root_xwin, atom, &windowIds[0], windowIds.size());
}

void
CwmAdvHints::
setNumDesktops(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_NUMBER_OF_DESKTOPS");

  int num_desks = screen.getNumDesks();

  CwmMachineInst->setIntegerProperty(root_xwin, atom, num_desks);
}

void
CwmAdvHints::
setCurrentDesktop(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  CwmDesk *desk = screen.getCurrentDesk();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_CURRENT_DESKTOP");

  int desk_num = desk->getNum();

  CwmMachineInst->setIntegerProperty(root_xwin, atom, desk_num);
}

void
CwmAdvHints::
setDesktopGeometry(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_DESKTOP_GEOMETRY");

  int size[2];

  size[0] = screen.getWidth ();
  size[1] = screen.getHeight();

  CwmMachineInst->setIntegerArrayProperty(root_xwin, atom, size, 2);
}

void
CwmAdvHints::
setDesktopViewport(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_DESKTOP_VIEWPORT");

  int size[2];

  size[0] = 0;
  size[1] = 0;

  CwmMachineInst->setIntegerArrayProperty(root_xwin, atom, size, 2);
}

void
CwmAdvHints::
setWorkArea(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_WORKAREA");

  int size[4];

  size[0] = 0;
  size[1] = 0;
  size[2] = screen.getWidth ();
  size[3] = screen.getHeight();

  CwmMachineInst->setIntegerArrayProperty(root_xwin, atom, size, 4);
}

void
CwmAdvHints::
setDesktopNames(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_DESKTOP_NAMES");

  int num_names = screen.getNumDesks();

  char **names = new char * [num_names];

  for (int i = 0; i < num_names; i++) {
    std::string name = screen.getDesk(i)->getName();

    names[i] = strdup(name.c_str());
  }

  CwmMachineInst->setStringListProperty(root_xwin, atom, names, num_names);

  int num_desks = screen.getNumDesks();

  for (int i = 0; i < num_desks; i++)
    free(names[i]);

  delete [] names;
}

bool
CwmAdvHints::
processRootClientMessage(XClientMessageEvent *event)
{
  //const CXAtom &layer_atom              = CwmMachineInst->getAtom("_NET_LAYER");
  //const CXAtom &state_atom              = CwmMachineInst->getAtom("_NET_STATE");
  //const CXAtom &close_window_atom       = CwmMachineInst->getAtom("_NET_CLOSE_WINDOW");
  //const CXAtom &move_resize_window_atom = CwmMachineInst->getAtom("_NET_MOVERESIZE_WINDOW");
  //const CXAtom &restack_window_atom     = CwmMachineInst->getAtom("_NET_RESTACK_WINDOW");
  //const CXAtom &request_frame_atom      = CwmMachineInst->getAtom("_NET_REQUEST_FRAME_EXTENTS");

  const CXAtom &activeAtom = CwmMachineInst->getAtom("_NET_ACTIVE_WINDOW");

  if (event->message_type == activeAtom.getXAtom()) {
    Window xwin = event->window;

    CwmWMWindow *win = CwmMachineInst->getWindowWMWindow(xwin);

    if (win) {
      CwmMachineInst->raiseWindow(xwin);

      CwmMachineInst->setFocusWindow(win);
    }
  }
  else {
    const CXAtom &atom = CwmMachineInst->getAtom(event->message_type);

    CwmMachineInst->logf("processRootClientMessage %\n", atom.getName().c_str());
  }

  return false;
}

void
CwmAdvHints::
setActiveWindow(CwmScreen &screen, CwmWMWindow *window)
{
  const CXAtom &atom = CwmMachineInst->getAtom("_NET_ACTIVE_WINDOW");

  if (window)
    CwmMachineInst->setWindowProperty(screen.getRoot()->getXWin(), atom, window->getXWin());
  else
    CwmMachineInst->setWindowProperty(screen.getRoot()->getXWin(), atom, 0);
}

void
CwmAdvHints::
setState(CwmWMWindow &window)
{
  const CXAtom &atom = CwmMachineInst->getAtom("_NET_STATE");

  CwmMachineInst->setIntegerProperty(window.getXWin(), atom, 0);
}

void
CwmAdvHints::
setDesktop(CwmWMWindow &window)
{
  int desk_num = window.getDeskNum();

  const CXAtom &atom = CwmMachineInst->getAtom("_NET_CURRENT_DESKTOP");

  CwmMachineInst->setIntegerProperty(window.getXWin(), atom, desk_num);
}
