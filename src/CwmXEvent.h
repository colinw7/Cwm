#ifndef CWM_X_EVENT_H
#define CWM_X_EVENT_H

#define CwmXEventInst CwmXEvent::getInstance()

class CwmXEvent {
 public:
  static CwmXEvent *getInstance();

 ~CwmXEvent();

  XEvent      *parseEvent(const std::string &def);
  std::string  parseEventToString(const std::string &def);
  bool         isKeyPressEvent(XKeyPressedEvent *, int);
  bool         isEscapeKeyEvent(XKeyPressedEvent *);

 private:
  CwmXEvent();
};

#endif
