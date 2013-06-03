#include "CwmI.h"
#include "CXAtom.h"

CwmGnome *
CwmGnome::
getInstance()
{
  static CwmGnome *instance;

  if (instance == NULL)
    instance = new CwmGnome();

  return instance;
}

void
CwmGnome::
init(CwmScreen &screen)
{
  createGnomeWindow(screen);

  createSupportedProperties(screen);

  setNumDesktops(screen);
  setCurrentDesktop(screen);
  setDesktopNames(screen);

  setListOfManagedClients(screen);
}

void
CwmGnome::
createGnomeWindow(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_SUPPORTING_WM_CHECK");

  Window xwin =
    CwmMachineInst->createWindow(None, -200, -200, 5, 5, 0, 0, NULL);

  CwmMachineInst->setWindowProperty(root_xwin, atom, xwin);
  CwmMachineInst->setWindowProperty(xwin     , atom, xwin);
}

void
CwmGnome::
createSupportedProperties(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_PROTOCOLS");

  const CXAtom *atoms[10];

  atoms[0] = &CwmMachineInst->getAtom("_WIN_WORKSPACE"      );
  atoms[1] = &CwmMachineInst->getAtom("_WIN_WORKSPACE_COUNT");
  atoms[2] = &CwmMachineInst->getAtom("_WIN_WORKSPACE_NAMES");
  atoms[3] = &CwmMachineInst->getAtom("_WIN_CLIENT_LIST"    );
  atoms[4] = &CwmMachineInst->getAtom("_WIN_LAYER"          );
  atoms[5] = &CwmMachineInst->getAtom("_WIN_STATE"          );

  CwmMachineInst->setAtomArrayProperty(root_xwin, atom, atoms, 6);
}

void
CwmGnome::
setListOfManagedClients(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_CLIENT_LIST");

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
CwmGnome::
setNumDesktops(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_WORKSPACE_COUNT");

  int num_desks = screen.getNumDesks();

  CwmMachineInst->setIntegerProperty(root_xwin, atom, num_desks);
}

void
CwmGnome::
setCurrentDesktop(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  CwmDesk *desk = screen.getCurrentDesk();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_WORKSPACE");

  int desk_num = desk->getNum();

  CwmMachineInst->setIntegerProperty(root_xwin, atom, desk_num);
}

void
CwmGnome::
setDesktopNames(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_WORKSPACE_NAMES");

  int num_names = screen.getNumDesks();

  char **names = new char * [num_names];

  for (int i = 0; i < num_names; i++) {
    string name = screen.getDesk(i)->getName();

    names[i] = strdup(name.c_str());
  }

  CwmMachineInst->setStringListProperty(root_xwin, atom, names, num_names);

  int num_desks = screen.getNumDesks();

  for (int i = 0; i < num_desks; i++)
    free(names[i]);

  delete [] names;
}

void
CwmGnome::
processRootClientMessage(XClientMessageEvent *event)
{
/*
  const CXAtom &win_layer_atom = CwmMachineInst->getAtom("_WIN_LAYER");
  const CXAtom &win_state_atom = CwmMachineInst->getAtom("_WIN_STATE");
*/

  const CXAtom &atom = CwmMachineInst->getAtom(event->message_type);

  CwmMachineInst->logf("processRootClientMessage %\n",
                       atom.getName().c_str());
}

void
CwmGnome::
setState(CwmWMWindow &window)
{
  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_STATE");

  CwmMachineInst->setIntegerProperty(window.getXWin(), atom, 0);
}

void
CwmGnome::
setDesktop(CwmWMWindow &window)
{
  int desk_num = window.getDeskNum();

  const CXAtom &atom = CwmMachineInst->getAtom("_WIN_WORKSPACE");

  CwmMachineInst->setIntegerProperty(window.getXWin(), atom, desk_num);
}
