#include "CwmDecorationI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmNWCornerChild::
CwmNWCornerChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_NW)
{
}

void
CwmNWCornerChild::
calcSize(int *x, int *y, int *width, int *height)
{
  *x      = 0;
  *y      = 0;
  *width  = decoration_->getButtonWidth() +
            decoration_->getLeftBorder ();
  *height = decoration_->getResizeHeight() +
            decoration_->getTopBorder   ();
}

void
CwmNWCornerChild::
detail()
{
  int x1 = 0;
  int y1 = 0;
  int x2 = getWidth()  - 1;
  int y2 = getHeight() - 1;

  int w = decoration_->getLeftBorder();
  int h = decoration_->getTopBorder ();

  int oe = decoration_->getOutsideEdgeWidth();
  int ie = decoration_->getInsideEdgeWidth ();

  // Outside Top Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawHLightLine(xwindow_, x1 + i, x2 - i, y1 + i);

  // Outside Left Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawVLightLine(xwindow_, x1 + i, y1 + i + 1, y2 - i);

  // Inside Top Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawHDarkLine(xwindow_, x1 + w - 1 + i, x2, y1 + h - 1 + i);

  // Inside Left Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawVDarkLine(xwindow_, x1 + w - 1 + i, y1 + h + i, y2);

  // Top Right and Bottom Left Splitters */

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawVDarkLine(xwindow_, x2, y1 + 1, y1 + h - 1);
    graphics_->drawHDarkLine(xwindow_, x1 + w - 2, x1 + 1, y2);
  }
}
