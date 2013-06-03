#ifndef CWM_FUNCTION_DEF_H
#define CWM_FUNCTION_DEF_H

class CwmScreen;

class CwmFunctionDef {
 private:
  CwmScreen     *screen_;
  CwmRootProc    root_proc_;
  CwmWMWindow   *window_;
  CwmWindowProc  window_proc_;
  CwmDeskIcon   *icon_;
  CwmIconProc    icon_proc_;
  CwmData        data_;

 public:
  CwmFunctionDef(const std::string &name);
 ~CwmFunctionDef();

  void process();

  void processWindow(CwmWMWindow *window, CwmData data);
  void processRoot(CwmScreen &screen, CwmData data);

  void setData(CwmData data);

  void setObjects(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon);

  static void processProc(CwmData data);
};

#endif
