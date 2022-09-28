#ifndef CWM_SCREEN_H
#define CWM_SCREEN_H

#include <CwmTypes.h>
#include <CRGBA.h>

#include <std_Xt.h>

class CwmScreenMgr {
 public:
  CwmScreenMgr();
 ~CwmScreenMgr();

  void term();

  CwmScreen &getScreenByNum   (int screen_num);
  CwmScreen &getScreenByWindow(Window xwin);

  void disableScreens();
  void enableScreens();

 private:
  CwmScreen *addScreen(int screen_num);

 private:
  typedef std::map<int   ,CwmScreen *> ScreenNumMap;
  typedef std::map<Window,CwmScreen *> ScreenRootMap;

  ScreenNumMap  screen_num_map_;
  ScreenRootMap screen_root_map_;
};

class CwmScreen {
 public:
  CwmScreen(int screen_num);
 ~CwmScreen();

  CXScreen  *getCXScreen() const { return cxscreen_; }
  int        getNum     () const { return num_;      }
  CwmWindow *getRoot    () const { return root_;     }

  CwmWMWindow *addWMWindow(Window xwin);
  void         removeWMWindow(CwmWMWindow *window);

  Colormap getColormap() const;

  void installColormap(CwmWMWindow *window);
  void uninstallColormap();

  int getWidth() const;
  int getHeight() const;
  int getDepth() const;

  Pixel getBlackPixel() const;
  Pixel getWhitePixel() const;

  CwmColor *getColor(Pixel fg, Pixel bg);
  Pixel     getPixel(const std::string &name, Pixel default_pixel);
  Pixel     getPixel(const CRGBA &rgba);

  CwmFont *getFont(const std::string &name);

  Cursor getCursor(CwmCursorType type) const;

  CwmGrid *getIconGrid() const { return icon_grid_; }

  CwmDeskMgr *getDeskMgr() const { return desk_mgr_; }

  CwmCirculateWindowStack &getCirculateWindowStack() const;

  uint getNumDesks();

  CwmDesk *getCurrentDesk();
  int      getCurrentDeskNum();

  CwmDesk *getDesk(int num);
  CwmDesk *getDesk(CwmWMWindow *window);
  int      getDeskNum(CwmWMWindow *window);

  void changeDesk(int desk_num);

  void showInfo(const std::string &str) const;
  void hideInfo() const;

  void showMoveFeedback();
  void hideMoveFeedback();

  void updateMoveFeedback(int x, int y);

  void showResizeFeedback();
  void hideResizeFeedback();

  void updateResizeFeedback(CwmWMWindow *window, int dx, int dy);

  void refresh();

  bool isTop(CwmWMWindow *window) const;

  void raise(CwmWMWindow *window) const;
  void lower(CwmWMWindow *window) const;

  void circulateUp  () const;
  void circulateDown() const;

  void selectNext();
  void selectPrev();

  void installColormap(Colormap cmap);

  void getDefaultPos(int width, int height, int *x, int *y);

  void drawOutline(int x1, int y1, int width1, int height1);

  CwmWindow *getPointerWindow();
  Window     getCoordWindow(int x, int y);
  bool       hasPointer();

  void updateCirculateWindowStack();

  void disable();
  void enable();

  GC createGC   (CwmColor *color);
  GC createXorGC(CwmColor *color);

  bool selectWMInput();

  bool getWindows(Window **windows, int *num_windows);

  void processDeskMenu();

 private:
  void initCursorColor();
  void initFocusColor();
  void initStippleBitmap();

  void getCirculateWindows();

 private:
  typedef std::list<CwmWMWindow *> WMWindowList;

  int                      num_ { 0 };

  CXScreen                *cxscreen_ { nullptr };

  CwmGraphics             *graphics_ { nullptr };
  CwmGraphics             *xor_graphics_ { nullptr };

  CwmWindow               *root_ { nullptr };

  CwmColor                *color_ { nullptr };

  CwmDeskMgr              *desk_mgr_ { nullptr };
  CwmColorMgr             *color_mgr_ { nullptr };
  CwmFontMgr              *font_mgr_ { nullptr };
  CwmCursorMgr            *cursor_mgr_ { nullptr };
  CwmColormapMgr          *colormap_mgr_ { nullptr };

  CwmGrid                 *icon_grid_ { nullptr };

  CwmMoveFeedback         *move_feedback_ { nullptr };
  CwmResizeFeedback       *resize_feedback_ { nullptr };

  CwmInfo                 *info_ { nullptr };

  int                      default_x_ { 0 };
  int                      default_y_ { 0 };

  CwmCirculateWindowStack *window_stack_ { nullptr };

  CwmTabWindow            *tab_window_ { nullptr };

  WMWindowList             windows_;
};

#endif
