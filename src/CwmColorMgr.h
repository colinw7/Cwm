#ifndef CWM_COLOR_MGR_H
#define CWM_COLOR_MGR_H

#include "CPair.h"
#include "CXColor.h"

class CwmColorMgr {
 private:
  typedef CPair<const CXColor &, const CXColor &> ColorPair;

  typedef std::map<ColorPair, CwmColor *>        ColorList;
  typedef std::map<std::string, const CXColor *> NamedColorMap;

  ColorList      colors_;
  NamedColorMap  named_colors_;
  CXColorMgr    *color_mgr_;

 public:
  CwmColorMgr(CwmScreen &screen);
 ~CwmColorMgr();

  CwmColor *getColor(Pixel fg, Pixel bg);
  Pixel     getPixel(const std::string &name, Pixel default_pixel);
  Pixel     getPixel(const CRGBA &rgba);

  Pixel getDarkPixel(Pixel pixel);
  Pixel getLightPixel(Pixel pixel);
  Pixel getInvPixel(Pixel pixel);
};

class CwmColor {
 private:
  const CXColor &fg_;
  const CXColor &bg_;

 public:
  CwmColor(const CXColor &fg, const CXColor &bg) :
   fg_(fg), bg_(bg) {
  }

  Pixel getFg   () const { return fg_.getPixel       (); }
  Pixel getBg   () const { return bg_.getPixel       (); }
  Pixel getLight() const { return bg_.getLightPixel  (); }
  Pixel getDark () const { return bg_.getDarkPixel   (); }
  Pixel getInv  () const { return bg_.getInversePixel(); }

  CRGBA getFgRGBA   () const { return fg_.getRGBA(); }
  CRGBA getBgRGBA   () const { return bg_.getRGBA(); }

  CRGBA getLightRGBA() const { return bg_.getRGBA().getLightRGBA  (); }
  CRGBA getDarkRGBA () const { return bg_.getRGBA().getDarkRGBA   (); }
  CRGBA getInvRGBA  () const { return bg_.getRGBA().getInverseRGBA(); }
};

#endif
