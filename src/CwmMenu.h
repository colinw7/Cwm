#ifndef CWM_MENU_H
#define CWM_MENU_H

enum CwmMenuItemType {
  CWM_MENU_TITLE_TYPE,
  CWM_MENU_BUTTON_TYPE,
  CWM_MENU_TOGGLE_TYPE,
  CWM_MENU_SPLITTER_TYPE,
  CWM_MENU_CASCADE_TYPE,
};

enum CwmMenuState {
  CWM_MENU_ACTIVE,
  CWM_MENU_INACTIVE,
  CWM_MENU_COMPLETED,
};

class CwmMenuDef {
 public:
  CwmMenuDef();
  CwmMenuDef(const CwmMenuDef &menu_def);
 ~CwmMenuDef();

  void addTitle(const std::string &text);
  void addButton(const std::string &image, const std::string &text, int mnemonic,
                 const std::string &accelerator, CwmMenuProc proc, CwmData data1=0,
                 CwmData data2=0, CwmData data3=0, CwmData data4=0, CwmData data5=0,
                 CwmData data6=0);
  void addToggle(const std::string &image, const std::string &text, int mnemonic,
                 const std::string &accelerator, CwmMenuProc proc, CwmData data1=0,
                 CwmData data2=0, CwmData data3=0, CwmData data4=0, CwmData data5=0,
                 CwmData data6=0);
  void addSplitter(const std::string &text);
  void addCascade(const std::string &text, CwmMenuDef *menu_def);
  void addEnd();

  void addEntry(CwmMenuEntry *entry);

  void setClientData(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon);

  int getSize();

  CwmMenuEntry *getEntry(int i);

 private:
  CwmMenuDef &operator=(const CwmMenuDef &menu_def);

 private:
  typedef std::vector<CwmMenuEntry *> MenuEntryList;

  MenuEntryList entries_;
};

class CwmMenuEntry {
 public:
  CwmMenuEntry(CwmMenuItemType type, const std::string &image, const std::string &text,
               int mnemonic, const std::string &accelerator, CwmMenuProc proc,
               CwmData data1, CwmData data2, CwmData data3, CwmData data4, CwmData data5,
               CwmData data6);
  CwmMenuEntry(const CwmMenuEntry &entry);

  virtual ~CwmMenuEntry();

  void setClientData(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon);

  CwmMenuItemType getType       () const { return type_       ; }
  std::string     getImage      () const { return image_      ; }
  std::string     getText       () const { return text_       ; }
  int             getMnemonic   () const { return mnemonic_   ; }
  std::string     getAccelerator() const { return accelerator_; }
  CwmMenuProc     getProc       () const { return proc_       ; }
  CwmData         getData1      () const { return data1_      ; }
  CwmData         getData2      () const { return data2_      ; }
  CwmData         getData3      () const { return data3_      ; }
  CwmData         getData4      () const { return data4_      ; }
  CwmData         getData5      () const { return data5_      ; }
  CwmData         getData6      () const { return data6_      ; }
  bool            getSelected   () const { return selected_   ; }

  void setSelected(bool flag) { selected_ = flag; }

 private:
  CwmMenuEntry &operator=(const CwmMenuEntry &entry);

 private:
  CwmMenuItemType type_;
  std::string     image_;
  std::string     text_;
  int             mnemonic_;
  std::string     accelerator_;
  CwmMenuProc     proc_;
  CwmData         data1_;
  CwmData         data2_;
  CwmData         data3_;
  CwmData         data4_;
  CwmData         data5_;
  CwmData         data6_;
  bool            selected_;
};

class CwmMenuTitleEntry : public CwmMenuEntry {
 public:
  CwmMenuTitleEntry(const std::string &text) :
   CwmMenuEntry(CWM_MENU_TITLE_TYPE, "", text, '\0', "", 0, 0, 0, 0, 0, 0, 0) {
  }
};

class CwmMenuButtonEntry : public CwmMenuEntry {
 public:
  CwmMenuButtonEntry(const std::string &image, const std::string &text, int mnemonic,
                     const std::string &accelerator, CwmMenuProc proc, CwmData data1 = 0,
                     CwmData data2 = 0, CwmData data3 = 0, CwmData data4 = 0,
                     CwmData data5 = 0, CwmData data6 = 0) :
   CwmMenuEntry(CWM_MENU_BUTTON_TYPE, image, text, mnemonic, accelerator, (CwmMenuProc) proc,
                (CwmData) data1, data2, data3, data4, data5, data6) {
  }
};

class CwmMenuToggleEntry : public CwmMenuEntry {
 public:
  CwmMenuToggleEntry(const std::string & /*image*/, const std::string &text, int mnemonic,
                     const std::string &accelerator, CwmMenuProc proc, CwmData data1 = 0,
                     CwmData data2 = 0, CwmData data3 = 0, CwmData data4 = 0,
                     CwmData data5 = 0, CwmData data6 = 0) :
   CwmMenuEntry(CWM_MENU_TOGGLE_TYPE, "", text, mnemonic, accelerator, (CwmMenuProc) proc,
                (CwmData) data1, data2, data3, data4, data5, data6) {
  }
};

class CwmMenuSplitterEntry : public CwmMenuEntry {
 public:
  CwmMenuSplitterEntry(const std::string &text) :
   CwmMenuEntry(CWM_MENU_SPLITTER_TYPE, "", text, '\0', "", 0, 0, 0, 0, 0, 0, 0) {
  }
};

class CwmMenuCascadeEntry : public CwmMenuEntry {
 public:
  CwmMenuCascadeEntry(const std::string &text, CwmMenuDef *menu_def) :
   CwmMenuEntry(CWM_MENU_CASCADE_TYPE, "", text, '\0', "", 0, (CwmData) menu_def, 0, 0, 0, 0, 0) {
  }
};

class CwmMenuKeyData;

class CwmMenu {
 public:
  static void processNamedMenu(CwmScreen &screen, const std::string &name);
  static void processNamedMenu(CwmWMWindow *window, const std::string &name);

  static void processMenu(CwmDeskIcon *icon, CwmMenuDef *menu_def);

  static void processWindowMenu(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                                CHAlignType halign=CHALIGN_TYPE_LEFT,
                                CVAlignType valign=CVALIGN_TYPE_TOP);
  static void processWindowMenu(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                                int x, int y, CHAlignType halign=CHALIGN_TYPE_LEFT,
                                CVAlignType valign=CVALIGN_TYPE_TOP);

  static void nextItemProc(CwmMenu *menu);
  static void prevItemProc(CwmMenu *menu);
  static void startCascadeProc(CwmMenu *menu);
  static void endCascadeProc(CwmMenu *menu);
  static void acceptItemProc(CwmMenu *menu);
  static void endMenuProc(CwmMenu *menu);

  static bool isStayUp() { return menu_stay_up_; }

  static bool isButtonDown() { return button_down_; }
  static void buttonPress() { button_down_ = true; }
  static void buttonRelease() { button_down_ = false; }

  static void setSelectedItem(CwmMenuItem *item) { selected_item_ = item; }

  static CwmMenuKeyData *getKeyData(XKeyPressedEvent *event);

  CwmMenu(CwmScreen &screen, CwmMenu *parent, CwmMenuDef *menu_def, int x, int y,
          CHAlignType halign, CVAlignType valign);
 ~CwmMenu();

  void map();
  void unmap();
  void redraw();
  void drawBackground();
  void nextItem();
  void prevItem();
  void complete();

  CwmMenuItem *getAnyMenuItemForWindow(Window xwin);
  CwmMenuItem *getMenuItemForWindow(CwmWindow *xwindow);
  CwmMenuItem *getMenuItemForWindow(Window xwin);

  CwmMenu *getParent() const { return parent_; }

  CwmScreen &getScreen() const { return screen_; }

  int getNumItems() const { return num_items_; }

  CwmMenuItem *getItem(int i) const { return items_[i]; }

  CwmGraphics *getGraphics() const { return graphics_; }

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  int getItemWidth() const { return item_width_; }

  bool isActive() const { return state_ == CWM_MENU_ACTIVE; }
  bool isCompleted() const { return state_ == CWM_MENU_COMPLETED; }
  bool isInactive() const { return state_ == CWM_MENU_INACTIVE; }

  void setActive() { state_ = CWM_MENU_ACTIVE; }
  void setCompleted() { state_ = CWM_MENU_COMPLETED; }
  void setInactive() { state_ = CWM_MENU_INACTIVE; }

  CwmMenuItem *getCurrent() const { return current_; }
  void setCurrent(CwmMenuItem *item) { current_ = item; }

  CwmMenu *getCascade() const { return cascade_; }
  void setCascade(CwmMenu *cascade) { cascade_ = cascade; }

 private:
  static void processWindowMenu1(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                                 int x, int y, CHAlignType halign, CVAlignType valign);

 private:
  CwmScreen    &screen_;
  CwmMenu      *parent_;
  CwmMenuItem **items_;
  int           num_items_;
  CwmMenuItem  *current_;
  int           x_;
  int           y_;
  int           width_;
  int           height_;
  int           item_width_;
  CwmWindow   *xwindow_;
  CwmGraphics  *graphics_;
  CwmGraphics  *title_graphics_;
  CwmGraphics  *stipple_graphics_;
  CwmMenuState  state_;
  CwmMenu      *cascade_;

  static bool         menu_stay_up_;
  static bool         button_down_;
  static CwmMenuItem *selected_item_;
  static CwmMenu     *last_menu_;
};

class CwmMenuItem {
 public:
  CwmMenuItem(CwmMenu *menu, CwmGraphics *graphics, CwmGraphics *stipple_graphics,
              CwmMenuEntry *entry);
 ~CwmMenuItem();

  void setSize();

  void createWindow(CwmScreen &screen, CwmWindow *menu_xwindow, int menu_y, int menu_width);

  bool isCascadeMenu();
  void processCascadeMenu(CwmMenu *menu);

  void redraw();

  void enter();
  void leave();

  bool isMnemonicEvent(XKeyPressedEvent *event);
  bool isAcceleratorEvent(XKeyPressedEvent *event);

  void select();

  void drawActive();
  void drawInactive();
  void drawImage();
  void drawText(CwmGraphics *graphics);
  void drawSplitter();

  int getWidth() const { return width_; }
  int getItemWidth() const { return item_width_; }
  int getHeight() const { return height_; }

  CwmWindow *getXWindow() const { return xwindow_; }

  bool isTitle() const { return type_ == CWM_MENU_TITLE_TYPE; }
  bool isButton() const { return type_ == CWM_MENU_BUTTON_TYPE; }
  bool isToggle() const { return type_ == CWM_MENU_TOGGLE_TYPE; }

  bool isCascade() const {
    return type_ == CWM_MENU_CASCADE_TYPE && data1_ != 0;
  }

  bool isActiveButton() const {
    return type_ == CWM_MENU_BUTTON_TYPE && proc_ != 0;
  }

  void setSelected(bool flag) { selected_ = flag; }
  bool getSelected() const { return selected_; }

  bool isActive()  const {
    return isActiveButton() || isToggle() || isCascade();
  }

 private:
  CwmMenu         *menu_;
  CwmGraphics     *graphics_;
  CwmGraphics     *stipple_graphics_;
  std::string      image_name_;
  std::string      text_;
  int              mnemonic_;
  CXNamedEvent    *accelerator_event_;
  CwmMenuItemType  type_;
  bool             selected_;
  CwmMenuProc      proc_;
  CwmData          data1_;
  CwmData          data2_;
  CwmData          data3_;
  CwmData          data4_;
  CwmData          data5_;
  CwmData          data6_;
  int              width_;
  int              height_;
  int              item_width_;
  CwmWindow       *xwindow_;
  CwmImage        *image_;
};

#define CwmNamedMenuMgrInst CwmNamedMenuMgr::getInstance()

class CwmNamedMenu;

class CwmNamedMenuMgr {
 public:
  static CwmNamedMenuMgr *getInstance();

  void define(const std::string &name, CwmMenuDef *menu_def);

  CwmMenuDef *lookupMenuDef(const std::string &name);
  CwmNamedMenu *lookup(const std::string &name);

  void deleteAll();

  CwmMenuDef *createDefaultRootMenu();

 private:
  typedef std::list<CwmNamedMenu *> NamedMenuList;

  NamedMenuList named_menus_;
};

class CwmNamedMenu {
 public:
  CwmNamedMenu(const std::string &name, CwmMenuDef *menu_def=0);
 ~CwmNamedMenu();

  std::string  getName() const { return name_; }
  CwmMenuDef  *getMenuDef() const { return menu_def_; }

 private:
  std::string  name_;
  CwmMenuDef  *menu_def_;
};

#endif
