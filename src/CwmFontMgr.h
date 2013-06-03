#include "CFont.h"

class CwmFontMgr {
 private:
  typedef vector<CwmFont *> FontList;

  CwmScreen &screen_;
  FontList   fonts_;

 public:
  CwmFontMgr(CwmScreen &screen);
 ~CwmFontMgr();

  CwmFont *getFont(const std::string &name);
};

struct CwmFont {
 private:
  CFontPtr cxfont_;

 public:
  CwmFont(CwmScreen &screen, const std::string &name);
 ~CwmFont();

  std::string getName() const;
  int         getHeight() const;
  int         getAscent() const;
  int         getDescent() const;

  void getTextSize(const std::string &text, int *width, int *height);

  void drawString(CwmWindow *xwindow, CwmGraphics *graphics,
                  int x, int y, const std::string &text);
  void drawString(CwmXPixmap *xpixmap, CwmGraphics *graphics,
                  int x, int y, const std::string &text);

 private:
  void parseName(const std::string &name, std::string *name1, CFontStyle *style,
                 int *size, int *angle);

  CFontStyle parseStyle(const std::string &style);
};
