#include "CwmDecorationI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmMenuChild::
CwmMenuChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE)
{
  info_string_ = "Menu";
  sides_       = CSIDE_TYPE_HORIZONTAL;
}

void
CwmMenuChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = decoration_->getLeftBorder();
  *y      = decoration_->getTopBorder ();
  *width  = decoration_->getButtonWidth();
  *height = decoration_->getTitleHeight();

  if (*x + *width >= frame->getWidth() - decoration_->getRightBorder() - 4)
    *width = 0;
}

void
CwmMenuChild::
detail()
{
  drawSidesOut(sides_);

  graphics_->drawButtonOut(xwindow_,
                           3, (getHeight() - 1)/2 - 2,
                           getWidth() - 8, 4,
                           1);
}
