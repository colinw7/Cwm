#include <CQWmPager.h>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QAbstractNativeEventFilter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QToolButton>

#include <QX11Info>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <iostream>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQWmPager *pager = new CQWmPager;

  pager->show();

  app.exec();
}

//---

namespace {

Display *s_display;

void setDisplay(Display *display) {
  s_display = display;
}

Atom getAtom(const std::string &name) {
  return XInternAtom(s_display, name.c_str(), False);
}

std::string getAtomName(const Atom &) {
  return "";
}

bool getAtomArrayProperty(Window xwin, const Atom &atom, std::vector<Atom> &atoms) {
  ulong  n;
  Atom   type;
  ulong  left;
  uchar *data;
  int    format;

  if (! s_display)
    return false;

  if (XGetWindowProperty(s_display, xwin, atom, 0, 65535, False, XA_ATOM,
                         &type, &format, &n, &left, &data) != Success)
    return false;

  if (format != 32 || left != 0)
    return false;

  for (ulong i = 0; i < n; ++i)
    atoms.push_back((reinterpret_cast<Atom *>(data))[i]);

  XFree(data);

  return true;
}

bool getWindowArrayProperty(Window xwin, const Atom &atom, std::vector<Window> &windows) {
  ulong  n;
  Atom   type;
  ulong  left;
  uchar *data;
  int    format;

  if (! s_display)
    return false;

  if (XGetWindowProperty(s_display, xwin, atom, 0, 65535, False, XA_WINDOW,
                         &type, &format, &n, &left, &data) != Success)
    return false;

  if (format != 32 || left != 0)
    return false;

  for (ulong i = 0; i < n; ++i)
    windows.push_back((reinterpret_cast<Window *>(data))[i]);

  XFree(data);

  return true;
}

Window getRoot(int /*screen_num*/=-1) {
  if (s_display)
    return DefaultRootWindow(s_display);

  return 0;
}

Window getWindowParent(Window xwin) {
  Window  root;
  Window  parent;
  Window *children;
  uint    num_children;

  if (! XQueryTree(s_display, xwin, &root, &parent, &children, &num_children))
    return None;

  if (children)
    XFree(children);

  if (parent == root)
    return None;

  return parent;
}

bool getWindowProperty(Window xwin, const Atom &atom, Window *value) {
  ulong  n;
  Atom   type;
  ulong  left;
  uchar *data;
  int    format;

  if (! s_display)
    return false;

  if (XGetWindowProperty(s_display, xwin, atom, 0, 1, False, XA_WINDOW,
                         &type, &format, &n, &left, &data) != Success)
    return false;

  if (format != 32 || n != 1 || left != 0)
    return false;

  *value = *(reinterpret_cast<Window *>(data));

  XFree(data);

  return true;
}

Atom getWMStateAtom() {
  return 0;
}

int getWMState(Window xwin) {
  ulong  n;
  Atom   type;
  ulong  left;
  uchar *data;
  int    format;

  if (! s_display)
    return -1;

  auto atom = getWMStateAtom();

  if (XGetWindowProperty(s_display, xwin, atom, 0, 3, False, atom,
                         &type, &format, &n, &left, &data) != Success)
    return -1;

  if (n == 0 || ! data)
    return -1;

  int state = (reinterpret_cast<int *>(data))[0];

  XFree(data);

  return state;
}

bool getWMName(Window xwin, std::string &name) {
  name = "";

  XTextProperty text_prop;

  if (! XGetWMName(s_display, xwin, &text_prop))
    return false;

  const char *cname = reinterpret_cast<const char *>(text_prop.value);

  if (cname && cname[0] != '\0')
    name = cname;

  return true;
}

bool getWindowGeometry(Window xwin, int *x, int *y, int *width, int *height, int *border)
{
  XWindowAttributes attr;

  if (XGetWindowAttributes(s_display, xwin, &attr)) {
    if (x)
      *x = attr.x;

    if (y)
      *y = attr.y;

    if (width)
      *width = attr.width;

    if (height)
      *height = attr.height;

    if (border)
      *border = attr.border_width;

    return true;
  }
  else {
    if (x)
      *x = 0;

    if (y)
      *y = 0;

    if (width)
      *width = 1;

    if (height)
      *height = 1;

    if (border)
      *border = 0;

    return false;
  }
}

bool getWindowPosition(Window xwin, int *x, int *y) {
  int x1, y1;

  if (! getWindowGeometry(xwin, &x1, &y1, nullptr, nullptr, nullptr))
    return false;

  auto parent = getWindowParent(xwin);

  while (parent != None) {
    int x2, y2;

    if (! getWindowGeometry(parent, &x2, &y2, nullptr, nullptr, nullptr))
      return false;

    x1 += x2;
    y1 += y2;

    parent = getWindowParent(parent);
  }

  *x = x1;
  *y = y1;

  return true;
}

bool sendShowDesktop(bool show) {
  Window root = getRoot();

  XClientMessageEvent event;

  memset(&event, 0, sizeof(XClientMessageEvent));

  event.type         = ClientMessage;
  event.serial       = 0;
  event.send_event   = True;
  event.display      = s_display;
  event.window       = root;
  event.message_type = getAtom("_NET_SHOWING_DESKTOP");
  event.format       = 32;
  event.data.l[0]    = long(show ? 1 : 0);

  if (! XSendEvent(s_display, root, False, (SubstructureNotifyMask|SubstructureRedirectMask),
                   reinterpret_cast<XEvent *>(&event)))
    return true;

  return false;
}

bool sendClose(Window window) {
  XClientMessageEvent event;

  memset(&event, 0, sizeof(XClientMessageEvent));

  event.type         = ClientMessage;
  event.serial       = 0;
  event.send_event   = True;
  event.display      = s_display;
  event.window       = window;
  event.message_type = getAtom("_NET_CLOSE_WINDOW");
  event.format       = 32;
  event.data.l[0]    = long(CurrentTime);
  event.data.l[1]    = 2;

  Window root = getRoot();

  if (! XSendEvent(s_display, root, False, (SubstructureNotifyMask|SubstructureRedirectMask),
                   reinterpret_cast<XEvent *>(&event)))
    return true;

  return false;
}

bool sendActivate(Window window) {
  XClientMessageEvent event;

  memset(&event, 0, sizeof(XClientMessageEvent));

  event.type         = ClientMessage;
  event.serial       = 0;
  event.send_event   = True;
  event.display      = s_display;
  event.window       = window;
  event.message_type = getAtom("_NET_ACTIVE_WINDOW");
  event.format       = 32;
  event.data.l[0]    = 2;
  event.data.l[1]    = long(CurrentTime);
  event.data.l[2]    = 0;

  Window root = getRoot();

  if (! XSendEvent(s_display, root, False, (SubstructureNotifyMask|SubstructureRedirectMask),
                   reinterpret_cast<XEvent *>(&event)))
    return true;

  return false;
}

bool flushEvents(bool sync=false) {
  XFlush(s_display);

  if (sync)
    XSync(s_display, False);

  return true;
}

}

//---

class CQWmPagerXcbEventFilter : public QAbstractNativeEventFilter {
 public:
  CQWmPagerXcbEventFilter(CQWmPager *pager) :
   pager_(pager) {
    root_ = QX11Info::appRootWindow();

    timeAtom_       = getAtom("_NET_WM_USER_TIME");
    clientListAtom_ = getAtom("_NET_CLIENT_LIST_STACKING");
    activeAtom_     = getAtom("_NET_ACTIVE_WINDOW");
  }

  bool nativeEventFilter(const QByteArray &eventType, void *message, long *) override {
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
  Atom       timeAtom_;
  Atom       clientListAtom_;
  Atom       activeAtom_;
};

CQWmPager::
CQWmPager() :
 CQBypassWindow()
{
  setWindowTitle("WmPager");

  //---

  Display *disp = QX11Info::display();

  setDisplay(disp);

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

  std::vector<Atom> atoms;

  const auto &supportedAtom = getAtom("_NET_SUPPORTED");

  auto root = QX11Info::appRootWindow();

  getAtomArrayProperty(root, supportedAtom, atoms);

  for (uint i = 0; i < atoms.size(); ++i) {
    auto id = uint(atoms[i]);

    auto name = getAtomName(atoms[i]);

    //supportedNames_[id] = name;

    supportedIds_[name] = id;
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
  const auto &clientListAtom = getAtom("_NET_CLIENT_LIST_STACKING");
  const auto &stateAtom      = getAtom("_NET_WM_STATE");
  const auto &skipAtom       = getAtom("_NET_WM_STATE_SKIP_PAGER");
  const auto &hiddenAtom     = getAtom("_NET_WM_STATE_HIDDEN");
  const auto &dockAtom       = getAtom("_NET_WM_WINDOW_TYPE_DOCK");
  const auto &activeAtom     = getAtom("_NET_ACTIVE_WINDOW");

  auto root = QX11Info::appRootWindow();

  std::vector<Window> windows;

  getWindowArrayProperty(root, clientListAtom, windows);

  Window activeWindow;

  getWindowProperty(root, activeAtom, &activeWindow);

  //---

  windows_.clear();

  for (uint i = 0; i < windows.size(); ++i) {
    auto xwin = windows[i];

    //---

    std::vector<Atom> atoms;

    if (getAtomArrayProperty(xwin, stateAtom, atoms)) {
      bool skip   = false;
      bool hidden = false;
      bool dock   = false;

      for (const auto &atom : atoms) {
        if (atom == skipAtom)
          skip = true;

        if (atom == hiddenAtom)
          hidden = true;

        if (atom == dockAtom)
          dock = true;
      }

      if (skip || hidden || dock)
        continue;
    }

    int state = getWMState(xwin);

    if (state >= 0 && state != NormalState)
      continue;

    //---

    std::string name;

    getWMName(xwin, name);

    bool active = (xwin == activeWindow);

    int x1, y1, width1, height1;

    if (! getWindowGeometry(xwin, &x1, &y1, &width1, &height1, nullptr))
      continue;

    if (! getWindowPosition(xwin, &x1, &y1))
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
  sendShowDesktop(b);
}

//------

CQWmPagerCanvas::
CQWmPagerCanvas(CQWmPager *pager) :
 pager_(pager)
{
  auto root = QX11Info::appRootWindow();

  int root_x, root_y;

  if (! getWindowGeometry(root, &root_x, &root_y, &rootWidth_, &rootHeight_, nullptr))
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

  for (int i = int(windows.size() - 1); i >= 0; --i) {
    if (windows[i].xwin == topWin_)
      continue;

    if (windows[i].rect.contains(pressPos_)) {
      pressWin_ = windows[i].xwin;
      break;
    }
  }

  if (! pressWin_ || pressWin_ == topWin_)
    return;

  if (me->button() == Qt::MiddleButton) {
    std::cerr << "Close: " << pressWin_ << "\n";

    sendClose(pressWin_);
  }
  else {
    std::cerr << "Activate: " << pressWin_ << "\n";

    sendActivate(pressWin_);

    //sendRestackWindow(pressWin_, topWin_);

    //QPoint gp = me->globalPos();

    //sendDragWindowBy(pressWin_, gp.x(), gp.y(), 1, _NET_WM_MOVERESIZE_MOVE);

    //sendWmState(pressWin_, _NET_WM_STATE_ADD, "_NET_WM_ACTION_ABOVE");

    flushEvents(/*sync*/true);
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

  //sendMoveWindowBy(pressWin_, dx, dy);

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
