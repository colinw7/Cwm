#ifndef CWM_HINTS_H
#define CWM_HINTS_H

#include <std_Xt.h>

#define CwmCustomHintMgrInst CwmCustomHintMgr::getInstance()

class CwmCustomHintMgr {
 public:
  static CwmCustomHintMgr *getInstance();

 ~CwmCustomHintMgr();

  void addCustomHintValue(const std::string &pattern, const std::string &name,
                          const char *type, CwmData value);
  bool getCustomHintValue(const std::string &window, const std::string &name,
                          CwmData value);

 private:
  CwmCustomHintMgr();

  CwmCustomHint *getCustomHint(const std::string &pattern);
  CwmCustomHint *addCustomHint(const std::string &pattern);

 private:
  typedef std::vector<CwmCustomHint *> CustomHintList;

  CustomHintList custom_hints_;
};

class CwmHints {
 public:
  CwmHints(CwmWMWindow *window);
 ~CwmHints();

  std::string getName() const { return name_; }

  std::string getIconName() const { return icon_name_; }

  int    getX() const { return x_; }
  int    getY() const { return y_; }
  int    getWidthInc() const { return width_inc_; }
  int    getHeightInc() const { return height_inc_; }
  int    getMinWidth() const { return min_width_; }
  int    getMinHeight() const { return min_height_; }
  int    getMaxWidth() const { return max_width_; }
  int    getMaxHeight() const { return max_height_; }
  int    getBaseWidth() const { return base_width_; }
  int    getBaseHeight() const { return base_height_; }
  double getMinAspect() const { return min_aspect_; }
  double getMaxAspect() const { return max_aspect_; }
  int    getWinGravity() const { return win_gravity_; }

  bool   getInput() const { return input_; }
  int    getInitialState() const { return initial_state_; }
  Window getIconWindow() const { return icon_window_; }
  Pixmap getIconPixmap() const { return icon_pixmap_; }
  Pixmap getIconMask() const { return icon_mask_; }
  int    getIconDepth() const { return icon_depth_; }
  int    getIconX() const { return icon_x_; }
  int    getIconY() const { return icon_y_; }
  Window getWindowGroup() const { return window_group_; }

  Window getTransientFor() const { return transient_for_; }

  std::string getResName() const { return res_name_; }
  std::string getResClass() const { return res_class_; }

  std::string getClientMachine() const { return client_machine_; }

  StringVectorT getCommandArgV() const { return command_argv_; }

  Window getColormapWindow() const {
    if (num_cmap_windows_ > 0)
      return cmap_windows_[0];
    else
      return None;
  }

  bool getTakeFocus() const { return take_focus_; }
  bool getSaveYourself() const { return save_yourself_; }
  bool getDeleteWindow() const { return delete_window_; }

  int getMwmFunctions() const { return mwm_functions_; }
  int getMwmDecorations() const {  return mwm_decorations_; }
  int getMwmInputMode() const {  return mwm_input_mode_; }
  int getMwmStatus() const {  return mwm_status_; }

  bool getMoveFunction() {
    return (mwm_functions_ & MWM_FUNC_MOVE);
  }

  bool getResizeFunction() {
    return (mwm_functions_ & MWM_FUNC_RESIZE);
  }

  bool getIconiseFunction() {
    return (mwm_functions_ & MWM_FUNC_MINIMIZE);
  }

  bool getMaximizeFunction() {
    return (mwm_functions_ & MWM_FUNC_MAXIMIZE);
  }

  bool getMinimizeFunction() {
    return (mwm_functions_ & MWM_FUNC_MINIMIZE);
  }

  bool getRestoreFunction() {
    return (mwm_functions_ & MWM_FUNC_RESTORE);
  }

  bool getCloseFunction() {
    return (mwm_functions_ & MWM_FUNC_CLOSE);
  }

  bool getTitleDecoration() {
    return (mwm_decorations_ & MWM_DECOR_TITLE);
  }

  bool getBorderDecoration() {
    return (mwm_decorations_ & MWM_DECOR_BORDER);
  }

  bool getResizeHandleDecoration() {
    return (mwm_decorations_ & MWM_DECOR_RESIZEH);
  }

  bool getMaximizeDecoration() {
    return (mwm_decorations_ & MWM_DECOR_MAXIMIZE);
  }

  bool getMinimizeDecoration() {
    return (mwm_decorations_ & MWM_DECOR_MINIMIZE);
  }

  bool getMenuDecoration() {
    return (mwm_decorations_ & MWM_DECOR_MENU);
  }

  bool getCloseDecoration() {
    return (mwm_functions_ & MWM_FUNC_CLOSE);
  }

  void readWindowHints();
  void readWMName();
  void readWMIconName();
  void readWMSizeHints();
  void readWMHints();
  void readTransientHint();
  void readClassHint();
  void readSessionHints();
  void readWMCMapWindows();
  void readWMProtocols();
  void readMwmHints();

 private:
  void init();
  void print();

 private:
  CwmWMWindow   *window_;
  Window         user_xwin_;

  // WM Name

  std::string    name_;

  // WM Icon Name

  std::string    icon_name_;

  // WM Size Hints

  int            x_;
  int            y_;
  int            width_inc_;
  int            height_inc_;
  int            min_width_;
  int            min_height_;
  int            max_width_;
  int            max_height_;
  int            base_width_;
  int            base_height_;
  double         min_aspect_;
  double         max_aspect_;
  int            win_gravity_;

  // WM Hinta

  bool           input_;
  int            initial_state_;
  Window         icon_window_;
  Pixmap         icon_pixmap_;
  Pixmap         icon_mask_;
  int            icon_depth_;
  int            icon_x_;
  int            icon_y_;
  Window         window_group_;

  // Transient Hint

  Window         transient_for_;

  // Class Hint

  std::string    res_name_;
  std::string    res_class_;

  // Session Hint

  std::string    client_machine_;
  StringVectorT  command_argv_;

  // CMap Windows

  Window        *cmap_windows_;
  int            num_cmap_windows_;

  // WM Protocols

  bool           take_focus_;
  bool           save_yourself_;
  bool           delete_window_;

  // Mwm Hints

  int            mwm_functions_;
  int            mwm_decorations_;
  int            mwm_input_mode_;
  int            mwm_status_;
};

#endif
