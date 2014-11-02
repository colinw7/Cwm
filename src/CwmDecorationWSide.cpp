#include <CwmDecorationI.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmWSideChild::
CwmWSideChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_W)
{
}

void
CwmWSideChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame = window_->getFrame();

  *x      = 0;
  *y      = decoration_->getResizeHeight() +
            decoration_->getTopBorder   ();
  *width  = decoration_->getLeftBorder();
  *height = frame->getHeight() -
            2*decoration_->getResizeHeight() -
            decoration_->getTopBorder() -
            decoration_->getBottomBorder();
}

void
CwmWSideChild::
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
    graphics_->drawVLightLine(xwindow_, x1 + i, y1, y2);

  // Inside Hightlight

  for (int i = 0; i < ie; i++)
    graphics_->drawVDarkLine (xwindow_, x2 - i, y1, y2);

  // Top and Bottom Splitters

  if (decoration_->hasResizeSplitter()) {
    graphics_->drawHLightLine(xwindow_, x1 + 1, x2 - 1, y1);
    graphics_->drawHDarkLine (xwindow_, x1 + 1, x2 - 1, y2);
  }
}
