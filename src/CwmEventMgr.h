#ifndef CWM_EVENT_MGR_H
#define CWM_EVENT_MGR_H

#define CwmEventMgrInst CwmEventMgr::getInstance()

class CwmEventMgr {
 private:
  XEvent last_event_;
  Time   last_event_time_;

  Window multiclick_window_;
  int    multiclick_button_;
  Time   multiclick_time_;
  int    multiclick_count_;

  Window save_event_window_;
  uint   save_event_mask_;

 public:
  static CwmEventMgr *getInstance();

 ~CwmEventMgr();

  bool eventPending();

  void    processEvents();
  XEvent *processNextEvent();
  bool    processEvent(XEvent *event);

  void printEvent(XEvent *event);

  Window      getEventWindow(XEvent *event);
  std::string getEventName(XEvent *event);
  int         getLastEventType();
  Time        getLastEventTime();

  Window waitForButtonClick(CwmWindow *xwindow, int buttons);
  Window waitForButtonPress(CwmWindow *xwindow, int buttons);
  Window waitForButtonRelease(CwmWindow *xwindow, int buttons);
  Window waitForKeyRelease(CwmWindow *xwindow);
  Window waitForKeyRelease(CwmScreen &screen);

  void flushEvents(bool sync);
  void flushExposeEvents(CwmWindow *xwindow);
  void flushEnterLeaveEvents(CwmWindow *xwindow);

  bool isDestroyPending(Window xwin);

  CwmWMWindow *selectWindow(CwmScreen &screen);

 private:
  CwmEventMgr();

  bool processMapRequest(XMapRequestEvent *);
  bool processMapNotify(XMapEvent *);
  bool processUnmapNotify(XUnmapEvent *);
  bool processConfigureRequest(XConfigureRequestEvent *);
  bool processConfigureNotify(XConfigureEvent *);
  bool processReparentNotify(XReparentEvent *);
  bool processEnterNotify(XEnterWindowEvent *);
  bool processLeaveNotify(XLeaveWindowEvent *);
  bool processButtonPress(XButtonPressedEvent *);
  bool processButtonRelease(XButtonReleasedEvent *);
  bool processKeyPress(XKeyPressedEvent *);
  bool processKeyRelease(XKeyReleasedEvent *);
  bool processMotionNotify(XMotionEvent *);
  bool processExpose(XExposeEvent *);
  bool processNoExpose(XNoExposeEvent *);
  bool processFocusIn(XFocusChangeEvent *);
  bool processFocusOut(XFocusChangeEvent *);
  bool processPropertyNotify(XPropertyEvent *);
  bool processVisibilityNotify(XVisibilityEvent *);
  bool processDestroyNotify(XDestroyWindowEvent *);
  bool processColormapNotify(XColormapEvent *);
  bool processClientMessage(XClientMessageEvent *);
  bool processMappingNotify(XMappingEvent *);
};

#endif
