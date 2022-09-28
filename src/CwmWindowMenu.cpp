#include <CwmI.h>

CwmWindowMenu::
CwmWindowMenu(CwmWMWindow *window1) :
 window(window1)
{
  menu_def = new CwmMenuDef();

  if (window->getMoveFunctionHint())
    menu_def->addButton("", "Move", 'M', "Alt<Key>F7",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::moveProc),
                        static_cast<CwmData>(window));

  if (window->getResizeFunctionHint())
    menu_def->addButton("", "Resize", 's', "Alt<Key>F8",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::resizeProc),
                        static_cast<CwmData>(window));

  if (window->getIconiseFunctionHint())
    menu_def->addButton("", "Iconise", 'I', "Alt<Key>F9",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::iconiseProc),
                        static_cast<CwmData>(window));

  if (window->getMaximizeFunctionHint())
    menu_def->addButton("", "Maximize", 'x', "Alt<Key>F10",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::maximizeProc),
                        static_cast<CwmData>(window));

  if (window->getMinimizeFunctionHint())
    menu_def->addButton("", "Minimize", 'n', "Alt<Key>F11",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::minimizeProc),
                        static_cast<CwmData>(window));

  if (window->getRestoreFunctionHint())
    menu_def->addButton("", "Restore", 'R', "Alt<Key>F5",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::restoreProc),
                        static_cast<CwmData>(window));

  if (window->getRaiseFunctionHint())
    menu_def->addButton("", "Raise", 'a', "Alt<Key>F2",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::raiseProc),
                        static_cast<CwmData>(window));

  if (window->getLowerFunctionHint())
    menu_def->addButton("", "Lower", 'L', "Alt<Key>F3",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::lowerProc),
                        static_cast<CwmData>(window));

  if (window->getRollUpFunctionHint())
    menu_def->addButton("", "Roll Up", 'U', "",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::rollUpProc),
                        static_cast<CwmData>(window));

  if (window->getRollDownFunctionHint())
    menu_def->addButton("", "Roll Down", 'D', "",
                        reinterpret_cast<CwmMenuProc>(&CwmWindowMenu::rollDownProc),
                        static_cast<CwmData>(window));

  if (window->getCloseFunctionHint()) {
    menu_def->addSplitter("--------");

    menu_def->addButton("", "Close", 'C', "Alt<Key>F4",
                        reinterpret_cast<CwmMenuProc>( &CwmWindowMenu::closeProc),
                        static_cast<CwmData>(window));
  }
}

CwmWindowMenu::
~CwmWindowMenu()
{
  delete menu_def;
}

void
CwmWindowMenu::
process()
{
  CwmScreen &screen = window->getScreen();

  screen.uninstallColormap();

  CwmMenu::processWindowMenu(screen, screen.getRoot(), menu_def);

  screen.installColormap(window);
}

void
CwmWindowMenu::
processAtPosition(CwmWindow *xwindow, int x, int y)
{
  CwmScreen &screen = window->getScreen();

  screen.uninstallColormap();

  CwmMenu::processWindowMenu(screen, xwindow, menu_def, x, y);

  screen.installColormap(window);
}

void
CwmWindowMenu::
moveProc(CwmWMWindow *window, CwmData)
{
  CwmMoveWindowInst->move(window);
}

void
CwmWindowMenu::
resizeProc(CwmWMWindow *window, CwmData)
{
  CwmResizeWindowInst->resize(window, CWM_RESIZE_FREE);
}

void
CwmWindowMenu::
resizeToSizeProc(CwmWMWindow *window, CwmData data)
{
  std::string *size_string = static_cast<std::string *>(data);

  int width, height;

  if (sscanf(size_string->c_str(), "%dx%d", &width, &height) != 2) {
    CwmMachineInst->logf("Invalid size string %s\n", (*size_string).c_str());
    return;
  }

  int width1  = window->getBaseWidthHint()  + width *window->getWidthIncHint() ;
  int height1 = window->getBaseHeightHint() + height*window->getHeightIncHint();

  window->resizeUser(width1, height1);
}

void
CwmWindowMenu::
iconiseProc(CwmWMWindow *window, CwmData)
{
  window->iconise();
}

void
CwmWindowMenu::
maximizeProc(CwmWMWindow *window, CwmData)
{
  window->maximize();
}

void
CwmWindowMenu::
minimizeProc(CwmWMWindow *window, CwmData)
{
  window->minimize();
}

void
CwmWindowMenu::
restoreProc(CwmWMWindow *window, CwmData)
{
  window->restoreSize();
}

void
CwmWindowMenu::
raiseProc(CwmWMWindow *window, CwmData)
{
  window->raise();
}

void
CwmWindowMenu::
lowerProc(CwmWMWindow *window, CwmData)
{
  window->lower();
}

void
CwmWindowMenu::
rollUpProc(CwmWMWindow *window, CwmData)
{
  window->rollUp();
}

void
CwmWindowMenu::
rollDownProc(CwmWMWindow *window, CwmData)
{
  window->rollDown();
}

void
CwmWindowMenu::
circulateUpProc(CwmWMWindow *window, CwmData)
{
  window->getScreen().circulateUp();
}

void
CwmWindowMenu::
circulateDownProc(CwmWMWindow *window, CwmData)
{
  window->getScreen().circulateDown();
}

void
CwmWindowMenu::
focusNextProc(CwmWMWindow *window, CwmData)
{
  CwmMachineInst->focusNext(window->getScreen());
}

void
CwmWindowMenu::
focusPrevProc(CwmWMWindow *window, CwmData)
{
  CwmMachineInst->focusPrev(window->getScreen());
}

void
CwmWindowMenu::
closeProc(CwmWMWindow *window, CwmData)
{
  window->close();
}

void
CwmWindowMenu::
processNamedMenuProc(CwmWMWindow *window, CwmData data)
{
  std::string *name = static_cast<std::string *>(data);

  CwmMenu::processNamedMenu(window, *name);
}

void
CwmWindowMenu::
pasteProc(CwmWMWindow *window, CwmData data)
{
  std::string *text = static_cast<std::string *>(data);

  if (data != 0)
    CwmPaste::pasteText(window, *text);
}

void
CwmWindowMenu::
runCommandProc(CwmWMWindow *window, CwmData data)
{
  std::string *cmd = static_cast<std::string *>(data);

  UnixCmd command(window->getScreen(), *cmd);

  command.run();
}

void
CwmWindowMenu::
runXCommandProc(CwmWMWindow *window, CwmData data)
{
  std::string *cmd = static_cast<std::string *>(data);

  UnixCmd command(window->getScreen(), *cmd);

  command.setAddDisplay(true);

  command.run();
}
