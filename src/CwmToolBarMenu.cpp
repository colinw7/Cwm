#include <CwmToolBarI.h>
#include <CImageLib.h>

const char *
CwmToolBarMenu::menu_image[] = {
"34 16 3 1",
"       c None",
"+      c #000000",
"@      c #2671A4",
"                                  ",
"                                  ",
"      ++++                        ",
"     +@@@@+                       ",
"    +@@++@@+                      ",
"   +@@+  +++++  ++  +++++++++++   ",
"  +@@+     +@+ +++ +@@+@@@@@@@@+  ",
"  +@+      +@@+@@@+@@++@@+@@+@@+  ",
"  +@+      +@@+@+@+@++@@++@@+@++  ",
"  +@+    ++++@@@+@@@++@@+@@++@+   ",
"  +@@++++@@++@@++@@+ +@++@@++@+   ",
"   +@@@@@++ +@+ +@++ +@++@+ +@+   ",
"    ++++++  +++ +++  ++ +++ ++    ",
"                                  ",
"                                  ",
"                                  "};

CwmToolBarMenu::
CwmToolBarMenu(CwmToolBar &toolbar) :
 toolbar_(toolbar)
{
  CwmScreen &screen = toolbar_.getScreen();

  //------

  Pixel fg = CwmResourceDataInst->getToolBarForeground(screen);
  Pixel bg = CwmResourceDataInst->getToolBarBackground(screen);

  CwmColor *color = screen.getColor(fg, bg);

  CwmFont *font = CwmResourceDataInst->getToolBarFont(screen);

  graphics_ = new CwmGraphics(screen, font, color);

  //------

  std::string name = "CwmToolBarMenu/menu_image";

  image_ = CwmImageMgrInst->getImage(screen, name);

  if (! image_) {
    CImageNameSrc src(name);

    CImagePtr image1 = CImageMgrInst->createImage(src);

    image1->readXPM(menu_image, int(sizeof(menu_image)/sizeof(char *)));

    image_ = CwmImageMgrInst->addImage(name, image1)->getImage(screen);
  }

  int width  = image_->getWidth ();
  int height = image_->getHeight();

  //------

  uint event_mask = ButtonPressMask | ButtonReleaseMask |
                    EnterWindowMask | LeaveWindowMask   |
                    KeyPressMask    | ExposureMask;

  xwindow_ =
    new CwmWindow(screen, toolbar_.getXWindow(),
                   2, (toolbar_.getHeight() - height)/2,
                   width + 2, height + 2,
                   event_mask, CWM_CURSOR_TITLE);

  //------

  xwindow_->setBackground(graphics_);

  xwindow_->map();
}

CwmToolBarMenu::
~CwmToolBarMenu()
{
  delete xwindow_;
}

void
CwmToolBarMenu::
redraw()
{
  xwindow_->setBackground(graphics_);

  xwindow_->clear();

  graphics_->drawButtonOut(xwindow_,
                           0, 0,
                           xwindow_->getWidth(),
                           xwindow_->getHeight(),
                           2);

  image_->draw(xwindow_, graphics_, 1, 1);
}

int
CwmToolBarMenu::
getWidth() const
{
  return xwindow_->getWidth();
}

int
CwmToolBarMenu::
getHeight() const
{
  return xwindow_->getHeight();
}

void
CwmToolBarMenu::
processMenu()
{
  std::string name = "ToolBar Run Menu";

  CwmMenuDef *menu_def = CwmNamedMenuMgrInst->lookupMenuDef(name);

  if (menu_def) {
    CwmScreen &screen = toolbar_.getScreen();

    menu_def->setClientData(&screen, nullptr, nullptr);

    CwmMenu::processWindowMenu(screen, xwindow_, menu_def, 0, 0,
                               CHALIGN_TYPE_LEFT, CVALIGN_TYPE_BOTTOM);
  }
}
