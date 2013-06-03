#include "CwmToolBarI.h"

CwmToolBarCmd::
CwmToolBarCmd(CwmToolBar &toolbar, CwmToolBarIconDef *icon_def, int x) :
 toolbar_(toolbar), screen_(toolbar.getScreen())
{
  string fg_color = icon_def->getFg();

  if (fg_color == "")
    fg_color = CwmResourceDataInst->getIconForeground();

  Pixel fg = screen_.getPixel(fg_color, screen_.getBlackPixel());

  string bg_color = icon_def->getBg();

  if (bg_color == "")
    bg_color = CwmResourceDataInst->getIconBackground();

  Pixel bg = screen_.getPixel(bg_color, screen_.getWhitePixel());

  CwmColor *color = screen_.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getIconFont(screen_);

  graphics_ = new CwmGraphics(screen_, font, color);

  //------

  image_ = CwmImageMgrInst->getImage(screen_, icon_def->getIcon(),
                                     16, 16);

  if (image_ == NULL)
    image_ = CwmImageMgrInst->getImage(screen_, "", 16, 16);

  //------

  command_ = icon_def->getCommand();

  add_display_ = icon_def->getAddDisplay();

  //------

  label_ = icon_def->getLabel();

  //------

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  int y = (toolbar_.getHeight() - image_->getHeight())/2;

  xwindow_ =
    new CwmWindow(screen_, toolbar_.getXWindow(),
                   x + 2, y,
                   image_->getWidth () + 2,
                   image_->getHeight() + 2,
                   event_mask,
                   CWM_CURSOR_TITLE);

  //------

  active_  = false;
  pressed_ = false;

  //------

  xwindow_->addCallback(CWM_CALLBACK_ENTER,
                        CwmToolBarCmd::enterProc,
                        (CwmData) this);
  xwindow_->addCallback(CWM_CALLBACK_LEAVE,
                        CwmToolBarCmd::leaveProc,
                        (CwmData) this);
  xwindow_->addCallback(CWM_CALLBACK_BUTTON_PRESS,
                        &CwmToolBarCmd::pressProc,
                        (CwmData) this);
  xwindow_->addCallback(CWM_CALLBACK_BUTTON_RELEASE,
                        &CwmToolBarCmd::releaseProc,
                        (CwmData) this);
  xwindow_->addCallback(CWM_CALLBACK_SINGLE_CLICK_1,
                        &CwmToolBarCmd::invokeProc,
                        (CwmData) this);
  xwindow_->addCallback(CWM_CALLBACK_IDLE,
                        &CwmToolBarCmd::idleProc,
                        (CwmData) this);

  //------

  xwindow_->map();
}

CwmToolBarCmd::
~CwmToolBarCmd()
{
  delete xwindow_;

  delete graphics_;
}

void
CwmToolBarCmd::
redraw()
{
  xwindow_->setBackground(graphics_);

  xwindow_->clear();

  if      (pressed_)
    graphics_->drawButtonIn(xwindow_,
                            0, 0,
                            xwindow_->getWidth (),
                            xwindow_->getHeight(),
                            1);
  else if (active_)
    graphics_->drawButtonOut(xwindow_,
                             0, 0,
                             xwindow_->getWidth (),
                             xwindow_->getHeight(),
                             1);

  image_->draw(xwindow_, graphics_, 1, 1);
}

void
CwmToolBarCmd::
pressProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmToolBarCmd *icon = (CwmToolBarCmd *) data;

  XButtonPressedEvent *event = (XButtonPressedEvent *) detail;

  if (event->button != 1)
    return;

  icon->pressed_ = true;

  icon->redraw();
}

void
CwmToolBarCmd::
releaseProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmToolBarCmd *icon = (CwmToolBarCmd *) data;

  XButtonPressedEvent *event = (XButtonPressedEvent *) detail;

  if (event->button != 1)
    return;

  icon->pressed_ = false;

  icon->redraw();
}

void
CwmToolBarCmd::
invokeProc(CwmWindow *, CwmData data, CwmData)
{
  CwmToolBarCmd *icon = (CwmToolBarCmd *) data;

  icon->invoke();
}

void
CwmToolBarCmd::
invoke()
{
  if (command_ != "") {
    UnixCmd command(screen_, command_);

    command.setAddDisplay(add_display_);

    command.run();
  }
}

int
CwmToolBarCmd::
getWidth() const
{
  return xwindow_->getWidth() + 4;
}

void
CwmToolBarCmd::
enterProc(CwmWindow *, CwmData data, CwmData)
{
  CwmToolBarCmd *icon = (CwmToolBarCmd *) data;

  icon->active_ = true;

  icon->redraw();
}

void
CwmToolBarCmd::
leaveProc(CwmWindow *, CwmData data, CwmData)
{
  CwmToolBarCmd *icon = (CwmToolBarCmd *) data;

  icon->active_ = false;

  icon->redraw();
}

void
CwmToolBarCmd::
idleProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmToolBarCmd *icon       = (CwmToolBarCmd *) data;
  CwmIdleState  *idle_state = (CwmIdleState  *) detail;

  if (idle_state->getActive())
    icon->screen_.showInfo(icon->label_);
  else
    icon->screen_.hideInfo();
}
