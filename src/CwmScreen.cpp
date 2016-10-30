#include <CwmScreen.h>
#include <CwmMachine.h>
#include <CwmWindow.h>
#include <CwmFontMgr.h>
#include <CwmColorMgr.h>
#include <CwmResourceData.h>
#include <CwmGraphics.h>
#include <CwmColormapMgr.h>
#include <CwmCursorMgr.h>
#include <CwmDeskIcon.h>
#include <CwmDeskMgr.h>
#include <CwmGrid.h>
#include <CwmMoveFeedback.h>
#include <CwmResizeFeedback.h>
#include <CwmWindowStack.h>
#include <CwmTabWindow.h>
#include <CwmInfo.h>
#include <CwmGnome.h>
#include <CwmAdvHints.h>
#include <CwmEventMgr.h>
#include <CXScreen.h>
#include <CFuncs.h>

#include <X11/XKBlib.h>
#include <climits>

CwmScreenMgr::
CwmScreenMgr()
{
}

CwmScreenMgr::
~CwmScreenMgr()
{
  term();
}

void
CwmScreenMgr::
term()
{
  std::for_each(screen_num_map_.begin(), screen_num_map_.end(), CDeleteMapSecond<ScreenNumMap>());

  screen_num_map_ .clear();
  screen_root_map_.clear();
}

CwmScreen &
CwmScreenMgr::
getScreenByNum(int screen_num)
{
  ScreenNumMap::iterator pscreen = screen_num_map_.find(screen_num);

  if (pscreen != screen_num_map_.end())
    return *(*pscreen).second;

  int num_screens = CwmMachineInst->getNumScreens();

  if (screen_num < 0 || screen_num >= num_screens) {
    CwmMachineInst->log("Invalid Screen Number " +
                        CStrUtil::toString(screen_num) + "\n");
    return getScreenByNum(0);
  }

  return *addScreen(screen_num);
}

CwmScreen &
CwmScreenMgr::
getScreenByWindow(Window xwin)
{
  if (! CwmMachineInst->isValidWindow(xwin)) {
    CwmMachineInst->log("Invalid Window " +
                        CStrUtil::toString(xwin) + "\n");
    return getScreenByNum(0);
  }

  Window root = CwmMachineInst->getWindowRoot(xwin);

  if (root == None) {
    CwmMachineInst->log("No Root for Window " +
                        CStrUtil::toString(xwin) + "\n");
    return getScreenByNum(0);
  }

  ScreenRootMap::iterator pscreen = screen_root_map_.find(root);

  if (pscreen != screen_root_map_.end())
    return *(*pscreen).second;

  CwmMachineInst->log("No Screen Found for Window " +
                      CStrUtil::toString(xwin) + "\n");

  return getScreenByNum(0);
}

CwmScreen *
CwmScreenMgr::
addScreen(int screen_num)
{
  CwmScreen *screen = new CwmScreen(screen_num);

  Window root = screen->getRoot()->getXWin();

  screen_num_map_ [screen_num] = screen;
  screen_root_map_[root      ] = screen;

  return screen;
}

void
CwmScreenMgr::
disableScreens()
{
  ScreenNumMap::iterator p1 = screen_num_map_.begin();
  ScreenNumMap::iterator p2 = screen_num_map_.end  ();

  for ( ; p1 != p2; ++p1)
    (*p1).second->disable();
}

void
CwmScreenMgr::
enableScreens()
{
  ScreenNumMap::iterator p1 = screen_num_map_.begin();
  ScreenNumMap::iterator p2 = screen_num_map_.end  ();

  for ( ; p1 != p2; ++p1)
    (*p1).second->enable();
}

//-----------------

CwmScreen::
CwmScreen(int screen_num) :
 num_(screen_num)
{
  cxscreen_ = new CXScreen(screen_num);

  //------

  root_ = new CwmWindow(*this, cxscreen_->getRoot(), 0, 0,
                        cxscreen_->getWidth(), cxscreen_->getHeight(), true);

  //------

  color_mgr_ = new CwmColorMgr(*this);
  font_mgr_  = new CwmFontMgr (*this);

  //------

  CwmFont  *font  = CwmResourceDataInst->getFont (*this);
  CwmColor *color = CwmResourceDataInst->getColor(*this);

  //------

  graphics_     = new CwmGraphics(*this, font, color);
  xor_graphics_ = new CwmGraphics(*this, font, color, true);

  //------

  colormap_mgr_ = new CwmColormapMgr(*this);

  cursor_mgr_ = new CwmCursorMgr(*this);

  icon_grid_ = new CwmGrid(*this, 16, 16);

  move_feedback_   = new CwmMoveFeedback  (*this);
  resize_feedback_ = new CwmResizeFeedback(*this);

  info_ = new CwmInfo(*this);

  desk_mgr_ = new CwmDeskMgr(*this, 4);

  desk_mgr_->changeDesk(0);

  //------

  window_stack_ = new CwmCirculateWindowStack(*this);

  tab_window_ = new CwmTabWindow(*this);

  //------

  //CwmGnomeInst->init(*this);

  CwmAdvHintsInst->init(*this);
}

CwmScreen::
~CwmScreen()
{
  delete icon_grid_;

  delete move_feedback_;
  delete resize_feedback_;

  delete info_;

  delete desk_mgr_;

  delete colormap_mgr_;

  delete cursor_mgr_;

  delete color_mgr_;

  delete window_stack_;

  delete graphics_;

  delete xor_graphics_;
}

CwmWMWindow *
CwmScreen::
addWMWindow(Window xwin)
{
  CwmWMWindow *window = CwmMachineInst->addWMWindow(*this, xwin);

  windows_.push_back(window);

  return window;
}

void
CwmScreen::
removeWMWindow(CwmWMWindow *window)
{
  windows_.remove(window);
}

Colormap
CwmScreen::
getColormap() const
{
  return cxscreen_->getColormap();
}

int
CwmScreen::
getWidth() const
{
  return cxscreen_->getWidth();
}

int
CwmScreen::
getHeight() const
{
  return cxscreen_->getHeight();
}

int
CwmScreen::
getDepth() const
{
  return cxscreen_->getDepth();
}

Pixel
CwmScreen::
getBlackPixel() const
{
  return cxscreen_->getBlackPixel();
}

Pixel
CwmScreen::
getWhitePixel() const
{
  return cxscreen_->getWhitePixel();
}

CwmColor *
CwmScreen::
getColor(Pixel fg, Pixel bg)
{
  return color_mgr_->getColor(fg, bg);
}

Pixel
CwmScreen::
getPixel(const std::string &name, Pixel default_pixel)
{
  return color_mgr_->getPixel(name, default_pixel);
}

Pixel
CwmScreen::
getPixel(const CRGBA &rgba)
{
  return cxscreen_->rgbaToPixel(rgba);
}

CwmFont *
CwmScreen::
getFont(const std::string &name)
{
  return font_mgr_->getFont(name);
}

Cursor
CwmScreen::
getCursor(CwmCursorType type) const
{
  return cursor_mgr_->getCursor(type);
}

CwmCirculateWindowStack &
CwmScreen::
getCirculateWindowStack() const
{
  window_stack_->update();

  return *window_stack_;
}

int
CwmScreen::
getNumDesks()
{
  return desk_mgr_->getNumDesks();
}

CwmDesk *
CwmScreen::
getCurrentDesk()
{
  return desk_mgr_->getCurrentDesk();
}

int
CwmScreen::
getCurrentDeskNum()
{
  return desk_mgr_->getCurrentDeskNum();
}

CwmDesk *
CwmScreen::
getDesk(int num)
{
  return desk_mgr_->getDesk(num);
}

CwmDesk *
CwmScreen::
getDesk(CwmWMWindow *window)
{
  return desk_mgr_->getDesk(window);
}

int
CwmScreen::
getDeskNum(CwmWMWindow *window)
{
  return desk_mgr_->getDeskNum(window);
}

void
CwmScreen::
changeDesk(int desk_num)
{
  desk_mgr_->changeDesk(desk_num);
}

void
CwmScreen::
showInfo(const std::string &str) const
{
  info_->show(str);
}

void
CwmScreen::
hideInfo() const
{
  info_->hide();
}

void
CwmScreen::
showMoveFeedback()
{
  move_feedback_->show();
}

void
CwmScreen::
hideMoveFeedback()
{
  move_feedback_->hide();
}

void
CwmScreen::
updateMoveFeedback(int x, int y)
{
  move_feedback_->update(x, y);
}

void
CwmScreen::
showResizeFeedback()
{
  resize_feedback_->show();
}

void
CwmScreen::
hideResizeFeedback()
{
  resize_feedback_->hide();
}

void
CwmScreen::
updateResizeFeedback(CwmWMWindow *window, int dx, int dy)
{
  resize_feedback_->update(window, dx, dy);
}

void
CwmScreen::
refresh()
{
  cxscreen_->refresh();
}

void
CwmScreen::
installColormap(CwmWMWindow *window)
{
  colormap_mgr_->install(window);
}

void
CwmScreen::
uninstallColormap()
{
  colormap_mgr_->uninstall();
}

bool
CwmScreen::
isTop(CwmWMWindow *window) const
{
  return getCirculateWindowStack().isTop(window);
}

void
CwmScreen::
raise(CwmWMWindow *window) const
{
  getCirculateWindowStack().raise(window);
}

void
CwmScreen::
lower(CwmWMWindow *window) const
{
  getCirculateWindowStack().lower(window);
}

void
CwmScreen::
circulateUp() const
{
  getCirculateWindowStack().circulateUp();
}

void
CwmScreen::
circulateDown() const
{
  getCirculateWindowStack().circulateDown();
}

void
CwmScreen::
selectNext()
{
  CwmMachineInst->grabKeyboard(getRoot()->getXWin());

  uint timeout, interval;

  CwmMachineInst->getKeyAutoRepeatRate(&timeout, &interval);

  CwmMachineInst->setKeyAutoRepeatRate(UINT_MAX, UINT_MAX);

  CwmEventMgrInst->waitForKeyRelease(*this);

  CwmMachineInst->setKeyAutoRepeatRate(timeout, interval);

  CwmMachineInst->ungrabKeyboard();

  tab_window_->hide();
}

void
CwmScreen::
selectPrev()
{
}

void
CwmScreen::
installColormap(Colormap cmap)
{
  CwmMachineInst->installColormap(cmap);
}

void
CwmScreen::
getDefaultPos(int window_width, int window_height, int *x, int *y)
{
  *x = default_x_;
  *y = default_y_;

  if (*x + window_width > getWidth()) {
    default_x_ = 0;

    *x = default_x_;
  }

  if (*y + window_height > getHeight()) {
    default_y_ = 0;

    *y = default_y_;
  }

  default_x_ += 16;
  default_y_ += 16;
}

void
CwmScreen::
drawOutline(int x1, int y1, int width1, int height1)
{
  xor_graphics_->drawRectangle(getRoot(), x1, y1, width1 -2, height1 - 2);
}

CwmWindow *
CwmScreen::
getPointerWindow()
{
  Window xwin = cxscreen_->getPointerWindow();

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(xwin);

  return xwindow;
}

Window
CwmScreen::
getCoordWindow(int x, int y)
{
  return cxscreen_->getCoordWindow(x, y);
}

bool
CwmScreen::
hasPointer()
{
  int x, y;

  return getRoot()->getPointerPosition(&x, &y);
}

void
CwmScreen::
updateCirculateWindowStack()
{
  window_stack_->update();
}

void
CwmScreen::
disable()
{
  desk_mgr_->disable();
}

void
CwmScreen::
enable()
{
  desk_mgr_->enable();
}

GC
CwmScreen::
createGC(CwmColor *color)
{
  GC gc;

  if (color != 0)
    gc = CwmMachineInst->createGC(color->getBg(), color->getFg());
  else
    gc = CwmMachineInst->createGC(0, 1);

  return gc;
}

GC
CwmScreen::
createXorGC(CwmColor *)
{
  GC gc = CwmMachineInst->createXorGC();

  return gc;
}

bool
CwmScreen::
selectWMInput()
{
  return cxscreen_->selectWMInput();
}

bool
CwmScreen::
getWindows(Window **windows, int *num_windows)
{
  return cxscreen_->getWindows(windows, num_windows);
}

void
CwmScreen::
processDeskMenu()
{
  desk_mgr_->processMenu();
}
