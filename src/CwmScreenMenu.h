class CwmScreenMenu {
 public:
  static void moveProc(CwmScreen &screen, CwmData data);
  static void resizeProc(CwmScreen &screen, CwmData data);
  static void iconiseProc(CwmScreen &screen, CwmData data);
  static void maximizeProc(CwmScreen &screen, CwmData data);
  static void minimizeProc(CwmScreen &screen, CwmData data);
  static void restoreProc(CwmScreen &screen, CwmData data);
  static void raiseProc(CwmScreen &screen, CwmData data);
  static void lowerProc(CwmScreen &screen, CwmData data);
  static void circulateUpProc(CwmScreen &screen, CwmData data);
  static void circulateDownProc(CwmScreen &screen, CwmData data);
  static void focusNextProc(CwmScreen &screen, CwmData data);
  static void focusPrevProc(CwmScreen &screen, CwmData data);
  static void selectNextProc(CwmScreen &screen, CwmData data);
  static void selectPrevProc(CwmScreen &screen, CwmData data);
  static void rollUpProc(CwmScreen &screen, CwmData data);
  static void rollDownProc(CwmScreen &screen, CwmData data);
  static void closeProc(CwmScreen &screen, CwmData data);
  static void processNamedMenuProc(CwmScreen &screen, CwmData data);
  static void runCommandProc(CwmScreen &screen, CwmData data);
  static void runXCommandProc(CwmScreen &screen, CwmData data);
  static void setRootImageProc(CwmScreen &screen, CwmData data);
  static void toggleDisplayedProc(CwmScreen &screen, CwmData data);
  static void refreshCwmProc(CwmScreen &screen, CwmData data);
  static void restartCwmProc(CwmScreen &screen, CwmData data);
  static void quitCwmProc(CwmScreen &screen, CwmData data);
  static void changeDeskProc(CwmScreen &screen, CwmData data);
};
