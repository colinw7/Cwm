class CwmWindowMenu {
 private:
  CwmWMWindow *window;
  CwmMenuDef  *menu_def;

 public:
  CwmWindowMenu(CwmWMWindow *window);
 ~CwmWindowMenu();

  void process();
  void processAtPosition(CwmWindow *window, int x, int y);

  static void moveProc(CwmWMWindow *window, CwmData data);
  static void resizeProc(CwmWMWindow *window, CwmData data);
  static void resizeToSizeProc(CwmWMWindow *window, CwmData data);
  static void iconiseProc(CwmWMWindow *window, CwmData data);
  static void maximizeProc(CwmWMWindow *window, CwmData data);
  static void minimizeProc(CwmWMWindow *window, CwmData data);
  static void restoreProc(CwmWMWindow *window, CwmData data);
  static void raiseProc(CwmWMWindow *window, CwmData data);
  static void lowerProc(CwmWMWindow *window, CwmData data);
  static void rollUpProc(CwmWMWindow *window, CwmData data);
  static void rollDownProc(CwmWMWindow *window, CwmData data);
  static void circulateUpProc(CwmWMWindow *window, CwmData data);
  static void circulateDownProc(CwmWMWindow *window, CwmData data);
  static void focusNextProc(CwmWMWindow *window, CwmData data);
  static void focusPrevProc(CwmWMWindow *window, CwmData data);
  static void closeProc(CwmWMWindow *window, CwmData data);
  static void processNamedMenuProc(CwmWMWindow *window, CwmData data);
  static void pasteProc(CwmWMWindow *window, CwmData data);
  static void runCommandProc(CwmWMWindow *window, CwmData data);
  static void runXCommandProc(CwmWMWindow *window, CwmData data);
};
