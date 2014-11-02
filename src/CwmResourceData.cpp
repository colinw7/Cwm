#include <CwmI.h>

CwmResourceData *
CwmResourceData::
getInstance()
{
  static CwmResourceData *instance;

  if (! instance)
    instance = new CwmResourceData();

  return instance;
}

CwmResourceData::
CwmResourceData()
{
  loadMenus();

  loadDeskIcons();

  loadToolBarIcons();

  loadDesks();

  loadGroups();

  loadRoot();
}

CwmResourceData::
~CwmResourceData()
{
}

void
CwmResourceData::
save()
{
  CwmConfigInst->save();
}

CwmFeedbackType
CwmResourceData::
parseShowFeedback(const std::string &str)
{
  int feedback = CWM_FEEDBACK_NONE;

  CStrWords words = CStrUtil::toWords(str, 0);

  for (int i = 0; i < words.size(); i++) {
    std::string word = words[i].getWord();

    if (i == 0 && words[i].getWord()[0] == '-')
      feedback = CWM_FEEDBACK_ALL;
    else
      feedback = CWM_FEEDBACK_NONE;

    int add;

    if (word[0] == '-') {
      add = false;

      word = word.substr(1);
    }
    else
      add = true;

    int flag;

    if      (CStrUtil::casecmp(word, "all") == 0)
      flag = CWM_FEEDBACK_ALL;
    else if (CStrUtil::casecmp(word, "none") == 0)
      flag = CWM_FEEDBACK_NONE;
    else if (CStrUtil::casecmp(word, "move") == 0)
      flag = CWM_FEEDBACK_MOVE;
    else if (CStrUtil::casecmp(word, "resize") == 0)
      flag = CWM_FEEDBACK_RESIZE;
    else
      continue;

    if (add)
      feedback |= flag;
    else
      feedback &= ~flag;
  }

  return (CwmFeedbackType) feedback;
}

CwmIconDecoration
CwmResourceData::
parseIconDecoration(const std::string &str)
{
  int decoration = CWM_ICON_DECORATION_NONE;

  CStrWords words = CStrUtil::toWords(str, 0);

  for (int i = 0; i < words.size(); i++) {
    if      (CStrUtil::casecmp(words[i].getWord(), "label") == 0)
      decoration |= CWM_ICON_DECORATION_LABEL;
    else if (CStrUtil::casecmp(words[i].getWord(), "image") == 0)
      decoration |= CWM_ICON_DECORATION_IMAGE;
    else if (CStrUtil::casecmp(words[i].getWord(), "clip") == 0)
      decoration |= CWM_ICON_DECORATION_CLIP_LABEL;
    else if (CStrUtil::casecmp(words[i].getWord(), "activelabel") == 0)
      decoration |= CWM_ICON_DECORATION_ACTIVE_LABEL;
    else
      continue;
  }

  if (! (decoration & CWM_ICON_DECORATION_LABEL) &&
      ! (decoration & CWM_ICON_DECORATION_IMAGE))
    decoration |= CWM_ICON_DECORATION_LABEL;

  if (! (decoration & CWM_ICON_DECORATION_IMAGE))
    decoration &= ~CWM_ICON_DECORATION_CLIP_LABEL;

  return (CwmIconDecoration) decoration;
}

void
CwmResourceData::
parseFeedbackGeometry(const std::string &str, CHAlignType *halign, CVAlignType *valign,
                      int *x_offset, int *y_offset)
{
  *halign   = CHALIGN_TYPE_CENTER;
  *valign   = CVALIGN_TYPE_CENTER;
  *x_offset = 0;
  *y_offset = 0;

  CStrWords words = CStrUtil::toTokens(str, " ,");

  if (words.size() != 2)
    return;

  std::string word = words.getWord(0).getWord();

  if      (CStrUtil::casecmp(word, "left"  ) == 0)
    *halign = CHALIGN_TYPE_LEFT;
  else if (CStrUtil::casecmp(word, "right" ) == 0)
    *halign = CHALIGN_TYPE_RIGHT;
  else if (CStrUtil::casecmp(word, "center") == 0 ||
           CStrUtil::casecmp(word, "centre") == 0)
    *halign = CHALIGN_TYPE_CENTER;
  else {
    if (! CStrUtil::isInteger(word))
      return;

    *x_offset = CStrUtil::toInteger(word);
  }

  word = words.getWord(1).getWord();

  if      (CStrUtil::casecmp(word, "top"   ) == 0)
    *valign = CVALIGN_TYPE_TOP;
  else if (CStrUtil::casecmp(word, "bottom") == 0)
    *valign = CVALIGN_TYPE_BOTTOM;
  else if (CStrUtil::casecmp(word, "center") == 0 ||
           CStrUtil::casecmp(word, "centre") == 0)
    *valign = CVALIGN_TYPE_CENTER;
  else {
    if (! CStrUtil::isInteger(word))
      return;

    *y_offset = CStrUtil::toInteger(word);
  }
}

CwmFocusHighlightType
CwmResourceData::
parseFocusHighlightType(const std::string &str)
{
  CwmFocusHighlightType type = CWM_FOCUS_HIGHLIGHT_ALL;

  if      (CStrUtil::casecmp(str, "none") == 0)
    type = CWM_FOCUS_HIGHLIGHT_NONE;
  else if (CStrUtil::casecmp(str, "title") == 0)
    type = CWM_FOCUS_HIGHLIGHT_TITLE;
  else if (CStrUtil::casecmp(str, "all") == 0)
    type = CWM_FOCUS_HIGHLIGHT_ALL;
  else
    type = CWM_FOCUS_HIGHLIGHT_NONE;

  return type;
}

std::string
CwmResourceData::
getResource(const std::string &path, const std::string &fallback)
{
  std::string value = CwmConfigInst->getValue(path, fallback);

  return value;
}

std::string
CwmResourceData::
getSectionResource(const std::string &path, const std::string &section, const std::string &fallback)
{
  std::string value = CwmConfigInst->getSectionValue(path, section, fallback);

  return value;
}

int
CwmResourceData::
getSectionResource(const std::string &path, const std::string &section, int fallback)
{
  int value = CwmConfigInst->getSectionValue(path, section, fallback);

  return value;
}

std::string
CwmResourceData::
getBackground()
{
  return getResource("window/background", "white");
}

Pixel
CwmResourceData::
getBackground(CwmScreen &screen)
{
  return screen.getPixel(getBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getForeground()
{
  return getResource("window/foreground", "black");
}

Pixel
CwmResourceData::
getForeground(CwmScreen &screen)
{
  return screen.getPixel(getForeground(), screen.getBlackPixel());
}

CwmColor *
CwmResourceData::
getColor(CwmScreen &screen)
{
  return screen.getColor(getForeground(screen), getBackground(screen));
}

std::string
CwmResourceData::
getFont()
{
  std::string default_font_name = "8x13";

  return getResource("window/font", default_font_name);
}

CwmFont *
CwmResourceData::
getFont(CwmScreen &screen)
{
  return screen.getFont(getFont());
}

std::string
CwmResourceData::
getCursorBackground()
{
  return getResource("cursor/background", "black");
}

Pixel
CwmResourceData::
getCursorBackground(CwmScreen &screen)
{
  return screen.getPixel(getCursorBackground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getCursorForeground()
{
  return getResource("cursor/foreground", "white");
}

Pixel
CwmResourceData::
getCursorForeground(CwmScreen &screen)
{
  return screen.getPixel(getCursorForeground(), screen.getWhitePixel());
}

CwmColor *
CwmResourceData::
getCursorColor(CwmScreen &screen)
{
  return screen.getColor(getCursorForeground(screen), getCursorBackground(screen));
}

std::string
CwmResourceData::
getIconBackground()
{
  return getResource("icon/background", getBackground());
}

Pixel
CwmResourceData::
getIconBackground(CwmScreen &screen)
{
  return screen.getPixel(getIconBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getIconForeground()
{
  return getResource("icon/foreground", getForeground());
}

Pixel
CwmResourceData::
getIconForeground(CwmScreen &screen)
{
  return screen.getPixel(getIconForeground(), screen.getBlackPixel());
}

CwmColor *
CwmResourceData::
getIconColor(CwmScreen &screen)
{
  return screen.getColor(getIconForeground(screen), getIconBackground(screen));
}

std::string
CwmResourceData::
getIconFont()
{
  return getResource("icon/font", getFont());
}

CwmFont *
CwmResourceData::
getIconFont(CwmScreen &screen)
{
  return screen.getFont(getIconFont());
}

std::string
CwmResourceData::
getIconImage()
{
  return getResource("icon/image", "");
}

CwmIconDecoration
CwmResourceData::
getIconDecoration()
{
  std::string str = getResource("icon/decoration", "label image");

  return parseIconDecoration(str);
}

int
CwmResourceData::
getIconLabelWidth()
{
  std::string str = getResource("icon/labelWidth", "9999");

  if (CStrUtil::isInteger(str))
    return CStrUtil::toInteger(str);

  return 9999;
}

std::string
CwmResourceData::
getInfoBackground()
{
  return getResource("info/background", getBackground());
}

Pixel
CwmResourceData::
getInfoBackground(CwmScreen &screen)
{
  return screen.getPixel(getInfoBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getInfoForeground()
{
  return getResource("info/foreground", getBackground());
}

Pixel
CwmResourceData::
getInfoForeground(CwmScreen &screen)
{
  return screen.getPixel(getInfoForeground(), screen.getBlackPixel());
}

CwmColor *
CwmResourceData::
getInfoColor(CwmScreen &screen)
{
  return screen.getColor(getInfoForeground(screen), getInfoBackground(screen));
}

std::string
CwmResourceData::
getInfoFont()
{
  return getResource("info/font", getFont());
}

CwmFont *
CwmResourceData::
getInfoFont(CwmScreen &screen)
{
  return screen.getFont(getInfoFont());
}

std::string
CwmResourceData::
getFocusBackground()
{
  return getResource("window/focusBackground", getBackground());
}

Pixel
CwmResourceData::
getFocusBackground(CwmScreen &screen)
{
  return screen.getPixel(getFocusBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getFocusForeground()
{
  return getResource("window/focusForeground", getForeground());
}

Pixel
CwmResourceData::
getFocusForeground(CwmScreen &screen)
{
  return screen.getPixel(getFocusForeground(), screen.getBlackPixel());
}

CwmColor *
CwmResourceData::
getFocusColor(CwmScreen &screen)
{
  return screen.getColor(getFocusForeground(screen), getFocusBackground(screen));
}

std::string
CwmResourceData::
getFocusFont()
{
  return getResource("window/focusFont", getFont());
}

CwmFont *
CwmResourceData::
getFocusFont(CwmScreen &screen)
{
  return screen.getFont(getFocusFont());
}

bool
CwmResourceData::
getClickToFocus()
{
  std::string str = getResource("window/clickToFocus", "No");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

bool
CwmResourceData::
getFocusAutoRaise()
{
  std::string str = getResource("window/focusAutoRaise", "No" );

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

bool
CwmResourceData::
getFocusGrabKeys()
{
  std::string str = getResource("window/focusGrabKeys", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

CwmFocusHighlightType
CwmResourceData::
getFocusHighlightType()
{
  std::string str = getResource("window/focusHighlightType", "all");

  return parseFocusHighlightType(str);
}

bool
CwmResourceData::
getNoIconFocus()
{
  std::string str = getResource("window/focusNoIcon", "No" );

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

std::string
CwmResourceData::
getMenuBackground()
{
  return getResource("menu/background", getBackground());
}

Pixel
CwmResourceData::
getMenuBackground(CwmScreen &screen)
{
  return screen.getPixel(getMenuBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getMenuForeground()
{
  return getResource("menu/foreground", getForeground());
}

Pixel
CwmResourceData::
getMenuForeground(CwmScreen &screen)
{
  return screen.getPixel(getMenuForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getMenuTitleBackground()
{
  return getSectionResource("menu/background", "title", getMenuBackground());
}

Pixel
CwmResourceData::
getMenuTitleBackground(CwmScreen &screen)
{
  return screen.getPixel(getMenuTitleBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getMenuTitleForeground()
{
  return getSectionResource("menu/foreground", "title", getMenuForeground());
}

Pixel
CwmResourceData::
getMenuTitleForeground(CwmScreen &screen)
{
  return screen.getPixel(getMenuTitleForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getMenuFont()
{
  return getResource("menu/font", getFont());
}

CwmFont *
CwmResourceData::
getMenuFont(CwmScreen &screen)
{
  return screen.getFont(getMenuFont());
}

std::string
CwmResourceData::
getMenuTitleFont()
{
  return getSectionResource("menu/font", "title", getMenuFont());
}

CwmFont *
CwmResourceData::
getMenuTitleFont(CwmScreen &screen)
{
  return screen.getFont(getMenuTitleFont());
}

CHAlignType
CwmResourceData::
getTitleAlign()
{
  std::string str = getSectionResource("window/align", "title", "center");

  if      (CStrUtil::casecmp(str, "left" ) == 0)
    return CHALIGN_TYPE_LEFT;
  else if (CStrUtil::casecmp(str, "right") == 0)
    return CHALIGN_TYPE_RIGHT;
  else
    return CHALIGN_TYPE_CENTER;
}

CwmTitlePatternType
CwmResourceData::
getTitlePatternType()
{
  std::string str = getSectionResource("window/titlePatternType", "title", "solid");

  if      (CStrUtil::casecmp(str, "solid"   ) == 0)
    return CWM_TITLE_PATTERN_SOLID;
  else if (CStrUtil::casecmp(str, "gradient") == 0)
    return CWM_TITLE_PATTERN_GRADIENT;
  else
    return CWM_TITLE_PATTERN_SOLID;
}

std::string
CwmResourceData::
getTitleForeground()
{
  return getSectionResource("window/titleForeground", "title", "#666666");
}

std::string
CwmResourceData::
getTitleBackground()
{
  return getSectionResource("window/titleBackground", "title", "#666666");
}

CwmColor *
CwmResourceData::
getTitleColor(CwmScreen &screen)
{
  Pixel fg = screen.getPixel(getTitleForeground(), screen.getBlackPixel());
  Pixel bg = screen.getPixel(getTitleBackground(), screen.getWhitePixel());

  return screen.getColor(fg, bg);
}

std::string
CwmResourceData::
getTitleFocusForeground()
{
  return getSectionResource("window/titleFocusForeground", "title", "#4c7fb2");
}

std::string
CwmResourceData::
getTitleFocusBackground()
{
  return getSectionResource("window/titleFocusBackground", "title", "#4c7fb2");
}

CwmColor *
CwmResourceData::
getTitleFocusColor(CwmScreen &screen)
{
  Pixel fg = screen.getPixel(getTitleFocusForeground(), screen.getBlackPixel());
  Pixel bg = screen.getPixel(getTitleFocusBackground(), screen.getWhitePixel());

  return screen.getColor(fg, bg);
}

std::string
CwmResourceData::
getTitleGradientForeground()
{
  return getSectionResource("window/gradientForeground", "title", "#999999");
}

std::string
CwmResourceData::
getTitleGradientBackground()
{
  return getSectionResource("window/gradientBackground", "title", "#7f7f7f");
}

CwmColor *
CwmResourceData::
getTitleGradientColor(CwmScreen &screen)
{
  Pixel fg = screen.getPixel(getTitleGradientForeground(), screen.getBlackPixel());
  Pixel bg = screen.getPixel(getTitleGradientBackground(), screen.getWhitePixel());

  return screen.getColor(fg, bg);
}

std::string
CwmResourceData::
getTitleGradientFocusForeground()
{
  return getSectionResource("window/gradientFocusForeground", "title", "#4c7fb2");
}

std::string
CwmResourceData::
getTitleGradientFocusBackground()
{
  return getSectionResource("window/gradientFocusBackground", "title", "#000099");
}

CwmColor *
CwmResourceData::
getTitleGradientFocusColor(CwmScreen &screen)
{
  Pixel fg = screen.getPixel(getTitleGradientFocusForeground(), screen.getBlackPixel());
  Pixel bg = screen.getPixel(getTitleGradientFocusBackground(), screen.getWhitePixel());

  return screen.getColor(fg, bg);
}

int
CwmResourceData::
getAutoRaiseDelay()
{
  std::string str = getResource("window/autoRaiseDelay", "500");

  if (CStrUtil::isInteger(str))
    return CStrUtil::toInteger(str);

  return 500;
}

bool
CwmResourceData::
getCirculateSkipIcons()
{
  std::string str = getResource("circulateSkipIcons", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getClientAutoPlace()
{
  std::string str = getResource("clientAutoPlace", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

int
CwmResourceData::
getDoubleClickTime()
{
  std::string str = getResource("doubleClickTime", "500");

  int doubleClickTime = 0;

  if (CStrUtil::isInteger(str))
    doubleClickTime = CStrUtil::toInteger(str);
  else
    doubleClickTime = 0;

  if (doubleClickTime == 0)
    doubleClickTime = CwmMachineInst->getMulticlickTime();

  return doubleClickTime;
}

bool
CwmResourceData::
getEnableWarp()
{
  std::string str = getResource("enableWarp", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getMoveOpaque()
{
  std::string str = getResource("window/moveOpaque", "No");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

int
CwmResourceData::
getMoveThreshold()
{
  std::string str = getResource("window/moveThreshold", "4");

  if (CStrUtil::isInteger(str))
    return CStrUtil::toInteger(str);

  return 4;
}

bool
CwmResourceData::
getInteractivePlacement()
{
  std::string str = getResource("window/interactivePlacement", "No");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

bool
CwmResourceData::
getPositionIsFrame()
{
  std::string str = getResource("window/positionIsFrame", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getPositionOnScreen()
{
  std::string str = getResource("window/positionOnScreen", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getWindowPressRaise()
{
  std::string str = getResource("window/pressRaise", "false");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return false;
}

CwmFeedbackType
CwmResourceData::
getFeedbackType()
{
  std::string str = getSectionResource("window/type", "feedback", "all");

  return parseShowFeedback(str);
}

std::string
CwmResourceData::
getFeedbackBackground()
{
  return getSectionResource("window/background", "feedback", getBackground());
}

Pixel
CwmResourceData::
getFeedbackBackground(CwmScreen &screen)
{
  return screen.getPixel(getFeedbackBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getFeedbackForeground()
{
  return getSectionResource("window/foreground", "feedback", getForeground());
}

Pixel
CwmResourceData::
getFeedbackForeground(CwmScreen &screen)
{
  return screen.getPixel(getFeedbackForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getFeedbackFont()
{
  return getSectionResource("window/font", "feedback", getFont());
}

CwmFont *
CwmResourceData::
getFeedbackFont(CwmScreen &screen)
{
  return screen.getFont(getFeedbackFont());
}

void
CwmResourceData::
getFeedbackGeometry(CHAlignType *halign, CVAlignType *valign, int *x_offset, int *y_offset)
{
  std::string str = getSectionResource("window/geometry", "feedback", "center,center");

  parseFeedbackGeometry(str, halign, valign, x_offset, y_offset);
}

std::string
CwmResourceData::
getToolBarBackground()
{
  return getResource("toolbar/background", getBackground());
}

Pixel
CwmResourceData::
getToolBarBackground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getToolBarForeground()
{
  return getResource("toolbar/foreground", getForeground());
}

Pixel
CwmResourceData::
getToolBarForeground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getToolBarFont()
{
  return getResource("toolbar/font", getFont());
}

CwmFont *
CwmResourceData::
getToolBarFont(CwmScreen &screen)
{
  return screen.getFont(getToolBarFont());
}

bool
CwmResourceData::
getToolBarShowClock()
{
  std::string str = getResource("toolbar/showClock", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getToolBarAddIcons()
{
  std::string str = getResource("toolbar/addIcons", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

bool
CwmResourceData::
getToolBarAddWindows()
{
  std::string str = getResource("toolbar/addWindows", "Yes");

  if (CStrUtil::isBool(str))
    return CStrUtil::toBool(str);

  return true;
}

int
CwmResourceData::
getToolBarHeight()
{
  std::string str = getResource("toolbar/height", "28");

  if (CStrUtil::isInteger(str))
    return CStrUtil::toInteger(str);

  return 28;
}

int
CwmResourceData::
getToolBarIconWidth()
{
  std::string str = getResource("toolbar/iconWidth", "108");

  if (CStrUtil::isInteger(str))
    return CStrUtil::toInteger(str);

  return 108;
}

std::string
CwmResourceData::
getToolBarClockBackground()
{
  return getSectionResource("toolbar/background", "clock", getToolBarBackground());
}

Pixel
CwmResourceData::
getToolBarClockBackground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarClockBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getToolBarClockForeground()
{
  return getSectionResource("toolbar/foreground", "clock", getToolBarForeground());
}

Pixel
CwmResourceData::
getToolBarClockForeground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarClockForeground(), screen.getBlackPixel());
}

CwmColor *
CwmResourceData::
getToolBarClockColor(CwmScreen &screen)
{
  return screen.getColor(getToolBarClockForeground(screen), getToolBarClockBackground(screen));
}

std::string
CwmResourceData::
getToolBarClockFont()
{
  return getSectionResource("toolbar/font", "clock", getToolBarFont());
}

CwmFont *
CwmResourceData::
getToolBarClockFont(CwmScreen &screen)
{
  return screen.getFont(getToolBarClockFont());
}

std::string
CwmResourceData::
getToolBarClockTimeFormat()
{
  return getSectionResource("toolbar/timeFormat", "clock", "%H:%M:%S");
}

std::string
CwmResourceData::
getToolBarIconBackground()
{
  return getSectionResource("toolbar/background", "icon", getToolBarBackground());
}

Pixel
CwmResourceData::
getToolBarIconBackground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarIconBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getToolBarIconForeground()
{
  return getSectionResource("toolbar/foreground", "icon", getToolBarForeground());
}

Pixel
CwmResourceData::
getToolBarIconForeground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarIconForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getToolBarIconFont()
{
  return getSectionResource("toolbar/font", "icon", getToolBarFont());
}

CwmFont *
CwmResourceData::
getToolBarIconFont(CwmScreen &screen)
{
  return screen.getFont(getToolBarIconFont());
}

std::string
CwmResourceData::
getToolBarIconFocusBackground()
{
  return getSectionResource("toolbar/focusBackground", "icon", getToolBarIconBackground());
}

Pixel
CwmResourceData::
getToolBarIconFocusBackground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarIconFocusBackground(), screen.getWhitePixel());
}

std::string
CwmResourceData::
getToolBarIconFocusForeground()
{
  return getSectionResource("toolbar/focusForeground", "icon", getToolBarIconForeground());
}

Pixel
CwmResourceData::
getToolBarIconFocusForeground(CwmScreen &screen)
{
  return screen.getPixel(getToolBarIconFocusForeground(), screen.getBlackPixel());
}

std::string
CwmResourceData::
getToolBarIconFocusFont()
{
  return getSectionResource("toolbar/focusFont", "icon", getToolBarIconFont());
}

CwmFont *
CwmResourceData::
getToolBarIconFocusFont(CwmScreen &screen)
{
  return screen.getFont(getToolBarIconFocusFont());
}

void
CwmResourceData::
loadMenus()
{
  StringVectorT names;
  StringVectorT paths;

  CwmConfigInst->getGroups("menus", names, paths);

  StringVectorT::iterator pname1 = names.begin();
  StringVectorT::iterator ppath1 = paths.begin();
  StringVectorT::iterator pname2 = names.end  ();

  for ( ; pname1 != pname2; ++pname1, ++ppath1) {
    CwmMenuDef *menu_def = loadMenu(*ppath1, *pname1);

    std::string name = CwmConfigInst->getValue(*ppath1 + "/name", *pname1);

    CwmNamedMenuMgrInst->define(name, menu_def);
  }
}

CwmMenuDef *
CwmResourceData::
loadMenu(const std::string &path, const std::string &)
{
  CwmMenuDef *menu_def = new CwmMenuDef();

  StringVectorT sections;

  CwmConfigInst->getSections(path, sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1) {
    if (CStrUtil::casecmp(*psection1, "") == 0)
      continue;

    std::string type = getSectionResource(path + "/type", *psection1, "button");

    if     (CStrUtil::casecmp(type, "title") == 0) {
      std::string name = getSectionResource(path + "/name", *psection1, "title");

      menu_def->addTitle(name);

      menu_def->addSplitter("----");
    }
    else if (CStrUtil::casecmp(type, "splitter") == 0) {
      menu_def->addSplitter("----");
    }
    else if (CStrUtil::casecmp(type, "button") == 0) {
      std::string name        = getSectionResource(path + "/name", *psection1, "button");
      std::string image       = getSectionResource(path + "/image", *psection1, "");
      std::string mnemonic    = getSectionResource(path + "/mnemonic", *psection1, "");
      std::string accelerator = getSectionResource(path + "/accelerator", *psection1, "");
      std::string function    = getSectionResource(path + "/function", *psection1, "f.null");
      std::string data        = getSectionResource(path + "/data", *psection1, "");

      CwmFunctionDef *function_def = new CwmFunctionDef(function);

      function_def->setData(new std::string(data));

      menu_def->addButton(image, name, (mnemonic.size() > 0 ? mnemonic[0] : '\0'), accelerator,
                          (CwmMenuProc) CwmFunctionDef::processProc, function_def);
    }
    else if (CStrUtil::casecmp(type, "toggle") == 0) {
      std::string name        = getSectionResource(path + "/name", *psection1, "toggle");
      std::string image       = getSectionResource(path + "/image", *psection1, "");
      std::string mnemonic    = getSectionResource(path + "/mnemonic", *psection1, "");
      std::string accelerator = getSectionResource(path + "/accelerator", *psection1, "");
      std::string function    = getSectionResource(path + "/function", *psection1, "f.null");
      std::string data        = getSectionResource(path + "/data", *psection1, "");

      CwmFunctionDef *function_def = new CwmFunctionDef(function);

      function_def->setData(new std::string(data));

      menu_def->addToggle(image, name, (mnemonic.size() > 0 ? mnemonic[0] : '\0'), accelerator,
                          (CwmMenuProc) CwmFunctionDef::processProc, function_def);
    }
    else if (CStrUtil::casecmp(type, "cascade") == 0) {
      std::string name  = getSectionResource(path + "/name", *psection1, "cascade");
      std::string path1 = getSectionResource(path + "/path", *psection1, "cascade");

      CwmMenuDef *menu_def1 = loadMenu(path + "/" + path1, path1);

      menu_def->addCascade(name, menu_def1);
    }
  }

  return menu_def;
}

void
CwmResourceData::
loadDeskIcons()
{
  StringVectorT sections;

  CwmConfigInst->getSections("desk_icons", sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1)
    loadDeskIcon("desk_icons", *psection1);
}

void
CwmResourceData::
loadDeskIcon(const std::string &path, const std::string &section)
{
  std::string icon       = getSectionResource(path + "/icon", section, "");
  std::string label      = getSectionResource(path + "/label", section, "");
  std::string command    = getSectionResource(path + "/command", section, "");
  std::string background = getSectionResource(path + "/background", section, "");
  std::string foreground = getSectionResource(path + "/foreground", section, "");
  int         x          = getSectionResource(path + "/x", section, 0);
  int         y          = getSectionResource(path + "/y", section, 0);

  CwmCustomIconMgrInst->addCustomIcon(icon, label, command, background, foreground, x, y);
}

void
CwmResourceData::
loadToolBarIcons()
{
  StringVectorT sections;

  CwmConfigInst->getSections("toolbar/icons", sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1)
    loadToolBarIcon("toolbar/icons", *psection1);
}

void
CwmResourceData::
loadToolBarIcon(const std::string &path, const std::string &section)
{
  std::string icon  = getSectionResource(path + "/icon" , section, "");
  std::string label = getSectionResource(path + "/label", section, "");

  bool add_display = false;

  std::string command = getSectionResource(path + "/command", section, "");

  if (command == "") {
    add_display = true;

    command = getSectionResource(path + "/xcommand", section, "");
  }

  std::string background = getSectionResource(path + "/background", section, "");
  std::string foreground = getSectionResource(path + "/foreground", section, "");

  CwmToolBarIconDefMgrInst->addToolBarIcon(icon, label, command, add_display,
                                           background, foreground);
}

void
CwmResourceData::
loadDesks()
{
  StringVectorT sections;

  CwmConfigInst->getSections("desks", sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1) {
    int         num     = getSectionResource("desks/num", *psection1, 1);
    std::string name    = getSectionResource("desks/name", *psection1, "");
    std::string icons   = getSectionResource("desks/icons", *psection1, "no");
    std::string toolbar = getSectionResource("desks/toolbar", *psection1, "yes");
    std::string image   = getSectionResource("desks/image", *psection1, "");

    CwmDeskResourceMgrInst->setDeskName(num - 1, name);

    CwmDeskResourceMgrInst->setDeskDisplayIcons
     (num - 1, CStrUtil::toBool(icons));
    CwmDeskResourceMgrInst->setDeskDisplayToolBar
     (num - 1, CStrUtil::toBool(toolbar));

    if (image != "")
      CwmDeskResourceMgrInst->setDeskImage(num - 1, image);
  }
}

void
CwmResourceData::
loadGroups()
{
  StringVectorT names;
  StringVectorT paths;

  CwmConfigInst->getGroups("groups", names, paths);

  StringVectorT::iterator pname1 = names.begin();
  StringVectorT::iterator ppath1 = paths.begin();
  StringVectorT::iterator pname2 = names.end  ();

  for ( ; pname1 != pname2; ++pname1, ++ppath1) {
    std::string pattern        = CwmConfigInst->getValue(*ppath1 + "/pattern", "");
    std::string icon           = CwmConfigInst->getValue(*ppath1 + "/icon", "");
    std::string iconSmall      = CwmConfigInst->getValue(*ppath1 + "/iconSmall", "");
    std::string decorations    = CwmConfigInst->getValue(*ppath1 + "/decorations", "");
    std::string functions      = CwmConfigInst->getValue(*ppath1 + "/functions", "");
    std::string focusAutoRaise = CwmConfigInst->getValue(*ppath1 + "/focusAutoRaise", "");
    std::string circulateSkip  = CwmConfigInst->getValue(*ppath1 + "/circulateSkip", "");
    std::string toolbarSkip    = CwmConfigInst->getValue(*ppath1 + "/toolbarSkip", "");

    if (icon != "")
      CwmCustomDeskIconMgrInst->setIcon(pattern, icon);

    if (iconSmall != "")
      CwmCustomDeskIconMgrInst->setIconSmall(pattern, iconSmall);

    if (decorations != "") {
      long flags = stringToDecorations(decorations);

      CwmCustomHintMgrInst->
       addCustomHintValue(pattern, CwmNdecorations, CwmTint, (CwmData) flags);
    }

    if (functions != "") {
      long flags = stringToFunctions(functions);

      CwmCustomHintMgrInst->
       addCustomHintValue(pattern, CwmNfunctions, CwmTint, (CwmData) flags);
    }

    if (focusAutoRaise != "")
      CwmCustomHintMgrInst->
       addCustomHintValue(pattern, CwmNfocusAutoRaise, CwmTint,
                          (CwmData) CStrUtil::toBool(focusAutoRaise));

    if (circulateSkip != "")
      CwmCustomHintMgrInst->
       addCustomHintValue(pattern, CwmNcirculateSkip, CwmTint,
                          (CwmData) CStrUtil::toBool(circulateSkip));

    if (toolbarSkip != "")
      CwmCustomHintMgrInst->
       addCustomHintValue(pattern, CwmNtoolbarSkip, CwmTint,
                          (CwmData) CStrUtil::toBool(toolbarSkip));

    loadGroupEvents(pattern, *ppath1 + "/events");
  }
}

void
CwmResourceData::
loadGroupEvents(const std::string &pattern, const std::string &path)
{
  StringVectorT sections;

  CwmConfigInst->getSections(path, sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1) {
    std::string area     = getSectionResource(path + "/area", *psection1, "");
    std::string event    = getSectionResource(path + "/event", *psection1, "");
    std::string function = getSectionResource(path + "/function", *psection1, "");
    std::string data     = getSectionResource(path + "/data", *psection1, "");

    int area1 = stringToArea(area);

    CXNamedEvent *event1 = new CXNamedEvent(event);

    if (! event1->getEvent()) {
      CwmMachineInst->logf("Bad Event %s\n", event.c_str());
      delete event1;
      continue;
    }

    CwmFunctionDef *function1 = new CwmFunctionDef(function);

    if (! function1) {
      CwmMachineInst->logf("Bad Function %s\n", event.c_str());
      delete event1;
      continue;
    }

    CwmWindowEventFunctionMgrInst->
      addPatternEventFunction(pattern, area1, event1, function1, (CwmData) new std::string(data));
  }
}

void
CwmResourceData::
loadRoot()
{
  std::string image = getResource("root/image", "");

  if (image != "")
    CwmInst->setRootImage(image);

  loadRootEvents();
}

void
CwmResourceData::
loadRootEvents()
{
  StringVectorT sections;

  CwmConfigInst->getSections("root/events", sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1) {
    std::string event    = getSectionResource("root/events/event"   , *psection1, "");
    std::string function = getSectionResource("root/events/function", *psection1, "");
    std::string data     = getSectionResource("root/events/data"    , *psection1, "");

    CXNamedEvent *event1 = new CXNamedEvent(event);

    if (! event1->getEvent()) {
      CwmMachineInst->logf("Bad Event %s\n", event.c_str());
      delete event1;
      continue;
    }

    CwmFunctionDef *function1 = new CwmFunctionDef(function);

    if (! function1) {
      CwmMachineInst->logf("Bad Function %s\n", event.c_str());
      delete event1;
      continue;
    }

    CwmRootEventFunctionMgrInst->add(event1, function1, new std::string(data));
  }
}

void
CwmResourceData::
loadStartupCommands(CwmScreen &screen)
{
  StringVectorT sections;

  CwmConfigInst->getSections("startup", sections);

  StringVectorT::iterator psection1 = sections.begin();
  StringVectorT::iterator psection2 = sections.end  ();

  for ( ; psection1 != psection2; ++psection1)
    loadStartupCommand(screen, "startup", *psection1);
}

void
CwmResourceData::
loadStartupCommand(CwmScreen &screen, const std::string &path, const std::string &section)
{
  bool add_display = false;

  std::string command = getSectionResource(path + "/command", section, "");

  if (command == "") {
    add_display = true;

    command = getSectionResource(path + "/xcommand", section, "");
  }

  UnixCmd cmd(screen, command);

  cmd.setAddDisplay(add_display);

  cmd.run();
}

int
CwmResourceData::
stringToDecorations(const std::string &str)
{
  int decorations = 0;

  StringVectorT words;

  CStrUtil::addWords(str, words);

  for (int i = 0; i < (int) words.size(); i++) {
    std::string word = words[i];

    if (i == 0) {
      if (word[0] == '-')
        decorations = MWM_DECOR_BORDER   | MWM_DECOR_RESIZEH |
                      MWM_DECOR_TITLE    | MWM_DECOR_MENU    |
                      MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE;
      else
        decorations = 0;
    }

    bool add = true;

    if (word[0] == '-') {
      add = false;

      word = word.substr(1);
    }

    int flag = 0;

    if      (CStrUtil::casecmp(word, "none") == 0)
      flag = 0;
    else if (CStrUtil::casecmp(word, "all") == 0)
      flag = MWM_DECOR_BORDER   | MWM_DECOR_RESIZEH |
             MWM_DECOR_TITLE    | MWM_DECOR_MENU    |
             MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE;
    else if (CStrUtil::casecmp(word, "border") == 0)
      flag = MWM_DECOR_BORDER;
    else if (CStrUtil::casecmp(word, "resizeh") == 0)
      flag = MWM_DECOR_RESIZEH;
    else if (CStrUtil::casecmp(word, "title") == 0)
      flag = MWM_DECOR_TITLE;
    else if (CStrUtil::casecmp(word, "menu") == 0)
      flag = MWM_DECOR_MENU;
    else if (CStrUtil::casecmp(word, "minimize") == 0)
      flag = MWM_DECOR_MINIMIZE;
    else if (CStrUtil::casecmp(word, "maximize") == 0)
      flag = MWM_DECOR_MAXIMIZE;

    if (add)
      decorations |= flag;
    else
      decorations &= ~flag;
  }

  return decorations;
}

int
CwmResourceData::
stringToFunctions(const std::string &str)
{
  int functions = 0;

  StringVectorT words;

  CStrUtil::addWords(str, words);

  for (int i = 0; i < (int) words.size(); i++) {
    std::string word = words[i];

    if (i == 0) {
      if (word[0] == '-')
        functions = MWM_FUNC_RESIZE   | MWM_FUNC_MOVE     |
                    MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE |
                    MWM_FUNC_CLOSE    | MWM_FUNC_RESTORE;
      else
        functions = 0;
    }

    bool add = true;

    if (word[0] == '-') {
      add = false;

      word = word.substr(1);
    }

    int flag = 0;

    if      (CStrUtil::casecmp(word, "none") == 0)
      flag = 0;
    else if (CStrUtil::casecmp(word, "all") == 0)
      flag = MWM_FUNC_RESIZE   | MWM_FUNC_MOVE     |
             MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE |
             MWM_FUNC_CLOSE    | MWM_FUNC_RESTORE;
    else if (CStrUtil::casecmp(word, "resize") == 0)
      flag = MWM_FUNC_RESIZE;
    else if (CStrUtil::casecmp(word, "move") == 0)
      flag = MWM_FUNC_MOVE;
    else if (CStrUtil::casecmp(word, "minimize") == 0)
      flag = MWM_FUNC_MINIMIZE;
    else if (CStrUtil::casecmp(word, "maximize") == 0)
      flag = MWM_FUNC_MAXIMIZE;
    else if (CStrUtil::casecmp(word, "close") == 0)
      flag = MWM_FUNC_CLOSE;
    else if (CStrUtil::casecmp(word, "restore") == 0)
      flag = MWM_FUNC_RESTORE;

    if (add)
      functions |= flag;
    else
      functions &= ~flag;
  }

  return functions;
}

int
CwmResourceData::
stringToArea(const std::string &str)
{
  if      (CStrUtil::casecmp(str, "Menu") == 0)
    return CWM_WINDOW_MENU_AREA;
  else if (CStrUtil::casecmp(str, "Title") == 0)
    return CWM_WINDOW_TITLE_AREA;
  else if (CStrUtil::casecmp(str, "Minimize") == 0)
    return CWM_WINDOW_MINIMIZE_AREA;
  else if (CStrUtil::casecmp(str, "Maximize") == 0)
    return CWM_WINDOW_MAXIMIZE_AREA;
  else if (CStrUtil::casecmp(str, "Border") == 0)
    return CWM_WINDOW_BORDER_AREA;
  else if (CStrUtil::casecmp(str, "User") == 0)
    return CWM_WINDOW_USER_AREA;
  else if (CStrUtil::casecmp(str, "Any") == 0)
    return CWM_WINDOW_ANY_AREA;
  else
    return CWM_WINDOW_ANY_AREA;
}
