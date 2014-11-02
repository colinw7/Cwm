#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmNECornerChild::
CwmNECornerChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_NE)
{
}

void
CwmNECornerChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = frame->getWidth() -
            decoration_->getButtonWidth() -
            decoration_->getRightBorder();
  *y      = 0;
  *width  = decoration_->getButtonWidth() +
            decoration_->getRightBorder();
  *height = decoration_->getResizeHeight() +
            decoration_->getTopBorder();
}

void
CwmNECornerChild::
detail()
{
  int x1 = 0;
  int y1 = 0;
  int x2 = getWidth()  - 1;
  int y2 = getHeight() - 1;

  int w = decoration_->getRightBorder();
  int h = decoration_->getTopBorder  ();

  int oe = decoration_->getOutsideEdgeWidth();
  int ie = decoration_->getInsideEdgeWidth ();

  // Outside Top Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawHLightLine(xwindow_, x1, x2 - i, y1 + i);

  // Outside Right Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawVDarkLine (xwindow_, x2 - i, y1 + i + 1, y2);

  // Inside Right Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawVLightLine(xwindow_, x2 - w + 1 - i, y1 + h + i, y2);

  // Inside Top Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawHDarkLine (xwindow_, x1, x2 - w + 1 - i, y1 + h - 1 + i);

  // Top Left and Bottom Right Splitters

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawVLightLine(xwindow_, x1, y1 + oe, y1 + (h - 1));
    graphics_->drawHDarkLine (xwindow_, x2 - (w - 2), x2 - oe, y2);
  }
}
