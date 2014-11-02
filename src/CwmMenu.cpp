#include <CwmI.h>

typedef void (*CwmMenuKeyProc) (CwmMenu *);

class CwmMenuKeyData {
 public:
  static CwmMenuKeyData *getKeyData(XKeyPressedEvent *event);

  CwmMenuKeyData(const std::string &key1, CwmMenuKeyProc proc1);
 ~CwmMenuKeyData();

  void init();

  void invoke(CwmMenu *menu);

 private:
  static CwmMenuKeyData menu_key_data_[];

  std::string     key_;
  CwmMenuKeyProc  proc_;
  CXNamedEvent   *event_;
};

CwmMenuKeyData
CwmMenuKeyData::menu_key_data_[] = {
  CwmMenuKeyData("<Key>Up"    , &CwmMenu::prevItemProc    ),
  CwmMenuKeyData("<Key>Down"  , &CwmMenu::nextItemProc    ),
  CwmMenuKeyData("<Key>Left"  , &CwmMenu::endCascadeProc  ),
  CwmMenuKeyData("<Key>Right" , &CwmMenu::startCascadeProc),
  CwmMenuKeyData("<Key>Return", &CwmMenu::acceptItemProc  ),
  CwmMenuKeyData("<Key>space" , &CwmMenu::acceptItemProc  ),
  CwmMenuKeyData("<Key>Escape", &CwmMenu::endMenuProc     ),
  CwmMenuKeyData(""           , 0                      ),
};

bool         CwmMenu::menu_stay_up_  = false;
bool         CwmMenu::button_down_   = false;
CwmMenuItem *CwmMenu::selected_item_ = 0;
CwmMenu     *CwmMenu::last_menu_     = 0;

CwmMenuKeyData::
CwmMenuKeyData(const std::string &key, CwmMenuKeyProc proc) :
 key_(key), proc_(proc), event_(0)
{
}

CwmMenuKeyData::
~CwmMenuKeyData()
{
  delete event_;
}

void
CwmMenuKeyData::
init()
{
  if (! event_ && key_ != "")
    event_ = new CXNamedEvent(key_);
}

CwmMenuKeyData *
CwmMenuKeyData::
getKeyData(XKeyPressedEvent *event)
{
  XEvent *event1 = (XEvent *) event;

  for (int i = 0; menu_key_data_[i].key_ != ""; i++) {
    menu_key_data_[i].init();

    if (! menu_key_data_[i].event_)
      continue;

    if (menu_key_data_[i].event_->matchEvent(event1))
      return &menu_key_data_[i];
  }

  return 0;
}

void
CwmMenuKeyData::
invoke(CwmMenu *menu)
{
  if (proc_ != 0)
    proc_(menu);
}

CwmMenuDef::
CwmMenuDef()
{
}

CwmMenuDef::
CwmMenuDef(const CwmMenuDef &menu_def)
{
  MenuEntryList::const_iterator pentry1 = menu_def.entries_.begin();
  MenuEntryList::const_iterator pentry2 = menu_def.entries_.end  ();

  for ( ; pentry1 != pentry2; ++pentry1) {
    CwmMenuEntry *entry = new CwmMenuEntry(**pentry1);

    addEntry(entry);
  }
}

CwmMenuDef::
~CwmMenuDef()
{
  std::for_each(entries_.begin(), entries_.end(), CDeletePointer());
}

void
CwmMenuDef::
addTitle(const std::string &text)
{
  CwmMenuTitleEntry *entry = new CwmMenuTitleEntry(text);

  addEntry(entry);
}

void
CwmMenuDef::
addButton(const std::string &image, const std::string &text, int mnemonic,
          const std::string &accelerator, CwmMenuProc proc, CwmData data1, CwmData data2,
          CwmData data3, CwmData data4, CwmData data5, CwmData data6)
{
  CwmMenuButtonEntry *entry =
    new CwmMenuButtonEntry(image, text, mnemonic, accelerator, proc,
                           data1, data2, data3, data4, data5, data6);

  addEntry(entry);
}

void
CwmMenuDef::
addToggle(const std::string &image, const std::string &text, int mnemonic,
          const std::string &accelerator, CwmMenuProc proc, CwmData data1, CwmData data2,
          CwmData data3, CwmData data4, CwmData data5, CwmData data6)
{
  CwmMenuToggleEntry *entry =
    new CwmMenuToggleEntry(image, text, mnemonic, accelerator, proc,
                           data1, data2, data3, data4, data5, data6);

  addEntry(entry);
}

void
CwmMenuDef::
addSplitter(const std::string &text)
{
  CwmMenuSplitterEntry *entry = new CwmMenuSplitterEntry(text);

  addEntry(entry);
}

void
CwmMenuDef::
addCascade(const std::string &text, CwmMenuDef *menu_def)
{
  CwmMenuCascadeEntry *entry = new CwmMenuCascadeEntry(text, menu_def);

  addEntry(entry);
}

void
CwmMenuDef::
addEntry(CwmMenuEntry *entry)
{
  entries_.push_back(entry);
}

void
CwmMenuDef::
setClientData(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon)
{
  MenuEntryList::const_iterator pentry1 = entries_.begin();
  MenuEntryList::const_iterator pentry2 = entries_.end  ();

  for ( ; pentry1 != pentry2; ++pentry1)
    (*pentry1)->setClientData(screen, window, icon);
}

int
CwmMenuDef::
getSize()
{
  return entries_.size();
}

CwmMenuEntry *
CwmMenuDef::
getEntry(int i)
{
  if (i < 0 || i >= (int) entries_.size())
    throw "Invalid Menu Def Index";

  return entries_[i];
}

CwmMenuEntry::
CwmMenuEntry(CwmMenuItemType type, const std::string &image, const std::string &text,
             int mnemonic, const std::string &accelerator, CwmMenuProc proc, CwmData data1,
             CwmData data2, CwmData data3, CwmData data4, CwmData data5, CwmData data6) :
 type_(type), image_(image), text_(text), mnemonic_(mnemonic),
 accelerator_(accelerator), proc_(proc), data1_(data1), data2_(data2),
 data3_(data3), data4_(data4), data5_(data5), data6_(data6), selected_(false)
{
}

CwmMenuEntry::
CwmMenuEntry(const CwmMenuEntry &entry)
{
  type_        = entry.type_;
  image_       = entry.image_;
  text_        = entry.text_;
  mnemonic_    = entry.mnemonic_;
  accelerator_ = entry.accelerator_;
  proc_        = entry.proc_;
  data1_       = entry.data1_;
  data2_       = entry.data2_;
  data3_       = entry.data3_;
  data4_       = entry.data4_;
  data5_       = entry.data5_;
  data6_       = entry.data6_;
  selected_    = false;

  if (type_ == CWM_MENU_CASCADE_TYPE) {
    CwmMenuDef *menu_def = (CwmMenuDef *) entry.data1_;

    data1_ = (CwmData ) new CwmMenuDef(*menu_def);
  }
}

CwmMenuEntry::
~CwmMenuEntry()
{
  if (type_ == CWM_MENU_CASCADE_TYPE) {
    CwmMenuDef *menu_def = (CwmMenuDef *) data1_;

    delete menu_def;
  }
}

void
CwmMenuEntry::
setClientData(CwmScreen *screen, CwmWMWindow *window, CwmDeskIcon *icon)
{
  if      (type_ == CWM_MENU_BUTTON_TYPE) {
    if (proc_ == (CwmMenuProc) CwmFunctionDef::processProc) {
      CwmFunctionDef *function = (CwmFunctionDef *) data1_;

      function->setObjects(screen, window, icon);
    }
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE) {
    if (proc_ == (CwmMenuProc) CwmFunctionDef::processProc) {
      CwmFunctionDef *function = (CwmFunctionDef *) data1_;

      function->setObjects(screen, window, icon);
    }
  }
  else if (type_ == CWM_MENU_CASCADE_TYPE) {
    CwmMenuDef *menu_def = (CwmMenuDef *) data1_;

    menu_def->setClientData(screen, window, icon);
  }
}

void
CwmMenu::
processNamedMenu(CwmScreen &screen, const std::string &name)
{
  CwmMenuDef *menu_def = CwmNamedMenuMgrInst->lookupMenuDef(name);

  if (! menu_def) {
    CwmMachineInst->logf("Named Menu %s not found\n", name.c_str());
    return;
  }

  menu_def->setClientData(&screen, 0, 0);

  processWindowMenu(screen, screen.getRoot(), menu_def);
}

void
CwmMenu::
processNamedMenu(CwmWMWindow *window, const std::string &name)
{
  CwmMenuDef *menu_def = CwmNamedMenuMgrInst->lookupMenuDef(name);

  if (! menu_def) {
    CwmMachineInst->logf("Named Menu %s not found\n", name.c_str());
    return;
  }

  CwmScreen &screen = window->getScreen();

  screen.uninstallColormap();

  menu_def->setClientData(0, window, 0);

  processWindowMenu(screen, window->getFrameWindow(), menu_def);

  screen.installColormap(window);
}

void
CwmMenu::
processMenu(CwmDeskIcon *icon, CwmMenuDef *menu_def)
{
  menu_def->setClientData(0, 0, icon);

  processWindowMenu(icon->getScreen(), icon->getXWindow(), menu_def);
}

void
CwmMenu::
processWindowMenu(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                  CHAlignType halign, CVAlignType valign)
{
  int x, y;

  xwindow->getPointerPosition(&x, &y);

  processWindowMenu1(screen, xwindow, menu_def, x, y, halign, valign);
}

void
CwmMenu::
processWindowMenu(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                  int x, int y, CHAlignType halign, CVAlignType valign)
{
  int root_x, root_y;

  xwindow->coordsToRoot(x, y, &root_x, &root_y);

  processWindowMenu1(screen, xwindow, menu_def, root_x, root_y, halign, valign);
}

void
CwmMenu::
processWindowMenu1(CwmScreen &screen, CwmWindow *xwindow, CwmMenuDef *menu_def,
                   int x, int y, CHAlignType halign, CVAlignType valign)
{
//if (last_menu_ != 0) {
//  delete last_menu_;
//
//  last_menu_ = 0;
//}

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask;

  if (! xwindow->grab(event_mask, CWM_CURSOR_SELECT))
    return;

  CwmMenu *menu = new CwmMenu(screen, 0, menu_def, x, y, halign, valign);

  menu->map();

  menu->redraw();

  CwmMachineInst->flushEvents();

  selected_item_ = 0;

  int last_event_type = CwmEventMgrInst->getLastEventType();

  XEvent *event;

  if (last_event_type == ButtonPress) {
    menu_stay_up_ = false;

    Time start_time = CwmEventMgrInst->getLastEventTime();
    Time end_time   = start_time;

    while ((end_time - start_time) < 200000 &&
           CwmEventMgrInst->eventPending()) {
      event = CwmEventMgrInst->processNextEvent();

      if (event->type == ButtonRelease) {
        menu_stay_up_ = true;
        break;
      }

      CwmEventMgrInst->processEvent(event);

      end_time = CwmEventMgrInst->getLastEventTime();
    }
  }
  else
    menu_stay_up_ = true;

  int          i;
  CwmMenuItem *menu_item;

  if (menu_stay_up_) {
    menu->nextItem();

    button_down_ = false;

    while (menu->isActive()) {
      while (button_down_ && menu->isActive() &&
             menu->current_ != 0 && menu->current_->isCascadeMenu())
        menu->current_->processCascadeMenu(menu);

      if (! menu->isActive())
        break;

      event = CwmEventMgrInst->processNextEvent();

      if      (event->type == ButtonPress) {
        if (event->xbutton.button != 1 &&
            event->xbutton.button != 3)
          continue;

        Window event_window = CwmMachineInst->getEventWindow(event);

        menu_item = menu->getMenuItemForWindow(event_window);

        if (menu->current_ != 0)
          menu->current_->leave();

        if (menu_item != 0)
          menu_item->enter();

        selected_item_ = menu_item;

        button_down_ = true;
      }
      else if (event->type == ButtonRelease) {
        menu->complete();

        button_down_ = false;
      }
      else if (event->type == EnterNotify) {
        if (button_down_) {
          Window event_window = CwmMachineInst->getEventWindow(event);

          menu_item = menu->getMenuItemForWindow(event_window);

          if (menu_item != 0)
            menu_item->enter();

          if (menu_item != 0 && ! menu_item->isCascadeMenu())
            selected_item_ = menu_item;
        }
      }
      else if (event->type == LeaveNotify) {
        if (button_down_) {
          Window event_window = CwmMachineInst->getEventWindow(event);

          menu_item = menu->getMenuItemForWindow(event_window);

          if (menu_item != 0)
            menu_item->leave();

          selected_item_ = 0;
        }
      }
      else if (event->type == KeyPress) {
        XKeyPressedEvent *event1 = (XKeyPressedEvent *) event;

        if (CwmXEventInst->isEscapeKeyEvent(event1))
          menu->complete();

        CwmMenuKeyData *key_data = CwmMenuKeyData::getKeyData(event1);

        if (key_data != 0) {
          key_data->invoke(menu);
          continue;
        }

        for (i = 0; i < menu->num_items_; i++) {
          if (menu->items_[i]->isMnemonicEvent(event1))
            break;

          if (menu->items_[i]->isAcceleratorEvent(event1))
            break;
        }

        if (i < menu->num_items_) {
          selected_item_ = menu->items_[i];

          menu->complete();
        }
      }
      else if (event->type == KeyRelease) {
        XKeyReleasedEvent *event1 = (XKeyReleasedEvent *) event;

        if (CwmXEventInst->isEscapeKeyEvent(event1))
          menu->complete();
      }
      else {
        CwmMachineInst->logf("Unhandled event %d\n", event->type);
      }
    }
  }
  else {
    menu_stay_up_ = false;

    while (menu->isActive()) {
      event = CwmEventMgrInst->processNextEvent();

      if      (event->type == ButtonPress) {
        if (event->xbutton.button != 1 &&
            event->xbutton.button != 3)
          continue;

        Window event_window = CwmMachineInst->getEventWindow(event);

        menu_item = menu->getMenuItemForWindow(event_window);

        selected_item_ = menu_item;

        menu->complete();
      }
      else if (event->type == ButtonRelease) {
        menu->complete();
      }
      else if (event->type == EnterNotify) {
        Window event_window = CwmMachineInst->getEventWindow(event);

        menu_item = menu->getMenuItemForWindow(event_window);

        if (menu_item != 0)
          menu_item->enter();

        if (menu_item != 0 && ! menu_item->isCascadeMenu())
          selected_item_ = menu_item;
      }
      else if (event->type == LeaveNotify) {
        Window event_window = CwmMachineInst->getEventWindow(event);

        menu_item = menu->getMenuItemForWindow(event_window);

        if (menu_item != 0)
          menu_item->leave();

        selected_item_ = 0;
      }
      else if (event->type == KeyPress) {
        XKeyPressedEvent *event1 = (XKeyPressedEvent *) event;

        if (CwmXEventInst->isEscapeKeyEvent(event1))
          menu->complete();
      }
      else if (event->type == KeyRelease) {
        XKeyReleasedEvent *event1 = (XKeyReleasedEvent *) event;

        if (CwmXEventInst->isEscapeKeyEvent(event1))
          menu->complete();
      }
      else {
        CwmMachineInst->logf("Unhandled event %d\n", event->type);
      }

      if (menu->cascade_ != 0) {
        if      (menu->cascade_->isCompleted())
          menu->complete();
        else if (menu->cascade_->isInactive())
          menu->cascade_ = 0;
      }

      while (menu->isActive() &&
             menu->current_ != 0 && menu->current_->isCascadeMenu())
        menu->current_->processCascadeMenu(menu);
    }
  }

  xwindow->ungrab();

  CwmIdleInst->disable();

  menu->unmap();

  CwmMachineInst->flushEvents();

  if (selected_item_ != 0)
    selected_item_->select();

  CwmIdleInst->enable();

  delete selected_item_;

  selected_item_ = 0;

  last_menu_ = menu;
}

void
CwmMenu::
nextItemProc(CwmMenu *menu)
{
  menu->nextItem();
}

void
CwmMenu::
prevItemProc(CwmMenu *menu)
{
  menu->prevItem();
}

void
CwmMenu::
startCascadeProc(CwmMenu *menu)
{
  if (menu->current_ != 0 && menu->current_->isCascadeMenu())
    menu->current_->processCascadeMenu(menu);
  else
    menu->nextItem();
}

void
CwmMenu::
endCascadeProc(CwmMenu *menu)
{
  if (menu->parent_ != 0) {
    menu->setCurrent(0);

    selected_item_ = 0;

    menu->state_ = CWM_MENU_INACTIVE;
  }
  else
    menu->prevItem();
}

void
CwmMenu::
acceptItemProc(CwmMenu *menu)
{
  if (! menu->current_)
    return;

  selected_item_ = menu->current_;

  menu->complete();
}

void
CwmMenu::
endMenuProc(CwmMenu *menu)
{
  menu->setCurrent(0);

  selected_item_ = 0;

  menu->complete();
}

CwmMenu::
CwmMenu(CwmScreen &screen, CwmMenu *parent, CwmMenuDef *menu_def1,
        int x1, int y1, CHAlignType halign, CVAlignType valign) :
 screen_(screen), parent_(parent)
{
  Pixel fg = CwmResourceDataInst->getMenuForeground(screen);
  Pixel bg = CwmResourceDataInst->getMenuBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getMenuFont(screen);

  graphics_ = new CwmGraphics(screen, font, color);

  //------

  fg = CwmResourceDataInst->getMenuTitleForeground(screen);
  bg = CwmResourceDataInst->getMenuTitleBackground(screen);

  CwmColor *title_color = screen.getColor(fg, bg);

  CwmFont *title_font = CwmResourceDataInst->getMenuTitleFont(screen);

  title_graphics_ = new CwmGraphics(screen, title_font, title_color);

  //------

  stipple_graphics_ = new CwmGraphics(screen, font, color);

  stipple_graphics_->setStippled();

  //------

  num_items_ = menu_def1->getSize();

  items_ = new CwmMenuItem * [num_items_];

  current_ = 0;

  x_          = x1;
  y_          = y1;
  width_      = 0;
  height_     = 0;
  item_width_ = 0;

  for (int i = 0; i < num_items_; i++) {
    CwmMenuItemType type = menu_def1->getEntry(i)->getType();

    if (type == CWM_MENU_TITLE_TYPE)
      items_[i] = new CwmMenuItem(this, title_graphics_, stipple_graphics_,
                                  menu_def1->getEntry(i));
    else
      items_[i] = new CwmMenuItem(this, graphics_, stipple_graphics_,
                                  menu_def1->getEntry(i));

    width_      = std::max(     width_, items_[i]->getWidth    ());
    item_width_ = std::max(item_width_, items_[i]->getItemWidth());

    height_ += items_[i]->getHeight();
  }

  width_  += 12;
  height_ += 4;

  if      (halign == CHALIGN_TYPE_RIGHT)
    x_ -= width_;
  else if (halign == CHALIGN_TYPE_CENTER)
    x_ -= width_/2;

  if      (valign == CVALIGN_TYPE_BOTTOM)
    y_ -= height_;
  else if (valign == CVALIGN_TYPE_CENTER)
    y_ -= height_/2;

  x_ = std::min(std::max(x_, 0), screen.getWidth () - width_ );
  y_ = std::min(std::max(y_, 0), screen.getHeight() - height_);

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ = new CwmWindow(screen, screen.getRoot(), x_, y_, width_, height_,
                           event_mask, CWM_CURSOR_MENU);

  xwindow_->setBackground(graphics_);

  xwindow_->setSaveUnder(true);

  //------

  int y2 = 2;

  for (int j = 0; j < num_items_; j++) {
    items_[j]->createWindow(screen, xwindow_, y2, width_);

    y2 += items_[j]->getHeight();
  }

  //------

  cascade_ = 0;
}

CwmMenu::
~CwmMenu()
{
  for (int i = 0; i < num_items_; i++)
    delete items_[i];

  delete [] items_;

  delete xwindow_;

  delete graphics_;
  delete stipple_graphics_;
}

void
CwmMenu::
map()
{
  xwindow_->mapSubwindows();

  xwindow_->map();

  state_ = CWM_MENU_ACTIVE;

  if (parent_ != 0)
    parent_->cascade_ = this;
}

void
CwmMenu::
unmap()
{
  xwindow_->unmap();

  state_ = CWM_MENU_INACTIVE;

  if (parent_ != 0)
    parent_->cascade_ = 0;
}

void
CwmMenu::
redraw()
{
  drawBackground();

  for (int i = 0; i < num_items_; i++)
    items_[i]->redraw();
}

void
CwmMenu::
drawBackground()
{
  graphics_->drawButtonOut(xwindow_, 0, 0, width_, height_, 2);
}

void
CwmMenu::
nextItem()
{
  int i;

  if (current_ != 0) {
    for (i = 0; i < num_items_; i++)
      if (items_[i] == current_)
        break;

    if (i >= num_items_)
      i = -1;
  }
  else
    i = -1;

  i++;

  for ( ; i < num_items_; i++)
    if (items_[i]->isActive())
      break;

  if (i >= num_items_)
    return;

  if (current_ != 0)
    current_->drawInactive();

  current_ = items_[i];

  current_->drawActive();
}

void
CwmMenu::
prevItem()
{
  int i;

  if (current_ != 0) {
    for (i = 0; i < num_items_; i++)
      if (items_[i] == current_)
        break;

    if (i >= num_items_)
      i = num_items_;
  }
  else
    i = num_items_;

  i--;

  for ( ; i >= 0; i--)
    if (items_[i]->isActive())
      break;

  if (i < 0)
    return;

  if (current_ != 0)
    current_->drawInactive();

  current_ = items_[i];

  current_->drawActive();
}

void
CwmMenu::
complete()
{
  CwmMenu *menu = this;

  while (menu != 0) {
    menu->state_ = CWM_MENU_COMPLETED;

    menu = menu->parent_;
  }
}

CwmMenuItem *
CwmMenu::
getAnyMenuItemForWindow(Window xwin)
{
  CwmMenu *menu1 = this;

  while (menu1 != 0) {
    CwmMenuItem *menu_item = menu1->getMenuItemForWindow(xwin);

    if (menu_item != 0)
      return menu_item;

    menu1 = menu1->parent_;
  }

  return 0;
}

CwmMenuItem *
CwmMenu::
getMenuItemForWindow(CwmWindow *xwindow)
{
  for (int i = 0; i < num_items_; i++) {
    CwmWindow *xwindow1 = items_[i]->getXWindow();

    if (xwindow1 == xwindow)
      return items_[i];
  }

  return 0;
}

CwmMenuItem *
CwmMenu::
getMenuItemForWindow(Window xwin)
{
  for (int i = 0; i < num_items_; i++) {
    CwmWindow *xwindow1 = items_[i]->getXWindow();

    if (xwindow1->getXWin() == xwin)
      return items_[i];
  }

  return 0;
}

CwmMenuItem::
CwmMenuItem(CwmMenu *menu, CwmGraphics *graphics, CwmGraphics *stipple_graphics,
            CwmMenuEntry *entry) :
 menu_(menu), graphics_(graphics), stipple_graphics_(stipple_graphics)
{
  image_name_ = entry->getImage();
  text_       = entry->getText ();

  mnemonic_ = entry->getMnemonic();

  if (entry->getAccelerator() != "")
    accelerator_event_ = new CXNamedEvent(entry->getAccelerator());
  else
    accelerator_event_ = 0;

  type_ = entry->getType();

  selected_ = entry->getSelected();

  proc_  = entry->getProc();
  data1_ = entry->getData1();
  data2_ = entry->getData2();
  data3_ = entry->getData3();
  data4_ = entry->getData4();
  data5_ = entry->getData5();
  data6_ = entry->getData6();

  //------

  if (image_name_ != "") {
    image_ = CwmImageMgrInst->getImage(menu->getScreen(), image_name_);

    if (image_ != 0 && (image_->getWidth() > 16 || image_->getHeight() > 16))
      image_ = CwmImageMgrInst->getImage(menu->getScreen(), image_name_, 16, 16);
  }
  else
    image_ = 0;

  //------

  setSize();

  //------

  xwindow_ = 0;
}

CwmMenuItem::
~CwmMenuItem()
{
  delete accelerator_event_;
}

void
CwmMenuItem::
setSize()
{
  width_  = 0;
  height_ = 0;

  if (image_ != 0) {
    width_ += image_->getWidth() + 4;

    height_ = image_->getHeight() + 4;
  }

  std::string accelerator_text;

  if (accelerator_event_ != 0)
    accelerator_text = accelerator_event_->getText();

  if      (type_ == CWM_MENU_BUTTON_TYPE) {
    int height1;

    graphics_->getTextSize(text_, &item_width_, &height1);

    height_ = std::max(height_, height1 + 6);

    width_ += item_width_;

    if (accelerator_text != "") {
      int width1;

      graphics_->getTextSize(accelerator_text, &width1, &height1);

      width_ += width1 + 8;

      height_ = std::max(height_, height1 + 6);
    }
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE) {
    int height1;

    width_ += 14;

    graphics_->getTextSize(text_, &item_width_, &height1);

    height_ = std::max(height_, height1 + 6);

    width_ += item_width_;

    if (accelerator_text != "") {
      int width1;

      graphics_->getTextSize(accelerator_text, &width1, &height1);

      width_ += width1 + 8;

      height_ = std::max(height_, height1 + 6);
    }
  }
  else if (type_ == CWM_MENU_CASCADE_TYPE) {
    int height1;

    graphics_->getTextSize(text_, &item_width_, &height1);

    width_ += item_width_ + 10;

    height_ = std::max(height_, height1 + 6);
  }
  else if (type_ == CWM_MENU_TITLE_TYPE) {
    int height1;

    graphics_->getTextSize(text_, &item_width_, &height1);

    width_ += item_width_;

    height_ = std::max(height_, height1 + 6);
  }
  else if (type_ == CWM_MENU_SPLITTER_TYPE) {
    item_width_ = 0;

    width_ = menu_->getWidth();

    height_ = std::max(height_, 4);
  }
}

void
CwmMenuItem::
createWindow(CwmScreen &screen, CwmWindow *menu_xwindow, int menu_y, int menu_width)
{
  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ = new CwmWindow(screen, menu_xwindow, 2, menu_y, (menu_width - 4), height_,
                           event_mask, CWM_CURSOR_MENU);

  xwindow_->setBackground(graphics_);
}

bool
CwmMenuItem::
isCascadeMenu()
{
  return (type_ == CWM_MENU_CASCADE_TYPE && data1_ != 0);
}

void
CwmMenuItem::
processCascadeMenu(CwmMenu *parent)
{
  CwmMenuDef *menu_def = (CwmMenuDef *) data1_;

  int root_x, root_y;

  xwindow_->coordsToRoot(2*parent->getWidth()/3, height_/2, &root_x, &root_y);

  CwmMenu *menu = new CwmMenu(parent->getScreen(), parent, menu_def, root_x, root_y,
                              CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);

  menu->map();

  menu->redraw();

  if (CwmMenu::isStayUp()) {
    int i;

    menu->nextItem();

    while (menu->isActive()) {
      while (CwmMenu::isButtonDown() && menu->isActive()) {
        CwmMenuItem *current = menu->getCurrent();

        if (! current || ! current->isCascadeMenu())
          break;

        current->processCascadeMenu(menu);
      }

      if (! menu->isActive())
        break;

      XEvent *event = CwmEventMgrInst->processNextEvent();

      if      (event->type == ButtonPress) {
        if (event->xbutton.button != 1 &&
            event->xbutton.button != 3)
          continue;

        Window event_window = CwmMachineInst->getEventWindow(event);

        CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

        CwmMenuItem *current = 0;

        if (menu_item != 0)
          current = menu_item->menu_->getCurrent();

        if (current != 0)
          current->leave();

        if (menu_item != 0)
          menu_item->enter();

        CwmMenu::setSelectedItem(menu_item);

        CwmMenu::buttonPress();
      }
      else if (event->type == ButtonRelease) {
        menu->complete();

        CwmMenu::buttonRelease();
      }
      else if (event->type == EnterNotify) {
        if (CwmMenu::isButtonDown()) {
          Window event_window = CwmMachineInst->getEventWindow(event);

          CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

          if (menu_item != 0) {
            menu_item->enter();

            if      (menu_item->menu_ == menu) {
              if (menu_item != 0 &&
                  menu_item->type_ != CWM_MENU_CASCADE_TYPE)
                CwmMenu::setSelectedItem(menu_item);
            }
            else if (menu_item->menu_ != 0) {
              if (menu_item != this) {
                CwmMenu *menu1 = menu;

                while (menu1 != 0 && menu1 != menu_item->menu_) {
                  menu1->setInactive();

                  menu1->setCurrent(0);

                  menu1 = menu1->getParent();
                }
              }
            }
          }
        }
      }
      else if (event->type == LeaveNotify) {
        if (CwmMenu::isButtonDown()) {
          Window event_window = CwmMachineInst->getEventWindow(event);

          CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

          if (menu_item != 0)
            menu_item->leave();

          CwmMenu::setSelectedItem(0);
        }
      }
      else if (event->type == KeyPress) {
        XKeyPressedEvent *event1 = (XKeyPressedEvent *) event;

        CwmMenuKeyData *key_data = CwmMenuKeyData::getKeyData(event1);

        if (key_data != 0) {
          key_data->invoke(menu);
          continue;
        }

        for (i = 0; i < menu->getNumItems(); i++) {
          CwmMenuItem *item = menu->getItem(i);

          if (item->isMnemonicEvent   (event1) ||
              item->isAcceleratorEvent(event1)) {
            CwmMenu::setSelectedItem(item);
            break;
          }
        }

        if (i < menu->getNumItems())
          menu->complete();
      }
    }
  }
  else {
    while (menu->isActive()) {
      XEvent *event = CwmEventMgrInst->processNextEvent();

      if      (event->type == ButtonPress) {
        if (event->xbutton.button != 1 &&
            event->xbutton.button != 3)
          continue;

        Window event_window = CwmMachineInst->getEventWindow(event);

        CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

        CwmMenu::setSelectedItem(menu_item);

        menu->complete();
      }
      else if (event->type == ButtonRelease) {
        menu->complete();
      }
      else if (event->type == EnterNotify) {
        Window event_window = CwmMachineInst->getEventWindow(event);

        CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

        if (menu_item != 0) {
          menu_item->enter();

          if      (menu_item->menu_ == menu) {
            if (menu_item != 0 && menu_item->type_ != CWM_MENU_CASCADE_TYPE)
              CwmMenu::setSelectedItem(menu_item);
          }
          else if (menu_item->menu_ != 0) {
            if (menu_item != this) {
              CwmMenu *menu1 = menu;

              while (menu1 != 0 && menu1 != menu_item->menu_) {
                menu1->setInactive();

                menu1->setCurrent(0);

                menu1 = menu1->getParent();
              }
            }
          }
        }
      }
      else if (event->type == LeaveNotify) {
        Window event_window = CwmMachineInst->getEventWindow(event);

        CwmMenuItem *menu_item = menu->getAnyMenuItemForWindow(event_window);

        if (menu_item != 0)
          menu_item->leave();

        CwmMenu::setSelectedItem(0);
      }

      CwmMenu *cascade = menu->getCascade();

      if (cascade != 0) {
        if      (cascade->isCompleted())
          menu->complete();
        else if (cascade->isInactive())
          menu->setCascade(0);
      }

      while (menu->isActive()) {
        CwmMenuItem *current =  menu->getCurrent();

        if (! current || ! current->isCascadeMenu())
          break;

        current->processCascadeMenu(menu);
      }
    }
  }

  menu->unmap();

  CwmMachineInst->flushEvents();

  parent->redraw();

  if (menu->isCompleted())
    parent->setCompleted();

//delete menu;
}

void
CwmMenuItem::
redraw()
{
  xwindow_->clear();

  CwmMenuItem *current = menu_->getCurrent();

  drawImage();

  if      (type_ == CWM_MENU_TITLE_TYPE)
    drawText(graphics_);
  else if (type_ == CWM_MENU_BUTTON_TYPE) {
    if (current == this)
      graphics_->drawButtonOut(xwindow_, 0, 0, menu_->getWidth() - 4, height_, 2);

    if (proc_ != 0)
      drawText(graphics_);
    else
      drawText(stipple_graphics_);
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE) {
    if (current == this)
      graphics_->drawButtonOut(xwindow_, 0, 0, menu_->getWidth() - 4, height_, 2);

    drawText(graphics_);
  }
  else if (type_ == CWM_MENU_SPLITTER_TYPE)
    drawSplitter();
  else if (type_ == CWM_MENU_CASCADE_TYPE) {
    if (current == this)
      graphics_->drawButtonOut(xwindow_, 0, 0, menu_->getWidth() - 4, height_, 2);

    if (data1_ != 0)
      drawText(graphics_);
    else
      drawText(stipple_graphics_);

    graphics_->drawRightArrowOut(xwindow_, menu_->getWidth() - 15, height_/2 - 5,
                                 menu_->getWidth() -  8, height_/2 + 4);
  }
}

void
CwmMenuItem::
enter()
{
  if      (type_ == CWM_MENU_BUTTON_TYPE && proc_ != 0) {
    menu_->setCurrent(this);

    drawActive();
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE) {
    menu_->setCurrent(this);

    drawActive();
  }
  else if (type_ == CWM_MENU_CASCADE_TYPE && data1_ != 0) {
    menu_->setCurrent(this);

    drawActive();
  }
}

void
CwmMenuItem::
leave()
{
  if      (type_ == CWM_MENU_BUTTON_TYPE && proc_ != 0) {
    menu_->setCurrent(0);

    drawInactive();
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE) {
    menu_->setCurrent(0);

    drawInactive();
  }
  else if (type_ == CWM_MENU_CASCADE_TYPE && data1_ != 0) {
    menu_->setCurrent(0);

    drawInactive();
  }
}

bool
CwmMenuItem::
isMnemonicEvent(XKeyPressedEvent *event)
{
  return (mnemonic_ != '\0' && CwmXEventInst->isKeyPressEvent(event, mnemonic_));
}

bool
CwmMenuItem::
isAcceleratorEvent(XKeyPressedEvent *event)
{
  if (! accelerator_event_)
    return false;

  return accelerator_event_->matchEvent((XEvent *) event);
}

void
CwmMenuItem::
select()
{
  if (proc_ != 0)
    proc_(data1_, data2_, data3_, data4_, data5_, data6_);
}

void
CwmMenuItem::
drawActive()
{
  xwindow_->clear();

  drawImage();

  graphics_->drawButtonOut(xwindow_, 0, 0, menu_->getWidth() - 4, height_, 2);

  drawText(graphics_);

  if (type_ == CWM_MENU_CASCADE_TYPE)
    graphics_->drawRightArrowOut(xwindow_, menu_->getWidth() - 15, height_/2 - 5,
                                 menu_->getWidth() -  8, height_/2 + 4);
}

void
CwmMenuItem::
drawInactive()
{
  xwindow_->clear();

  drawImage();

  drawText(graphics_);

  if (type_ == CWM_MENU_CASCADE_TYPE)
    graphics_->drawRightArrowOut(xwindow_, menu_->getWidth() - 15, height_/2 - 5,
                                 menu_->getWidth() -  8, height_/2 + 4);
}

void
CwmMenuItem::
drawImage()
{
  if (! image_)
    return;

  int x = 2;
  int y = (height_ - image_->getHeight())/2;

  image_->draw(xwindow_, graphics_, x, y);
}

void
CwmMenuItem::
drawText(CwmGraphics *graphics1)
{
  int x = 4;
  int y = 4;

  if (image_ != 0)
    x += image_->getWidth() + 4;

  if (type_ == CWM_MENU_TOGGLE_TYPE) {
    if (selected_)
      graphics1->drawButtonOut(xwindow_, 4, height_/2 - 4, 8, 8, 2);

    x += 14;
  }

  int i;

  for (i = 0; i < (int) text_.size(); i++)
    if (text_[i] == mnemonic_)
      break;

  if (i < (int) text_.size()) {
    int x1 = x;

    std::string text1 = text_.substr(0, i);

    graphics1->drawText(xwindow_, x1, y, text1);

    int width1, height1;

    graphics1->getTextSize(text1, &width1, &height1);

    x1 += width1;

    text1 = text_.substr(i, 1);

    graphics1->drawText(xwindow_, x1, y, text1);

    graphics1->getTextSize(text1, &width1, &height1);

    graphics1->drawLine(xwindow_, x1         , y + graphics1->getFontAscent() + 1,
                        x1 + width1, y + graphics1->getFontAscent() + 1);

    x1 += width1;

    text1 = text_.substr(i + 1);

    graphics1->drawText(xwindow_, x1, y, text1);
  }
  else
    graphics1->drawText(xwindow_, x, y, text_);

  std::string accelerator_text;

  if (accelerator_event_ != 0)
    accelerator_text = accelerator_event_->getText();

  if      (type_ == CWM_MENU_BUTTON_TYPE && accelerator_text != "") {
    x += menu_->getItemWidth() + 8;

    graphics1->drawText(xwindow_, x, y, accelerator_text);
  }
  else if (type_ == CWM_MENU_TOGGLE_TYPE && accelerator_text != "") {
    x += menu_->getItemWidth() + 8;

    graphics1->drawText(xwindow_, x, y, accelerator_text);
  }
}

void
CwmMenuItem::
drawSplitter()
{
  graphics_->drawEtchedLine(xwindow_, 1, 1, menu_->getWidth() - 1, 1);
}

CwmNamedMenuMgr *
CwmNamedMenuMgr::
getInstance()
{
  static CwmNamedMenuMgr *instance;

  if (! instance)
    instance = new CwmNamedMenuMgr();

  return instance;
}

void
CwmNamedMenuMgr::
define(const std::string &name, CwmMenuDef *menu_def)
{
  CwmNamedMenu *named_menu = lookup(name);

  if (named_menu != 0)
    return;

  named_menu = new CwmNamedMenu(name, menu_def);

  named_menus_.push_back(named_menu);
}

CwmMenuDef *
CwmNamedMenuMgr::
lookupMenuDef(const std::string &name)
{
  CwmNamedMenu *named_menu = lookup(name);

  if (named_menu != 0)
    return named_menu->getMenuDef();

  if (CStrUtil::casecmp(name, "Root Functions") == 0)
    return createDefaultRootMenu();

  return 0;
}

CwmNamedMenu *
CwmNamedMenuMgr::
lookup(const std::string &name)
{
  NamedMenuList::const_iterator pnamed_menu1 = named_menus_.begin();
  NamedMenuList::const_iterator pnamed_menu2 = named_menus_.end  ();

  for ( ; pnamed_menu1 != pnamed_menu2; ++pnamed_menu1)
    if (CStrUtil::casecmp((*pnamed_menu1)->getName(), name) == 0)
      return *pnamed_menu1;

  return 0;
}

void
CwmNamedMenuMgr::
deleteAll()
{
  std::for_each(named_menus_.begin(), named_menus_.end(), CDeletePointer());

  named_menus_.clear();
}

CwmMenuDef *
CwmNamedMenuMgr::
createDefaultRootMenu()
{
  static CwmMenuDef *menu_def;

  if (! menu_def) {
    menu_def = new CwmMenuDef();

    menu_def->addTitle   ("Root Menu" );
    menu_def->addSplitter("----------");
    menu_def->addButton  ("", "New Window", '\0', "",
                          (CwmMenuProc) &CwmScreenMenu::runXCommandProc, (CwmData) "xterm &");
    menu_def->addButton  ("", "Restart"   , '\0', "",
                          (CwmMenuProc) &CwmScreenMenu::restartCwmProc, 0);
    menu_def->addSplitter("----------");
    menu_def->addButton  ("", "Quit"      , '\0', "",
                          (CwmMenuProc) &CwmScreenMenu::quitCwmProc, 0);
  }

  return menu_def;
}

CwmNamedMenu::
CwmNamedMenu(const std::string &name, CwmMenuDef *menu_def) :
 name_(name)
{
  if (menu_def)
    menu_def_ = new CwmMenuDef(*menu_def);
  else
    menu_def_ = new CwmMenuDef();
}

CwmNamedMenu::
~CwmNamedMenu()
{
  delete menu_def_;
}
