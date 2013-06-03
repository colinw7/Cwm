#ifndef CIMAGE_DRAW_H
#define CIMAGE_DRAW_H

#include <CImageLib.h>

class CImageGC;

class CImageDraw {
 private:
  CImagePtr image_;
  bool      compose_;

 public:
  CImageDraw(CImagePtr image);

  CImagePtr getImage() const { return image_; }

  void setImage(CImagePtr image);

  void clear(const CImageGC &gc);
  void fill (const CImageGC &gc);

  void drawButton(const CImageGC &gc, int x, int y, int width, int height, int thickness);

  void drawImage(const CImageGC &gc, int x, int y, CImagePtr image);

  void drawPolygon(const CImageGC &gc, int *x, int *y, int num_xy);
  void fillPolygon(const CImageGC &gc, int *x, int *y, int num_xy);

  void drawRectangle(const CImageGC &gc, int x, int y, int width, int height);
  void fillRectangle(const CImageGC &gc, int x, int y, int width, int height);

  void drawHLine(const CImageGC &gc, int x, int y, int length);
  void drawVLine(const CImageGC &gc, int x, int y, int length);
  void drawDLine(const CImageGC &gc, int x1, int y1, int x2, int y2);
  void drawLine (const CImageGC &gc, int x1, int y1, int x2, int y2);

  void drawPoint(const CImageGC &gc, int x, int y);

 private:
  void drawColorIndexHLine(int x, int y, int length, int color_ind);
  void drawRGBAHLine(int x, int y, int length, const CRGBA &rgba);
  void drawColorIndexVLine(int x, int y, int length, int color_ind);
  void drawRGBAVLine(int x, int y, int length, const CRGBA &rgba);
};

#endif
