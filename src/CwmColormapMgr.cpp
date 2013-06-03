#include "CwmI.h"

CwmColormapMgr::
CwmColormapMgr(CwmScreen &screen) :
 screen_(screen), window_(NULL), cmap_(screen.getColormap())
{
}

CwmColormapMgr::
~CwmColormapMgr()
{
}

void
CwmColormapMgr::
install(CwmWMWindow *window)
{
  Colormap cmap;

  Window cmap_window = window->getColormapWindowHint();

  if (cmap_window != None)
    cmap = CwmMachineInst->getWindowColormap(cmap_window);
  else
    cmap = window->getColormap();

  install(window, cmap);
}

void
CwmColormapMgr::
uninstall()
{
  Colormap cmap = screen_.getColormap();

  install(NULL, cmap);
}

void
CwmColormapMgr::
install(CwmWMWindow *window, Colormap cmap)
{
  if (cmap != cmap_)
    screen_.installColormap(cmap);

  window_ = window;
  cmap_   = cmap;
}
