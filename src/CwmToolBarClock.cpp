#include <CwmToolBarI.h>
#include <CXtTimer.h>
#include <COSTime.h>

class CwmToolBarClockTimer : public CXtTimer {
 private:
  CwmToolBarClock *clock_;

 public:
  CwmToolBarClockTimer(CwmToolBarClock *clock);

  void timeOut();
};

CwmToolBarClock::
CwmToolBarClock(CwmToolBar &toolbar1) :
 toolbar_(toolbar1)
{
  CwmScreen &screen = toolbar_.getScreen();

  //------

  CwmColor *color = CwmResourceDataInst->getToolBarClockColor(screen);
  CwmFont  *font  = CwmResourceDataInst->getToolBarClockFont (screen);

  graphics_ = new CwmGraphics(screen, font, color);

  //------

  format_ = toolbar_.getTimeFormat();

  //------

  timer_ = 0;

  //------

  std::string time_string;

  if (format_ != "")
    time_string = COSTime::getTimeString(format_);
  else
    time_string = COSTime::getTimeString("%H:%M:%S");

  auto len = time_string.size();

  for (uint i = 0; i < len; i++)
    time_string[i] = 'X';

  int width1, height1;

  graphics_->getTextSize(time_string, &width1, &height1);

  int width = width1 + 4;

  int height = std::min(graphics_->getFontHeight() + 4, toolbar_.getHeight() - 6);

  //------

  xwindow_ =
    new CwmWindow(screen, toolbar_.getXWindow(), toolbar_.getWidth() - width - 4,
                  (toolbar_.getHeight() - height)/2, width, height, 0, CWM_CURSOR_TITLE);

  xwindow_->setBackground(graphics_);

  xwindow_->map();

  //------

  xwindow_->addCallback(CWM_CALLBACK_IDLE, &CwmToolBarClock::idleProc, static_cast<CwmData>(this));

  enable();
}

CwmToolBarClock::
~CwmToolBarClock()
{
  delete xwindow_;
}

void
CwmToolBarClock::
redraw()
{
  xwindow_->clear();

  std::string time_string;

  if (format_ != "")
    time_string = COSTime::getTimeString(format_);
  else
    time_string = COSTime::getTimeString("%H:%M:%S");

  graphics_->drawTextCentered(xwindow_, 2, 2, xwindow_->getWidth() - 4,
                              xwindow_->getHeight() - 4, time_string);

  graphics_->drawButtonIn(xwindow_, 0, 0, xwindow_->getWidth(), xwindow_->getHeight(), 1);
}

void
CwmToolBarClock::
enable()
{
  redraw();

  delete timer_;

  timer_ = new CwmToolBarClockTimer(this);
}

void
CwmToolBarClock::
disable()
{
  delete timer_;

  timer_ = 0;
}

int
CwmToolBarClock::
getWidth() const
{
  return xwindow_->getWidth() + 2;
}

int
CwmToolBarClock::
getHeight() const
{
  return xwindow_->getHeight();
}

void
CwmToolBarClock::
idleProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmToolBarClock *clock      = static_cast<CwmToolBarClock *>(data);
  CwmIdleState    *idle_state = static_cast<CwmIdleState    *>(detail);

  CwmScreen &screen = clock->toolbar_.getScreen();

  if (idle_state->getActive()) {
    std::string time_string = COSTime::getTimeString("%c");

    screen.showInfo(time_string);
  }
  else
    screen.hideInfo();
}

CwmToolBarClockTimer::
CwmToolBarClockTimer(CwmToolBarClock *clock) :
 CXtTimer(1000, CTIMER_FLAGS_REPEAT), clock_(clock)
{
}

void
CwmToolBarClockTimer::
timeOut()
{
  clock_->redraw();
}
