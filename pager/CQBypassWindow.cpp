#include <CQBypassWindow.h>
#include <QMouseEvent>
#include <QPainter>

CQBypassWindow::
CQBypassWindow() :
 QWidget()
{
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

  QFontMetrics fm(font());

  int th = fm.height();

  setContentsMargins(2, th + 4, 2, 2);

  setMouseTracking(true);
}

void
CQBypassWindow::
mousePressEvent(QMouseEvent *me)
{
  pressPos_ = me->globalPos();

  updateSize(me->pos());

  pressed_ = true;

  if (closeRect_.contains(me->pos()))
    exit(0);
}

void
CQBypassWindow::
mouseMoveEvent(QMouseEvent *me)
{
  if (! pressed_) {
    updateSize(me->pos());
  }
  else {
    QPoint pos = me->globalPos();

    int dx = pos.x() - pressPos_.x();
    int dy = pos.y() - pressPos_.y();

    if      ((side_ & uint(Side::LEFT)) && (side_ & uint(Side::TOP)))
      setGeometry(x() + dx, y() + dy, width() - dx, height() - dy);
    else if ((side_ & uint(Side::LEFT)) && (side_ & uint(Side::BOTTOM)))
      setGeometry(x() + dx, y(), width() - dx, height() + dy);
    else if ((side_ & uint(Side::RIGHT)) && (side_ & uint(Side::TOP)))
      setGeometry(x(), y() + dy, width() + dx, height() - dy);
    else if ((side_ & uint(Side::RIGHT)) && (side_ & uint(Side::BOTTOM)))
      setGeometry(x(), y(), width() + dx, height() + dy);
    else if (side_ & uint(Side::LEFT))
      setGeometry(x() + dx, y(), width() - dx, height());
    else if (side_ & uint(Side::RIGHT))
      setGeometry(x(), y(), width() + dx, height());
    else if (side_ & uint(Side::TOP))
      setGeometry(x(), y() + dy, width(), height() - dy);
    else if (side_ & uint(Side::BOTTOM))
      setGeometry(x(), y(), width(), height() + dy);
    else
      move(x() + dx, y() + dy);

    pressPos_ = pos;
  }
}

void
CQBypassWindow::
mouseReleaseEvent(QMouseEvent *)
{
  setCursor(Qt::ArrowCursor);

  pressed_ = false;
}

void
CQBypassWindow::
updateSize(const QPoint &pos)
{
  int x = pos.x();
  int y = pos.y();

  side_ = 0;

  int b = 3;

  if (x <             b) side_ |= uint(Side::LEFT);
  if (y <             b) side_ |= uint(Side::TOP);
  if (x >= width () - b) side_ |= uint(Side::RIGHT);
  if (y >= height() - b) side_ |= uint(Side::BOTTOM);

  if      ((side_ & uint(Side::LEFT)) && (side_ & uint(Side::TOP)))
    setCursor(Qt::SizeFDiagCursor);
  else if ((side_ & uint(Side::LEFT)) && (side_ & uint(Side::BOTTOM)))
    setCursor(Qt::SizeBDiagCursor);
  else if ((side_ & uint(Side::RIGHT)) && (side_ & uint(Side::TOP)))
    setCursor(Qt::SizeBDiagCursor);
  else if ((side_ & uint(Side::RIGHT)) && (side_ & uint(Side::BOTTOM)))
    setCursor(Qt::SizeFDiagCursor);
  else if (side_ & uint(Side::LEFT))
    setCursor(Qt::SizeHorCursor);
  else if (side_ & uint(Side::RIGHT))
    setCursor(Qt::SizeHorCursor);
  else if (side_ & uint(Side::TOP))
    setCursor(Qt::SizeVerCursor);
  else if (side_ & uint(Side::BOTTOM))
    setCursor(Qt::SizeVerCursor);
  else
    setCursor(Qt::ArrowCursor);
}

void
CQBypassWindow::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  QFontMetrics fm(font());

  int tw = fm.width(windowTitle());
  int th = fm.height();

  int titleHeight = th + 4;

  QRect r(0, 0, width(), titleHeight);

  p.fillRect(r, Qt::white);

  int cw = 16;
  int bm = 4;

  // draw title text
  {
  p.save();

  p.setClipRect(r);

  p.drawText(4, 2 + fm.ascent(), windowTitle());

  p.restore();
  }

  // draw title bars
  {
  p.save();

  QRect tr(tw + 2*bm, 0, width() - tw - 4*bm - cw, titleHeight);

  p.setClipRect(tr);

  p.setPen(QColor(120, 120, 120));

  int my = titleHeight/2;
  int nl = titleHeight/8;

  for (int y = 0; y < nl; ++y) {
    int yy1 = my - y*2 - 1;
    int yy2 = my + y*2 + 1;

    p.drawLine(4, yy1, width() - 4, yy1);
    p.drawLine(4, yy2, width() - 4, yy2);
  }

  p.restore();
  }

  // draw close
  {
  QPen pen;

  pen.setColor(Qt::black);
  pen.setWidth(6);

  p.setPen(pen);

  int by = titleHeight/2;
  int bx = width() - cw/2 - bm;
  int bs = cw/2 - 2;

  closeRect_ = QRect(bx - bs, by - bs, 2*bs, 2*bs);

  p.drawLine(closeRect_.left(), closeRect_.top(), closeRect_.right(), closeRect_.bottom());
  p.drawLine(closeRect_.left(), closeRect_.bottom(), closeRect_.right(), closeRect_.top());
  }
}
