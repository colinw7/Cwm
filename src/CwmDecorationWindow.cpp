#include <CwmDecorationI.h>

CwmWindowChild::
CwmWindowChild(CwmDecoration *decoration, CwmWMWindow *window,
               uint event_mask, CwmCursorType cursor) :
 decoration_(decoration), window_(window), xwindow_(None),
 selected_(false), info_string_("")
{
  xwindow_ = new CwmWindow(window_->getScreen(),
                           window_->getFrameWindow(),
                           0, 0, 1, 1,
                           event_mask, cursor);

  graphics_ = window_->getGraphics();

  xwindow_->setBackground(graphics_);

  xwindow_->addCallback(CWM_CALLBACK_IDLE,
                        &CwmWindowChild::idleProc,
                        static_cast<CwmData>(this));
}

CwmWindowChild::
~CwmWindowChild()
{
  delete xwindow_;
}

CwmScreen &
CwmWindowChild::
getScreen() const
{
  return window_->getScreen();
}

void
CwmWindowChild::
update()
{
  int x, y, width, height;

  calcSize(&x, &y, &width, &height);

  if (width <= 0 || height <= 0) {
    unmap();

    return;
  }

  xwindow_->moveResize(x, y, width, height);

  map();
}

void
CwmWindowChild::
draw()
{
  xwindow_->setBackground(graphics_);

  CwmEventMgrInst->flushExposeEvents(xwindow_);

  xwindow_->clear();

  detail();
}

void
CwmWindowChild::
drawBorder()
{
  if (selected_)
    drawBorderIn();
  else
    drawBorderOut();
}

void
CwmWindowChild::
drawBorderOut()
{
  graphics_->drawButtonOut(xwindow_, 0, 0, getWidth(), getHeight(), 1);
}

void
CwmWindowChild::
drawBorderIn()
{
  graphics_->drawButtonIn(xwindow_, 0, 0, getWidth(), getHeight(), 1);
}

void
CwmWindowChild::
drawSides(int sides)
{
  if (selected_)
    drawSidesIn(sides);
  else
    drawSidesOut(sides);
}

void
CwmWindowChild::
drawSidesOut(int sides)
{
  graphics_->drawSidesOut(xwindow_, sides, 0, 0, getWidth(), getHeight(), 1);
}

void
CwmWindowChild::
drawSidesIn(int sides)
{
  graphics_->drawSidesIn(xwindow_, sides, 0, 0, getWidth(), getHeight(), 1);
}

void
CwmWindowChild::
drawText(const std::string &text)
{
  CHAlignType align = CwmResourceDataInst->getTitleAlign();

  graphics_->drawTextAligned(xwindow_,
                             1, 1,
                             getWidth() - 2, getHeight() - 2,
                             align, CVALIGN_TYPE_CENTER,
                             text);
}

void
CwmWindowChild::
map()
{
  xwindow_->map();
}

void
CwmWindowChild::
unmap()
{
  xwindow_->unmap();
}

void
CwmWindowChild::
select()
{
  if (selected_)
    return;

  selected_ = true;

  draw();

  CwmMachineInst->flushEvents();
}

void
CwmWindowChild::
deselect()
{
  if (! selected_)
    return;

  selected_ = false;

  draw();

  CwmMachineInst->flushEvents();
}

bool
CwmWindowChild::
getMapped() const
{
  return xwindow_->getMapped();
}

int
CwmWindowChild::
getX() const
{
  return xwindow_->getX();
}

int
CwmWindowChild::
getY() const
{
  return xwindow_->getY();
}

int
CwmWindowChild::
getWidth() const
{
  return xwindow_->getWidth();
}

int
CwmWindowChild::
getHeight() const
{
  return xwindow_->getHeight();
}

void
CwmWindowChild::
idleProc(CwmWindow *, CwmData data, CwmData detail)
{
  CwmWindowChild *window_child = static_cast<CwmWindowChild *>(data);
  CwmIdleState   *idle_state   = static_cast<CwmIdleState   *>(detail);

  CwmScreen &screen = window_child->window_->getScreen();

  if (window_child->info_string_ == "")
    return;

  if (idle_state->getActive())
    screen.showInfo(window_child->info_string_);
  else
    screen.hideInfo();
}
