#include "CwmI.h"

CwmUserWindow::
CwmUserWindow(CwmWMWindow *window, Window xwin, int x, int y,
              int width, int height, int border) :
 window_(window), border_(border)
{
  xwindow_ = new CwmWindow(window_->getScreen(), xwin,
                           x, y, width, height, true);
}

CwmUserWindow::
~CwmUserWindow()
{
  xwindow_->map();

  delete xwindow_;
}

Window
CwmUserWindow::
getXWin() const
{
  return xwindow_->getXWin();
}

void
CwmUserWindow::
map()
{
  xwindow_->map();
}

void
CwmUserWindow::
unmap()
{
  xwindow_->unmap();
}

void
CwmUserWindow::
setXWindow(Window xwin)
{
  xwindow_->setXWin(xwin);
}

void
CwmUserWindow::
fixSize(int *width, int *height)
{
  *width  -= window_->getBaseWidthHint ();
  *height -= window_->getBaseHeightHint();

  incrementRound(width , window_->getWidthIncHint ());
  incrementRound(height, window_->getHeightIncHint());

  *width  += window_->getBaseWidthHint ();
  *height += window_->getBaseHeightHint();
}

void
CwmUserWindow::
moveResize(int x, int y, int width, int height)
{
  x_ = x;
  y_ = y;

  xwindow_->moveResize(0, 0, width, height);

  sendConfigureNotify();
}

void
CwmUserWindow::
resize(int width, int height)
{
  xwindow_->resize(width, height);

  sendConfigureNotify();
}

void
CwmUserWindow::
sizeToUser(int *width, int *height)
{
  *width  = getWidth ();
  *height = getHeight();

  *width  -= window_->getBaseWidthHint();
  *height -= window_->getBaseHeightHint();

  *width  = (*width  + window_->getWidthIncHint ()/2)/
            window_->getWidthIncHint ();
  *height = (*height + window_->getHeightIncHint()/2)/
            window_->getHeightIncHint();
}

void
CwmUserWindow::
sendConfigureNotify()
{
  if (! xwindow_->isValid())
    return;

  CwmFrameWindow *frame = window_->getFrame();

  CwmMachineInst->sendConfigureNotifyEvent(xwindow_->getXWin(),
                                           x_, y_, getWidth(), getHeight(), 0,
                                           frame->getXWindow()->getXWin());
}

int
CwmUserWindow::
getWidth() const
{
  return xwindow_->getWidth();
}

int
CwmUserWindow::
getHeight() const
{
  return xwindow_->getHeight();
}

void
CwmUserWindow::
incrementRound(int *number, int increment)
{
  if (increment <= 1)
    return;

  *number = ((*number + increment/2)/increment)*increment;
}
