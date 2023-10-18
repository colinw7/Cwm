#include <CwmI.h>

void
CwmScreenMenu::
moveProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  CwmMoveWindowInst->move(window);
}

void
CwmScreenMenu::
resizeProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  CwmResizeWindowInst->resize(window, CWM_RESIZE_FREE);
}

void
CwmScreenMenu::
iconiseProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->iconise();
}

void
CwmScreenMenu::
maximizeProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->maximize();
}

void
CwmScreenMenu::
minimizeProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->minimize();
}

void
CwmScreenMenu::
restoreProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->restoreSize();
}

void
CwmScreenMenu::
raiseProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->raise();
}

void
CwmScreenMenu::
lowerProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->lower();
}

void
CwmScreenMenu::
circulateUpProc(CwmScreen &screen, CwmData)
{
  screen.circulateUp();
}

void
CwmScreenMenu::
circulateDownProc(CwmScreen &screen, CwmData)
{
  screen.circulateDown();
}

void
CwmScreenMenu::
focusNextProc(CwmScreen &screen, CwmData)
{
  CwmMachineInst->focusNext(screen);
}

void
CwmScreenMenu::
focusPrevProc(CwmScreen &screen, CwmData)
{
  CwmMachineInst->focusPrev(screen);
}

void
CwmScreenMenu::
selectNextProc(CwmScreen &screen, CwmData)
{
  screen.selectNext();
}

void
CwmScreenMenu::
selectPrevProc(CwmScreen &screen, CwmData)
{
  screen.selectPrev();
}

void
CwmScreenMenu::
rollUpProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->rollUp();
}

void
CwmScreenMenu::
rollDownProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->rollDown();
}

void
CwmScreenMenu::
closeProc(CwmScreen &screen, CwmData)
{
  CwmWMWindow *window = CwmEventMgrInst->selectWindow(screen);

  if (! window)
    return;

  window->close();
}

void
CwmScreenMenu::
processNamedMenuProc(CwmScreen &screen, CwmData data)
{
  std::string *name = static_cast<std::string *>(data);

  CwmMenu::processNamedMenu(screen, *name);
}

void
CwmScreenMenu::
runCommandProc(CwmScreen &screen, CwmData data)
{
  std::string *cmd = static_cast<std::string *>(data);

  UnixCmd command(screen, *cmd);

  command.run();
}

void
CwmScreenMenu::
runXCommandProc(CwmScreen &screen, CwmData data)
{
  std::string *cmd = static_cast<std::string *>(data);

  UnixCmd command(screen, *cmd);

  command.setAddDisplay(true);

  command.run();
}

void
CwmScreenMenu::
setRootImageProc(CwmScreen &screen, CwmData data)
{
  std::string *name = static_cast<std::string *>(data);

  CwmDesk *desk = screen.getCurrentDesk();

  desk->setRootImage(*name);

  desk->drawRootImage();
}

void
CwmScreenMenu::
toggleDisplayedProc(CwmScreen &, CwmData)
{
}

void
CwmScreenMenu::
refreshCwmProc(CwmScreen &screen, CwmData)
{
  screen.refresh();
}

void
CwmScreenMenu::
restartCwmProc(CwmScreen &screen, CwmData data)
{
  std::string *cmd = static_cast<std::string *>(data);

  if (cmd == nullptr || *cmd == "") {
    int    argc;
    char **argv;

    CwmInst->getArgs(&argc, &argv);

    std::string cmd1;

    for (int i = 0; i < argc; ++i) {
      cmd1 += argv[i];
      cmd1 += " ";
    }

    CwmInst->restart(screen, cmd1);
  }
  else
    CwmInst->restart(screen, *cmd);
}

void
CwmScreenMenu::
quitCwmProc(CwmScreen &, CwmData)
{
  CwmInst->quit();
}

void
CwmScreenMenu::
changeDeskProc(CwmScreen &screen, CwmData data)
{
  std::string *str = static_cast<std::string *>(data);

  int desk_num = int(CStrUtil::toInteger(*str));

  screen.changeDesk(desk_num - 1);
}
