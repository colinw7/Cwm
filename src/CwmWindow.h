#ifndef CWM_WINDOW_H
#define CWM_WINDOW_H

#include <std_Xt.h>

class CwmXWindowCallback {
 public:
  CwmXWindowCallback(CwmWindow *xwindow, CwmXWindowCallType type,
                     CwmXWindowCallProc proc, CwmData data);
 ~CwmXWindowCallback();

  void invokeIfType(CwmXWindowCallType type, CwmData detail);
  void invoke(CwmData detail);
  bool match(CwmXWindowCallType type, CwmXWindowCallProc proc, CwmData data);

 private:
  CwmWindow          *xwindow_;
  CwmXWindowCallType  type_;
  CwmXWindowCallProc  proc_;
  CwmData             data_;
};

class CwmWindow {
 public:
  CwmWindow(CwmScreen &screen, CwmWindow *parent, int x, int y,
             int width, int height, uint event_mask, CwmCursorType cursor);
  CwmWindow(CwmScreen &screen, Window parent, int x, int y,
             int width, int height, uint event_mask, CwmCursorType cursor);
  CwmWindow(CwmScreen &screen, Window xwin, int x, int y,
             int width, int height, bool mapped);
 ~CwmWindow();

  void setBackground(CwmGraphics *graphics);
  void setBackgroundPixmap(CwmXPixmap *pixmap);

  void setSaveUnder(bool flag);

  void map();
  void mapRaised();
  void mapSubwindows();
  void unmap();

  void lower();
  void raise();

  void move(int x, int y);
  void resize(int width, int height);
  void moveResize(int x, int y, int width, int height);

  void clear();

  void setStackMode(int stack_mode, CwmWMWindow *sibling);

  void configure(uint mask, XWindowChanges *xwc);

  bool isValid();

  void warpPointer(int x, int y);

  bool selectInput(uint event_mask);
  bool addInput(uint event_mask);

  bool getChildWindows(Window **windows, int *num_windows);

  void grabButtonPress();
  void ungrabButtonPress();

  void grabButton1Press();
  void ungrabButton1Press();

  bool grab(uint event_mask, CwmCursorType cursor_type);
  bool ungrab();

  bool grabPointer(uint event_mask, CwmCursorType cursor_type);
  bool ungrabPointer();

  bool getPointerPosition(int *x, int *y);

  void coordsToRoot(int x, int y, int *root_x, int *root_y);

  void setBorderWidth(int width);

  void addToSaveSet();
  void removeFromSaveSet();

  void sendEvent(XEvent *event, int event_mask);
  void sendPointerEvent(XEvent *event);

  void allowPointerEvents();
  void replayPointerEvents();

  void addCallback(CwmXWindowCallType type,
                   CwmXWindowCallProc proc, CwmData data);
  void callCallbacks(CwmXWindowCallType type, CwmData detail);
  void deleteCallback(CwmXWindowCallType type,
                      CwmXWindowCallProc proc, CwmData data);

  void reparent(CwmWindow *parent, int x, int y);

  Pixmap toMask();

  CwmScreen &getScreen () const { return screen_;  }
  Window     getXWin   () const { return xwin_;    }
  int        getX      () const { return x_;       }
  int        getY      () const { return y_;       }
  int        getWidth  () const { return width_;   }
  int        getHeight () const { return height_;  }
  bool       getMapped () const { return mapped_;  }

  void setXWin(Window xwin) { xwin_ = xwin; }

  bool isViewable();

  void getPosition(int *x, int *y);
  void getSize(int *width, int *height);
  void getGeometry(int *x, int *y, int *width, int *height, int *border);

  bool translateCoords(CwmWindow *dest, int src_x, int src_y,
                       int *dest_x, int *dest_y);

 private:
  void create(CwmScreen &screen, Window parent,
              uint event_mask, CwmCursorType cursor);

 private:
  typedef std::list<CwmXWindowCallback *> XWindowCallbackList;

  CwmScreen           &screen_;
  Window               xwin_;
  int                  x_;
  int                  y_;
  int                  width_;
  int                  height_;
  bool                 mapped_;
  bool                 owner_;
  XWindowCallbackList  callbacks_;
};

#endif
