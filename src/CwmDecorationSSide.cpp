#include "CwmDecorationI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmSSideChild::
CwmSSideChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_S)
{
}

void
CwmSSideChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = decoration_->getButtonWidth() +
            decoration_->getLeftBorder ();
  *y      = frame->getHeight() -
            decoration_->getBottomBorder();
  *width  = frame->getWidth () -
            2*decoration_->getButtonWidth() -
            decoration_->getLeftBorder () -
            decoration_->getRightBorder();
  *height = decoration_->getBottomBorder();
}

void
CwmSSideChild::
detail()
{
  int x1 = 0;
  int y1 = 0;
  int x2 = getWidth () - 1;
  int y2 = getHeight() - 1;

  int oe = decoration_->getOutsideEdgeWidth();
  int ie = decoration_->getInsideEdgeWidth ();

  // Outside Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawHDarkLine(xwindow_, x1, x2, y2 - i);

  // Inside Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawHLightLine(xwindow_, x1, x2, y1 + i);

  // Top and Bottom Splitters

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawVDarkLine (xwindow_, x2, y2 - 1, y1 + 1);
    graphics_->drawVLightLine(xwindow_, x1, y2 - 1, y1    );
  }
}
