#include <CwmDeskI.h>

CwmDeskIconMgr::
CwmDeskIconMgr(CwmDesk &desk) :
 desk_(desk)
{
  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_CREATE, &CwmDeskIconMgr::createNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_DESTROY, &CwmDeskIconMgr::destroyNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_ICONISE, &CwmDeskIconMgr::iconiseNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    addProc(CWM_WINDOW_NOTIFY_RESTORE, &CwmDeskIconMgr::restoreNotifyProc, this);

  CwmDeskMgr *desk_mgr = desk_.getDeskMgr();

  desk_mgr->addNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_START,
                          &CwmDeskIconMgr::deskChangeStartNotifyProc, this);
  desk_mgr->addNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_END,
                          &CwmDeskIconMgr::deskChangeEndNotifyProc, this);

  move_event_    = 0;
  lower_event_   = 0;
  raise_event_   = 0;
  close_event_   = 0;
  restore_event_ = 0;
}

CwmDeskIconMgr::
~CwmDeskIconMgr()
{
  CwmDeskMgr *desk_mgr = desk_.getDeskMgr();

  desk_mgr->removeNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_START,
                             &CwmDeskIconMgr::deskChangeStartNotifyProc, this);
  desk_mgr->removeNotifyProc(CWM_DESK_MGR_NOTIFY_CHANGE_END,
                             &CwmDeskIconMgr::deskChangeEndNotifyProc, this);

  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_CREATE, &CwmDeskIconMgr::createNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_DESTROY, &CwmDeskIconMgr::destroyNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_ICONISE, &CwmDeskIconMgr::iconiseNotifyProc, this);
  CwmWindowGlobalNotifyMgrInst->
    removeProc(CWM_WINDOW_NOTIFY_RESTORE, &CwmDeskIconMgr::restoreNotifyProc, this);

  delete move_event_;
  delete lower_event_;
  delete raise_event_;
  delete close_event_;
  delete restore_event_;
}

void
CwmDeskIconMgr::
createNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isWindowOnDesk(window)) {
    CwmDeskIcon *icon = new CwmDeskIcon(*mgr, window);

    mgr->addDeskIcon(icon);
  }
}

void
CwmDeskIconMgr::
destroyNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isWindowOnDesk(window)) {
    CwmDeskIcon *icon = mgr->lookup(window);

    if (icon != 0)
      mgr->removeDeskIcon(icon);

    delete icon;
  }
}

void
CwmDeskIconMgr::
iconiseNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isWindowOnDesk(window)) {
    CwmDeskIcon *icon = mgr->lookup(window);

    if (icon != 0)
      icon->map();
  }
}

void
CwmDeskIconMgr::
restoreNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isWindowOnDesk(window)) {
    CwmDeskIcon *icon = mgr->lookup(window);

    if (icon != 0)
      icon->unmap();
  }
}

void
CwmDeskIconMgr::
deskChangeStartNotifyProc(CwmDeskMgr *desk_mgr, CwmDeskMgrNotifyType, CwmData data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isCurrentDesk(desk_mgr))
    mgr->unmapIcons();
}

void
CwmDeskIconMgr::
deskChangeEndNotifyProc(CwmDeskMgr *desk_mgr, CwmDeskMgrNotifyType, CwmData data)
{
  CwmDeskIconMgr *mgr = (CwmDeskIconMgr *) data;

  if (mgr->isCurrentDesk(desk_mgr))
    mgr->mapIcons();
}

bool
CwmDeskIconMgr::
isCurrentDesk(CwmDeskMgr *desk_mgr)
{
  return (desk_mgr->getCurrentDesk() == &desk_);
}

bool
CwmDeskIconMgr::
isWindowOnDesk(CwmWMWindow *window)
{
  return (window->getDeskNum() == desk_.getNum());
}

void
CwmDeskIconMgr::
addDeskIcon(CwmDeskIcon *icon)
{
  desk_icon_map_[icon->getXWin()] = icon;
}

void
CwmDeskIconMgr::
removeDeskIcon(CwmDeskIcon *icon)
{
  CwmDeskIconMap::iterator picon = desk_icon_map_.find(icon->getXWin());

  if (picon != desk_icon_map_.end())
    desk_icon_map_.erase(picon);
}

void
CwmDeskIconMgr::
mapIcons()
{
  for (auto &desk_icon : desk_icon_map_)
    desk_icon.second->map();
}

void
CwmDeskIconMgr::
unmapIcons()
{
  for (auto &desk_icon : desk_icon_map_) 
    desk_icon.second->unmap();
}

CwmDeskIcon *
CwmDeskIconMgr::
lookup(CwmWMWindow *window)
{
  Window xwin = window->getXWin();

  return lookup(xwin);
}

CwmDeskIcon *
CwmDeskIconMgr::
lookup(CwmWindow *xwindow)
{
  Window xwin = xwindow->getXWin();

  return lookup(xwin);
}

CwmDeskIcon *
CwmDeskIconMgr::
lookup(Window xwin)
{
  CwmDeskIconMap::iterator picon = desk_icon_map_.find(xwin);

  if (picon != desk_icon_map_.end())
    return (*picon).second;
  else
    return 0;
}

CXNamedEvent *
CwmDeskIconMgr::
getMoveEvent()
{
  if (move_event_ == 0)
    move_event_ = new CXNamedEvent("Alt<Key>F7");

  return move_event_;
}

CXNamedEvent *
CwmDeskIconMgr::
getLowerEvent()
{
  if (lower_event_ == 0)
    lower_event_ = new CXNamedEvent("Alt<Key>F3");

  return lower_event_;
}

CXNamedEvent *
CwmDeskIconMgr::
getRaiseEvent()
{
  if (raise_event_ == 0)
    raise_event_ = new CXNamedEvent("Alt<Key>F2");

  return raise_event_;
}

CXNamedEvent *
CwmDeskIconMgr::
getRestoreEvent()
{
  if (restore_event_ == 0)
    restore_event_ = new CXNamedEvent("Alt<Key>F5");

  return restore_event_;
}

CXNamedEvent *
CwmDeskIconMgr::
getCloseEvent()
{
  if (close_event_ == 0)
    close_event_ = new CXNamedEvent("Alt<Key>F4");

  return close_event_;
}

CwmDeskIcon::
CwmDeskIcon(CwmDeskIconMgr &mgr, CwmWMWindow *window) :
 mgr_(mgr), desk_(mgr_.getDesk()), screen_(window->getScreen()), window_(window)
{
  CwmColor *color = CwmResourceDataInst->getIconColor(screen_);
  CwmFont  *font  = CwmResourceDataInst->getIconFont (screen_);

  graphics_ = new CwmGraphics(*window, font, color);

  //------

  if (window->getIconXHint() != -1)
    icon_x_ = window->getIconXHint();
  else
    icon_x_ = 4;

  if (window->getIconYHint() != -1)
    icon_y_ = window->getIconYHint();
  else
    icon_y_ = 4;

  //------

  if (CwmResourceDataInst->getIconImageDecoration())
    image_ = window->getImage();
  else
    image_ = 0;

  pixmap_mask_  = 0;
  label_mask_   = 0;
  mask_created_ = false;

  //------

  if (CwmResourceDataInst->getIconLabelDecoration()) {
    std::string name = window->getIconName();

    int icon_label_width = CwmResourceDataInst->getIconLabelWidth();

    fmt_string_ = new CFmtString(name, icon_label_width);
  }
  else
    fmt_string_ = 0;

  //------

  calcSize();

  //------

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ =
    new CwmWindow(screen_, screen_.getRoot(), icon_x_, icon_y_,
                  icon_width_, icon_height_, event_mask, CWM_CURSOR_TITLE);

  //------

  menu_def_ = 0;

  //------

  xwindow_->addCallback(CWM_CALLBACK_DOUBLE_CLICK_1,
                        CwmDeskIcon::doubleClickProc, (CwmData) this);
}

CwmDeskIcon::
~CwmDeskIcon()
{
  delete graphics_;

  delete xwindow_;

  delete menu_def_;

  delete pixmap_mask_;

  delete label_mask_;
}

Window
CwmDeskIcon::
getXWin() const
{
  return getXWindow()->getXWin();
}

void
CwmDeskIcon::
move()
{
  bool has_focus = CwmMachineInst->isFocusWindow(window_);

  if (has_focus) {
    CwmMachineInst->setFocusWindow(0);

    redraw();
  }

  removeFromGrid();

  xwindow_->raise();

  int dx, dy;

  CwmMoveWindowInst->moveInteractive(screen_, xwindow_, icon_x_, icon_y_,
                                     icon_width_, icon_height_, &dx, &dy);

  if (icon_x_ + dx < 0)
    dx = -icon_x_;

  if (icon_y_ + dy < 0)
    dy = -icon_y_;

  if (icon_x_ + icon_width_  + dx > screen_.getWidth ())
    dx = screen_.getWidth () - icon_x_ - icon_width_ ;

  if (icon_y_ + icon_height_ + dy > screen_.getHeight())
    dy = screen_.getHeight() - icon_y_ - icon_height_;

  icon_x_ += dx;
  icon_y_ += dy;

  xwindow_->lower();

  addToGrid();

  if (has_focus)
    CwmMachineInst->setFocusWindow(window_);

  if (has_focus)
    redraw();
}

void
CwmDeskIcon::
raise()
{
  xwindow_->raise();
}

void
CwmDeskIcon::
lower()
{
  xwindow_->lower();
}

void
CwmDeskIcon::
restore()
{
  unmap();

  window_->restore();
}

void
CwmDeskIcon::
close()
{
  window_->close();
}

void
CwmDeskIcon::
redraw()
{
  if (! mask_created_)
    createMask();

  //------

  std::string name = window_->getIconName();

  fmt_string_->setString(name);

  int width, height;

  getLabelSize(&width, &height);

  if (width != label_width_ || height != label_height_) {
    label_width_  = width;
    label_height_ = height;

    //------

    icon_width_ = std::max(label_width_ + 2* label_dx_, pixmap_width_ + 2*pixmap_dx_);

    icon_height_ = pixmap_dy_ + pixmap_height_ + std::max(pixmap_dy_, label_dy_) +
                   label_dy_  + label_height_;

    //------

    pixmap_offset_ = (icon_width_ - pixmap_width_)/2;
    label_offset_  = (icon_width_ - label_width_ )/2;

    //------

    xwindow_->moveResize(icon_x_, icon_y_, icon_width_, icon_height_);

    //------

    createMask();
  }

  //------

  if (CwmResourceDataInst->getIconImageDecoration())
    image_->draw(xwindow_, graphics_, pixmap_dx_ + pixmap_offset_, pixmap_dy_);

  if (CwmResourceDataInst->getIconLabelDecoration()) {
    int y = pixmap_dy_ + pixmap_height_ + std::max(pixmap_dy_, label_dy_);

    int num_lines = fmt_string_->getNumLines();

    for (int i = 0; i < num_lines; ++i) {
      int width1, height1;

      std::string line = CStrUtil::stripSpaces(fmt_string_->getLine(i));

      graphics_->getTextSize(line, &width1, &height1);

      graphics_->drawTextCentered(xwindow_, label_dx_ + label_offset_, y,
                                  label_width_, height1, line);

      y += height1 + 1;
    }
  }

  if (CwmResourceDataInst->getIconImageBorder())
    graphics_->drawButtonOut(xwindow_, 0, 0, xwindow_->getWidth (), xwindow_->getHeight(), 2);

  if (CwmMachineInst->isFocusWindow(window_)) {
    //int dy = (pixmap_width_ > label_width_ ? 1 : 0);

    int x1 = 0;
    int x2 = xwindow_->getWidth() - 1;

    int y1 = 0;
    int y2 = xwindow_->getHeight() - 1;

    int x[4] = { x1, x2, x2, x1 };
    int y[4] = { y1, y1, y2, y2 };

    for (int i1 = 0; i1 < 4; ++i1) {
      int i2 = (i1 + 1) % 4;

      graphics_->drawLine(xwindow_, x[i1], y[i1], x[i2], y[i2]);
    }
  }
}

void
CwmDeskIcon::
createMask()
{
  int op = ShapeSet;

  if (CwmResourceDataInst->getIconImageDecoration()) {
    delete pixmap_mask_;

    pixmap_mask_ = image_->createMask();

    CwmMachineInst->shapeCombineMask(xwindow_->getXWin(), pixmap_mask_->getXPixmap(),
                                     pixmap_dx_ + pixmap_offset_, pixmap_dy_, op);

    op = ShapeUnion;
  }

  if (CwmResourceDataInst->getIconLabelDecoration()) {
    delete label_mask_;

    label_mask_ = new CwmMask(screen_, graphics_, label_width_, label_height_);

    int y = 0;

    int num_lines = fmt_string_->getNumLines();

    for (int i = 0; i < num_lines; ++i) {
      int width1, height1;

      std::string line = CStrUtil::stripSpaces(fmt_string_->getLine(i));

      graphics_->getTextSize(line, &width1, &height1);

      label_mask_->drawString((label_width_ - width1)/2, y, line);

      y += height1 + 1;
    }

    label_mask_->combine(xwindow_, label_dx_ + label_offset_,
                         pixmap_dy_ + pixmap_height_ + std::max(pixmap_dy_, label_dy_), op);
  }

  mask_created_ = true;
}

void
CwmDeskIcon::
map()
{
  xwindow_->map();

  xwindow_->lower();

  //------

  window_->addNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_IN, &CwmDeskIcon::focusInNotifyProc, this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_OUT, &CwmDeskIcon::focusOutNotifyProc, this);

  //------

  addToGrid();
}

void
CwmDeskIcon::
unmap()
{
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_IN, &CwmDeskIcon::focusInNotifyProc, this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_OUT, &CwmDeskIcon::focusOutNotifyProc, this);

  //------

  xwindow_->unmap();

  removeFromGrid();
}

bool
CwmDeskIcon::
getMapped() const
{
  return xwindow_->getMapped();
}

void
CwmDeskIcon::
setBackground(CwmGraphics *graphics)
{
  xwindow_->setBackground(graphics);

  redraw();
}

bool
CwmDeskIcon::
isWindow(Window xwin)
{
  if (xwindow_->getXWin() == xwin)
    return true;

  return false;
}

bool
CwmDeskIcon::
isWindow(CwmWindow *xwindow)
{
  if (xwindow_ == xwindow)
    return true;

  return false;
}

void
CwmDeskIcon::
addToGrid()
{
  int x11 = icon_x_;
  int y11 = icon_y_;
  int x21 = icon_x_ + icon_width_  - 1;
  int y21 = icon_y_ + icon_height_ - 1;

  int x12 = x11;
  int y12 = y11;
  int x22 = x21;
  int y22 = y21;

  CwmGrid *grid = screen_.getIconGrid();

  grid->add(this, &x12, &y12, &x22, &y22, CWM_INSERT_LEFT, CWM_INSERT_BOTTOM);

  icon_x_ = x12;
  icon_y_ = y12;

  xwindow_->move(icon_x_, icon_y_);
}

void
CwmDeskIcon::
removeFromGrid()
{
  CwmGrid *grid = screen_.getIconGrid();

  grid->remove(this);
}

void
CwmDeskIcon::
processKeyPress(XKeyPressedEvent *event)
{
  XEvent *event1 = (XEvent *) event;

  if      (mgr_.getMoveEvent   ()->matchEvent(event1))
    move();
  else if (mgr_.getLowerEvent  ()->matchEvent(event1))
    lower();
  else if (mgr_.getRaiseEvent  ()->matchEvent(event1))
    raise();
  else if (mgr_.getRestoreEvent()->matchEvent(event1))
    restore();
  else if ( mgr_.getCloseEvent ()->matchEvent(event1))
    close();
}

void
CwmDeskIcon::
calcSize()
{
  pixmap_dx_ = 0;
  pixmap_dy_ = 0;

  if (CwmResourceDataInst->getIconImageBorder()) {
    pixmap_dx_ += 2;
    pixmap_dy_ += 2;
  }

  if (CwmResourceDataInst->getIconImageDecoration()) {
    pixmap_width_  = image_->getWidth ();
    pixmap_height_ = image_->getHeight();
  }

  //------

  label_dx_ = 0;
  label_dy_ = 0;

  if (CwmResourceDataInst->getIconImageBorder()) {
    label_dx_ += 2;
    label_dy_ += 2;
  }

  if (CwmResourceDataInst->getIconImageDecoration() &&
      CwmResourceDataInst->getIconLabelDecoration())
    label_dy_ = 2;

  getLabelSize(&label_width_, &label_height_);

  //------

  icon_width_ = std::max( label_width_ + 2* label_dx_, pixmap_width_ + 2*pixmap_dx_);

  icon_height_ = pixmap_dy_ + pixmap_height_ + std::max(pixmap_dy_, label_dy_) +
                 label_dy_  + label_height_;

  //------

  pixmap_offset_ = (icon_width_ - pixmap_width_ - 2*pixmap_dx_)/2;
  label_offset_  = (icon_width_ - label_width_  - 2*label_dy_ )/2;

}

void
CwmDeskIcon::
getLabelSize(int *width, int *height)
{
  *width  = 0;
  *height = 0;

  if (! CwmResourceDataInst->getIconLabelDecoration())
    return;

  int num_lines = fmt_string_->getNumLines();

  for (int i = 0; i < num_lines; ++i) {
    std::string line = fmt_string_->getLine(i);

    int width1, height1;

    graphics_->getTextSize(line, &width1, &height1);

    if (width1 > *width)
      *width = width1;

    *height += height1 + 1;
  }

  bool clip = true;

  if (CwmMachineInst->isFocusWindow(window_) &&
      CwmResourceDataInst->getIconActiveLabelDecoration())
    clip = false;

  if (! CwmResourceDataInst->getIconClipLabelDecoration())
    clip = false;

  if (! CwmResourceDataInst->getIconImageDecoration())
    clip = false;

  if (clip)
    *width = pixmap_width_;
  else {
    if (*width < pixmap_width_)
      *width = pixmap_width_;
  }
}

void
CwmDeskIcon::
focusIn()
{
  if (! CwmResourceDataInst->getIconLabelDecoration())
    return;

  calcSize();

  xwindow_->resize(icon_width_, icon_height_);
}

void
CwmDeskIcon::
focusOut()
{
  if (! CwmResourceDataInst->getIconLabelDecoration())
    return;

  calcSize();

  xwindow_->resize(icon_width_, icon_height_);
}

void
CwmDeskIcon::
processMenu()
{
  if (menu_def_ == 0)
    createMenuDef();

  CwmMenu::processWindowMenu(screen_, xwindow_, menu_def_);
}

void
CwmDeskIcon::
createMenuDef()
{
  menu_def_ = new CwmMenuDef();

  menu_def_->addButton("", "Restore", 'R', "Alt<Key>F5",
                       (CwmMenuProc) CwmDeskIcon::restoreProc, (CwmData) this);

  menu_def_->addButton("", "Move", 'M', "Alt<Key>F7",
                       (CwmMenuProc) CwmDeskIcon::moveProc, (CwmData) this);

  menu_def_->addButton("", "Raise", 'a', "Alt<Key>F2",
                       (CwmMenuProc) CwmDeskIcon::raiseProc, (CwmData) this);

  menu_def_->addButton("", "Lower", 'L', "Alt<Key>F3",
                       (CwmMenuProc) CwmDeskIcon::lowerProc, (CwmData) this);

  if (window_->getCloseFunctionHint()) {
    menu_def_->addSplitter("--------");

    menu_def_->addButton("", "Close", 'C', "Alt<Key>F4",
                         (CwmMenuProc) CwmDeskIcon::closeProc, (CwmData) this);
  }
}

void
CwmDeskIcon::
doubleClickProc(CwmWindow *, CwmData data, CwmData)
{
  CwmDeskIcon *icon = (CwmDeskIcon *) data;

  icon->restore();
}

void
CwmDeskIcon::
restoreProc(CwmDeskIcon *icon, CwmData)
{
  icon->restore();
}

void
CwmDeskIcon::
moveProc(CwmDeskIcon *icon, CwmData)
{
  icon->move();
}

void
CwmDeskIcon::
raiseProc(CwmDeskIcon *icon, CwmData)
{
  icon->raise();
}

void
CwmDeskIcon::
lowerProc(CwmDeskIcon *icon, CwmData)
{
  icon->lower();
}

void
CwmDeskIcon::
closeProc(CwmDeskIcon *icon, CwmData)
{
  icon->close();
}

void
CwmDeskIcon::
circulateUpProc(CwmDeskIcon *icon, CwmData)
{
  icon->screen_.circulateUp();
}

void
CwmDeskIcon::
circulateDownProc(CwmDeskIcon *icon, CwmData)
{
  icon->screen_.circulateDown();
}

void
CwmDeskIcon::
focusNextProc(CwmDeskIcon *icon, CwmData)
{
  CwmMachineInst->focusNext(icon->screen_);
}

void
CwmDeskIcon::
focusPrevProc(CwmDeskIcon *icon, CwmData)
{
  CwmMachineInst->focusPrev(icon->screen_);
}

void
CwmDeskIcon::
namedMenuProc(CwmDeskIcon *icon, CwmData data)
{
  std::string *name = (std::string *) data;

  CwmMenuDef *menu_def_ = CwmNamedMenuMgrInst->lookupMenuDef(*name);

  if (menu_def_ == 0) {
    CwmMachineInst->logf("Menu %s not found\n", (*name).c_str());
    return;
  }

  CwmMenu::processMenu(icon, menu_def_);
}

void
CwmDeskIcon::
focusInNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmDeskIcon *icon = (CwmDeskIcon *) data;

  icon->focusIn();
}

void
CwmDeskIcon::
focusOutNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmDeskIcon *icon = (CwmDeskIcon *) data;

  icon->focusOut();
}

CwmCustomDeskIconMgr *
CwmCustomDeskIconMgr::
getInstance()
{
  static CwmCustomDeskIconMgr *instance;

  if (! instance)
    instance = new CwmCustomDeskIconMgr();

  return instance;
}

void
CwmCustomDeskIconMgr::
setIcon(const std::string &pattern, const std::string &icon)
{
  CwmCustomDeskIcon *custom_icon = lookup(pattern);

  custom_icon->setIcon(icon);
}

void
CwmCustomDeskIconMgr::
setIconSmall(const std::string &pattern, const std::string &icon)
{
  CwmCustomDeskIcon *custom_icon = lookup(pattern);

  custom_icon->setIconSmall(icon);
}

std::string
CwmCustomDeskIconMgr::
getIcon(CwmWMWindow *window)
{
  std::string res_name  = window->getResName ();
  std::string res_class = window->getResClass();

  int num_icons = icons_.size();

  for (int i = 0; i < num_icons; i++) {
    if (icons_[i]->compare(res_name ) ||
        icons_[i]->compare(res_class))
      return icons_[i]->getIcon();
  }

  return "";
}

std::string
CwmCustomDeskIconMgr::
getIconSmall(CwmWMWindow *window)
{
  std::string res_name  = window->getResName ();
  std::string res_class = window->getResClass();

  int num_icons = icons_.size();

  for (int i = 0; i < num_icons; i++) {
    if (icons_[i]->compare(res_name) || icons_[i]->compare(res_class))
      return icons_[i]->getIconSmall();
  }

  return "";
}

void
CwmCustomDeskIconMgr::
deleteAll()
{
  int num_icons = icons_.size();

  for (int i = 0; i < num_icons; i++)
    delete icons_[i];

  icons_.clear();
}

CwmCustomDeskIcon *
CwmCustomDeskIconMgr::
lookup(const std::string &pattern)
{
  int num_icons = icons_.size();

  for (int i = 0; i < num_icons; i++)
    if (icons_[i]->isPattern(pattern))
      return icons_[i];

  CwmCustomDeskIcon *custom_icon = new CwmCustomDeskIcon(pattern);

  icons_.push_back(custom_icon);

  return custom_icon;
}

CwmCustomDeskIcon::
CwmCustomDeskIcon(const std::string &pattern) :
 pattern_(pattern), compile_(0), icon_(""), icon_small_("")
{
  compile_ = new CGlob(pattern_.c_str());
}

CwmCustomDeskIcon::
~CwmCustomDeskIcon()
{
  delete compile_;
}

bool
CwmCustomDeskIcon::
compare(const std::string &name)
{
  return compile_->compare(name);
}
