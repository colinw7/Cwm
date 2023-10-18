#include <CwmI.h>

CwmFunctionDef::
CwmFunctionDef(const std::string &name)
{
  screen_      = nullptr;
  window_      = nullptr;
  icon_        = nullptr;
  root_proc_   = nullptr;
  window_proc_ = nullptr;
  icon_proc_   = nullptr;

  if (name[0] != 'f' || name[1] != '.') {
    CwmMachineInst->logf("Unrecognised function def %s must start with 'f.'\n", name.c_str());
    return;
  }

  std::string name1 = name.substr(2);

  if      (CStrUtil::casecmp(name1, "move") == 0) {
    root_proc_   = &CwmScreenMenu::moveProc;
    window_proc_ = &CwmWindowMenu::moveProc;
    icon_proc_   = &CwmDeskIcon::moveProc;
  }
  else if (CStrUtil::casecmp(name1, "resize") == 0) {
    root_proc_   = &CwmScreenMenu::resizeProc;
    window_proc_ = &CwmWindowMenu::resizeProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "resize_to") == 0) {
    root_proc_   = nullptr;
    window_proc_ = &CwmWindowMenu::resizeToSizeProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "iconise") == 0 ||
           CStrUtil::casecmp(name1, "iconize") == 0) {
    root_proc_   = &CwmScreenMenu::iconiseProc;
    window_proc_ = &CwmWindowMenu::iconiseProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "minimize") == 0) {
    root_proc_   = &CwmScreenMenu::minimizeProc;
    window_proc_ = &CwmWindowMenu::minimizeProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "maximize") == 0) {
    root_proc_   = &CwmScreenMenu::maximizeProc;
    window_proc_ = &CwmWindowMenu::minimizeProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "restore") == 0) {
    root_proc_   = &CwmScreenMenu::restoreProc;
    window_proc_ = nullptr;
    icon_proc_   = &CwmDeskIcon::restoreProc;
  }
  else if (CStrUtil::casecmp(name1, "raise") == 0) {
    root_proc_   = &CwmScreenMenu::raiseProc;
    window_proc_ = &CwmWindowMenu::raiseProc;
    icon_proc_   = &CwmDeskIcon::raiseProc;
  }
  else if (CStrUtil::casecmp(name1, "lower") == 0) {
    root_proc_   = &CwmScreenMenu::lowerProc;
    window_proc_ = &CwmWindowMenu::lowerProc;
    icon_proc_   = &CwmDeskIcon::lowerProc;
  }
  else if (CStrUtil::casecmp(name1, "circle_up") == 0) {
    root_proc_   = &CwmScreenMenu::circulateUpProc;
    window_proc_ = &CwmWindowMenu::circulateUpProc;
    icon_proc_   = &CwmDeskIcon::circulateUpProc;
  }
  else if (CStrUtil::casecmp(name1, "circle_down") == 0) {
    root_proc_   = &CwmScreenMenu::circulateDownProc;
    window_proc_ = &CwmWindowMenu::circulateDownProc;
    icon_proc_   = &CwmDeskIcon::circulateDownProc;
  }
  else if (CStrUtil::casecmp(name1, "focus_next") == 0) {
    root_proc_   = &CwmScreenMenu::focusNextProc;
    window_proc_ = &CwmWindowMenu::focusNextProc;
    icon_proc_   = &CwmDeskIcon::focusNextProc;
  }
  else if (CStrUtil::casecmp(name1, "focus_prev") == 0) {
    root_proc_   = &CwmScreenMenu::focusPrevProc;
    window_proc_ = &CwmWindowMenu::focusPrevProc;
    icon_proc_   = &CwmDeskIcon::focusPrevProc;
  }
  else if (CStrUtil::casecmp(name1, "select_next") == 0) {
    root_proc_   = &CwmScreenMenu::selectNextProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "select_prev") == 0) {
    root_proc_   = &CwmScreenMenu::selectPrevProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "roll_up") == 0) {
    root_proc_   = &CwmScreenMenu::rollUpProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "roll_down") == 0) {
    root_proc_   = &CwmScreenMenu::rollDownProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "close") == 0) {
    root_proc_   = &CwmScreenMenu::closeProc;
    window_proc_ = &CwmWindowMenu::closeProc;
    icon_proc_   = &CwmDeskIcon::closeProc;
  }
  else if (CStrUtil::casecmp(name1, "menu") == 0) {
    root_proc_   = &CwmScreenMenu::processNamedMenuProc;
    window_proc_ = &CwmWindowMenu::processNamedMenuProc;
    icon_proc_   = &CwmDeskIcon::namedMenuProc;
  }
  else if (CStrUtil::casecmp(name1, "window_menu") == 0) {
    root_proc_   = nullptr;
    window_proc_ = &CwmWMWindow::processWindowMenu;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "paste") == 0) {
    root_proc_   = nullptr;
    window_proc_ = &CwmWindowMenu::pasteProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "exec") == 0) {
    root_proc_   = &CwmScreenMenu::runCommandProc;
    window_proc_ = &CwmWindowMenu::runCommandProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "execx") == 0) {
    root_proc_   = &CwmScreenMenu::runXCommandProc;
    window_proc_ = &CwmWindowMenu::runXCommandProc;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "setroot") == 0) {
    root_proc_   = &CwmScreenMenu::setRootImageProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "toolbar") == 0) {
    root_proc_   = &CwmScreenMenu::toggleDisplayedProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "refresh") == 0) {
    root_proc_   = &CwmScreenMenu::refreshCwmProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "restart") == 0) {
    root_proc_   = &CwmScreenMenu::restartCwmProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "quit") == 0) {
    root_proc_   = &CwmScreenMenu::quitCwmProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "change_desk") == 0) {
    root_proc_   = &CwmScreenMenu::changeDeskProc;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else if (CStrUtil::casecmp(name1, "null") == 0) {
    root_proc_   = nullptr;
    window_proc_ = nullptr;
    icon_proc_   = nullptr;
  }
  else {
    CwmMachineInst->logf("Unrecognised function def %s\n", name1.c_str());
  }
}

CwmFunctionDef::
~CwmFunctionDef()
{
}

void
CwmFunctionDef::
process()
{
  if     (window_) {
    if (window_proc_)
      (*window_proc_)(window_, data_);
    else
      CwmMachineInst->log("Invalid Function for Window\n");
  }
  else if (icon_) {
    if (icon_proc_)
      (*icon_proc_)(icon_, data_);
    else
      CwmMachineInst->log("Invalid Function for Icon\n");
  }
  else if (screen_) {
    if (root_proc_)
      (*root_proc_)(*screen_, data_);
    else
      CwmMachineInst->log("Invalid Function for Root\n");
  }
}

void
CwmFunctionDef::
processWindow(CwmWMWindow *window, CwmData data)
{
  if (window_proc_)
    (*window_proc_)(window, data);
  else
    CwmMachineInst->log("Invalid Function for Window\n");
}

void
CwmFunctionDef::
processRoot(CwmScreen &screen, CwmData data)
{
  if (root_proc_)
    (*root_proc_)(screen, data);
  else
    CwmMachineInst->log("Invalid Function for Root\n");
}

void
CwmFunctionDef::
processProc(CwmData data)
{
  auto *function_def = static_cast<CwmFunctionDef *>(data);

  function_def->process();
}

void
CwmFunctionDef::
setData(CwmData data)
{
  data_ = data;
}

void
CwmFunctionDef::
setObjects(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon)
{
  screen_ = screen; window_ = window; icon_ = icon;
}
