#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask      |
  SubstructureRedirectMask;

CwmContainerChild::
CwmContainerChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE)
{
}

void
CwmContainerChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = decoration_->getLeftBorder();
  *y      = decoration_->getTopBorder() +
            decoration_->getTitleHeight();
  *width  = frame->getWidth() -
            decoration_->getLeftBorder() -
            decoration_->getRightBorder();
  *height = frame->getHeight() -
            decoration_->getTopBorder() -
            decoration_->getBottomBorder() -
            decoration_->getTitleHeight();
}

void
CwmContainerChild::
detail()
{
}
