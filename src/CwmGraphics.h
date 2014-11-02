#ifndef CWM_GRAPHICS_H
#define CWM_GRAPHICS_H

enum CwmGradientDir {
  CWM_GRADIENT_DIR_VERTICAL,
  CWM_GRADIENT_DIR_HORIZONTAL,
  CWM_GRADIENT_DIR_LDIAGONAL,
  CWM_GRADIENT_DIR_RDIAGONAL
};

class CwmGraphics {
 public:
  CwmGraphics(CwmScreen &screen, CwmFont *font, CwmColor *color, bool is_xor=false);
  CwmGraphics(CwmWMWindow &window, CwmFont *font, CwmColor *color, bool is_xor=false);
  CwmGraphics(CwmXPixmap &pixmap, CwmFont *font, CwmColor *color, bool is_xor=false);
 ~CwmGraphics();

  void init();

  void drawText(CwmWindow *xwindow, int x, int y, const std::string &text);

  void drawTextAligned(CwmWindow *xwindow, int x, int y, int width, int height,
                       CHAlignType h_align, CVAlignType v_align, const std::string &text);
  void drawTextLeft(CwmWindow *xwindow, int x, int y, int width, int height,
                    const std::string &text);
  void drawTextRight(CwmWindow *xwindow, int x, int y, int width, int height,
                     const std::string &text);
  void drawTextCentered(CwmWindow *xwindow, int x, int y, int width, int height,
                        const std::string &text);

  void drawButtonOut(CwmWindow *xwindow, int x, int y, int width, int height, int thick);
  void drawButtonIn(CwmWindow *xwindow, int x, int y, int width, int height, int thick);

  void drawSidesOut(CwmWindow *xwindow, int sides, int x, int y, int width, int height, int thick);
  void drawSidesIn(CwmWindow *xwindow, int sides, int x, int y, int width, int height, int thick);

  void drawEtchedLine(CwmWindow *xwindow, int x, int y, int width, int height);

  void drawRightArrowOut(CwmWindow *xwindow, int x, int y, int width, int height);

  void drawLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2);
  void drawHLine(CwmWindow *xwindow, int x1, int x2, int y);
  void drawHLine(CwmWindow *xwindow, int x1, int x2, int y, const CRGBA &rgba);
  void drawVLine(CwmWindow *xwindow, int x, int y1, int y2);
  void drawVLine(CwmWindow *xwindow, int x, int y1, int y2, const CRGBA &rgba);
  void drawDLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2);
  void drawDarkLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2);
  void drawHDarkLine(CwmWindow *xwindow, int x1, int x2, int y);
  void drawVDarkLine(CwmWindow *xwindow, int x, int y1, int y2);
  void drawLightLine(CwmWindow *xwindow, int x1, int y1, int x2, int y2);
  void drawHLightLine(CwmWindow *xwindow, int x1, int x2, int y);
  void drawVLightLine(CwmWindow *xwindow, int x, int y1, int y2);

  void drawRectangle(CwmWindow *xwindow, int x, int y, int width, int height);
  void drawRectangle(CwmXPixmap *xpixmap, int x, int y, int width, int height);

  void fillRectangle(CwmWindow *xwindow, int x, int y, int width, int height);
  void fillRectangle(CwmXPixmap *xpixmap, int x, int y, int width, int height);

  void drawPoint(CwmWindow *xwindow, int x, int y);

  void getTextSize(const std::string &text, int *width, int *height);

  int getFontHeight();
  int getFontAscent();
  int getFontDescent();

  GC        getGC   () const { return gc_   ; }
  CwmFont  *getFont () const { return font_ ; }
  CwmColor *getColor() const { return color_; }

  void setFont(CwmFont *font) { font_ = font; }
  void setColor(CwmColor *color) { color_ = color; }

  void copyArea(CwmWindow *src, CwmWindow *dest, int src_x, int src_y,
                int src_width, int src_height, int desy_x, int desy_y);
  void copyArea(CwmXPixmap *src, CwmWindow *dest, int src_x, int src_y,
                int src_width, int src_height, int desy_x, int desy_y);
  void copyArea(CwmWindow *src, CwmXPixmap *dest, int src_x, int src_y,
                int src_width, int src_height, int desy_x, int desy_y);
  void copyArea(CwmXPixmap *src, CwmXPixmap *dest, int src_x, int src_y,
                int src_width, int src_height, int desy_x, int desy_y);

  void setForeground();
  void setForeground(const CRGBA &rgba);
  void setForeground(int ind);
  void setBackground();
  void setDarkForeground();
  void setLightForeground();

  void setStippled();

  void gradientFillRectangle(CwmWindow *xwindow, int x, int y, int width,
                             int height, const CRGBA &start, const CRGBA &end,
                             CwmGradientDir direction);

  void startClip(CwmXPixmap *xpixmap, int dx=0, int dy=0);
  void endClip();

 private:
  void setTextSize(const std::string &text);
  void drawTextI(CwmWindow *xwindow, int x, int y, int width, int height,
                 CVAlignType v_align, const std::string &text);
  void drawLineI(CwmWindow *xwindow, int x1, int y1, int x2, int y2);

  void startClip(int x, int y, int width, int height);

 private:
  CwmScreen  &screen_;
  GC          gc_;
  CwmFont    *font_;
  CwmColor   *color_;
  int         text_width_;
  int         text_height_;
  CwmXPixmap *stipple_bitmap_;
};

#endif
