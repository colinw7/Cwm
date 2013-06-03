#include "CwmI.h"

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

  CwmWMWindow::WMWindowList::const_iterator pchild1 = children.begin();
  CwmWMWindow::WMWindowList::const_iterator pchild2 = children.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    stackChildren(*pchild1);

    window_stack_.push_back(*pchild1);
  }
}

void
CwmWindowGroup::
stackParents(CwmWMWindow *window)
{
  CwmWMWindow *parent = window->getParent();

  if (parent == NULL)
    return;

  const CwmWMWindow::WMWindowList children = parent->getChildren();

  CwmWMWindow::WMWindowList::const_iterator pchild1 = children.begin();
  CwmWMWindow::WMWindowList::const_iterator pchild2 = children.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    if (*pchild1 == window)
      continue;

    stackChildren(*pchild1);

    window_stack_.push_back(*pchild1);
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

  int num_window_stacks = window_stack_.size();

  for (int i = 0; i < num_window_stacks; ++i)
    windows[i] = window_stack_[i]->getFrameWindow()->getXWin();

  CwmMachineInst->restackWindows(windows, window_stack_.size());

  delete [] windows;
}

void
CwmWindowGroup::
map()
{
  int num_window_stacks = window_stack_.size();

  for (int i = 0; i < num_window_stacks; ++i)
    window_stack_[i]->getFrameWindow()->map();
}

void
CwmWindowGroup::
unmap()
{
  int num_window_stacks = window_stack_.size();

  for (int i = 0; i < num_window_stacks; ++i)
    window_stack_[i]->getFrameWindow()->unmap();
}

bool
CwmWindowGroup::
contains(CwmWMWindow *window)
{
  int num_window_stacks = window_stack_.size();

  for (int i = 0; i < num_window_stacks; ++i)
    if (window_stack_[i] == window)
      return true;

  return false;
}

int
CwmWindowGroup::
size() const
{
  return window_stack_.size();
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
operator[](int i) const
{
  if (i < 0 || i >= (int) window_stack_.size())
    throw "Invalid Index";

  return *(window_stack_[i]);
}

CwmCirculateWindowStack::
CwmCirculateWindowStack(CwmScreen &screen) :
 screen_(screen), screen_window_groups_(NULL)
{
  update();
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

  int num_screen_window_groups_ = screen_window_groups_->size();

  for (int i = 0; i < num_screen_window_groups_; ++i) {
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

      if (desk_icon == NULL)
        continue;
    }

    window_groups_.push_back(&(*screen_window_groups_)[i]);
  }
}

CwmCirculateWindowStack::
~CwmCirculateWindowStack()
{
  delete screen_window_groups_;
}

void
CwmCirculateWindowStack::
circulateUp()
{
  int num_window_groups = window_groups_.size();

  if (num_window_groups > 1)
    raise(&getBottomGroup().getBottomWindow());
}

void
CwmCirculateWindowStack::
circulateDown()
{
  int num_window_groups = window_groups_.size();

  if (num_window_groups > 1)
    lower(&getTopGroup().getBottomWindow());
}

void
CwmCirculateWindowStack::
lower(CwmWMWindow *window)
{
  int num_window_groups = window_groups_.size();

  if (num_window_groups < 2) {
    window->getFrameWindow()->lower();
    return;
  }

  CwmWindowGroup *bottom_group = &getBottomGroup();
  CwmWindowGroup *lower_group  = getWindowGroup(window);

  if (lower_group == NULL) {
    window->getFrameWindow()->lower();
    return;
  }

  CwmWMWindow &bottom_window = bottom_group->getBottomWindow();
  CwmWMWindow &lower_window  = lower_group ->getTopWindow();

  XWindowChanges xwc;

  xwc.sibling    = bottom_window.getFrameWindow()->getXWin();
  xwc.stack_mode = Below;

  CwmMachineInst->configureWindow(lower_window.getFrameWindow()->getXWin(),
                                  CWSibling | CWStackMode,
                                  &xwc);

  lower_group->restack();
}

void
CwmCirculateWindowStack::
raise(CwmWMWindow *window)
{
  int num_window_groups = window_groups_.size();

  if (num_window_groups < 2) {
    window->getFrameWindow()->raise();
    return;
  }

  CwmWindowGroup *top_group   = &getTopGroup();
  CwmWindowGroup *raise_group = getWindowGroup(window);

  if (raise_group == NULL) {
    window->getFrameWindow()->raise();
    return;
  }

  CwmWMWindow &top_window   = top_group  ->getTopWindow();
  CwmWMWindow &raise_window = raise_group->getTopWindow();

  XWindowChanges xwc;

  xwc.sibling    = top_window.getFrameWindow()->getXWin();
  xwc.stack_mode = Above;

  CwmMachineInst->configureWindow(raise_window.getFrameWindow()->getXWin(),
                                  CWSibling | CWStackMode,
                                  &xwc);

  raise_group->restack();
}

CwmWindowGroup *
CwmCirculateWindowStack::
getWindowGroup(CwmWMWindow *window)
{
  for (int i = 0; i < (int) window_groups_.size(); i++)
    if (window_groups_[i]->contains(window))
      return window_groups_[i];

  return NULL;
}

void
CwmCirculateWindowStack::
restack()
{
  int num_window_groups = window_groups_.size();

  for (int i = 0; i < num_window_groups; i++)
    window_groups_[i]->restack();

  update();
}

int
CwmCirculateWindowStack::
size() const
{
  int num_window_groups = window_groups_.size();

  return num_window_groups;
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
operator[](int i) const
{
  int num_window_groups = window_groups_.size();

  if (i < 0 || i >= num_window_groups)
    throw "Invalid Index";

  return *window_groups_[i];
}

bool
CwmCirculateWindowStack::
isTop(CwmWMWindow *window)
{
  int num_window_groups = window_groups_.size();

  if (num_window_groups == 0)
    return false;

  CwmWindowGroup &window_group = getTopGroup();

  CwmWMWindow *top_window = &window_group.getTopWindow();

  return (window == top_window);
}

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

  for (int i = (int) num_children - 1; i >= 0; i--) {
    CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(children[i]);

    if (window == NULL || window->getParent() != NULL)
      continue;

    CwmWindowGroup *window_group = new CwmWindowGroup(window);

    window_groups_.push_back(window_group);
  }

  if (children != NULL)
    XFree((char *) children);
}

CwmScreenWindowGroups::
~CwmScreenWindowGroups()
{
  int num_window_groups = window_groups_.size();

  for (int i = 0; i < num_window_groups; i++)
    delete window_groups_[i];
}

int
CwmScreenWindowGroups::
size() const
{
  int num_window_groups = window_groups_.size();

  return num_window_groups;
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
operator[](int i) const
{
  int num_window_groups = window_groups_.size();

  if (i < 0 || i >= num_window_groups)
    throw "Invalid Index";

  return *(window_groups_[i]);
}
