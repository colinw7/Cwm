#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmMinimizeChild::
CwmMinimizeChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE)
{
  info_string_ = "Minimize";
  sides_       = CSIDE_TYPE_ALL;
}

void
CwmMinimizeChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame    = window_    ->getFrame   ();
  CwmWindowChild *close    = decoration_->getClose   ();
  CwmWindowChild *maximize = decoration_->getMaximize();

  *x      = frame->getWidth() -
            decoration_->getRightBorder() -
            decoration_->getButtonWidth();
  *y      = decoration_->getTopBorder  ();
  *width  = decoration_->getButtonWidth();
  *height = decoration_->getTitleHeight();

  if (close->getMapped())
    *x -= close->getWidth();

  if (maximize->getMapped())
    *x -= maximize->getWidth();

  if (*x <= decoration_->getLeftBorder() + 4)
    *width = 0;
}

void
CwmMinimizeChild::
detail()
{
  drawSides(sides_);

  int width  = getWidth ();
  int height = getHeight();

  int size = 4;
  int xc   = width /2;
  int yc   = height/2;

  graphics_->drawButtonOut(xwindow_, xc - size/2, yc - size/2, size, size, 1);
}
