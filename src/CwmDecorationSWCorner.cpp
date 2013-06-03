#include "CwmDecorationI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmSWCornerChild::
CwmSWCornerChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_SW)
{
}

void
CwmSWCornerChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = 0;
  *y      = frame->getHeight() -
            decoration_->getResizeHeight() -
            decoration_->getBottomBorder();
  *width  = decoration_->getButtonWidth () +
            decoration_->getLeftBorder  ();
  *height = decoration_->getResizeHeight() +
            decoration_->getBottomBorder();
}

void
CwmSWCornerChild::
detail()
{
  int x1 = 0;
  int y1 = 0;
  int x2 = getWidth()  - 1;
  int y2 = getHeight() - 1;

  int w = decoration_->getLeftBorder  ();
  int h = decoration_->getBottomBorder();

  int oe = decoration_->getOutsideEdgeWidth();
  int ie = decoration_->getInsideEdgeWidth ();

  // Outside Bottom Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawHDarkLine(xwindow_, x1 + i + 1, x2, y2 - i);

  // Outside Left Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawVLightLine(xwindow_, x1 + i, y1, y2 - i);

  // Inside Bottom Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawHLightLine(xwindow_, x1 + w + i, x2, y2 - h + 1 - i);

  // Inside Left Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawVDarkLine(xwindow_, x1 + w + 1 + i, y1, y2 - h + 1 - i);

  // Top Left and Bottom Right Splitters

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawHLightLine(xwindow_, x1 + oe, x1 + (w - 1), y1);
    graphics_->drawVDarkLine (xwindow_, x2, y2 - (h - 2), y2 - oe);
  }
}
