#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmCloseChild::
CwmCloseChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE)
{
  info_string_ = "Close";
  sides_       = CSIDE_TYPE_NOT_RIGHT;
}

void
CwmCloseChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = frame->getWidth() -
            decoration_->getRightBorder() -
            decoration_->getButtonWidth();
  *y      = decoration_->getTopBorder();
  *width  = decoration_->getButtonWidth();
  *height = decoration_->getTitleHeight();

  if (*x <= decoration_->getLeftBorder() + 4)
    *width = 0;
}

void
CwmCloseChild::
detail()
{
  drawSides(sides_);

  int xc = getWidth ()/2;
  int yc = getHeight()/2;

  int size = 8;

  int x1 = xc - size/2;
  int x2 = xc + size/2;
  int y1 = yc - size/2;
  int y2 = yc + size/2;

  graphics_->drawDarkLine(xwindow_, x1    , y1 + 1, x2 - 1, y2    );
  graphics_->drawDarkLine(xwindow_, x1    , y1    , x2    , y2    );
  graphics_->drawDarkLine(xwindow_, x1 + 1, y1    , x2    , y2 - 1);
  graphics_->drawDarkLine(xwindow_, x1 + 1, y2    , x2    , y1 + 1);
  graphics_->drawDarkLine(xwindow_, x1    , y2    , x2    , y1    );
  graphics_->drawDarkLine(xwindow_, x1    , y2 - 1, x2 - 1, y1    );
}
