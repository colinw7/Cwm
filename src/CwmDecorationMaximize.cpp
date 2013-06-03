#include "CwmDecorationI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmMaximizeChild::
CwmMaximizeChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE)
{
  info_string_ = "Maximize";
  sides_       = CSIDE_TYPE_ALL;
}

void
CwmMaximizeChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_    ->getFrame();
  CwmWindowChild *close = decoration_->getClose();

  *x      = frame->getWidth() -
            decoration_->getRightBorder() -
            decoration_->getButtonWidth();
  *y      = decoration_->getTopBorder();
  *width  = decoration_->getButtonWidth();
  *height = decoration_->getTitleHeight();

  if (close->getMapped())
    *x -= close->getWidth();

  if (*x <= decoration_->getLeftBorder() + 4)
    *width = 0;
}

void
CwmMaximizeChild::
detail()
{
  drawSides(sides_);

  int width  = getWidth ();
  int height = getHeight();

  int size = std::min(width - 8, height - 8);
  int xc   = width /2;
  int yc   = height/2;

  if (window_->isMaximized() || window_->isMinimized())
    graphics_->drawButtonIn (xwindow_, xc - size/2, yc - size/2, size, size, 1);
  else
    graphics_->drawButtonOut(xwindow_, xc - size/2, yc - size/2, size, size, 1);
}
