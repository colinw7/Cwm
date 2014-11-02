#ifndef CWM_RESOURCE_DATA_H
#define CWM_RESOURCE_DATA_H

#include <CAlignType.h>

#define CwmNdecorations    "decorations"
#define CwmNfunctions      "functions"
#define CwmNfocusAutoRaise "focusAutoRaise"
#define CwmNcirculateSkip  "circulateSkip"
#define CwmNtoolbarSkip    "toolbarSkip"

#define CwmTint "int"

#define CwmResourceDataInst CwmResourceData::getInstance()

enum CwmIconDecoration {
  CWM_ICON_DECORATION_NONE         = 0,
  CWM_ICON_DECORATION_LABEL        = (1<<0),
  CWM_ICON_DECORATION_IMAGE        = (1<<1),
  CWM_ICON_DECORATION_CLIP_LABEL   = (1<<2),
  CWM_ICON_DECORATION_ACTIVE_LABEL = (1<<3),
};

enum CwmFeedbackType {
  CWM_FEEDBACK_NONE   = 0,
  CWM_FEEDBACK_MOVE   = (1<<0),
  CWM_FEEDBACK_RESIZE = (1<<1),
  CWM_FEEDBACK_ALL    = (CWM_FEEDBACK_MOVE | CWM_FEEDBACK_RESIZE),
};

class CwmResourceData {
 public:
  static CwmResourceData *getInstance();

 ~CwmResourceData();

  void save();

  std::string getBackground();
  Pixel       getBackground(CwmScreen &screen);
  std::string getForeground();
  Pixel       getForeground(CwmScreen &screen);
  CwmColor*   getColor(CwmScreen &screen);
  std::string getFont();
  CwmFont*    getFont(CwmScreen &screen);

  std::string getCursorBackground();
  std::string getCursorForeground();
  Pixel       getCursorBackground(CwmScreen &screen);
  Pixel       getCursorForeground(CwmScreen &screen);
  CwmColor*   getCursorColor(CwmScreen &screen);

  std::string       getIconBackground();
  Pixel             getIconBackground(CwmScreen &screen);
  std::string       getIconForeground();
  Pixel             getIconForeground(CwmScreen &screen);
  CwmColor*         getIconColor(CwmScreen &screen);
  std::string       getIconFont();
  CwmFont*          getIconFont(CwmScreen &screen);
  std::string       getIconImage();
  CwmIconDecoration getIconDecoration();
  int               getIconLabelWidth();

  bool getIconImageDecoration() {
    return getIconDecoration() & CWM_ICON_DECORATION_IMAGE;
  }
  bool getIconLabelDecoration() {
    return getIconDecoration() & CWM_ICON_DECORATION_LABEL;
  }
  bool getIconClipLabelDecoration() {
    return getIconDecoration() & CWM_ICON_DECORATION_CLIP_LABEL;
  }
  bool getIconActiveLabelDecoration() {
    return getIconDecoration() & CWM_ICON_DECORATION_ACTIVE_LABEL;
  }
  bool getIconImageBorder() {
    return false;
  }

  std::string getInfoBackground();
  Pixel       getInfoBackground(CwmScreen &screen);
  std::string getInfoForeground();
  Pixel       getInfoForeground(CwmScreen &screen);
  CwmColor*   getInfoColor(CwmScreen &screen);
  std::string getInfoFont();
  CwmFont*    getInfoFont(CwmScreen &screen);

  std::string            getFocusBackground();
  Pixel                  getFocusBackground(CwmScreen &screen);
  std::string            getFocusForeground();
  Pixel                  getFocusForeground(CwmScreen &screen);
  CwmColor              *getFocusColor(CwmScreen &screen);
  std::string            getFocusFont();
  CwmFont               *getFocusFont(CwmScreen &screen);
  bool                   getClickToFocus();
  bool                   getFocusAutoRaise();
  bool                   getFocusGrabKeys();
  CwmFocusHighlightType  getFocusHighlightType();
  bool                   getNoIconFocus();

  std::string getMenuBackground();
  Pixel       getMenuBackground(CwmScreen &screen);
  std::string getMenuForeground();
  Pixel       getMenuForeground(CwmScreen &screen);
  std::string getMenuTitleBackground();
  Pixel       getMenuTitleBackground(CwmScreen &screen);
  std::string getMenuTitleForeground();
  Pixel       getMenuTitleForeground(CwmScreen &screen);
  std::string getMenuFont();
  CwmFont*    getMenuFont(CwmScreen &screen);
  std::string getMenuTitleFont();
  CwmFont*    getMenuTitleFont(CwmScreen &screen);

  CHAlignType          getTitleAlign();
  CwmTitlePatternType  getTitlePatternType();
  std::string          getTitleForeground();
  std::string          getTitleBackground();
  CwmColor            *getTitleColor(CwmScreen &screen);
  std::string          getTitleFocusForeground();
  std::string          getTitleFocusBackground();
  CwmColor            *getTitleFocusColor(CwmScreen &screen);
  std::string          getTitleGradientForeground();
  std::string          getTitleGradientBackground();
  CwmColor            *getTitleGradientColor(CwmScreen &screen);
  std::string          getTitleGradientFocusForeground();
  std::string          getTitleGradientFocusBackground();
  CwmColor            *getTitleGradientFocusColor(CwmScreen &screen);
  int                  getAutoRaiseDelay();
  bool                 getCirculateSkipIcons();
  bool                 getClientAutoPlace();
  int                  getDoubleClickTime();
  bool                 getEnableWarp();
  bool                 getMoveOpaque();
  int                  getMoveThreshold();
  bool                 getInteractivePlacement();
  bool                 getPositionIsFrame();
  bool                 getPositionOnScreen();
  bool                 getWindowPressRaise();

  CwmFeedbackType getFeedbackType();

  bool getFeedbackMove() {
    return getFeedbackType() & CWM_FEEDBACK_MOVE;
  }

  bool getFeedbackResize() {
    return getFeedbackType() & CWM_FEEDBACK_RESIZE;
  }

  void getFeedbackGeometry(CHAlignType *halign, CVAlignType *valign,
                           int *x_offset, int *y_offset);

  std::string getFeedbackBackground();
  Pixel       getFeedbackBackground(CwmScreen &screen);
  std::string getFeedbackForeground();
  Pixel       getFeedbackForeground(CwmScreen &screen);
  std::string getFeedbackFont();
  CwmFont*    getFeedbackFont(CwmScreen &screen);

  std::string getToolBarBackground();
  Pixel       getToolBarBackground(CwmScreen &screen);
  std::string getToolBarForeground();
  Pixel       getToolBarForeground(CwmScreen &screen);
  std::string getToolBarFont();
  CwmFont*    getToolBarFont(CwmScreen &screen);
  bool        getToolBarShowClock();
  bool        getToolBarAddIcons();
  bool        getToolBarAddWindows();
  int         getToolBarHeight();
  int         getToolBarIconWidth();

  std::string getToolBarClockBackground();
  Pixel       getToolBarClockBackground(CwmScreen &screen);
  std::string getToolBarClockForeground();
  Pixel       getToolBarClockForeground(CwmScreen &screen);
  CwmColor*   getToolBarClockColor(CwmScreen &screen);
  std::string getToolBarClockFont();
  CwmFont*    getToolBarClockFont(CwmScreen &screen);
  std::string getToolBarClockTimeFormat();

  std::string getToolBarIconBackground();
  Pixel       getToolBarIconBackground(CwmScreen &screen);
  std::string getToolBarIconForeground();
  Pixel       getToolBarIconForeground(CwmScreen &screen);
  std::string getToolBarIconFont();
  CwmFont*    getToolBarIconFont(CwmScreen &screen);

  std::string getToolBarIconFocusBackground();
  Pixel       getToolBarIconFocusBackground(CwmScreen &screen);
  std::string getToolBarIconFocusForeground();
  Pixel       getToolBarIconFocusForeground(CwmScreen &screen);
  std::string getToolBarIconFocusFont();
  CwmFont*    getToolBarIconFocusFont(CwmScreen &screen);

  void loadStartupCommands(CwmScreen &screen);

 private:
  CwmResourceData();

  CwmFeedbackType   parseShowFeedback(const std::string &str);
  CwmIconDecoration parseIconDecoration(const std::string &str);

  void parseFeedbackGeometry(const std::string &str, CHAlignType *halign,
                             CVAlignType *valign, int *x_offset, int *y_offset);

  CwmFocusHighlightType parseFocusHighlightType(const std::string &str);

  std::string getSectionResource(const std::string &path, const std::string &section,
                                 const std::string &fallback);
  int         getSectionResource(const std::string &path, const std::string &section,
                                 int fallback);
  std::string getResource(const std::string &path, const std::string &fallback);

  void        loadMenus();
  CwmMenuDef *loadMenu(const std::string &menuPath, const std::string &menuName);
  void        loadDeskIcons();
  void        loadDeskIcon(const std::string &path, const std::string &section);
  void        loadToolBarIcons();
  void        loadToolBarIcon(const std::string &path, const std::string &section);
  void        loadDesks();
  void        loadGroups();
  void        loadGroupEvents(const std::string &pattern, const std::string &path);
  void        loadRoot();
  void        loadRootEvents();
  void        loadStartupCommand(CwmScreen &screen, const std::string &path,
                                 const std::string &section);
  int         stringToDecorations(const std::string &str);
  int         stringToFunctions(const std::string &str);
  int         stringToArea(const std::string &str);
};

#endif
