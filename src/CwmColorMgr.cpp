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

  auto pcolor = colors_.find(color_pair);

  if (pcolor != colors_.end())
    return (*pcolor).second;

  CwmColor *color = new CwmColor(fg_color, bg_color);

  colors_[color_pair] = color;

  return color;
}

Pixel
CwmColorMgr::
getPixel(const std::string &name, Pixel default_pixel)
{
  std::string name1 = CStrUtil::toLower(name);

  auto pnamed_color = named_colors_.find(name1);

  if (pnamed_color != named_colors_.end())
    return (*pnamed_color).second->getPixel();

  const CXColor *color;
  double         r, g, b;

  if (CRGBName::lookup(name, &r, &g, &b))
    color = &color_mgr_->getCXColor(CRGBA(r, g, b));
  else
    color = &color_mgr_->getCXColor(default_pixel);

  named_colors_[name1] = color;

  return color->getPixel();
}
