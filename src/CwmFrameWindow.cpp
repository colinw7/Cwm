#include "CwmI.h"

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask      |
  SubstructureRedirectMask;

static const int border = 2;

CwmFrameWindow::
CwmFrameWindow(CwmWMWindow *window1) :
 window(window1), xwindow(None), positioned(false)
{
  CwmScreen &screen = window->getScreen();

  xwindow = new CwmWindow(screen, screen.getRoot(), 0, 0, 1, 1,
                          event_mask, CWM_CURSOR_TITLE);

  graphics = window->getGraphics();

  xwindow->setBackground(graphics);
}

CwmFrameWindow::
~CwmFrameWindow()
{
  delete xwindow;
}

void
CwmFrameWindow::
map()
{
  xwindow->map();
}

void
CwmFrameWindow::
unmap()
{
  xwindow->unmap();
}

void
CwmFrameWindow::
position()
{
  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  CwmDecoration *decoration = window->getDecoration();

  if (! window->getRolledUp()) {
    int x      = user->getX() -
                 decoration->getLeftBorder();
    int y      = user->getY() -
                 decoration->getTopBorder() -
                 decoration->getTitleHeight();
    int width  = user->getWidth () +
                 decoration->getLeftBorder() +
                 decoration->getRightBorder();
    int height = user->getHeight() +
                 decoration->getTopBorder   () +
                 decoration->getBottomBorder() +
                 decoration->getTitleHeight ();

    CwmScreen &screen = window->getScreen();

    if (! positioned) {
      if (CwmResourceDataInst->getClientAutoPlace() &&
          user->getX() == -1 && user->getY() == -1)
        screen.getDefaultPos(width, height, &x, &y);

      positioned = true;
    }

    //------

    if (width > screen.getWidth())
      width = screen.getWidth();

    if (height > screen.getHeight())
      height = screen.getHeight();

    if (x + width > screen.getWidth())
      x = screen.getWidth() - width;

    if (y + height > screen.getHeight())
      y = screen.getHeight() - height;

    if (x < 0)
      x = 0;

    if (y < 0)
      y = 0;

    //------

    xwindow->moveResize(x, y, width, height);

    //------

    int user_x = x + decoration->getLeftBorder ();
    int user_y = y + decoration->getTopBorder  () +
                     decoration->getTitleHeight();

    int save_width  = user->getWidth ();
    int save_height = user->getHeight();

    int user_width  = width  -
                      decoration->getLeftBorder () -
                      decoration->getRightBorder();
    int user_height = height -
                      decoration->getTopBorder   () -
                      decoration->getBottomBorder() -
                      decoration->getTitleHeight ();

    user->fixSize(&user_width, &user_height);

    if (width != save_width || height != save_height)
      user->moveResize(user_x, user_y, user_width, user_height);
  }
  else {
    int x      = user->getX();
    int y      = user->getY() -
                 decoration->getTitleHeight();
    int width  = user->getWidth();
    int height = decoration->getTitleHeight();

    xwindow->moveResize(x, y, width, height);
  }
}

void
CwmFrameWindow::
draw()
{
  xwindow->setBackground(graphics);

  CwmEventMgrInst->flushExposeEvents(xwindow);

  xwindow->clear();

  graphics->drawButtonOut(xwindow,
                          0, 0,
                          getWidth(), getHeight(),
                          2);

  graphics->drawButtonIn(xwindow,
                         border - 1, border - 1,
                         getWidth () - 2*border + 2,
                         getHeight() - 2*border + 2,
                         1);
}

void
CwmFrameWindow::
move(int x, int y)
{
  xwindow->move(x, y);

  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  CwmDecoration *decoration = window->getDecoration();

  int user_x      = x + decoration->getLeftBorder ();
  int user_y      = y + decoration->getTopBorder  () +
                        decoration->getTitleHeight();
  int user_width  = user->getWidth();
  int user_height = user->getHeight();

  user->moveResize(user_x, user_y, user_width, user_height);
}

void
CwmFrameWindow::
moveResize(int x, int y, int width, int height)
{
  xwindow->moveResize(x, y, width, height);

  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  CwmDecoration *decoration = window->getDecoration();

  int user_x      = x + decoration->getLeftBorder();
  int user_y      = y + decoration->getTopBorder () +
                        decoration->getTitleHeight();
  int user_width  = width  -
                    decoration->getLeftBorder () -
                    decoration->getRightBorder();
  int user_height = height -
                    decoration->getTopBorder   () -
                    decoration->getBottomBorder() -
                    decoration->getTitleHeight ();

  user->fixSize(&user_width, &user_height);

  user->moveResize(user_x, user_y, user_width, user_height);

  window->redecorate();
}

void
CwmFrameWindow::
minimize()
{
  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  CwmDecoration *decoration = window->getDecoration();

  int user_x      = user->getX();
  int user_y      = user->getY();
  int user_width  = window->getMinWidthHint();
  int user_height = window->getMinHeightHint();

  user->fixSize(&user_width, &user_height);

  int frame_x      = user_x -
                     decoration->getLeftBorder ();
  int frame_y      = user_y -
                     decoration->getTopBorder  () -
                     decoration->getTitleHeight();
  int frame_width  = user_width +
                     decoration->getLeftBorder () +
                     decoration->getRightBorder();
  int frame_height = user_height +
                     decoration->getTopBorder   () +
                     decoration->getBottomBorder() +
                     decoration->getTitleHeight ();

  window->moveResize(frame_x, frame_y, frame_width, frame_height);

  window->redecorate();
/*
  xwindow->moveResize(frame_x, frame_y, frame_width, frame_height);

  user->moveResize(user_x, user_y, user_width, user_height);

  window->redecorate();
*/
}

void
CwmFrameWindow::
maximize()
{
  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  CwmDecoration *decoration = window->getDecoration();

  CwmScreen &screen = window->getScreen();

  int user_x      = decoration->getLeftBorder ();
  int user_y      = decoration->getTopBorder  () +
                    decoration->getTitleHeight();
  int user_width  = screen.getWidth() -
                    decoration->getLeftBorder () -
                    decoration->getRightBorder();
  int user_height = screen.getHeight() -
                    decoration->getTopBorder   () -
                    decoration->getBottomBorder() -
                    decoration->getTitleHeight ();

  if (user_width > window->getMaxWidthHint())
    user_width = window->getMaxWidthHint();

  if (user_height > window->getMaxHeightHint())
    user_height = window->getMaxHeightHint();

  user->fixSize(&user_width, &user_height);

  int frame_x      = user_x -
                     decoration->getLeftBorder ();
  int frame_y      = user_y -
                     decoration->getTopBorder  () -
                     decoration->getTitleHeight();
  int frame_width  = user_width +
                     decoration->getLeftBorder () +
                     decoration->getRightBorder();
  int frame_height = user_height +
                     decoration->getTopBorder   () +
                     decoration->getBottomBorder() +
                     decoration->getTitleHeight ();

  window->moveResize(frame_x, frame_y, frame_width, frame_height);

  window->redecorate();
/*
  xwindow->moveResize(frame_x, frame_y, frame_width, frame_height);

  user->moveResize(user_x, user_y, user_width, user_height);

  window->redecorate();
*/
}

int
CwmFrameWindow::
getX() const
{
  return xwindow->getX();
}

int
CwmFrameWindow::
getY() const
{
  return xwindow->getY();
}

int
CwmFrameWindow::
getWidth() const
{
  return xwindow->getWidth();
}

int
CwmFrameWindow::
getHeight() const
{
  return xwindow->getHeight();
}
