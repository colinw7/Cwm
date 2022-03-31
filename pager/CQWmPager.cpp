#include <CQWmPager.h>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QAbstractNativeEventFilter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QToolButton>

#include <CXLib.h>
#include <QX11Info>
#include <xcb/xcb.h>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQWmPager *pager = new CQWmPager;

  pager->show();

  app.exec();
}

class CQWmPagerXcbEventFilter : public QAbstractNativeEventFilter {
 public:
  CQWmPagerXcbEventFilter(CQWmPager *pager) :
   pager_(pager) {
    root_ = QX11Info::appRootWindow();

    timeAtom_       = CXMachineInst->getAtom("_NET_WM_USER_TIME");
    clientListAtom_ = CXMachineInst->getAtom("_NET_CLIENT_LIST_STACKING");
    activeAtom_     = CXMachineInst->getAtom("_NET_ACTIVE_WINDOW");
  }

  bool nativeEventFilter(const QByteArray &eventType, void *message, long *) {
    if (eventType == "xcb_generic_event_t") {
      auto *ev = static_cast<xcb_generic_event_t *>(message);

      int type = (ev->response_type & 0x7f);

      if      (type == XCB_PROPERTY_NOTIFY) {
        auto *ev1 = reinterpret_cast<xcb_property_notify_event_t *>(ev);

        pager_->windowChanged(ev1->window);

        //std::cerr << "property: " << ev1->window << " " <<
        //  pager_->getSupportedAtomName(ev1->atom) << std::endl;
      }
      else if (type == XCB_CONFIGURE_NOTIFY) {
        auto *ev1 = reinterpret_cast<xcb_configure_notify_event_t *>(ev);

        pager_->windowChanged(ev1->window);

        //std::cerr << "configure: " << ev1->window << std::endl;
      }
      else {
        //std::cerr << type << std::endl;
      }
    }

    return false;
  }

 private:
  CQWmPager *pager_;
  Window     root_ { 0 };
  CXAtom     timeAtom_;
  CXAtom     clientListAtom_;
  CXAtom     activeAtom_;
};

CQWmPager::
CQWmPager() :
 CQBypassWindow()
{
  setWindowTitle("WmPager");

  //---

  Display *disp = QX11Info::display();

  CXMachineInst->setDisplay(disp);

  //---

  auto *layout = new QVBoxLayout(this);

  auto *toolbar = new QFrame;

  layout->addWidget(toolbar);

  auto *tlayout = new QHBoxLayout(toolbar);

  auto *button = new QToolButton;

  button->setText("Desktop");
  button->setCheckable(true);

  connect(button, SIGNAL(clicked(bool)), this, SLOT(desktopSlot(bool)));

  tlayout->addWidget(button);
  tlayout->addStretch(1);

  //---

  auto *tabLayout = new QHBoxLayout;

  layout->addLayout(tabLayout);

  //---

  auto *tab = new QTabWidget;

  tabLayout->addWidget(tab);

  canvas_ = new CQWmPagerCanvas(this);
  tree_   = new CQWmPagerTree(this);

  tab->addTab(canvas_, "Visual");
  tab->addTab(tree_  , "Textual");

  //---

  std::vector<CXAtom> atoms;

  const auto &supportedAtom = CXMachineInst->getAtom("_NET_SUPPORTED");

  auto root = QX11Info::appRootWindow();

  CXMachineInst->getAtomArrayProperty(root, supportedAtom, atoms);

  for (uint i = 0; i < atoms.size(); ++i) {
    auto id = uint(atoms[i].getXAtom());

    supportedNames_[id] = atoms[i].getName();

    supportedIds_[atoms[i].getName()] = id;
  }

  //---

  qApp->installNativeEventFilter(new CQWmPagerXcbEventFilter(this));

  updateWindows();
}

std::string
CQWmPager::
getSupportedAtomName(uint id) const
{
  auto p = supportedNames_.find(id);

  if (p == supportedNames_.end())
    return "";

  return (*p).second;
}

uint
CQWmPager::
getSupportedAtomId(const std::string &id) const
{
  auto p = supportedIds_.find(id);

  if (p == supportedIds_.end())
    return 0;

  return (*p).second;
}

void
CQWmPager::
updateWindows()
{
  const auto &clientListAtom = CXMachineInst->getAtom("_NET_CLIENT_LIST_STACKING");
  const auto &stateAtom      = CXMachineInst->getAtom("_NET_WM_STATE");
  const auto &skipAtom       = CXMachineInst->getAtom("_NET_WM_STATE_SKIP_PAGER");
  const auto &hiddenAtom     = CXMachineInst->getAtom("_NET_WM_STATE_HIDDEN");
  const auto &dockAtom       = CXMachineInst->getAtom("_NET_WM_WINDOW_TYPE_DOCK");
  const auto &activeAtom     = CXMachineInst->getAtom("_NET_ACTIVE_WINDOW");

  auto root = QX11Info::appRootWindow();

  std::vector<Window> windows;

  CXMachineInst->getWindowArrayProperty(root, clientListAtom, windows);

  Window activeWindow;

  CXMachineInst->getWindowProperty(root, activeAtom, &activeWindow);

  //---

  windows_.clear();

  for (uint i = 0; i < windows.size(); ++i) {
    auto xwin = windows[i];

    //---

    std::vector<CXAtom> atoms;

    if (CXMachineInst->getAtomArrayProperty(xwin, stateAtom, atoms)) {
      bool skip   = false;
      bool hidden = false;
      bool dock   = false;

      for (const auto &atom : atoms) {
        if (atom.getXAtom() == skipAtom.getXAtom())
          skip = true;

        if (atom.getXAtom() == hiddenAtom.getXAtom())
          hidden = true;

        if (atom.getXAtom() == dockAtom.getXAtom())
          dock = true;
      }

      if (skip || hidden || dock)
        continue;
    }

    int state = CXMachineInst->getWMState(xwin);

    if (state >= 0 && state != NormalState)
      continue;

    //---

    std::string name;

    CXMachineInst->getWMName(xwin, name);

    bool active = (xwin == activeWindow);

    int x1, y1, width1, height1;

    if (! CXMachineInst->getWindowGeometry(xwin, &x1, &y1, &width1, &height1))
      continue;

    if (! CXMachineInst->getWindowPosition(xwin, &x1, &y1))
      continue;

    QRect r(x1, y1, width1, height1);

    windows_.push_back(WmWindow(uint(xwin), r, name, active));
  }

  //---

  canvas_->update();

  tree_->reload();
}

void
CQWmPager::
windowChanged(uint xwin)
{
  bool changed = false;

  if (xwin == QX11Info::appRootWindow())
    changed = true;

  if (! changed) {
    for (const auto &window : windows_) {
      if (window.xwin == xwin) {
        changed = true;
        break;
      }
    }
  }

  if (changed)
    updateWindows();
}

void
CQWmPager::
desktopSlot(bool b)
{
  CXMachineInst->sendShowDesktop(b);
}

//------

CQWmPagerCanvas::
CQWmPagerCanvas(CQWmPager *pager) :
 pager_(pager)
{
  auto root = QX11Info::appRootWindow();

  int root_x, root_y;

  if (! CXMachineInst->getWindowGeometry(root, &root_x, &root_y, &rootWidth_, &rootHeight_))
    return;
}

void
CQWmPagerCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  width_  = width ();
  height_ = height();

  auto f = font();

  f.setPointSizeF(0.6*f.pointSizeF());

  p.setFont(f);

  QFontMetrics fm(f);

  for (auto window : pager_->windows()) {
    int x1 = window.rect.left  ();
    int y1 = window.rect.top   ();
    int x2 = window.rect.right ();
    int y2 = window.rect.bottom();

    auto p1 = mapPoint(QPoint(x1, y1));
    auto p2 = mapPoint(QPoint(x2, y2));

    QRect r(p1.x(), p1.y(), p2.x() - p1.x() + 1, p2.y() - p1.y() + 1);

    QColor c(0, 0, 0, 128);

    p.fillRect(r, c);

    p.setPen(Qt::white);

    p.drawRect(r);

    QRect r1(p1.x(), p1.y(), p2.x() - p1.x() + 1, fm.height());

    p.fillRect(r1, Qt::white);

    p.setPen(window.active ? Qt::blue : Qt::black);

    p.drawText(r.left(), r.top() + fm.ascent(), window.name.c_str());
  }
}

void
CQWmPagerCanvas::
mousePressEvent(QMouseEvent *me)
{
  if (me->button() == Qt::RightButton) {
    return;
  }

  const auto &windows = pager_->windows();

  if (windows.empty())
    return;

  pressPos_ = unmapPoint(me->pos());

  topWin_ = windows[windows.size() - 1].xwin;

  pressWin_ = 0;

  for (size_t i = size_t(windows.size() - 1); i >= 0; --i) {
    if (windows[i].xwin == topWin_)
      continue;

    if (windows[i].rect.contains(pressPos_)) {
      pressWin_ = windows[i].xwin;
      break;
    }
  }

  if (! pressWin_ || pressWin_ == topWin_)
    return;

  if (me->button() == Qt::MidButton) {
    std::cerr << "Close: " << pressWin_ << "\n";

    CXMachineInst->sendClose(pressWin_);
  }
  else {
    std::cerr << "Activate: " << pressWin_ << "\n";

    CXMachineInst->sendActivate(pressWin_);

    //CXMachineInst->sendRestackWindow(pressWin_, topWin_);

    //QPoint gp = me->globalPos();

    //CXMachineInst->sendDragWindowBy(pressWin_, gp.x(), gp.y(), 1, _NET_WM_MOVERESIZE_MOVE);

    //CXMachineInst->sendWmState(pressWin_, _NET_WM_STATE_ADD, "_NET_WM_ACTION_ABOVE");

    CXMachineInst->flushEvents(/*sync*/true);
  }
}

void
CQWmPagerCanvas::
mouseMoveEvent(QMouseEvent *me)
{
  const auto &windows = pager_->windows();

  if (windows.empty())
    return;

  auto p = unmapPoint(me->pos());

  //int dx = p.x() - pressPos_.x();
  //int dy = p.y() - pressPos_.y();

  if (! pressWin_ || pressWin_ == topWin_)
    return;

  //CXMachineInst->sendMoveWindowBy(pressWin_, dx, dy);

  pressPos_ = p;

  update();
}

void
CQWmPagerCanvas::
mouseReleaseEvent(QMouseEvent *)
{
}

QPoint
CQWmPagerCanvas::
mapPoint(const QPoint &p) const
{
  // pixel to window
  double x = width_ *((1.0*p.x())/rootWidth_ );
  double y = height_*((1.0*p.y())/rootHeight_);

  return QPoint(int(x), int(y));
}

QPoint
CQWmPagerCanvas::
unmapPoint(const QPoint &p) const
{
  // window to pixel
  double x = rootWidth_ *((1.0*p.x())/width_ );
  double y = rootHeight_*((1.0*p.y())/height_);

  return QPoint(int(x), int(y));
}

//------

CQWmPagerTree::
CQWmPagerTree(CQWmPager *pager) :
 pager_(pager)
{
}

void
CQWmPagerTree::
reload()
{
  clear();

  for (auto window : pager_->windows()) {
    auto *item = new QTreeWidgetItem(QStringList() << window.name.c_str());

    addTopLevelItem(item);
  }
}
