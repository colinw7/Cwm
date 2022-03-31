#ifndef CQWmPager_H
#define CQWmPager_H

#include <CQBypassWindow.h>
#include <QTreeWidget>

class CQWmPagerCanvas;
class CQWmPagerTree;

struct WmWindow {
  WmWindow(uint w, const QRect &r, const std::string &n, bool a) :
   xwin(w), rect(r), name(n), active(a) {
  }

  uint        xwin   { 0 };
  QRect       rect;
  std::string name;
  bool        active { false };
};

class CQWmPager : public CQBypassWindow {
  Q_OBJECT

 public:
  typedef std::vector<WmWindow> Windows;

 public:
  CQWmPager();

  const Windows &windows() const { return windows_; }

  std::string getSupportedAtomName(uint id) const;
  uint        getSupportedAtomId(const std::string &id) const;

  void windowChanged(uint xwin);

  void updateWindows();

  QSize sizeHint() const { return QSize(512, 512); }

 private slots:
  void desktopSlot(bool b);

 private:
  using SupportAtomIds   = std::map<std::string, uint>;
  using SupportAtomNames = std::map<uint, std::string>;

  CQWmPagerCanvas* canvas_;
  CQWmPagerTree*   tree_;
  SupportAtomIds   supportedIds_;
  SupportAtomNames supportedNames_;
  Windows          windows_;
};

class CQWmPagerCanvas : public QWidget {
  Q_OBJECT

 public:
  CQWmPagerCanvas(CQWmPager *pager);

  void paintEvent(QPaintEvent *);

  void mousePressEvent  (QMouseEvent *me);
  void mouseMoveEvent   (QMouseEvent *me);
  void mouseReleaseEvent(QMouseEvent *me);

  QPoint mapPoint  (const QPoint &p) const;
  QPoint unmapPoint(const QPoint &p) const;

 private:
  CQWmPager *pager_      { nullptr };
  int        rootWidth_  { 0 };
  int        rootHeight_ { 0 };
  int        width_      { 0 };
  int        height_     { 0 };
  uint       pressWin_   { 0 };
  QPoint     pressPos_;
  uint       topWin_     { 0 };
};

class CQWmPagerTree : public QTreeWidget {
  Q_OBJECT

 public:
  CQWmPagerTree(CQWmPager *pager);

  void reload();

 private:
  CQWmPager *pager_ { nullptr };
};

#endif
