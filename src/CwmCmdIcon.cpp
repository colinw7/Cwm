#include <CwmI.h>

// IconMar: Handles Toolbar Command Icons

CwmCmdIconMgr *
CwmCmdIconMgr::
getInstance()
{
  static CwmCmdIconMgr *instance;

  if (! instance)
    instance = new CwmCmdIconMgr();

  return instance;
}

CwmCmdIconMgr::
CwmCmdIconMgr()
{
}

CwmCmdIconMgr::
~CwmCmdIconMgr()
{
  deleteAll();
}

void
CwmCmdIconMgr::
addCmdIcon(CwmScreen &screen, CwmCustomIcon &icon_def)
{
  CwmCmdIcon *cmd_icon = new CwmCmdIcon(screen, icon_def);

  command_icon_list_.push_back(cmd_icon);

  Window xwin = cmd_icon->getXWindow()->getXWin();

  command_icon_map_[xwin] = cmd_icon;
}

CwmCmdIcon *
CwmCmdIconMgr::
lookup(const CwmWindow &window)
{
  Window xwin = window.getXWin();

  return lookup(xwin);
}

CwmCmdIcon *
CwmCmdIconMgr::
lookup(Window xwin)
{
  CmdIconMap::iterator picon1 = command_icon_map_.find(xwin);

  if (picon1 == command_icon_map_.end())
    return 0;

  return (*picon1).second;
}

void
CwmCmdIconMgr::
deleteAll()
{
  for (auto &command_icon : command_icon_list_)
    delete command_icon;

  command_icon_list_.clear();
}

//--------------

CwmCmdIcon::
CwmCmdIcon(CwmScreen &screen, CwmCustomIcon &icon_def) :
 screen_(screen)
{
  graphics_ = icon_def.getGraphics(screen);

  //------

  icon_x_ = icon_def.getX();
  icon_y_ = icon_def.getY();

  pixmap_dx_ = 0;
  label_dx_  = 0;

  //------

  image_ = icon_def.getImage(screen);

  pixmap_mask_ = None;
  label_mask_  = None;

  //------

  pixmap_width_  = image_->getWidth ();
  pixmap_height_ = image_->getHeight();

  label_ = icon_def.getLabel();

  graphics_->getTextSize(label_, &label_width_, &label_height_);

  if (label_width_ > pixmap_width_)
    pixmap_dx_ += (label_width_  - pixmap_width_)/2;
  else
    label_dx_  += (pixmap_width_ - label_width_ )/2;

  //------

  icon_width_  = std::max(pixmap_width_, label_width_);
  icon_height_ = pixmap_height_ + label_height_;

  //------

  command_ = icon_def.getCommand();

  //------

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ = new CwmWindow(screen_, screen_.getRoot(),
                           icon_x_, icon_y_, icon_width_, icon_height_,
                           event_mask, CWM_CURSOR_TITLE);

  //------

  xwindow_->addCallback(CWM_CALLBACK_DOUBLE_CLICK_1, &CwmCmdIcon::invokeProc, (CwmData) this);

  //------

  xwindow_->map();

  xwindow_->lower();

  //------

  reposition();
}

CwmCmdIcon::
~CwmCmdIcon()
{
  delete xwindow_;
  delete graphics_;
  delete pixmap_mask_;
  delete label_mask_;
}

void
CwmCmdIcon::
reposition()
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

  grid->remove(this);

  grid->add(this, &x12, &y12, &x22, &y22, CWM_INSERT_TOP, CWM_INSERT_LEFT);

  int dx = x12 - x11;
  int dy = y12 - y11;

  icon_x_ += dx;
  icon_y_ += dy;

  xwindow_->move(icon_x_, icon_y_);
}

void
CwmCmdIcon::
move()
{
  int dx, dy;

  xwindow_->raise();

  CwmMoveWindowInst->moveInteractive(screen_, xwindow_, icon_x_, icon_y_,
                                     icon_width_, icon_height_, &dx, &dy);

  if (icon_x_ + dx < 0)
    dx = -icon_x_;

  if (icon_y_ + dy < 0)
    dy = -icon_y_;

  if (icon_x_ + icon_width_  + dx > screen_.getWidth())
    dx = screen_.getWidth()  - icon_x_ - icon_width_;

  if (icon_y_ + icon_height_ + dy > screen_.getHeight())
    dy = screen_.getHeight() - icon_y_ - icon_height_;

  icon_x_ += dx;
  icon_y_ += dy;

  reposition();

  xwindow_->lower();

  redraw();
}

void
CwmCmdIcon::
redraw()
{
#ifdef SHAPE
  if (pixmap_mask_ == None)
    createMask();
#endif

  image_->draw(xwindow_, graphics_, pixmap_dx_, 0);

  if (label_ != "")
    graphics_->drawTextCentered(xwindow_, label_dx_, pixmap_height_,
                                label_width_, label_height_, label_);
}

void
CwmCmdIcon::
createMask()
{
  pixmap_mask_ = image_->createMask();

  CwmMachineInst->shapeCombineMask(xwindow_->getXWin(), pixmap_mask_->getXPixmap(),
                                   pixmap_dx_, 0, ShapeSet);

  if (label_ != "") {
    label_mask_ = new CwmMask(screen_, graphics_, label_width_, label_height_);

    label_mask_->drawString(0, 0, label_);

    label_mask_->combine(xwindow_, label_dx_, pixmap_height_, ShapeUnion);
  }
  else
    label_mask_ = 0;
}

void
CwmCmdIcon::
invokeProc(CwmWindow *, CwmData data, CwmData)
{
  CwmCmdIcon *icon = (CwmCmdIcon *) data;

  icon->invoke();
}

void
CwmCmdIcon::
invoke()
{
  if (command_.size() > 0) {
    UnixCmd command(screen_, command_);

    command.run();
  }
}
