#include "CwmI.h"

CwmCustomIconMgr *
CwmCustomIconMgr::
getInstance()
{
  static CwmCustomIconMgr *instance;

  if (instance == NULL)
    instance = new CwmCustomIconMgr();

  return instance;
}

CwmCustomIconMgr::
CwmCustomIconMgr()
{
}

CwmCustomIconMgr::
~CwmCustomIconMgr()
{
  std::for_each(custom_icons_.begin(), custom_icons_.end(), CDeletePointer());
}

void
CwmCustomIconMgr::
addCustomIcon(const string &icon, const string &label, const string &command,
              const string &bg, const string &fg, int x, int y)
{
  CwmCustomIcon *custom_icon =
    new CwmCustomIcon(icon, label, command, bg, fg, x, y);

  custom_icons_.push_back(custom_icon);
}

void
CwmCustomIconMgr::
addCustomIcons(CwmScreen &screen)
{
  CustomIconList::const_iterator picon1 = custom_icons_.begin();
  CustomIconList::const_iterator picon2 = custom_icons_.end  ();

  for ( ; picon1 != picon2; ++picon1)
    CwmCmdIconMgrInst->addCmdIcon(screen, **picon1);
}

CwmCustomIcon::
CwmCustomIcon(const string &icon, const string &label, const string &command,
              const string &bg, const string &fg, int x, int y) :
 icon_(icon), label_(label), command_(command), bg_(bg), fg_(fg), x_(x), y_(y)
{
}

CwmCustomIcon::
~CwmCustomIcon()
{
}

string
CwmCustomIcon::
getIcon() const
{
  return icon_;
}

string
CwmCustomIcon::
getLabel() const
{
  return label_;
}

string
CwmCustomIcon::
getCommand() const
{
  return command_;
}

string
CwmCustomIcon::
getBg() const
{
  if (bg_.size() > 0)
    return bg_;
  else
    return CwmResourceDataInst->getIconBackground();
}

string
CwmCustomIcon::
getFg() const
{
  if (fg_.size() > 0)
    return fg_;
  else
    return CwmResourceDataInst->getIconForeground();
}

int
CwmCustomIcon::
getX() const
{
  return x_;
}

int
CwmCustomIcon::
getY() const
{
  return y_;
}

CwmImage *
CwmCustomIcon::
getImage(CwmScreen &screen) const
{
  CwmImage *image = CwmImageMgrInst->getImage(screen, getIcon());

  if (image != NULL && (image->getWidth() > 64 || image->getHeight() > 64))
    image = CwmImageMgrInst->getImage(screen, getIcon(), 64, 64);

  if (image == NULL)
    image = CwmImageMgrInst->getImage(screen, "", 64, 64);

  return image;
}

Pixel
CwmCustomIcon::
getBgPixel(CwmScreen &screen) const
{
  string bg_color = getBg();

  return screen.getPixel(bg_color, screen.getWhitePixel());
}

Pixel
CwmCustomIcon::
getFgPixel(CwmScreen &screen) const
{
  string fg_color = getFg();

  return screen.getPixel(fg_color, screen.getBlackPixel());
}

CwmColor *
CwmCustomIcon::
getColor(CwmScreen &screen) const
{
  Pixel fg = getFgPixel(screen);
  Pixel bg = getBgPixel(screen);

  return screen.getColor(fg, bg);
}

CwmFont *
CwmCustomIcon::
getFont(CwmScreen &screen) const
{
  return CwmResourceDataInst->getIconFont(screen);
}

CwmGraphics *
CwmCustomIcon::
getGraphics(CwmScreen &screen) const
{
  CwmColor *color = getColor(screen);
  CwmFont  *font  = getFont (screen);

  CwmGraphics *graphics = new CwmGraphics(screen, font, color);

  return graphics;
}
