#include <CwmI.h>
#include <CFontMgr.h>
#include <CXFont.h>
#include <CXrtFont.h>

CwmFontMgr::
CwmFontMgr(CwmScreen &screen) :
 screen_(screen)
{
}

CwmFontMgr::
~CwmFontMgr()
{
  std::for_each(fonts_.begin(), fonts_.end(), CDeletePointer());
}

CwmFont *
CwmFontMgr::
getFont(const std::string &name)
{
  FontList::const_iterator pfont1 = fonts_.begin();
  FontList::const_iterator pfont2 = fonts_.end  ();

  for ( ; pfont1 != pfont2; ++pfont1)
    if (CStrUtil::casecmp((*pfont1)->getName(), name) == 0)
      return *pfont1;

  //------

  CwmFont *font = new CwmFont(screen_, name);

  fonts_.push_back(font);

  //------

  return font;
}

CwmFont::
CwmFont(CwmScreen &, const std::string &name)
{
  int         size;
  std::string name1;
  CFontStyle  style;
  int         angle;

  parseName(name, &name1, &style, &size, &angle);

  cxfont_ = CFontMgrInst->lookupFont(name1, style, size, angle);
}

CwmFont::
~CwmFont()
{
}

std::string
CwmFont::
getName() const
{
  return cxfont_->getFamily();
}

int
CwmFont::
getHeight() const
{
  return cxfont_->getICharHeight();
}

int
CwmFont::
getAscent() const
{
  return cxfont_->getICharAscent();
}

int
CwmFont::
getDescent() const
{
  return cxfont_->getICharDescent();
}

void
CwmFont::
getTextSize(const std::string &text, int *width, int *height)
{
  *width  = cxfont_->getIStringWidth(text);
  *height = cxfont_->getICharHeight();
}

void
CwmFont::
drawString(CwmWindow *xwindow, CwmGraphics *graphics, int x, int y, const std::string &text)
{
  CXFont *xfont = cxfont_.cast<CXFont>();

  CXrtFont *xrt_font = xfont->getXrtFont();

  xrt_font->draw(xwindow->getXWin(), graphics->getGC(), x, y, text);
}

void
CwmFont::
drawString(CwmXPixmap *xpixmap, CwmGraphics *graphics, int x, int y, const std::string &text)
{
  CXFont *xfont = cxfont_.cast<CXFont>();

  CXrtFont *xrt_font = xfont->getXrtFont();

  xrt_font->draw(xpixmap->getXPixmap(), graphics->getGC(), x, y, text);
}

void
CwmFont::
parseName(const std::string &name, std::string *name1, CFontStyle *style, int *size, int *angle)
{
  std::vector<std::string> words;

  CStrUtil::addFields(name, words, "-");

  *name1 = "courier";
  *size  = 14;
  *angle = 0;

  if (words.size() > 0)
    *name1 = words[0];

  std::string style1 = "normal";

  if (words.size() > 1)
    style1 = words[1];

  *style = parseStyle(style1);

  if (words.size() > 2)
    if (CStrUtil::isInteger(words[2]))
      *size = CStrUtil::toInteger(words[2]);

  if (words.size() > 3) {
    if (CStrUtil::isInteger(words[3]))
      *angle = CStrUtil::toInteger(words[3]);
  }
}

CFontStyle
CwmFont::
parseStyle(const std::string &style)
{
  return CFont::stringToFontStyle(style);
}
