#include <CwmDeskI.h>
#include <CwmAdvHints.h>

CwmDeskMgr::
CwmDeskMgr(CwmScreen &screen, int num_desks) :
 screen_(screen)
{
  for (int i = 0; i < num_desks; i++) {
    CwmDesk *desk = new CwmDesk(this, i);

    desks_.push_back(desk);
  }

  current_desk_ = nullptr;
  menu_def_     = nullptr;
}

CwmDeskMgr::
~CwmDeskMgr()
{
  notify_procs_.clear();

  for (auto &desk : desks_)
    delete desk;
}

void
CwmDeskMgr::
changeDesk(int desk_num)
{
  if (desk_num < 0 || desk_num >= int(desks_.size()))
    return;

  changeDesk(desks_[uint(desk_num)]);
}

void
CwmDeskMgr::
changeDesk(CwmDesk *desk)
{
  if (desk == current_desk_)
    return;

  callNotifyProcs(CWM_DESK_MGR_NOTIFY_CHANGE_START);

  if (current_desk_)
    current_desk_->hide();

  current_desk_ = desk;

  current_desk_->show();

  //CwmGnomeInst->setCurrentDesktop(screen_);
  //CwmGnomeInst->setListOfManagedClients(screen_);

  CwmAdvHintsInst->setCurrentDesktop(screen_);

  CwmAdvHintsInst->setClientList(screen_);
  CwmAdvHintsInst->setStackedClientList(screen_);

  callNotifyProcs(CWM_DESK_MGR_NOTIFY_CHANGE_END);
}

void
CwmDeskMgr::
processMenu()
{
  CwmMenuDef *menu_def = getMenuDef();

  for (uint i = 0; i < uint(desks_.size()); i++) {
    CwmMenuEntry *entry = menu_def->getEntry(i + 2);

    entry->setSelected(desks_[i] == current_desk_);
  }

  CwmMenu::processWindowMenu(screen_, screen_.getRoot(), menu_def);
}

CwmMenuDef *
CwmDeskMgr::
getMenuDef()
{
  if (menu_def_)
    return menu_def_;

  menu_def_ = new CwmMenuDef();

  menu_def_->addTitle   ("Desk");
  menu_def_->addSplitter("----");

  for (uint i = 0; i < uint(desks_.size()); i++) {
    auto desk_name = desks_[i]->getName();
    auto mnemonic  = desks_[i]->getMnemonic();

    menu_def_->addToggle("", desk_name, mnemonic, "",
                         function_cast<CwmMenuProc>(CwmDeskMgr::moveToDesk),
                         static_cast<CwmData>(this),
                         static_cast<CwmData>(desks_[i]));
  }

  return menu_def_;
}

void
CwmDeskMgr::
moveToDesk(CwmDeskMgr *desk_mgr, CwmDesk *desk)
{
  desk_mgr->changeDesk(desk);
}

int
CwmDeskMgr::
getDeskNum(CwmWMWindow *window)
{
  CwmDesk *desk = getDesk(window);

  if (desk)
    return desk->getNum();

  return 0;
}

CwmDesk *
CwmDeskMgr::
getDesk(int num)
{
  if (num < 0 || num >= int(desks_.size())) {
    CwmMachineInst->logf("Invalid Desk Number %d\n", num);
    return nullptr;
  }

  return desks_[uint(num)];
}

CwmDesk *
CwmDeskMgr::
getDesk(CwmWMWindow *window)
{
  auto num_desks = desks_.size();

  for (uint i = 0; i < num_desks; i++) {
    CwmDesk *desk = desks_[i];

    CwmDesk::WMWindowList windows = desk->getWindows();

    for (auto &window1 : windows)
      if (window1 == window)
        return desk;
  }

  return nullptr;
}

int
CwmDeskMgr::
getCurrentDeskNum() const
{
  return current_desk_->getNum();
}

CwmDeskResourceMgr *
CwmDeskResourceMgr::
getInstance()
{
  static CwmDeskResourceMgr *instance;

  if (! instance)
    instance = new CwmDeskResourceMgr();

  return instance;
}

CwmDeskResourceMgr::
CwmDeskResourceMgr()
{
}

CwmDeskResourceMgr::
~CwmDeskResourceMgr()
{
}

std::string
CwmDeskResourceMgr::
getDeskName(int desk_num)
{
  if (desk_name_.find(desk_num) != desk_name_.end())
    return desk_name_[desk_num];
  else
    return CStrUtil::toString(desk_num);
}

bool
CwmDeskResourceMgr::
getDeskDisplayIcons(int desk_num)
{
  if (desk_display_icons_.find(desk_num) != desk_display_icons_.end())
    return desk_display_icons_[desk_num];
  else
    return false;
}

bool
CwmDeskResourceMgr::
getDeskDisplayToolBar(int desk_num)
{
  if (desk_display_toolbar_.find(desk_num) != desk_display_toolbar_.end())
    return desk_display_toolbar_[desk_num];
  else
    return true;
}

std::string
CwmDeskResourceMgr::
getDeskImage(int desk_num)
{
  if (desk_image_name_.find(desk_num) != desk_image_name_.end())
    return desk_image_name_[desk_num];
  else
    return "";
}

void
CwmDeskResourceMgr::
setDeskName(int desk_num, const std::string &name)
{
  desk_name_[desk_num] = name;
}

void
CwmDeskResourceMgr::
setDeskDisplayIcons(int desk_num, bool display_icons)
{
  desk_display_icons_[desk_num] = display_icons;
}

void
CwmDeskResourceMgr::
setDeskDisplayToolBar(int desk_num, bool display_toolbar)
{
  desk_display_toolbar_[desk_num] = display_toolbar;
}

void
CwmDeskResourceMgr::
setDeskImage(int desk_num, const std::string &image_name)
{
  desk_image_name_[desk_num] = image_name;
}

void
CwmDeskMgr::
addNotifyProc(CwmDeskMgrNotifyType type,
              CwmDeskMgrNotifyProc proc, void *data)
{
  CwmDeskMgrNotifyData *notify_proc =
    new CwmDeskMgrNotifyData(type, proc, data);

  notify_procs_.push_back(notify_proc);
}

void
CwmDeskMgr::
removeNotifyProc(CwmDeskMgrNotifyType type, CwmDeskMgrNotifyProc proc,
                 void *data)
{
  DeskMgrNotifyDataList::const_iterator pnotify_proc1 = notify_procs_.begin();
  DeskMgrNotifyDataList::const_iterator pnotify_proc2 = notify_procs_.end  ();

  while (pnotify_proc1 != pnotify_proc2) {
    if ((*pnotify_proc1)->match(type, proc, data)) {
      notify_procs_.remove(*pnotify_proc1);

      delete *pnotify_proc1;

      pnotify_proc1 = notify_procs_.begin();
      pnotify_proc2 = notify_procs_.end  ();
    }
    else
      ++pnotify_proc1;
  }
}

void
CwmDeskMgr::
callNotifyProcs(CwmDeskMgrNotifyType type)
{
  DeskMgrNotifyDataList::const_iterator pnotify_proc1 = notify_procs_.begin();
  DeskMgrNotifyDataList::const_iterator pnotify_proc2 = notify_procs_.end  ();

  for ( ; pnotify_proc1 != pnotify_proc2; ++pnotify_proc1)
    (*pnotify_proc1)->setCalled(false);

  //---

  pnotify_proc1 = notify_procs_.begin();

  while (pnotify_proc1 != pnotify_proc2) {
    if ((*pnotify_proc1)->isType(type) && ! (*pnotify_proc1)->getCalled()) {
      (*pnotify_proc1)->setCalled(true);

      (*pnotify_proc1)->call(this);

      pnotify_proc1 = notify_procs_.begin();
      pnotify_proc2 = notify_procs_.end  ();
    }
    else
      ++pnotify_proc1;
  }
}

void
CwmDeskMgr::
enable()
{
  auto num_desks = desks_.size();

  for (uint i = 0; i < num_desks; i++)
    desks_[i]->enable();
}

void
CwmDeskMgr::
disable()
{
  auto num_desks = desks_.size();

  for (uint i = 0; i < num_desks; i++)
    desks_[i]->disable();
}

CwmDesk::
CwmDesk(CwmDeskMgr *mgr, int num) :
 mgr_(mgr), num_(num), root_image_(nullptr)
{
  name_ = CwmDeskResourceMgrInst->getDeskName(num);

  mnemonic_ = name_[0];

  root_image_spec_ = CwmDeskResourceMgrInst->getDeskImage(num);

  if (root_image_spec_ == "")
    root_image_spec_ = CwmInst->getRootImage();

  if (CwmDeskResourceMgrInst->getDeskDisplayIcons(num))
    icon_mgr_ = new CwmDeskIconMgr(*this);
  else
    icon_mgr_ = nullptr;

  if (CwmDeskResourceMgrInst->getDeskDisplayToolBar(num))
    toolbar_mgr_ = new CwmToolBarMgr(*this);
  else
    toolbar_mgr_ = nullptr;
}

CwmDesk::
~CwmDesk()
{
  delete icon_mgr_;
  delete toolbar_mgr_;
}

void
CwmDesk::
addWMWindow(CwmWMWindow *window)
{
  windows_.push_back(window);

  CwmScreen &screen = mgr_->getScreen();

  //CwmGnomeInst->setListOfManagedClients(screen);

  CwmAdvHintsInst->setClientList(screen);
  CwmAdvHintsInst->setStackedClientList(screen);
}

void
CwmDesk::
removeWMWindow(CwmWMWindow *window)
{
  windows_.remove(window);

  CwmScreen &screen = mgr_->getScreen();

  //CwmGnomeInst->setListOfManagedClients(screen);

  CwmAdvHintsInst->setClientList(screen);
  CwmAdvHintsInst->setStackedClientList(screen);
}

void
CwmDesk::
hide()
{
  for (auto &window : windows_)
    if (! window->isIconised())
      window->unmap();
}

void
CwmDesk::
show()
{
  for (auto &window : windows_)
    if (! window->isIconised())
      window->map();

  drawRootImage();
}

void
CwmDesk::
setRootImage(const std::string &spec)
{
  root_image_spec_ = spec;

  initRootImage();
}

void
CwmDesk::
drawRootImage()
{
  if (root_image_ == nullptr)
    initRootImage();

  CwmGrabServer grab;

  root_image_->draw();
}

void
CwmDesk::
initRootImage()
{
  CwmGrabServer grab;

  root_image_ = new CwmRootImage(mgr_->getScreen(), root_image_spec_);
}

CwmScreen &
CwmDesk::
getScreen() const
{
  return mgr_->getScreen();
}

CwmToolBar *
CwmDesk::
getToolBar() const
{
  if (toolbar_mgr_)
    return toolbar_mgr_->getToolBar();
  else
    return nullptr;
}

void
CwmDesk::
enable()
{
  if (toolbar_mgr_)
    toolbar_mgr_->enable();
}

void
CwmDesk::
disable()
{
  if (toolbar_mgr_)
    toolbar_mgr_->disable();
}
