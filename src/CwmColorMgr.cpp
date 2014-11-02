#include <CwmI.h>
#include <CXColor.h>
#include <CXScreen.h>
#include <CRGBName.h>

CwmColorMgr::
CwmColorMgr(CwmScreen &screen)
{
  color_mgr_ = new CXColorMgr(*screen.getCXScreen());
}

CwmColorMgr::
~CwmColorMgr()
{
  delete color_mgr_;
}

CwmColor *
CwmColorMgr::
getColor(Pixel fg, Pixel bg)
{
  const CXColor &fg_color = color_mgr_->getCXColor(fg);
  const CXColor &bg_color = color_mgr_->getCXColor(bg);

  ColorPair color_pair(fg_color, bg_color);

  ColorList::const_iterator pcolor1 = colors_.find(color_pair);
  ColorList::const_iterator pcolor2 = colors_.end ();

  if (pcolor1 != pcolor2)
    return (*pcolor1).second;

  CwmColor *color = new CwmColor(fg_color, bg_color);

  colors_[color_pair] = color;

  return color;
}

Pixel
CwmColorMgr::
getPixel(const std::string &name, Pixel default_pixel)
{
  std::string name1 = CStrUtil::toLower(name);

  NamedColorMap::const_iterator pnamed_color1 = named_colors_.find(name1);
  NamedColorMap::const_iterator pnamed_color2 = named_colors_.end ();

  if (pnamed_color1 != pnamed_color2)
    return (*pnamed_color1).second->getPixel();

  const CXColor *color;
  double         r, g, b;

  if (CRGBName::lookup(name, &r, &g, &b))
    color = &color_mgr_->getCXColor(CRGBA(r, g, b));
  else
    color = &color_mgr_->getCXColor(default_pixel);

  named_colors_[name1] = color;

  return color->getPixel();
}
