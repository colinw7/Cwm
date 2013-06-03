// _WIN_STATE flags

#define WIN_STATE_STICKY          (1<<0) /*everyone knows sticky*/
#define WIN_STATE_MINIMIZED       (1<<1) /*Reserved - definition is unclear*/
#define WIN_STATE_MAXIMIZED_VERT  (1<<2) /*window in maximized V state*/
#define WIN_STATE_MAXIMIZED_HORIZ (1<<3) /*window in maximized H state*/
#define WIN_STATE_HIDDEN          (1<<4) /*not on taskbar but window visible*/
#define WIN_STATE_SHADED          (1<<5) /*shaded (MacOS / Afterstep style)*/
#define WIN_STATE_HID_WORKSPACE   (1<<6) /*not on current desktop*/
#define WIN_STATE_HID_TRANSIENT   (1<<7) /*owner of transient is hidden*/
#define WIN_STATE_FIXED_POSITION  (1<<8) /*window is fixed in position even*/
#define WIN_STATE_ARRANGE_IGNORE  (1<<9) /*ignore for auto arranging*/

// _WIN_HINTS flags

#define WIN_HINTS_SKIP_FOCUS      (1<<0) /*"alt-tab" skips this win*/
#define WIN_HINTS_SKIP_WINLIST    (1<<1) /*do not show in window list*/
#define WIN_HINTS_SKIP_TASKBAR    (1<<2) /*do not show on taskbar*/
#define WIN_HINTS_GROUP_TRANSIENT (1<<3) /*Reserved - definition is unclear*/
#define WIN_HINTS_FOCUS_ON_CLICK  (1<<4) /*app only accepts focus if clicked*/

// _WIN_LAYER values

#define WIN_LAYER_DESKTOP    0
#define WIN_LAYER_BELOW      2
#define WIN_LAYER_NORMAL     4
#define WIN_LAYER_ONTOP      6
#define WIN_LAYER_DOCK       8
#define WIN_LAYER_ABOVE_DOCK 10
#define WIN_LAYER_MENU       12

#define CwmGnomeInst CwmGnome::getInstance()

class CwmGnome {
 public:
  static CwmGnome *getInstance();

  void init(CwmScreen &screen);

  void setListOfManagedClients(CwmScreen &screen);
  void setNumDesktops(CwmScreen &screen);
  void setCurrentDesktop(CwmScreen &screen);
  void setDesktopNames(CwmScreen &screen);

  void processRootClientMessage(XClientMessageEvent *event);

  void setState(CwmWMWindow &window);
  void setDesktop(CwmWMWindow &window);

 private:
  void createGnomeWindow(CwmScreen &screen);
  void createSupportedProperties(CwmScreen &screen);
};
