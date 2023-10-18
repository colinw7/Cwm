#include <CImageDraw.h>
#include <CImageGC.h>
#include <CMathRound.h>

#include <CLine2D.h>
#include <CBresenham.h>

CImageDraw::
CImageDraw(CImagePtr image) :
 image_(image), compose_(false)
{
}

void
CImageDraw::
setImage(CImagePtr image)
{
  image_ = image;
}

void
CImageDraw::
clear(const CImageGC &gc)
{
  if (image_->hasColormap()) {
    int color_ind;

    gc.getBackground(color_ind);

    image_->setColorIndexData(uint(color_ind));
  }
  else {
    CRGBA rgba;

    gc.getBackground(rgba);

    image_->setRGBAData(rgba);
  }
}

void
CImageDraw::
fill(const CImageGC &gc)
{
  if (image_->hasColormap()) {
    int color_ind;

    gc.getForeground(color_ind);

    image_->setColorIndexData(uint(color_ind));
  }
  else {
    CRGBA rgba;

    gc.getForeground(rgba);

    image_->setRGBAData(rgba);
  }
}

void
CImageDraw::
drawImage(const CImageGC &, int x, int y, CImagePtr image)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int iwidth1  = int(image->getWidth ());
  int iheight1 = int(image->getHeight());

  int x1 = x;
  int x2 = std::min(x + iwidth1  - 1, iwidth  - 1);
  int y1 = y;
  int y2 = std::min(y + iheight1 - 1, iheight - 1);

  if (image_->hasColormap()) {
    int color_ind;

    for (int yy = y1; yy <= y2; ++yy)
      for (int xx = x1; xx <= x2; ++xx) {
        color_ind = image->getColorIndexPixel(xx - x1, yy - y1);

        image_->drawColorIndexPoint(xx, yy, color_ind);
      }
  }
  else {
    CRGBA rgba;

    for (int yy = y1; yy <= y2; ++yy)
      for (int xx = x1; xx <= x2; ++xx) {
        image->getRGBAPixel(xx - x1, yy - y1, rgba);

        image_->drawRGBAPoint(xx, yy, rgba);
      }
  }
}

void
CImageDraw::
drawPolygon(const CImageGC &gc, int *x, int *y, int num_xy)
{
  int i1 = num_xy - 1;
  int i2 = 0;

  for ( ; i2 < num_xy; i1 = i2, ++i2)
    drawLine(gc, x[i1], y[i1], x[i2], y[i2]);
}

void
CImageDraw::
fillPolygon(const CImageGC &gc, int *x, int *y, int num_xy)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int ymin = iheight;
  int ymax = -1;

  for (int i1 = 0; i1 < num_xy; ++i1) {
    ymin = std::min(ymin, y[i1]);
    ymax = std::max(ymax, y[i1]);
  }

  ymin = std::max(ymin, 0);
  ymax = std::min(ymax, iheight - 1);

  for (int yy = ymin; yy <= ymax; ++yy) {
    int xmin = iwidth;
    int xmax = -1;

    for (int i1 = num_xy - 1, i2 = 0; i2 < num_xy; i1 = i2, ++i2) {
      if ((y[i1] < yy && y[i2] < yy) ||
          (y[i1] > yy && y[i2] > yy) ||
          y[i1] == y[i2])
        continue;

      double factor = double(x[i2] - x[i1])/double(y[i2] - y[i1]);

      int xx = CMathRound::Round((yy - y[i1])*factor + x[i1]);

      xmin = std::min(xmin, xx);
      xmax = std::max(xmax, xx);
    }

    xmin = std::max(xmin, 0);
    xmax = std::min(xmax, iwidth - 1);

    if (xmin > xmax)
      continue;

    drawLine(gc, xmin, yy, xmax, yy);
  }
}

void
CImageDraw::
drawRectangle(const CImageGC &gc, int x, int y, int width, int height)
{
  int x1 = x + width  - 1;
  int y1 = y + height - 1;

  drawLine(gc, x , y , x1, y );
  drawLine(gc, x1, y , x1, y1);
  drawLine(gc, x1, y1, x , y1);
  drawLine(gc, x , y1, x , y );
}

void
CImageDraw::
fillRectangle(const CImageGC &gc, int x, int y, int width, int height)
{
  if (compose_) {
    int iwidth  = int(image_->getWidth ());
    int iheight = int(image_->getHeight());

    int x1 = x;
    int x2 = std::min(x + width  - 1, iwidth  - 1);
    int y1 = y;
    int y2 = std::min(y + height - 1, iheight - 1);

    if (image_->hasColormap()) {
      int color_ind;

      gc.getForeground(color_ind);

      for (int yy = y1; yy <= y2; ++yy)
        for (int xx = x1; xx <= x2; ++xx)
          image_->drawColorIndexPoint(xx, yy, color_ind);
    }
    else {
      CRGBA rgba;

      gc.getForeground(rgba);

      for (int yy = y1; yy <= y2; ++yy)
        for (int xx = x1; xx <= x2; ++xx)
          image_->drawRGBAPoint(xx, yy, rgba);
    }
  }
  else {
    int x2 = x + width  - 1;
    int y2 = y + height - 1;

    if (image_->hasColormap()) {
      int color_ind;

      gc.getForeground(color_ind);

      image_->fillColorIndexRectangle(x, y, x2, y2, color_ind);
    }
    else {
      CRGBA rgba;

      gc.getForeground(rgba);

      image_->fillRGBARectangle(x, y, x2, y2, rgba);
    }
  }
}

void
CImageDraw::
drawButton(const CImageGC &gc, int x, int y, int width, int height,
           int thickness)
{
  if (image_->hasColormap())
    return;

  CRGBA rgba;

  gc.getForeground(rgba);

  CRGBA rgba1 = rgba;
  CRGBA rgba2 = rgba;

  rgba1.scaleAlpha(0.4);
  rgba2.scaleAlpha(0.8);

  // top

  {
    int x1 = x;
    int x2 = x + width - 2;
    int y1 = y;

    for (int i = 0; i < thickness; ++i, x1++, x2--)
      drawRGBAHLine(x1, y1 + i, x2 - x1 + 1, rgba1);
  }

  // left

  {
    int x1 = x;
    int y1 = y + 1;
    int y2 = y + height - 2;

    for (int i = 0; i < thickness; ++i, y1++, y2--)
      drawRGBAVLine(x1 + i, y1, y2 - y1 + 1, rgba1);
  }

  // bottom

  {
    int x1 = x;
    int y1 = y + height - 1;
    int x2 = x + width - 1;

    for (int i = 0; i < thickness; ++i, x1++, x2--)
      drawRGBAHLine(x1, y1 - i, x2 - x1 + 1, rgba2);
  }

  // right

  {
    int x1 = x + width - 1;
    int y1 = y;
    int y2 = y + height - 2;

    for (int i = 0; i < thickness; ++i, y1++, y2--)
      drawRGBAVLine(x1 - i, y1, y2 - y1 + 1, rgba2);
  }

  fillRectangle(gc, x + thickness, y + thickness,
                width - 2*thickness, height - 2*thickness);
}

void
CImageDraw::
drawHLine(const CImageGC &gc, int x, int y, int length)
{
  if (image_->hasColormap()) {
    int color_ind;

    gc.getForeground(color_ind);

    drawColorIndexHLine(x, y, length, color_ind);
  }
  else {
    CRGBA rgba;

    gc.getForeground(rgba);

    drawRGBAHLine(x, y, length, rgba);
  }
}

void
CImageDraw::
drawColorIndexHLine(int x, int y, int length, int color_ind)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int x1 = x;
  int x2 = std::min(x + length - 1, iwidth - 1);

  if (y < 0 || y >= iheight)
    return;

  for (int xx = x1; xx <= x2; ++xx)
    image_->drawColorIndexPoint(xx, y, color_ind);
}

void
CImageDraw::
drawRGBAHLine(int x, int y, int length, const CRGBA &rgba)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int x1 = x;
  int x2 = std::min(x + length - 1, iwidth - 1);

  if (y < 0 || y >= iheight)
    return;

  for (int xx = x1; xx <= x2; ++xx)
    image_->drawRGBAPoint(xx, y, rgba);
}

void
CImageDraw::
drawVLine(const CImageGC &gc, int x, int y, int length)
{
  if (image_->hasColormap()) {
    int color_ind;

    gc.getForeground(color_ind);

    drawColorIndexVLine(x, y, length, color_ind);
  }
  else {
    CRGBA rgba;

    gc.getForeground(rgba);

    drawRGBAVLine(x, y, length, rgba);
  }
}

void
CImageDraw::
drawColorIndexVLine(int x, int y, int length, int color_ind)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int y1 = y;
  int y2 = std::min(y + length - 1, iheight - 1);

  if (x < 0 || x >= iwidth)
    return;

  for (int yy = y1; yy <= y2; ++yy)
    image_->drawColorIndexPoint(x, yy, color_ind);
}

void
CImageDraw::
drawRGBAVLine(int x, int y, int length, const CRGBA &rgba)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  int y1 = y;
  int y2 = std::min(y + length - 1, iheight - 1);

  if (x < 0 || x >= iwidth)
    return;

  for (int yy = y1; yy <= y2; ++yy)
    image_->drawRGBAPoint(x, yy, rgba);
}

void
CImageDraw::
drawDLine(const CImageGC &gc, int x1, int y1, int x2, int y2)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  if (x1 < 0 || x1 >= iwidth  || x2 < 0 || x2 >= iwidth  ||
      y1 < 0 || y1 >= iheight || y2 < 0 || y2 >= iheight)
    return;

  int dx = (x2 > x1 ? 1 : -1);
  int dy = (y2 > y1 ? 1 : -1);

  int x = x1;
  int y = y1;

  if (image_->hasColormap()) {
    int color_ind;

    gc.getForeground(color_ind);

    while (true) {
      if      (dx ==  1 && x > x2) break;
      else if (dx == -1 && x < x2) break;
      if      (dy ==  1 && y > y2) break;
      else if (dy == -1 && y < y2) break;

      image_->drawColorIndexPoint(x, y, color_ind);

      x += dx;
      y += dy;
    }
  }
  else {
    CRGBA rgba;

    gc.getForeground(rgba);

    while (true) {
      if      (dx ==  1 && x > x2) break;
      else if (dx == -1 && x < x2) break;
      if      (dy ==  1 && y > y2) break;
      else if (dy == -1 && y < y2) break;

      image_->drawRGBAPoint(x, y, rgba);

      x += dx;
      y += dy;
    }
  }
}

void
CImageDraw::
drawLine(const CImageGC &gc, int x1, int y1, int x2, int y2)
{
  class CImageDrawBresenham : public CBresenham {
   public:
    CImageDrawBresenham(CImageDraw *draw, const CImageGC *gc) :
     draw_(draw), gc_(gc) {
    }

    void drawPoint(int x, int y) override {
      draw_->drawPoint(*gc_, x, y);
    }

   private:
    CImageDraw     *draw_;
    const CImageGC *gc_;
  };

  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  CLine2D line(x1, y1, x2, y2);
  CBBox2D bbox(0, 0, iwidth - 1, iheight - 1);

  CLine2D cline;

  if (! line.clip(bbox, cline))
    return;

  CImageDrawBresenham bresenham(this, &gc);

  bresenham.drawLine(int(cline.start().x), int(cline.start().y),
                     int(cline.end  ().x), int(cline.end  ().y));
}

void
CImageDraw::
drawPoint(const CImageGC &gc, int x, int y)
{
  int iwidth  = int(image_->getWidth ());
  int iheight = int(image_->getHeight());

  if (x < 0 || x >= iwidth  ||
      y < 0 || y >= iheight)
    return;

  if (image_->hasColormap()) {
    int color_ind;

    gc.getForeground(color_ind);

    image_->drawColorIndexPoint(x, y, color_ind);
  }
  else {
    CRGBA rgba;

    gc.getForeground(rgba);

    image_->drawRGBAPoint(x, y, rgba);
  }
}
