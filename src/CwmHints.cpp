#include <CwmI.h>

class CwmCustomHintValue {
 public:
  CwmCustomHintValue(const std::string &name, const std::string &type, CwmData value);
 ~CwmCustomHintValue();

  bool isType(const std::string &type);

  CwmData getValue() const { return value_; }

  bool isName(const std::string &name);

 private:
  std::string name_;
  std::string type_;
  CwmData     value_;
};

class CwmCustomHint {
 public:
  CwmCustomHint(const std::string &pattern);
 ~CwmCustomHint();

  void addValue(const std::string &name, const char *type, CwmData value);

  bool compare(const std::string &name);

  bool isPattern(const std::string &pattern);

  CwmCustomHintValue *lookup(const std::string &name);

 private:
  typedef std::vector<CwmCustomHintValue *> CustomHintValueList;

  std::string          pattern_;
  CGlob               *compile_;
  CustomHintValueList  values_;
};

CwmHints::
CwmHints(CwmWMWindow *window) :
 window_(window)
{
  user_xwin_ = window->getXWin();

  name_      = "";
  icon_name_ = "";

  x_           = 0;
  y_           = 0;
  width_inc_   = 1;
  height_inc_  = 1;
  min_width_   = 1;
  min_height_  = 1;
  max_width_   = 9999;
  max_height_  = 9999;
  base_width_  = 1;
  base_height_ = 1;
  min_aspect_  = 1;
  max_aspect_  = 1;
  win_gravity_ = 0;

  input_         = true;
  initial_state_ = NormalState;
  icon_window_   = None;
  icon_pixmap_   = None;
  icon_mask_     = None;
  icon_depth_    = 0;
  icon_x_        = -1;
  icon_y_        = -1;
  window_group_  = None;

  transient_for_ = None;

  res_name_  = "";
  res_class_ = "";

  client_machine_ = "";

  cmap_windows_     = 0;
  num_cmap_windows_ = 0;

  take_focus_    = false;
  save_yourself_ = false;
  delete_window_ = false;

  mwm_functions_   = 0;
  mwm_decorations_ = 0;
  mwm_input_mode_  = 0;
  mwm_status_      = 0;

  init();
}

CwmHints::
~CwmHints()
{
}

void
CwmHints::
init()
{
  readWMName       ();
  readWMIconName   ();
  readWMSizeHints  ();
  readWMHints      ();
  readTransientHint();
  readClassHint    ();
  readSessionHints ();
  readWMCMapWindows();
  readWMProtocols  ();
  readMwmHints     ();

  //------

  if (transient_for_ != None) {
    mwm_functions_ &= ~MWM_FUNC_MINIMIZE;
    mwm_functions_ &= ~MWM_FUNC_MAXIMIZE;
    mwm_functions_ &= ~MWM_FUNC_RESTORE;
  }

  //------

  if (! CwmCustomHintMgrInst->getCustomHintValue
         (res_name_, CwmNdecorations, (CwmData) &mwm_decorations_))
    CwmCustomHintMgrInst->getCustomHintValue
     (res_class_, CwmNdecorations, (CwmData) &mwm_decorations_);

  if (! CwmCustomHintMgrInst->getCustomHintValue
         (res_name_, CwmNfunctions, (CwmData) &mwm_functions_))
    CwmCustomHintMgrInst->getCustomHintValue
     (res_class_, CwmNfunctions, (CwmData) &mwm_functions_);

  //------

  if (mwm_functions_ == MWM_FUNC_ALL)
    mwm_functions_ = MWM_FUNC_RESIZE   | MWM_FUNC_MOVE     |
                     MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE |
                     MWM_FUNC_CLOSE    | MWM_FUNC_RESTORE;

  if (mwm_decorations_ == MWM_DECOR_ALL)
    mwm_decorations_ = MWM_DECOR_BORDER   | MWM_DECOR_RESIZEH |
                       MWM_DECOR_TITLE    | MWM_DECOR_MENU    |
                       MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE;

  if (! (mwm_functions_ & MWM_FUNC_RESIZE))
    mwm_decorations_ &= ~MWM_DECOR_RESIZEH;

  if (! (mwm_functions_ & MWM_FUNC_MINIMIZE))
    mwm_decorations_ &= ~MWM_DECOR_MINIMIZE;

  if (! (mwm_functions_ & MWM_FUNC_MAXIMIZE))
    mwm_decorations_ &= ~MWM_DECOR_MAXIMIZE;

  if (mwm_decorations_ & MWM_DECOR_MENU     ||
      mwm_decorations_ & MWM_DECOR_MINIMIZE ||
      mwm_decorations_ & MWM_DECOR_MAXIMIZE)
    mwm_decorations_ |= MWM_DECOR_TITLE;

  if (mwm_decorations_ & MWM_DECOR_RESIZEH)
    mwm_decorations_ |= MWM_DECOR_BORDER;

  //------

  if (CwmInst->getPrintHints())
    print();
}

void
CwmHints::
readWMName()
{
  CwmMachineInst->getWMName(user_xwin_, name_);
}

void
CwmHints::
readWMIconName()
{
  CwmMachineInst->getWMIconName(user_xwin_, icon_name_);
}

void
CwmHints::
readWMSizeHints()
{
  int         supplied;
  XSizeHints *size_hints;

  CwmMachineInst->getWMNormalHints(user_xwin_, &size_hints, &supplied);

  if      (size_hints->flags & USPosition) {
    x_ = size_hints->x;
    y_ = size_hints->y;
  }
  else if (size_hints->flags & PPosition) {
    x_ = size_hints->x;
    y_ = size_hints->y;

    if (x_ == 0 && y_ == 0) {
      x_ = -1;
      y_ = -1;
    }
  }
  else {
    x_ = -1;
    y_ = -1;
  }

  if (size_hints->flags & PResizeInc) {
    width_inc_  = size_hints->width_inc;
    height_inc_ = size_hints->height_inc;
  }

  if (width_inc_  <= 0)
    width_inc_  = 1;

  if (height_inc_ <= 0)
    height_inc_ = 1;

  if (! (size_hints->flags & PMinSize ) &&
      ! (size_hints->flags & PBaseSize)) {
    size_hints->min_width   = 0;
    size_hints->min_height  = 0;
    size_hints->base_width  = 0;
    size_hints->base_height = 0;
  }

  if (size_hints->flags & PMinSize) {
    min_width_  = size_hints->min_width;
    min_height_ = size_hints->min_height;
  }
  else {
    min_width_  = size_hints->base_width;
    min_height_ = size_hints->base_height;
  }

  if (size_hints->flags & PBaseSize) {
    base_width_  = size_hints->base_width;
    base_height_ = size_hints->base_height;
  }
  else {
    base_width_  = size_hints->min_width;
    base_height_ = size_hints->min_height;
  }

  if (min_width_ <= 0)
    min_width_  = 1;

  if (min_height_ <= 0)
    min_height_ = 1;

  if (size_hints->flags & PMaxSize) {
    max_width_  = size_hints->max_width;
    max_height_ = size_hints->max_height;
  }
  else {
    CwmScreen &screen = window_->getScreen();

    max_width_  = screen.getWidth();
    max_height_ = screen.getHeight();
  }

  if (max_width_  < min_width_)
    max_width_ = min_width_;

  if (max_height_ < min_height_)
    max_height_ = min_height_;

  if (size_hints->flags & PAspect) {
    min_aspect_ = ((double) size_hints->min_aspect.x)/size_hints->min_aspect.y;
    max_aspect_ = ((double) size_hints->max_aspect.x)/size_hints->max_aspect.y;
  }
  else {
    min_aspect_ = 0.0;
    max_aspect_ = 0.0;
  }

  if (size_hints->flags & PWinGravity)
    win_gravity_ = size_hints->win_gravity;
  else
    win_gravity_ = NorthWestGravity;
}

void
CwmHints::
readWMHints()
{
  XWMHints *wm_hints;

  CwmMachineInst->getWMHints(user_xwin_, &wm_hints);

  if (! wm_hints)
    return;

  if (wm_hints->flags & InputHint)
    input_ = wm_hints->input;

  if (wm_hints->flags & StateHint)
    initial_state_ = wm_hints->initial_state;

  if (wm_hints->flags & IconWindowHint &&
      CwmMachineInst->isValidWindow(wm_hints->icon_window))
    icon_window_ = wm_hints->icon_window;

  if (wm_hints->flags & IconPixmapHint && icon_window_ == None)
    icon_pixmap_ = wm_hints->icon_pixmap;

  if (wm_hints->flags & IconMaskHint)
    icon_mask_ = wm_hints->icon_mask;

  if (wm_hints->flags & IconPositionHint) {
    icon_x_ = wm_hints->icon_x;
    icon_y_ = wm_hints->icon_y;
  }

  if (wm_hints->flags & WindowGroupHint)
    window_group_ = wm_hints->window_group;
}

void
CwmHints::
readTransientHint()
{
  CwmMachineInst->getWMTransientFor(user_xwin_, &transient_for_);
}

void
CwmHints::
readClassHint()
{
  XClassHint *class_hint;

  CwmMachineInst->getWMClassHint(user_xwin_, &class_hint);

  char *res_name  = class_hint->res_name;
  char *res_class = class_hint->res_class;

  res_name_  = "";
  res_class_ = "";

  if (res_name != 0)
    res_name_ = res_name;

  if (res_name_ == "")
    res_name_ = name_;

  if (res_class != 0)
    res_class_ = res_class;

  if (res_class_ == "" && res_name_ != "") {
    res_class_ = res_name_;

    res_class_[0] = toupper(res_class_[0]);
  }
}

void
CwmHints::
readSessionHints()
{
  CwmMachineInst->getWMClientMachine(user_xwin_, client_machine_);

  int    argc;
  char **argv;

  command_argv_.clear();

  CwmMachineInst->getWMCommand(user_xwin_, &argc, &argv);

  for (int i = 0; i < argc; i++)
    command_argv_.push_back(std::string(argv[i]));
}

void
CwmHints::
readWMCMapWindows()
{
  if (cmap_windows_ != 0)
    XFree((char *) cmap_windows_);

  CwmMachineInst->getWMColormapWindows(user_xwin_, &cmap_windows_, &num_cmap_windows_);
}

void
CwmHints::
readWMProtocols()
{
  take_focus_    = false;
  save_yourself_ = false;
  delete_window_ = false;

  const CXAtom **protocols     = 0;
  int            num_protocols = 0;

  CwmMachineInst->getWMProtocols(user_xwin_, &protocols, &num_protocols);

  for (int i = 0; i < num_protocols; i++) {
    if (CwmMachineInst->isWMTakeFocusAtom(*protocols[i]))
      take_focus_ = true;

    if (CwmMachineInst->isWMSaveYourselfAtom(*protocols[i]))
      save_yourself_ = true;

    if (CwmMachineInst->isWMDeleteWindowAtom(*protocols[i]))
      delete_window_ = true;
  }

  if (protocols != 0)
    delete [] protocols;
}

void
CwmHints::
readMwmHints()
{
  mwm_functions_   = MWM_FUNC_RESIZE   | MWM_FUNC_MOVE     |
                     MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE |
                     MWM_FUNC_CLOSE    | MWM_FUNC_RESTORE;
  mwm_decorations_ = MWM_DECOR_BORDER   | MWM_DECOR_RESIZEH |
                     MWM_DECOR_TITLE    | MWM_DECOR_MENU    |
                     MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE;
  mwm_input_mode_  = MWM_INPUT_MODELESS;
  mwm_status_      = 0;

  MotifWmHints mwm_hints;

  if (! CwmMachineInst->getWMMwmHints(user_xwin_, mwm_hints))
    return;

  if (mwm_hints.flags & MWM_HINTS_FUNCTIONS)
    mwm_functions_ = mwm_hints.functions;

  if (mwm_hints.flags & MWM_HINTS_DECORATIONS)
    mwm_decorations_ = mwm_hints.decorations;

  if (mwm_hints.flags & MWM_HINTS_INPUT_MODE)
    mwm_input_mode_ = mwm_hints.input_mode;

  if (mwm_hints.flags & MWM_HINTS_STATUS)
    mwm_status_ = mwm_hints.status;
}

void
CwmHints::
print()
{
  if (name_ != "")
    CwmMachineInst->logf("name = %s\n", name_.c_str());

  if (icon_name_ != "")
    CwmMachineInst->logf("icon_name = %s\n", icon_name_.c_str());

  CwmMachineInst->logf("width_inc        = %d\n", width_inc_);
  CwmMachineInst->logf("height_inc       = %d\n", height_inc_);
  CwmMachineInst->logf("min_width        = %d\n", min_width_);
  CwmMachineInst->logf("min_height       = %d\n", min_height_);
  CwmMachineInst->logf("max_width        = %d\n", max_width_);
  CwmMachineInst->logf("max_height       = %d\n", max_height_);
  CwmMachineInst->logf("base_width       = %d\n", base_width_);
  CwmMachineInst->logf("base_height      = %d\n", base_height_);
  CwmMachineInst->logf("min_aspect       = %d\n", min_aspect_);
  CwmMachineInst->logf("max_aspect       = %d\n", max_aspect_);
  CwmMachineInst->logf("win_gravity      = %d\n", win_gravity_);
  CwmMachineInst->logf("input            = %d\n", input_);
  CwmMachineInst->logf("initial_state    = %d\n", initial_state_);
  CwmMachineInst->logf("icon_window      = %x\n", (uint) icon_window_);
  CwmMachineInst->logf("icon_pixmap      = %x\n", (uint) icon_pixmap_);
  CwmMachineInst->logf("icon_mask        = %x\n", (uint) icon_mask_);
  CwmMachineInst->logf("icon_depth       = %d\n", icon_depth_);
  CwmMachineInst->logf("icon_x           = %d\n", icon_x_);
  CwmMachineInst->logf("icon_y           = %d\n", icon_y_);
  CwmMachineInst->logf("window_group     = %x\n", (uint) window_group_);
  CwmMachineInst->logf("transient        = %x\n", (uint) transient_for_);
  CwmMachineInst->logf("res_name         = %s\n", res_name_.c_str());
  CwmMachineInst->logf("res_class        = %s\n", res_class_.c_str());
  CwmMachineInst->logf("client_machine   = %s\n", client_machine_.c_str());

  for (int i = 0; command_argv_.size(); i++)
    CwmMachineInst->logf("command_argv[%d] = %s\n", i, command_argv_[i].c_str());

  CwmMachineInst->logf("num_cmap_windows = %d\n", num_cmap_windows_);
  CwmMachineInst->logf("take_focus       = %d\n", take_focus_);
  CwmMachineInst->logf("save_yourself    = %d\n", save_yourself_);
  CwmMachineInst->logf("delete_window    = %d\n", delete_window_);
  CwmMachineInst->logf("mwm_functions    = %d\n", mwm_functions_);
  CwmMachineInst->logf("mwm_decorations  = %d\n", mwm_decorations_);
  CwmMachineInst->logf("mwm_input_mode   = %d\n", mwm_input_mode_);
  CwmMachineInst->logf("mwm_status       = %d\n", mwm_status_);
}

CwmCustomHintMgr *
CwmCustomHintMgr::
getInstance()
{
  static CwmCustomHintMgr *instance;

  if (! instance)
    instance = new CwmCustomHintMgr();

  return instance;
}

CwmCustomHintMgr::
CwmCustomHintMgr()
{
}

CwmCustomHintMgr::
~CwmCustomHintMgr()
{
  for (auto &custom_hint : custom_hints_)
    delete custom_hint;
}

void
CwmCustomHintMgr::
addCustomHintValue(const std::string &pattern, const std::string &name,
                   const char *type, CwmData value)
{
  CwmCustomHint *custom_hint = getCustomHint(pattern);

  custom_hint->addValue(name, type, value);
}

bool
CwmCustomHintMgr::
getCustomHintValue(const std::string &window, const std::string &name, CwmData value)
{
  if (window == "")
    return false;

  CustomHintList::iterator phint1 = custom_hints_.begin();
  CustomHintList::iterator phint2 = custom_hints_.end  ();

  for ( ; phint1 != phint2; ++phint1)
    if ((*phint1)->compare(window))
      break;

  if (phint1 == phint2)
    return false;

  CwmCustomHint *custom_hint = *phint1;

  CwmCustomHintValue *custom_value = custom_hint->lookup(name);

  if (! custom_value)
    return false;

  if (custom_value->isType(CwmTint)) {
    int *pvalue = (int *) value;

    *pvalue = (long) custom_value->getValue();
  }
  else
    return false;

  return true;
}

CwmCustomHint *
CwmCustomHintMgr::
getCustomHint(const std::string &pattern)
{
  CustomHintList::iterator phint1 = custom_hints_.begin();
  CustomHintList::iterator phint2 = custom_hints_.end  ();

  for ( ; phint1 != phint2; ++phint1)
    if ((*phint1)->isPattern(pattern))
      return *phint1;

  CwmCustomHint *custom_hint = addCustomHint(pattern);

  return custom_hint;
}

CwmCustomHint *
CwmCustomHintMgr::
addCustomHint(const std::string &pattern)
{
  CwmCustomHint *custom_hint = new CwmCustomHint(pattern);

  custom_hints_.push_back(custom_hint);

  return custom_hint;
}

CwmCustomHintValue::
CwmCustomHintValue(const std::string &name, const std::string &type, CwmData value) :
 name_(name), type_(type), value_(value)
{
}

CwmCustomHintValue::
~CwmCustomHintValue()
{
}

bool
CwmCustomHintValue::
isType(const std::string &type)
{
  return (strcmp(type_.c_str(), type.c_str()) == 0);
}

bool
CwmCustomHintValue::
isName(const std::string &name)
{
  return (name == name_);
}

CwmCustomHint::
CwmCustomHint(const std::string &pattern) :
 pattern_(pattern)
{
  compile_ = new CGlob(pattern);
}

CwmCustomHint::
~CwmCustomHint()
{
  delete compile_;

  for (auto &value : values_)
    delete value;
}

void
CwmCustomHint::
addValue(const std::string &name, const char *type, CwmData value)
{
  CwmCustomHintValue *custom_value = new CwmCustomHintValue(name, type, value);

  values_.push_back(custom_value);
}

bool
CwmCustomHint::
compare(const std::string &name)
{
  return compile_->compare(name);
}

CwmCustomHintValue *
CwmCustomHint::
lookup(const std::string &name)
{
  CustomHintValueList::const_iterator pvalue1 = values_.begin();
  CustomHintValueList::const_iterator pvalue2 = values_.end  ();

  for ( ; pvalue1 != pvalue2; ++pvalue1)
    if ((*pvalue1)->isName(name))
      return *pvalue1;

  return 0;
}

bool
CwmCustomHint::
isPattern(const std::string &pattern)
{
  return (pattern == pattern_);
}
