#ifndef CwmAdvHints_H
#define CwmAdvHints_H

#include <std_Xt.h>

#define CwmAdvHintsInst CwmAdvHints::getInstance()

class CwmScreen;
class CwmWMWindow;

class CwmAdvHints {
 public:
  static CwmAdvHints *getInstance();

  void init(CwmScreen &screen);

  void setWMName();

  void setClientList(CwmScreen &screen);
  void setStackedClientList(CwmScreen &screen);

  void setNumDesktops(CwmScreen &screen);
  void setCurrentDesktop(CwmScreen &screen);

  void setDesktopGeometry(CwmScreen &screen);
  void setDesktopViewport(CwmScreen &screen);
  void setWorkArea(CwmScreen &screen);

  void setDesktopNames(CwmScreen &screen);

  bool processRootClientMessage(XClientMessageEvent *event);

  void setActiveWindow(CwmScreen &screen, CwmWMWindow *window);

  void setState(CwmWMWindow &window);
  void setDesktop(CwmWMWindow &window);

 private:
  void createAdvHintsWindow(CwmScreen &screen);
  void createSupportedProperties(CwmScreen &screen);

 private:
  Window xwin_ { None };
};

#endif
