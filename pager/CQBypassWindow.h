#include <QWidget>

class CQBypassWindow : public QWidget {
  Q_OBJECT

 public:
  enum class Side {
    LEFT   = (1<<0),
    RIGHT  = (1<<1),
    TOP    = (1<<2),
    BOTTOM = (1<<3)
  };

 public:
  CQBypassWindow();

  void mousePressEvent(QMouseEvent *me);
  void mouseMoveEvent(QMouseEvent *me);
  void mouseReleaseEvent(QMouseEvent *me);

  void paintEvent(QPaintEvent *);

  void updateSize(const QPoint &pos);

 private:
  QPoint pressPos_;
  uint   side_ { 0 };
  bool   pressed_ { false };
  QRect  closeRect_;
};
