#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmSECornerChild::
CwmSECornerChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_SE)
{
}

void
CwmSECornerChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = frame->getWidth () -
            decoration_->getButtonWidth() -
            decoration_->getRightBorder();
  *y      = frame->getHeight() -
            decoration_->getResizeHeight() -
            decoration_->getBottomBorder();
  *width  = decoration_->getButtonWidth () +
            decoration_->getRightBorder ();
  *height = decoration_->getResizeHeight() +
            decoration_->getBottomBorder();
}

void
CwmSECornerChild::
detail()
{
  int x1 = 0;
  int y1 = 0;
  int x2 = getWidth()  - 1;
  int y2 = getHeight() - 1;

  int w = decoration_->getRightBorder ();
  int h = decoration_->getBottomBorder();

  int oe = decoration_->getOutsideEdgeWidth();
  int ie = decoration_->getInsideEdgeWidth ();

  // Outside Bottom Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawHDarkLine(xwindow_, x1 + i, x2 - i, y2 - i);

  // Outside Right Hightlight

  for (int i = 0; i < oe; i++)
    graphics_->drawVDarkLine(xwindow_, x2 - i, y2 - i - 1, y1 + i);

  // Inside Right Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawVLightLine(xwindow_, x2 - w + 1 - i, y1, y2 - h - i);

  // Inside Bottom Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawHLightLine(xwindow_, x1, x2 - w + 1 - i, y2 - h + 1 - i);

  // Top Right and Bottom Left Splitters

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawVLightLine(xwindow_, x1, y2 - 1, y2 - h + 1);
    graphics_->drawHLightLine(xwindow_, x2 - w + 1, x2 - 1, y1);
  }
}
