#include <CwmToolBarI.h>

static const int MIN_ICON_WIDTH = 16;
static const int DEF_ICON_WIDTH = 96;

static const int ICONIZED_BUTTON_WIDTH  = 8;
static const int ICONIZED_BUTTON_HEIGHT = 6;

CwmToolBarIconArea::
CwmToolBarIconArea(CwmToolBar &toolbar1, int x, int width) :
 toolbar_(toolbar1)
{
  CwmScreen &screen = toolbar_.getScreen();

  //------

  Pixel fg = CwmResourceDataInst->getToolBarForeground(screen);
  Pixel bg = CwmResourceDataInst->getToolBarBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getToolBarFont(screen);

  graphics_ = new CwmGraphics(screen, font, color);

  //------

  def_icon_width_ = CwmResourceDataInst->getToolBarIconWidth();

  if (def_icon_width_ <= 0 || def_icon_width_ >= screen.getWidth())
    def_icon_width_ = DEF_ICON_WIDTH;

  icon_width_ = def_icon_width_;

  xwindow_ =
    new CwmWindow(screen, toolbar_.getXWindow(),
                   x, 2, width, toolbar_.getHeight() - 4,
                   0, CWM_CURSOR_TITLE);

  xwindow_->map();

  //------

  restore_event_ = nullptr;
  close_event_   = nullptr;
}

CwmToolBarIconArea::
~CwmToolBarIconArea()
{
  delete xwindow_;
}

void
CwmToolBarIconArea::
addIcon(CwmWMWindow *window)
{
  CwmToolBarIcon *icon = new CwmToolBarIcon(*this, window);

  icons_.push_back(icon);

  redraw();
}

void
CwmToolBarIconArea::
removeIcon(CwmWMWindow *window)
{
  ToolBarIconList::iterator picon1 = icons_.begin();
  ToolBarIconList::iterator picon2 = icons_.end  ();

  for ( ; picon1 != picon2; ++picon1)
    if ((*picon1)->getWindow() == window)
      break;

  if (picon1 != picon2) {
    CwmToolBarIcon *icon = *picon1;

    icons_.erase(picon1);

    delete icon;

    redraw();
  }
}

CwmToolBarIcon *
CwmToolBarIconArea::
getIcon(CwmWindow *xwindow)
{
  for (auto &icon : icons_)
    if (icon->getXWindow() == xwindow)
      return icon;

  return nullptr;
}

CwmToolBarIcon *
CwmToolBarIconArea::
getIcon(Window xwin)
{
  for (auto &icon : icons_)
    if (icon->getXWindow()->getXWin() == xwin)
      return icon;

  return nullptr;
}

CwmToolBarIcon *
CwmToolBarIconArea::
getIcon(CwmWMWindow *window)
{
  for (auto &icon : icons_)
    if (icon->getWindow() == window)
      return icon;

  return nullptr;
}

void
CwmToolBarIconArea::
redraw()
{
  xwindow_->setBackground(graphics_);

  xwindow_->clear();

  resizeIcons();

  sortIcons();

  for (auto &icon : icons_) {
    if (icon->onCurrentDesk())
      icon->getXWindow()->map();
    else
      icon->getXWindow()->unmap();
  }

  for (auto &icon : icons_)
    if (icon->onCurrentDesk())
      icon->redraw();
}

void
CwmToolBarIconArea::
sortIcons()
{
  if (! toolbar_.getAddWindows())
    return;

  icons_.sort(CwmToolBarIcon::CwmToolIconCmp());

  int j = 0;

  for (auto &icon : icons_) {
    if (! icon->onCurrentDesk())
      continue;

    icon->move(j*icon_width_, 0);

    j++;
  }
}

void
CwmToolBarIconArea::
resizeIcons()
{
  int num_icons = 0;

  for (auto &icon : icons_) {
    if (icon->onCurrentDesk())
      num_icons++;
  }

  if (num_icons == 0)
    return;

  int width = num_icons*icon_width_;

  int delta = width - getWidth();

  if (delta > 0) {
    while (delta > 0) {
      delta -= num_icons;

      icon_width_--;
    }

    if (icon_width_ < MIN_ICON_WIDTH)
      icon_width_ = MIN_ICON_WIDTH;
  }
  else {
    while (delta < 0) {
      delta += num_icons;

      icon_width_++;
    }

    if (delta > 0)
      icon_width_--;

    if (icon_width_ > def_icon_width_)
      icon_width_ = def_icon_width_;
  }

  int j = 0;

  for (auto &icon : icons_) {
    if (! icon->onCurrentDesk())
      continue;

    icon->moveResize(j*icon_width_, 0, icon_width_, xwindow_->getHeight(), true);

    j++;
  }
}

int
CwmToolBarIconArea::
getX() const
{
  return xwindow_->getX();
}

int
CwmToolBarIconArea::
getWidth() const
{
  return xwindow_->getWidth();
}

CXNamedEvent *
CwmToolBarIconArea::
getRestoreEvent()
{
  if (! restore_event_)
    restore_event_ = new CXNamedEvent("Alt<Key>F5");

  return restore_event_;
}

CXNamedEvent *
CwmToolBarIconArea::
getCloseEvent()
{
  if (! close_event_)
    close_event_ = new CXNamedEvent("Alt<Key>F4");

  return close_event_;
}

CwmToolBarIcon::
CwmToolBarIcon(CwmToolBarIconArea &icon_area1, CwmWMWindow *window1) :
 icon_area_(icon_area1), toolbar_(icon_area1.getToolBar()), window_(window1)
{
  CwmScreen &screen = toolbar_.getScreen();

  //------

  Pixel fg = CwmResourceDataInst->getToolBarIconForeground(screen);
  Pixel bg = CwmResourceDataInst->getToolBarIconBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getToolBarIconFont(screen);

  normal_graphics_ = new CwmGraphics(screen, font, color);

  //------

  fg = CwmResourceDataInst->getToolBarIconFocusForeground(screen);
  bg = CwmResourceDataInst->getToolBarIconFocusBackground(screen);

  color = screen.getColor(fg, bg);

  font = CwmResourceDataInst->getToolBarIconFocusFont(screen);

  focus_graphics_ = new CwmGraphics(screen, font, color);

  //------

  graphics_ = normal_graphics_;

  //------

  border_     = toolbar_.getBorder();
  positioned_ = false;
  image_      = nullptr;

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ = new CwmWindow(screen, icon_area_.getXWindow(),
                           0, 2,
                           icon_area_.getIconWidth(),
                           toolbar_.getHeight() - 4,
                           event_mask,
                           CWM_CURSOR_TITLE);

  xwindow_->setBackground(graphics_);

  xwindow_->addCallback(CWM_CALLBACK_DOUBLE_CLICK_1,
                        &CwmToolBarIcon::doubleClickProc,
                        static_cast<CwmData>(this));
  xwindow_->addCallback(CWM_CALLBACK_IDLE,
                        &CwmToolBarIcon::idleProc,
                        static_cast<CwmData>(this));

  //------

  setImage();

  //------

  menu_def_ = nullptr;

  //------

  window_->addNotifyProc(CWM_WINDOW_NOTIFY_MOVE,
                         &CwmToolBarIcon::moveNotifyProc,
                         this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_ICONISE,
                         &CwmToolBarIcon::iconiseNotifyProc,
                         this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_RESTORE,
                         &CwmToolBarIcon::restoreNotifyProc,
                         this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_DESTROY,
                         &CwmToolBarIcon::destroyNotifyProc,
                         this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_IN,
                         &CwmToolBarIcon::focusInNotifyProc,
                         this);
  window_->addNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_OUT,
                         &CwmToolBarIcon::focusOutNotifyProc,
                         this);
}

CwmToolBarIcon::
~CwmToolBarIcon()
{
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_MOVE,
                            &CwmToolBarIcon::moveNotifyProc,
                            this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_ICONISE,
                            &CwmToolBarIcon::iconiseNotifyProc,
                            this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_RESTORE,
                            &CwmToolBarIcon::restoreNotifyProc,
                            this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_DESTROY,
                            &CwmToolBarIcon::destroyNotifyProc,
                            this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_IN,
                            &CwmToolBarIcon::focusInNotifyProc,
                            this);
  window_->removeNotifyProc(CWM_WINDOW_NOTIFY_FOCUS_OUT,
                            &CwmToolBarIcon::focusOutNotifyProc,
                            this);

  //------

  delete xwindow_;
}

void
CwmToolBarIcon::
setImage()
{
  image_ = window_->getImage(16, 16);
}

void
CwmToolBarIcon::
redraw()
{
  if (window_->isIconised())
    graphics_ = normal_graphics_;

  xwindow_->setBackground(graphics_);

  xwindow_->clear();

  graphics_->drawButtonOut(xwindow_,
                           0, 0,
                           xwindow_->getWidth(),
                           xwindow_->getHeight(),
                           border_);

  if (image_) {
    int x1 = border_ + 2;
    int y1 = xwindow_->getHeight()/2;

    image_->draw(xwindow_, graphics_, x1, y1 - 8);
  }

  if (window_->isIconised()) {
    int x1 = xwindow_->getWidth() - border_ - 2 - ICONIZED_BUTTON_WIDTH;
    int y1 = xwindow_->getHeight()/2;

    graphics_->drawButtonOut(xwindow_,
                             x1, y1 - 3,
                             ICONIZED_BUTTON_WIDTH, ICONIZED_BUTTON_HEIGHT,
                             border_);
  }

  std::string name = window_->getIconName();

  int x1 = border_ + 2;

  if (image_)
    x1 += image_->getWidth() + 2;

  int y1 = border_ + 1;

  int width1  = xwindow_->getWidth()  - x1 - 2;
  int height1 = xwindow_->getHeight() - y1 - 1;

  if (window_->isIconised())
    width1 -= 10;

  graphics_->drawTextCentered(xwindow_,
                              x1, y1, width1, height1,
                              name);
}

void
CwmToolBarIcon::
moveResize(int x, int y, int width, int height, bool force)
{
  if (force || ! positioned_) {
    xwindow_->moveResize(x, y, width, height);

    positioned_ = true;
  }
}

void
CwmToolBarIcon::
move(int x, int y)
{
  xwindow_->move(x, y);
}

bool
CwmToolBarIcon::
onCurrentDesk()
{
  return window_->onCurrentDesk();
}

void
CwmToolBarIcon::
processKeyPress(XKeyPressedEvent *event)
{
  XEvent *event1 = reinterpret_cast<XEvent *>(event);

  if      (icon_area_.getRestoreEvent()->matchEvent(event1))
    window_->restore();
  else if (icon_area_.getCloseEvent  ()->matchEvent(event1))
    window_->close();
}

void
CwmToolBarIcon::
focusIn()
{
  graphics_ = focus_graphics_;

  redraw();
}

void
CwmToolBarIcon::
focusOut()
{
  graphics_ = normal_graphics_;

  redraw();
}

void
CwmToolBarIcon::
processMenu()
{
  if (! menu_def_)
    createMenuDef();

  CwmScreen &screen = window_->getScreen();

  CwmMenu::processWindowMenu(screen, xwindow_, menu_def_);
}

void
CwmToolBarIcon::
createMenuDef()
{
  menu_def_ = new CwmMenuDef();

  menu_def_->addButton("", "Restore", 'R', "Alt<Key>F5",
                       function_cast<CwmMenuProc>(CwmToolBarIcon::restoreProc),
                       static_cast<CwmData>(this));

  if (window_->getCloseFunctionHint()) {
    menu_def_->addSplitter("--------");

    menu_def_->addButton("", "Close", 'C', "Alt<Key>F4",
                         function_cast<CwmMenuProc>(CwmToolBarIcon::closeProc),
                         static_cast<CwmData>(this));
  }
}

void
CwmToolBarIcon::
doubleClickProc(CwmWindow *, CwmData data, CwmData)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  if (icon->window_->isIconised())
    icon->window_->restore();
}

void
CwmToolBarIcon::
restoreProc(CwmToolBarIcon *icon, CwmData)
{
  icon->window_->restore();
}

void
CwmToolBarIcon::
closeProc(CwmToolBarIcon *icon, CwmData)
{
  icon->window_->close();
}

void
CwmToolBarIcon::
idleProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmToolBarIcon *icon       = static_cast<CwmToolBarIcon *>(data);
  CwmIdleState   *idle_state = static_cast<CwmIdleState *>(detail);

  std::string name = icon->window_->getName();

  CwmScreen &screen = icon->window_->getScreen();

  if (idle_state->getActive())
    screen.showInfo(name);
  else
    screen.hideInfo();
}

void
CwmToolBarIcon::
moveNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  icon->toolbar_.redraw();
}

void
CwmToolBarIcon::
iconiseNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  icon->toolbar_.redraw();
}

void
CwmToolBarIcon::
restoreNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  CwmToolBar &toolbar = icon->getToolBar();

  if (toolbar.getAddIcons())
    toolbar.removeIcon(window);
  else
    toolbar.redraw();
}

void
CwmToolBarIcon::
destroyNotifyProc(CwmWMWindow *window, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  CwmToolBar &toolbar = icon->getToolBar();

  toolbar.removeIcon(window);
}

void
CwmToolBarIcon::
focusInNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  icon->focusIn();
}

void
CwmToolBarIcon::
focusOutNotifyProc(CwmWMWindow *, CwmWindowNotifyType, void *data)
{
  CwmToolBarIcon *icon = static_cast<CwmToolBarIcon *>(data);

  icon->focusOut();
}

int
CwmToolBarIcon::CwmToolIconCmp::
operator()(CwmToolBarIcon *icon1, CwmToolBarIcon *icon2)
{
  if (  icon1->getWindow()->isIconised() &&
      ! icon2->getWindow()->isIconised())
    return false;

  if (! icon1->getWindow()->isIconised() &&
        icon2->getWindow()->isIconised())
    return true;

  CwmUserWindow *user1 = icon1->getWindow()->getUser();
  CwmUserWindow *user2 = icon2->getWindow()->getUser();

  if (user1 == nullptr || user2 == nullptr)
    return false;

  int cmp = user1->getX() - user2->getX();

  if (cmp == 0)
    cmp = user1->getY() - user2->getY();

  return (cmp < 0);
}
