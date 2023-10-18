#include <CwmI.h>

CwmTabWindow::
CwmTabWindow(CwmScreen &screen) :
 screen_(screen), xwindow_(nullptr), graphics_(nullptr)
{
  init();
}

CwmTabWindow::
~CwmTabWindow()
{
  delete graphics_;
  delete xwindow_;
}

void
CwmTabWindow::
show(const std::string &str)
{
  int x = screen_.getWidth ()/2;
  int y = screen_.getHeight()/2;

  int width, height;

  graphics_->getTextSize(str, &width, &height);

  width  += 4;
  height += 4;

  x -= width /2;
  y -= height/2;

  if (xwindow_ && str_ == str && x_ == x && y_ == y && width_ == width && height_ == height)
    return;

  hide();

  str_    = str;
  x_      = x;
  y_      = y;
  width_  = width;
  height_ = height;

  xwindow_ = new CwmWindow(screen_, screen_.getRoot(), x_, y_, width_, height_,
                           0, CWM_CURSOR_TITLE);

  xwindow_->setBackground(graphics_);

  xwindow_->mapRaised();

  graphics_->drawTextCentered(xwindow_, 2, 2, width_ - 4, height_ - 4, str_);

  graphics_->drawButtonOut(xwindow_, 0, 0, width_, height_, 1);
}

void
CwmTabWindow::
hide()
{
  delete xwindow_;

  xwindow_ = nullptr;
}

void
CwmTabWindow::
init()
{
  CwmColor *color = CwmResourceDataInst->getInfoColor(screen_);
  CwmFont  *font  = CwmResourceDataInst->getInfoFont (screen_);

  graphics_ = new CwmGraphics(screen_, font, color);
}
