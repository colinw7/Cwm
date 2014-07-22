#include "CwmI.h"

CwmGraphics::
CwmGraphics(CwmScreen &screen, CwmFont *font, CwmColor *color, bool is_xor) :
 screen_(screen), font_(font), color_(color)
{
  if (! is_xor)
    gc_ = screen_.createGC(color_);
  else
    gc_ = screen_.createXorGC(color_);

  init();
}

CwmGraphics::
CwmGraphics(CwmWMWindow &window, CwmFont *font, CwmColor *color, bool is_xor) :
 screen_(window.getScreen()), font_(font), color_(color)
{
  if (! is_xor)
    gc_ = screen_.createGC(color_);
  else
    gc_ = screen_.createXorGC(color_);

  init();
}

CwmGraphics::
CwmGraphics(CwmXPixmap &pixmap, CwmFont *font, CwmColor *color, bool /*is_xor*/) :
 screen_(pixmap.getScreen()), font_(font), color_(color)
{
  gc_ = pixmap.createGC();

  init();
}

CwmGraphics::
~CwmGraphics()
{
  CwmMachineInst->freeGC(gc_);

  delete stipple_bitmap_;
}

void
CwmGraphics::
init()
{
  stipple_bitmap_ = NULL;
}

void
CwmGraphics::
drawText(CwmWindow *xwindow, int x, int y, const string &text)
{
  setForeground();

  font_->drawString(xwindow, this, x, y, text);
}

void
CwmGraphics::
drawTextAligned(CwmWindow *xwindow, int x, int y, int width, int height,
                CHAlignType h_align, CVAlignType /*v_align*/, const string &text)
{
  if      (h_align == CHALIGN_TYPE_LEFT)
    drawTextLeft(xwindow, x, y, width, height, text);
  else if (h_align == CHALIGN_TYPE_RIGHT)
    drawTextRight(xwindow, x, y, width, height, text);
  else
    drawTextCentered(xwindow, x, y, width, height, text);
}

void
CwmGraphics::
drawTextLeft(CwmWindow *xwindow, int x, int y, int width, int height, const string &text)
{
  setTextSize(text);

  drawTextI(xwindow, x, y, width, height, CVALIGN_TYPE_CENTER, text);
}

void
CwmGraphics::
drawTextRight(CwmWindow *xwindow, int x, int y, int width, int height, const string &text)
{
  setTextSize(text);

  int dx = width - text_width_;

  dx = std::max(dx, 0);

  drawTextI(xwindow, x + dx, y, width, height, CVALIGN_TYPE_CENTER, text);
}

void
CwmGraphics::
drawTextCentered(CwmWindow *xwindow, int x, int y, int width, int height, const string &text)
{
  setTextSize(text);

  int dx = (width - text_width_)/2;

  dx = std::max(dx, 0);

  drawTextI(xwindow, x + dx, y, width, height, CVALIGN_TYPE_CENTER, text);
}

void
CwmGraphics::
setTextSize(const string &text)
{
  getTextSize(text, &text_width_, &text_height_);
}

int
CwmGraphics::
getFontHeight()
{
  return font_->getHeight();
}

int
CwmGraphics::
getFontAscent()
{
  return font_->getAscent();
}

int
CwmGraphics::
getFontDescent()
{
  return font_->getDescent();
}

void
CwmGraphics::
getTextSize(const string &text, int *width, int *height)
{
  font_->getTextSize(text, width, height);
}

void
CwmGraphics::
drawTextI(CwmWindow *xwindow, int x, int y, int width, int height,
          CVAlignType v_align, const string &text)
{
  int dy = 0;

  if (v_align == CVALIGN_TYPE_TOP)
    dy += height - text_height_;
  else if (v_align == CVALIGN_TYPE_BOTTOM)
    ;
  else {
    dy += (height - text_height_)/2;
  }

  dy = std::max(dy, 0);

  startClip(x, y, width, height);

  setForeground();

  font_->drawString(xwindow, this, x, y + dy, text);

  endClip();
}

void
CwmGraphics::
startClip(int x, int y, int width, int height)
{
  CwmMachineInst->startRectClip(gc_, x, y, width, height);
}

void
CwmGraphics::
startClip(CwmXPixmap *xpixmap, int dx, int dy)
{
  CwmMachineInst->startPixmapClip(gc_, xpixmap->getXPixmap(), dx, dy);
}

void
CwmGraphics::
endClip()
{
  CwmMachineInst->endPixmapClip(gc_);
}

void
CwmGraphics::
drawButtonOut(CwmWindow *xwindow, int x, int y, int width, int height, int thick)
{
  int x1 = x;
  int y1 = y;
  int x2 = x + width  - 1;
  int y2 = y + height - 1;

  setDarkForeground();

  int i;

  for (i = 0; i < thick; i++) {
    drawLineI(xwindow, x2 - i, y2 - i, x1 + i, y2 - i);
    drawLineI(xwindow, x2 - i, y2 - i, x2 - i, y1 + i);
  }

  setLightForeground();

  for (i = 0; i < thick; i++) {
    drawLineI(xwindow, x1 + i, y1 + i, x2 - i, y1 + i);
    drawLineI(xwindow, x1 + i, y1 + i, x1 + i, y2 - i);
  }
}

void
CwmGraphics::
drawButtonIn(CwmWindow *xwindow, int x, int y, int width, int height, int thick)
{
  int x1 = x;
  int y1 = y;
  int x2 = x + width  - 1;
  int y2 = y + height - 1;

  setLightForeground();

  int i;

  for (i = 0; i < thick; i++) {
    drawLineI(xwindow, x2 - i, y2 - i, x1 + i, y2 - i);
    drawLineI(xwindow, x2 - i, y2 - i, x2 - i, y1 + i);
  }

  setDarkForeground();

  for (i = 0; i < thick; i++) {
    drawLineI(xwindow, x1 + i, y1 + i, x2 - i, y1 + i);
    drawLineI(xwindow, x1 + i, y1 + i, x1 + i, y2 - i);
  }
}

void
CwmGraphics::
drawSidesOut(CwmWindow *xwindow, int sides, int x, int y, int width, int height, int thick)
{
  int x1 = x;
  int y1 = y;
  int x2 = x + width  - 1;
  int y2 = y + height - 1;

  setDarkForeground();

  int i;

  for (i = 0; i < thick; i++) {
    if (sides & CSIDE_TYPE_BOTTOM)
      drawLineI(xwindow, x2 - i, y2 - i, x1 + i, y2 - i);

    if (sides & CSIDE_TYPE_RIGHT)
      drawLineI(xwindow, x2 - i, y2 - i, x2 - i, y1 + i);
  }

  setLightForeground();

  for (i = 0; i < thick; i++) {
    if (sides & CSIDE_TYPE_TOP)
      drawLineI(xwindow, x1 + i, y1 + i, x2 - i, y1 + i);

    if (sides & CSIDE_TYPE_LEFT)
      drawLineI(xwindow, x1 + i, y1 + i, x1 + i, y2 - i);
  }
}

void
CwmGraphics::
drawSidesIn(CwmWindow *xwindow, int sides, int x, int y, int width, int height, int thick)
{
  int x1 = x;
  int y1 = y;
  int x2 = x + width  - 1;
  int y2 = y + height - 1;

  setLightForeground();

  int i;

  for (i = 0; i < thick; i++) {
    if (sides & CSIDE_TYPE_BOTTOM)
      drawLineI(xwindow, x2 - i, y2 - i, x1 + i, y2 - i);

    if (sides & CSIDE_TYPE_RIGHT)
      drawLineI(xwindow, x2 - i, y2 - i, x2 - i, y1 + i);
  }

  setDarkForeground();

  for (i = 0; i < thick; i++) {
    if (sides & CSIDE_TYPE_TOP)
      drawLineI(xwindow, x1 + i, y1 + i, x2 - i, y1 + i);

    if (sides & CSIDE_TYPE_LEFT)
      drawLineI(xwindow, x1 + i, y1 + i, x1 + i, y2 - i);
  }
}

void
CwmGraphics::
drawEtchedLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  setDarkForeground();

  drawLineI(xwindow, x1, y1    , x2, y2    );

  setLightForeground();

  drawLineI(xwindow, x1, y1 + 1, x2, y2 + 1);
}

void
CwmGraphics::
drawRightArrowOut(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  setDarkForeground();

  drawLineI(xwindow, x1, y2    , x2, y2 - 3);
  drawLineI(xwindow, x1, y2 - 1, x2, y2 - 4);

  setLightForeground();

  drawLineI(xwindow, x1, y1    , x1, y2    );
  drawLineI(xwindow, x1, y1    , x1, y2 - 2);
  drawLineI(xwindow, x1, y1    , x2, y1 + 3);
  drawLineI(xwindow, x1, y1 + 1, x2, y1 + 4);
}

void
CwmGraphics::
drawLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  setForeground();

  drawLineI(xwindow, x1, y1, x2, y2);
}

void
CwmGraphics::
drawHLine(CwmWindow *xwindow, int x1, int x2, int y)
{
  drawLine(xwindow, x1, y, x2, y);
}

void
CwmGraphics::
drawHLine(CwmWindow *xwindow, int x1, int x2, int y, const CRGBA &rgba)
{
  setForeground(rgba);

  drawLineI(xwindow, x1, y, x2, y);
}

void
CwmGraphics::
drawVLine(CwmWindow *xwindow, int x, int y1, int y2)
{
  drawLine(xwindow, x, y1, x, y2);
}

void
CwmGraphics::
drawVLine(CwmWindow *xwindow, int x, int y1, int y2, const CRGBA &rgba)
{
  setForeground(rgba);

  drawLineI(xwindow, x, y1, x, y2);
}

void
CwmGraphics::
drawDLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  if (x1 < 0 || x1 >= xwindow->getWidth () ||
      x2 < 0 || x2 >= xwindow->getWidth () ||
      y1 < 0 || y1 >= xwindow->getHeight() ||
      y2 < 0 || y2 >= xwindow->getHeight())
    return;

  int dx = (x2 > x1 ? 1 : -1);
  int dy = (y2 > y1 ? 1 : -1);

  int x = x1;
  int y = y1;

  while (true) {
    if      (dx ==  1 && x > x2) break;
    else if (dx == -1 && x < x2) break;
    if      (dy ==  1 && y > y2) break;
    else if (dy == -1 && y < y2) break;

    drawPoint(xwindow, x, y);

    x += dx;
    y += dy;
  }
}

void
CwmGraphics::
drawDarkLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  setDarkForeground();

  drawLineI(xwindow, x1, y1, x2, y2);
}

void
CwmGraphics::
drawVDarkLine(CwmWindow *xwindow, int x, int y1, int y2)
{
  drawDarkLine(xwindow, x, y1, x, y2);
}

void
CwmGraphics::
drawHDarkLine(CwmWindow *xwindow, int x1, int x2, int y)
{
  drawDarkLine(xwindow, x1, y, x2, y);
}

void
CwmGraphics::
drawLightLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  setLightForeground();

  drawLineI(xwindow, x1, y1, x2, y2);
}

void
CwmGraphics::
drawVLightLine(CwmWindow *xwindow, int x, int y1, int y2)
{
  drawLightLine(xwindow, x, y1, x, y2);
}

void
CwmGraphics::
drawHLightLine(CwmWindow *xwindow, int x1, int x2, int y)
{
  drawLightLine(xwindow, x1, y, x2, y);
}

void
CwmGraphics::
drawLineI(CwmWindow *xwindow, int x1, int y1, int x2, int y2)
{
  CwmMachineInst->drawLine(xwindow->getXWin(), gc_, x1, y1, x2, y2);
}

void
CwmGraphics::
drawRectangle(CwmWindow *xwindow, int x, int y, int width, int height)
{
  CwmMachineInst->drawRectangle(xwindow->getXWin(), gc_, x, y, width, height);
}

void
CwmGraphics::
drawRectangle(CwmXPixmap *xpixmap, int x, int y, int width, int height)
{
  CwmMachineInst->drawRectangle(xpixmap->getXPixmap(), gc_, x, y, width, height);
}

void
CwmGraphics::
fillRectangle(CwmWindow *xwindow, int x, int y, int width, int height)
{
  CwmMachineInst->fillRectangle(xwindow->getXWin(), gc_, x, y, width, height);
}

void
CwmGraphics::
fillRectangle(CwmXPixmap *xpixmap, int x, int y, int width, int height)
{
  CwmMachineInst->fillRectangle(xpixmap->getXPixmap(), gc_, x, y, width, height);
}

void
CwmGraphics::
drawPoint(CwmWindow *xwindow, int x, int y)
{
  CwmMachineInst->drawPoint(xwindow->getXWin(), gc_, x, y);
}

void
CwmGraphics::
setForeground()
{
  CwmMachineInst->setForeground(gc_, color_->getFg());
}

void
CwmGraphics::
setForeground(const CRGBA &rgba)
{
  CwmMachineInst->setForeground(gc_, screen_.getPixel(rgba));
}

void
CwmGraphics::
setForeground(int ind)
{
  CwmMachineInst->setForeground(gc_, ind);
}

void
CwmGraphics::
setBackground()
{
  CwmMachineInst->setBackground(gc_, color_->getBg());
}

void
CwmGraphics::
setDarkForeground()
{
  CwmMachineInst->setForeground(gc_, color_->getDark());
}

void
CwmGraphics::
setLightForeground()
{
  CwmMachineInst->setForeground(gc_, color_->getLight());
}

void
CwmGraphics::
copyArea(CwmWindow *src, CwmWindow *dest, int src_x, int src_y,
         int src_width, int src_height, int desy_x, int desy_y)
{
  CwmMachineInst->copyArea(src->getXWin(), dest->getXWin(), gc_,
                           src_x, src_y, src_width, src_height, desy_x, desy_y);
}

void
CwmGraphics::
copyArea(CwmXPixmap *src, CwmWindow *dest, int src_x, int src_y,
         int src_width, int src_height, int desy_x, int desy_y)
{
  if (src->getDepth() == screen_.getDepth())
    CwmMachineInst->copyArea(src->getXPixmap(), dest->getXWin(), gc_,
                             src_x, src_y, src_width, src_height, desy_x, desy_y);
  else
    CwmMachineInst->copyPlanes(src->getXPixmap(), src->getDepth(),
                               dest->getXWin(), screen_.getDepth(),
                               gc_, src_x, src_y, src_width, src_height, desy_x, desy_y);
}

void
CwmGraphics::
copyArea(CwmWindow *src, CwmXPixmap *dest, int src_x, int src_y,
         int src_width, int src_height, int desy_x, int desy_y)
{
  if (dest->getDepth() == screen_.getDepth())
    CwmMachineInst->copyArea(src->getXWin(), dest->getXPixmap(), gc_,
                             src_x, src_y, src_width, src_height, desy_x, desy_y);
  else
    CwmMachineInst->copyPlanes(src->getXWin(), screen_.getDepth(),
                               dest->getXPixmap(), dest->getDepth(),
                               gc_, src_x, src_y, src_width, src_height, desy_x, desy_y);
}

void
CwmGraphics::
copyArea(CwmXPixmap *src, CwmXPixmap *dest, int src_x, int src_y,
         int src_width, int src_height, int desy_x, int desy_y)
{
  if (dest->getDepth() == src->getDepth())
    CwmMachineInst->copyArea(src->getXPixmap(), dest->getXPixmap(), gc_,
                             src_x, src_y, src_width, src_height, desy_x, desy_y);
  else
    CwmMachineInst->copyPlanes(src->getXPixmap(), src->getDepth(),
                               dest->getXPixmap(), dest->getDepth(),
                               gc_, src_x, src_y, src_width, src_height, desy_x, desy_y);
}

void
CwmGraphics::
setStippled()
{
  if (stipple_bitmap_ == NULL)
    stipple_bitmap_ = CwmMachineInst->createStippleCwmXPixmap(screen_);

  CwmMachineInst->setFillStipple(gc_, stipple_bitmap_->getXPixmap());
}

void
CwmGraphics::
gradientFillRectangle(CwmWindow *xwindow, int x, int y, int width, int height,
                      const CRGBA &start, const CRGBA &end, CwmGradientDir direction)
{
  CRGBA rgba(start);

  if      (direction == CWM_GRADIENT_DIR_VERTICAL) {
    CRGBA drgba = (end - start)/(height - 1.0);

    for (int i = 0; i < height; i++) {
      setForeground(rgba);

      drawHLine(xwindow, 0, width - 1, i);

      rgba += drgba;
    }
  }
  else if (direction == CWM_GRADIENT_DIR_HORIZONTAL) {
    CRGBA drgba = (end - start)/(width - 1.0);

    for (int i = 0; i < width; i++) {
      setForeground(rgba);

      drawVLine(xwindow, i, 0, height - 1);

      rgba += drgba;
    }
  }
  else if (direction == CWM_GRADIENT_DIR_LDIAGONAL) {
    CRGBA drgba1 = (end - start)/(2*( width - 1.0));
    CRGBA drgba2 = (end - start)/(2*(height - 1.0));

    for (int yy = 0; yy < height; yy++) {
      CRGBA rgba1 = rgba;

      for (int xx = 0; xx < width; xx++) {
        setForeground(rgba);

        drawPoint(xwindow, x + xx, y + yy);

        rgba += drgba1;
      }

      rgba = rgba1 + drgba2;
    }
  }
  else if (direction == CWM_GRADIENT_DIR_RDIAGONAL) {
    CRGBA drgba1 = (end - start)/(2*( width - 1.0));
    CRGBA drgba2 = (end - start)/(2*(height - 1.0));

    for (int yy = 0; yy < height; yy++) {
      CRGBA rgba1 = rgba;

      for (int xx = width - 1; xx >= 0; xx--) {
        setForeground(rgba);

        drawPoint(xwindow, x + xx, y + yy);

        rgba += drgba1;
      }

      rgba = rgba1 + drgba2;
    }
  }
}
