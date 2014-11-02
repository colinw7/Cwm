#include <CwmI.h>

CwmMoveFeedback::
CwmMoveFeedback(CwmScreen &screen1) :
 screen(screen1)
{
  Pixel fg = CwmResourceDataInst->getFeedbackForeground(screen);
  Pixel bg = CwmResourceDataInst->getFeedbackBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getFeedbackFont(screen);

  graphics = new CwmGraphics(screen, font, color);

  //------

  feedback_x = 0;
  feedback_y = 0;

  initGeometry();

  //------

  int width, height;

  std::string max_str = "-8888,-8888";

  graphics->getTextSize(max_str, &width, &height);

  width  += 8;
  height += 8;

  int x = feedback_x - width /2;
  int y = feedback_y - height/2;

  x = std::min(std::max(x, 0), screen.getWidth () - width );
  y = std::min(std::max(y, 0), screen.getHeight() - height);

  xwindow = new CwmWindow(screen, screen.getRoot(), x, y, width, height, 0, CWM_CURSOR_MOVE);

  xwindow->setBackground(graphics);

  xwindow->setSaveUnder(true);

  //------

  displayed = FALSE;
}

CwmMoveFeedback::
~CwmMoveFeedback()
{
  delete graphics;

  delete xwindow;
}

void
CwmMoveFeedback::
initGeometry()
{
  CHAlignType halign;
  CVAlignType valign;

  CwmResourceDataInst->getFeedbackGeometry(&halign, &valign, &feedback_x, &feedback_y);

  if      (halign == CHALIGN_TYPE_LEFT)
    ;
  else if (halign == CHALIGN_TYPE_RIGHT)
    feedback_x += screen.getWidth() - 1;
  else if (halign == CHALIGN_TYPE_CENTER)
    feedback_x += screen.getWidth()/2;

  if      (valign == CVALIGN_TYPE_TOP)
    feedback_y = 0;
  else if (valign == CVALIGN_TYPE_BOTTOM)
    feedback_y += screen.getHeight() - 1;
  else if (valign == CVALIGN_TYPE_CENTER)
    feedback_y += screen.getHeight()/2;
}

void
CwmMoveFeedback::
show()
{
  if (displayed)
    return;

  displayed = true;

  xwindow->mapRaised();
}

void
CwmMoveFeedback::
hide()
{
  if (! displayed)
    return;

  displayed = false;

  xwindow->unmap();
}

void
CwmMoveFeedback::
update(int x, int y)
{
  if (! displayed)
    return;

  xwindow->clear();

  graphics->drawButtonOut(xwindow, 0, 0, xwindow->getWidth (), xwindow->getHeight(), 2);

  std::string position_string = CStrUtil::toString(x) + "," + CStrUtil::toString(y);

  graphics->drawTextCentered(xwindow, 4, 4, xwindow->getWidth () - 8,
                             xwindow->getHeight() - 8, position_string);
}
