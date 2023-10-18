#include <CwmI.h>

CwmCustomIconMgr *
CwmCustomIconMgr::
getInstance()
{
  static CwmCustomIconMgr *instance;

  if (! instance)
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
  for (auto &custom_icon : custom_icons_)
    delete custom_icon;
}

void
CwmCustomIconMgr::
addCustomIcon(const std::string &icon, const std::string &label, const std::string &command,
              const std::string &bg, const std::string &fg, int x, int y)
{
  CwmCustomIcon *custom_icon = new CwmCustomIcon(icon, label, command, bg, fg, x, y);

  custom_icons_.push_back(custom_icon);
}

void
CwmCustomIconMgr::
addCustomIcons(CwmScreen &screen)
{
  for (auto &icon : custom_icons_)
    CwmCmdIconMgrInst->addCmdIcon(screen, *icon);
}

CwmCustomIcon::
CwmCustomIcon(const std::string &icon, const std::string &label, const std::string &command,
              const std::string &bg, const std::string &fg, int x, int y) :
 icon_(icon), label_(label), command_(command), bg_(bg), fg_(fg), x_(x), y_(y)
{
}

CwmCustomIcon::
~CwmCustomIcon()
{
}

std::string
CwmCustomIcon::
getIcon() const
{
  return icon_;
}

std::string
CwmCustomIcon::
getLabel() const
{
  return label_;
}

std::string
CwmCustomIcon::
getCommand() const
{
  return command_;
}

std::string
CwmCustomIcon::
getBg() const
{
  if (bg_.size() > 0)
    return bg_;
  else
    return CwmResourceDataInst->getIconBackground();
}

std::string
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

  if (image && (image->getWidth() > 64 || image->getHeight() > 64))
    image = CwmImageMgrInst->getImage(screen, getIcon(), 64, 64);

  if (! image)
    image = CwmImageMgrInst->getImage(screen, "", 64, 64);

  return image;
}

Pixel
CwmCustomIcon::
getBgPixel(CwmScreen &screen) const
{
  std::string bg_color = getBg();

  return screen.getPixel(bg_color, screen.getWhitePixel());
}

Pixel
CwmCustomIcon::
getFgPixel(CwmScreen &screen) const
{
  std::string fg_color = getFg();

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
