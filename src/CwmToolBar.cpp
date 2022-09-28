#include <CwmToolBarI.h>

CwmToolBarMgr::
CwmToolBarMgr(CwmDesk &desk1) :
 desk_(desk1)
{
  toolbar_ = new CwmToolBar(*this);

  CwmDeskMgr *desk_mgr = desk_.getDeskMgr();

  desk_mgr->addNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_START,
                          &CwmToolBarMgr::deskChangeStartNotifyProc,
                          this);
  desk_mgr->addNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_END,
                          &CwmToolBarMgr::deskChangeEndNotifyProc,
                          this);
}

CwmToolBarMgr::
~CwmToolBarMgr()
{
  CwmDeskMgr *desk_mgr = desk_.getDeskMgr();

  desk_mgr->removeNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_START,
                             &CwmToolBarMgr::deskChangeStartNotifyProc,
                             this);
  desk_mgr->removeNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_END,
                             &CwmToolBarMgr::deskChangeEndNotifyProc,
                             this);

  delete toolbar_;
}

void
CwmToolBarMgr::
deskChangeStartNotifyProc(CwmDeskMgr *desk_mgr, CwmDeskMgrNotifyType, CwmData data)
{
  CwmToolBarMgr *mgr = static_cast<CwmToolBarMgr *>(data);

  if (desk_mgr->getCurrentDesk() == &mgr->desk_)
    mgr->toolbar_->unmap();
}

void
CwmToolBarMgr::
deskChangeEndNotifyProc(CwmDeskMgr *desk_mgr, CwmDeskMgrNotifyType, CwmData data)
{
  CwmToolBarMgr *mgr = static_cast<CwmToolBarMgr *>(data);

  if (desk_mgr->getCurrentDesk() == &mgr->desk_)
    mgr->toolbar_->map();
}

void
CwmToolBarMgr::
enable()
{
  toolbar_->enable();
}

void
CwmToolBarMgr::
disable()
{
  toolbar_->disable();
}

CwmToolBar::
CwmToolBar(CwmToolBarMgr &mgr1) :
 mgr_(mgr1)
{
  CwmScreen &screen = getScreen();

  //------

  Pixel fg = CwmResourceDataInst->getToolBarForeground(screen);
  Pixel bg = CwmResourceDataInst->getToolBarBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getToolBarFont(screen);

  graphics_ = new CwmGraphics(screen, font, color);

  //------

  int height1 = CwmResourceDataInst->getToolBarHeight();

  if (height1 <= 0)
    height1 = graphics_->getFontHeight() + 10;

  int x      = 0;
  int y      = screen.getHeight() - height1;
  int width  = screen.getWidth();
  int height = height1;

  border_ = 2;

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ =
    new CwmWindow(screen, screen.getRoot(), x, y, width, height,
                   event_mask, CWM_CURSOR_TITLE);

  xwindow_->setBackground(graphics_);

  //------

  show_clock_ = CwmResourceDataInst->getToolBarShowClock();

  add_icons_   = CwmResourceDataInst->getToolBarAddIcons();
  add_windows_ = CwmResourceDataInst->getToolBarAddWindows();

  time_format_ = CwmResourceDataInst->getToolBarClockTimeFormat();

  //------

  icon_area_x1_ = 2;
  icon_area_x2_ = width - 2;

  //------

  if (show_clock_) {
    clock_ = new CwmToolBarClock(*this);

    icon_area_x2_ -= clock_->getWidth();
  }
  else
    clock_ = 0;

  //------

  menu_ = new CwmToolBarMenu(*this);

  icon_area_x1_ += menu_->getWidth();

  //------

  CwmToolBarIconDefMgrInst->addToolBarIcons(*this);

  //------

  icon_area_ = new CwmToolBarIconArea(*this, icon_area_x1_, icon_area_x2_ - icon_area_x1_);

  //------

  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_CREATE, &CwmToolBar::createNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_ICONISE, &CwmToolBar::iconiseNotifyProc, this);
}

CwmToolBar::
~CwmToolBar()
{
  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_CREATE, &CwmToolBar::createNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_ICONISE, &CwmToolBar::iconiseNotifyProc, this);

  //------

  delete graphics_;

  delete icon_area_;

  delete clock_;

  delete menu_;

  delete xwindow_;
}

void
CwmToolBar::
addCmdIcon(CwmToolBarIconDef *icon_def)
{
  CwmToolBarCmd *cmd_icon =
    new CwmToolBarCmd(*this, icon_def, icon_area_x1_);

  cmd_icons_.push_back(cmd_icon);

  icon_area_x1_ += cmd_icon->getWidth();
}

void
CwmToolBar::
addIcon(CwmWMWindow *window)
{
  icon_area_->addIcon(window);
}

void
CwmToolBar::
removeIcon(CwmWMWindow *window)
{
  icon_area_->removeIcon(window);
}

CwmScreen &
CwmToolBar::
getScreen() const
{
  return mgr_.getDesk().getScreen();
}

void
CwmToolBar::
redraw()
{
  graphics_->drawButtonOut(xwindow_, 0, 0, xwindow_->getWidth(), xwindow_->getHeight(), border_);

  if (clock_ != 0)
    clock_->redraw();

  menu_->redraw();

  ToolBarCmdList::const_iterator picon1 = cmd_icons_.begin();
  ToolBarCmdList::const_iterator picon2 = cmd_icons_.end  ();

  for ( ; picon1 != picon2; ++picon1)
    (*picon1)->redraw();

  icon_area_->redraw();
}

bool
CwmToolBar::
isWindow(CwmWindow *xwindow1)
{
  return (xwindow_ == xwindow1);
}

bool
CwmToolBar::
isWindow(Window xwin)
{
  return (xwindow_->getXWin() == xwin);
}

bool
CwmToolBar::
isMenuWindow(CwmWindow *xwindow)
{
  return (menu_->getXWindow() == xwindow);
}

bool
CwmToolBar::
isMenuWindow(Window xwin)
{
  return (menu_->getXWindow()->getXWin() == xwin);
}

bool
CwmToolBar::
isToolIcon(CwmWindow *xwindow)
{
  return (getToolIcon(xwindow) != 0);
}

bool
CwmToolBar::
isToolIcon(Window xwin)
{
  return (getToolIcon(xwin) != 0);
}

bool
CwmToolBar::
isToolIcon(CwmWMWindow *window)
{
  return (getToolIcon(window) != 0);
}

CwmToolBarIcon *
CwmToolBar::
getToolIcon(CwmWindow *xwindow)
{
  return icon_area_->getIcon(xwindow);
}

CwmToolBarIcon *
CwmToolBar::
getToolIcon(Window xwin)
{
  return icon_area_->getIcon(xwin);
}

CwmToolBarIcon *
CwmToolBar::
getToolIcon(CwmWMWindow *window)
{
  return icon_area_->getIcon(window);
}

void
CwmToolBar::
processMenu()
{
  CwmScreen &screen = getScreen();

  CwmCirculateWindowStack &window_stack = screen.getCirculateWindowStack();

  CwmMenuDef *menu_def = buildMenu(window_stack);

  CwmMenu::processWindowMenu(screen, xwindow_, menu_def);

  delete menu_def;
}

void
CwmToolBar::
processMenuMenu()
{
  menu_->processMenu();
}

CwmMenuDef *
CwmToolBar::
buildMenu(CwmCirculateWindowStack &window_stack)
{
  auto *menu_def = new CwmMenuDef();

  menu_def->addTitle   ("Window List");
  menu_def->addSplitter("-----------");

  for (uint i = 0; i < uint(window_stack.size()); i++) {
    CwmWindowGroup &window_group = window_stack[i];

    if (window_group.size() == 0)
      continue;

    CwmWMWindow &window = window_group.getBottomWindow();

    if (window_group.size() > 1) {
      CwmMenuDef *menu_def1 = buildMenuCascade(window_group);

      menu_def->addCascade(window.getName(), menu_def1);
    }
    else
      menu_def->addButton("", window.getName(), '\0', "",
                          reinterpret_cast<CwmMenuProc>(raiseWindowProc),
                          static_cast<CwmData>(&window));
  }

  return menu_def;
}

CwmMenuDef *
CwmToolBar::
buildMenuCascade(CwmWindowGroup &window_group)
{
  auto *menu_def = new CwmMenuDef();

  CwmWMWindow &bottom_window = window_group.getBottomWindow();

  for (uint i = 0; i < uint(window_group.size()); i++) {
    CwmWMWindow &window = window_group[i];

    if (&window != &bottom_window)
      menu_def->addButton("", window.getName(), '\0', "",
                          reinterpret_cast<CwmMenuProc>(raiseWindowProc),
                          static_cast<CwmData>(&window));
  }

  return menu_def;
}

void
CwmToolBar::
raiseWindowProc(CwmWMWindow *window, CwmData)
{
  if (window->isIconised())
    window->restore();
  else
    window->raise();

  CwmToolBar *toolbar = window->getDesk()->getToolBar();

  if (toolbar != 0 && toolbar->getAddWindows())
    toolbar->redraw();
}

void
CwmToolBar::
map()
{
  xwindow_->map();
}

void
CwmToolBar::
unmap()
{
  xwindow_->unmap();
}

void
CwmToolBar::
enable()
{
  if (clock_ != 0)
    clock_->enable();
}

void
CwmToolBar::
disable()
{
  if (clock_ != 0)
    clock_->disable();
}

int
CwmToolBar::
getWidth() const
{
  return xwindow_->getWidth();
}

int
CwmToolBar::
getHeight() const
{
  return xwindow_->getHeight();
}

void
CwmToolBar::
createNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  if (window->getToolBarSkip())
    return;

  CwmToolBar *toolbar = static_cast<CwmToolBar *>(data);

  if (toolbar->getAddWindows())
    toolbar->addIcon(window);
}

void
CwmToolBar::
iconiseNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  if (window->getToolBarSkip())
    return;

  CwmToolBar *toolbar = static_cast<CwmToolBar *>(data);

  if (toolbar->getAddIcons())
    toolbar->addIcon(window);
}

bool
CwmToolBar::
processExpose(CwmWindow *xwindow)
{
  if (isWindow(xwindow)) {
    redraw();

    return true;
  }

  CwmToolBarIcon *icon = getToolIcon(xwindow);

  if (icon != 0) {
    icon->redraw();

    return true;
  }

  return false;
}

bool
CwmToolBar::
processButtonPress(Window xwin, int button)
{
  if (isWindow(xwin)) {
    if      (button == 1)
      getScreen().processDeskMenu();
    else if (button == 3)
      processMenu();

    return true;
  }

  if (isMenuWindow(xwin)) {
    if      (button == 1)
      processMenuMenu();
    else if (button == 3)
      processMenuMenu();

    return true;
  }

  CwmToolBarIcon *tool_icon = getToolIcon(xwin);

  if (tool_icon != 0) {
    if      (button == 1) {
      CwmWMWindow *window = tool_icon->getWindow();

      if (! window->isIconised())
        window->raise();
    }
    else if (button == 3) {
      CwmWMWindow *window = tool_icon->getWindow();

      if (  window->isIconised())
        tool_icon->processMenu();
      else
        window->processWindowMenu();
    }

    return true;
  }

  return false;
}
