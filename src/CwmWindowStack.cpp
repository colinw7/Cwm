#include <CwmWindowStack.h>
#include <CwmMachine.h>
#include <CwmScreen.h>
#include <CwmWindow.h>
#include <CwmWMWindow.h>
#include <CwmDeskMgr.h>
#include <CwmDeskIcon.h>
#include <CwmResourceData.h>
#include <CwmAdvHints.h>

// Window Group of Base Window and Transient Children in Top to Bottom Order

CwmWindowGroup::
CwmWindowGroup(CwmWMWindow *window)
{
  stackChildren(window);

  window_stack_.push_back(window);

  stackParents(window);
}

CwmWindowGroup::
~CwmWindowGroup()
{
}

void
CwmWindowGroup::
stackChildren(CwmWMWindow *window)
{
  const CwmWMWindow::WMWindowList children = window->getChildren();

  for (auto &child : children) {
    stackChildren(child);

    window_stack_.push_back(child);
  }
}

void
CwmWindowGroup::
stackParents(CwmWMWindow *window)
{
  CwmWMWindow *parent = window->getParent();

  if (! parent)
    return;

  const CwmWMWindow::WMWindowList children = parent->getChildren();

  for (auto &child : children) {
    if (child == window)
      continue;

    stackChildren(child);

    window_stack_.push_back(child);
  }

  window_stack_.push_back(parent);

  stackParents(parent);
}

void
CwmWindowGroup::
restack()
{
  if (window_stack_.empty())
    return;

  Window *windows = new Window [window_stack_.size()];

  auto num_window_stacks = window_stack_.size();

  for (uint i = 0; i < num_window_stacks; ++i)
    windows[i] = window_stack_[i]->getFrameWindow()->getXWin();

  CwmMachineInst->restackWindows(windows, int(window_stack_.size()));

  delete [] windows;
}

void
CwmWindowGroup::
map()
{
  auto num_window_stacks = window_stack_.size();

  for (uint i = 0; i < num_window_stacks; ++i)
    window_stack_[i]->getFrameWindow()->map();
}

void
CwmWindowGroup::
unmap()
{
  auto num_window_stacks = window_stack_.size();

  for (uint i = 0; i < num_window_stacks; ++i)
    window_stack_[i]->getFrameWindow()->unmap();
}

bool
CwmWindowGroup::
contains(CwmWMWindow *window)
{
  auto num_window_stacks = window_stack_.size();

  for (uint i = 0; i < num_window_stacks; ++i)
    if (window_stack_[i] == window)
      return true;

  return false;
}

uint
CwmWindowGroup::
size() const
{
  return uint(window_stack_.size());
}

CwmWMWindow &
CwmWindowGroup::
getTopWindow() const
{
  return *(window_stack_[0]);
}

CwmWMWindow &
CwmWindowGroup::
getBottomWindow() const
{
  return *(window_stack_[window_stack_.size() - 1]);
}

CwmWMWindow &
CwmWindowGroup::
operator[](uint i) const
{
  if (i >= uint(window_stack_.size()))
    throw "Invalid Index";

  return *(window_stack_[i]);
}

//---

CwmCirculateWindowStack::
CwmCirculateWindowStack(CwmScreen &screen) :
 screen_(screen), screen_window_groups_(0)
{
  update();
}

CwmCirculateWindowStack::
~CwmCirculateWindowStack()
{
  delete screen_window_groups_;
}

void
CwmCirculateWindowStack::
update()
{
  delete screen_window_groups_;

  int types = CWM_WINDOW_TYPE_NORMAL;

  if (! CwmResourceDataInst->getCirculateSkipIcons())
    types |= CWM_WINDOW_TYPE_ICONISED;

  window_groups_.clear();

  screen_window_groups_ = new CwmScreenWindowGroups(screen_);

  auto num_screen_window_groups_ = screen_window_groups_->size();

  for (uint i = 0; i < num_screen_window_groups_; ++i) {
    CwmWMWindow &window = (*screen_window_groups_)[i].getBottomWindow();

    if (window.getCirculateSkip())
      continue;

    if (window.isIconised()) {
      if (! (types & CWM_WINDOW_TYPE_ICONISED))
        continue;

      CwmDeskMgr *desk_mgr = screen_.getDeskMgr();

      CwmDesk *desk = desk_mgr->getCurrentDesk();

      CwmDeskIconMgr *desk_icon_mgr = desk->getDeskIconMgr();

      CwmDeskIcon *desk_icon = desk_icon_mgr->lookup(&window);

      if (! desk_icon)
        continue;
    }

    window_groups_.push_back(&(*screen_window_groups_)[i]);
  }
}

void
CwmCirculateWindowStack::
circulateUp()
{
  auto num_window_groups = window_groups_.size();

  if (num_window_groups > 1)
    raise(&getBottomGroup().getBottomWindow());
}

void
CwmCirculateWindowStack::
circulateDown()
{
  auto num_window_groups = window_groups_.size();

  if (num_window_groups > 1)
    lower(&getTopGroup().getBottomWindow());
}

void
CwmCirculateWindowStack::
lower(CwmWMWindow *window)
{
  auto num_window_groups = window_groups_.size();

  if (num_window_groups < 2) {
    window->getFrameWindow()->lower();
    return;
  }

  CwmWindowGroup *bottom_group = &getBottomGroup();
  CwmWindowGroup *lower_group  = getWindowGroup(window);

  if (! lower_group) {
    window->getFrameWindow()->lower();
    return;
  }

  CwmWMWindow &bottom_window = bottom_group->getBottomWindow();
  CwmWMWindow &lower_window  = lower_group ->getTopWindow();

  XWindowChanges xwc;

  xwc.sibling    = bottom_window.getFrameWindow()->getXWin();
  xwc.stack_mode = Below;

  CwmMachineInst->configureWindow(lower_window.getFrameWindow()->getXWin(),
                                  CWSibling | CWStackMode, &xwc);

  lower_group->restack();

  CwmAdvHintsInst->setStackedClientList(screen_);
}

void
CwmCirculateWindowStack::
raise(CwmWMWindow *window)
{
  auto num_window_groups = window_groups_.size();

  if (num_window_groups < 2) {
    window->getFrameWindow()->raise();
    return;
  }

  CwmWindowGroup *top_group   = &getTopGroup();
  CwmWindowGroup *raise_group = getWindowGroup(window);

  if (! raise_group) {
    window->getFrameWindow()->raise();
    return;
  }

  CwmWMWindow &top_window   = top_group  ->getTopWindow();
  CwmWMWindow &raise_window = raise_group->getTopWindow();

  XWindowChanges xwc;

  xwc.sibling    = top_window.getFrameWindow()->getXWin();
  xwc.stack_mode = Above;

  CwmMachineInst->configureWindow(raise_window.getFrameWindow()->getXWin(),
                                  CWSibling | CWStackMode, &xwc);

  raise_group->restack();

  CwmAdvHintsInst->setStackedClientList(screen_);
}

CwmWindowGroup *
CwmCirculateWindowStack::
getWindowGroup(CwmWMWindow *window)
{
  for (uint i = 0; i < uint(window_groups_.size()); i++)
    if (window_groups_[i]->contains(window))
      return window_groups_[i];

  return 0;
}

void
CwmCirculateWindowStack::
restack()
{
  auto num_window_groups = window_groups_.size();

  for (uint i = 0; i < num_window_groups; i++)
    window_groups_[i]->restack();

  update();

  CwmAdvHintsInst->setStackedClientList(screen_);
}

uint
CwmCirculateWindowStack::
size() const
{
  auto num_window_groups = window_groups_.size();

  return uint(num_window_groups);
}

CwmWindowGroup &
CwmCirculateWindowStack::
getTopGroup() const
{
  return *(window_groups_[0]);
}

CwmWindowGroup &
CwmCirculateWindowStack::
getBottomGroup() const
{
  return *(window_groups_[window_groups_.size() - 1]);
}

CwmWindowGroup &
CwmCirculateWindowStack::
operator[](uint i) const
{
  auto num_window_groups = window_groups_.size();

  if (i >= uint(num_window_groups))
    throw "Invalid Index";

  return *window_groups_[i];
}

bool
CwmCirculateWindowStack::
isTop(CwmWMWindow *window)
{
  auto num_window_groups = window_groups_.size();

  if (num_window_groups == 0)
    return false;

  CwmWindowGroup &window_group = getTopGroup();

  CwmWMWindow *top_window = &window_group.getTopWindow();

  return (window == top_window);
}

//------

CwmScreenWindowGroups::
CwmScreenWindowGroups(CwmScreen &screen)
{
  Window root_xwin = screen.getRoot()->getXWin();

  if (! CwmMachineInst->isValidWindow(root_xwin))
    return;

  Window *children;
  int     num_children;

  if (! CwmMachineInst->getWindowChildren(root_xwin, &children, &num_children))
    return;

  for (int i = int(num_children - 1); i >= 0; i--) {
    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(children[i]);

    if (! window || window->getParent() != 0)
      continue;

    CwmWindowGroup *window_group = new CwmWindowGroup(window);

    window_groups_.push_back(window_group);
  }

  if (children != 0)
    XFree(children);
}

CwmScreenWindowGroups::
~CwmScreenWindowGroups()
{
  auto num_window_groups = window_groups_.size();

  for (uint i = 0; i < num_window_groups; i++)
    delete window_groups_[i];
}

uint
CwmScreenWindowGroups::
size() const
{
  auto num_window_groups = window_groups_.size();

  return uint(num_window_groups);
}

CwmWindowGroup &
CwmScreenWindowGroups::
getTopGroup() const
{
  return (*window_groups_[0]);
}

CwmWindowGroup &
CwmScreenWindowGroups::
getBottomGroup() const
{
  return (*window_groups_[window_groups_.size() - 1]);
}

CwmWindowGroup &
CwmScreenWindowGroups::
operator[](uint i) const
{
  auto num_window_groups = window_groups_.size();

  if (i >= num_window_groups)
    throw "Invalid Index";

  return *(window_groups_[i]);
}
