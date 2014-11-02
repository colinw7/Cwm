#include <CwmI.h>

CwmMask::
CwmMask(CwmScreen &screen, CwmGraphics *graphics, int width, int height) :
 screen_(screen), width_(width), height_(height)
{
  xpixmap_ = new CwmXPixmap(screen_, width_, height_, 1);

  graphics_ = new CwmGraphics(*xpixmap_, graphics->getFont(), 0);

  graphics_->setForeground(0);

  graphics_->fillRectangle(xpixmap_, 0, 0, width_, height_);

  graphics_->setForeground(1);
}

CwmMask::
~CwmMask()
{
  delete xpixmap_;

  delete graphics_;
}

void
CwmMask::
drawString(int x, int y, const std::string &str)
{
  CwmFont *font = graphics_->getFont();

  font->drawString(xpixmap_, graphics_, x, y, str);
}

void
CwmMask::
combine(CwmWindow *xwindow, int x, int y, int op)
{
  CwmMachineInst->shapeCombineMask(xwindow->getXWin(), xpixmap_->getXPixmap(), x, y, op);
}
