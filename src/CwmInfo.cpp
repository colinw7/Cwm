#include "CwmI.h"
#include "CXtTimer.h"

class CwmInfoTimer : public CXtTimer {
 private:
  CwmInfo *info_;

 public:
  CwmInfoTimer(CwmInfo *info);

  void timeOut();
};

static const int CWM_INFO_TIMEOUT = 2000;

CwmInfo::
CwmInfo(CwmScreen &screen1) :
 screen(screen1), xwindow(None), graphics(NULL)
{
  init();
}

CwmInfo::
~CwmInfo()
{
  delete graphics;

  if (xwindow != None)
    delete xwindow;

  delete timer;
}

void
CwmInfo::
show(const string &str)
{
  hide();

  int x, y;

  screen.getRoot()->getPointerPosition(&x, &y);

  int width, height;

  graphics->getTextSize(str, &width, &height);

  width  += 8;
  height += 4;

  if (x + width > screen.getWidth())
    x = screen.getWidth() - width;

  if (y + height > screen.getHeight())
    y = screen.getHeight() - height;

  xwindow =
    new CwmWindow(screen, screen.getRoot(),
                  x, y - height, width, height,
                  0, CWM_CURSOR_TITLE);

  xwindow->setBackground(graphics);

  xwindow->mapRaised();

  graphics->drawTextCentered(xwindow,
                             2, 2, width - 4, height - 4,
                             str);

  graphics->drawButtonOut(xwindow,
                          0, 0, width, height,
                          1);

  timer = new CwmInfoTimer(this);
}

void
CwmInfo::
hide()
{
  delete timer;

  timer = NULL;

  delete xwindow;

  xwindow = NULL;
}

void
CwmInfo::
init()
{
  CwmColor *color = CwmResourceDataInst->getInfoColor(screen);
  CwmFont  *font  = CwmResourceDataInst->getInfoFont (screen);

  graphics = new CwmGraphics(screen, font, color);

  timer = NULL;
}

void
CwmInfo::
timeOut()
{
  hide();
}

CwmInfoTimer::
CwmInfoTimer(CwmInfo *info) :
 CXtTimer(CWM_INFO_TIMEOUT, CTIMER_FLAGS_NO_DELETE), info_(info)
{
}

void
CwmInfoTimer::
timeOut()
{
  info_->timeOut();
}
