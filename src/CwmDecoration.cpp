#include <CwmDecorationI.h>

CwmDecoration::
CwmDecoration(CwmWMWindow *window) :
 window_(window), has_focus_(false), rolled_up_(false)
{
  n_side_child_ = new CwmNSideChild(this, window);
  s_side_child_ = new CwmSSideChild(this, window);
  e_side_child_ = new CwmESideChild(this, window);
  w_side_child_ = new CwmWSideChild(this, window);

  nw_corner_child_ = new CwmNWCornerChild(this, window);
  ne_corner_child_ = new CwmNECornerChild(this, window);
  sw_corner_child_ = new CwmSWCornerChild(this, window);
  se_corner_child_ = new CwmSECornerChild(this, window);

  maximize_child_ = new CwmMaximizeChild(this, window);
  minimize_child_ = new CwmMinimizeChild(this, window);
  close_child_    = new CwmCloseChild   (this, window);

  menu_child_ = new CwmMenuChild(this, window);

  title_child_ = new CwmTitleChild(this, window);

  container_child_ = new CwmContainerChild(this, window);

  if (window->getBorderDecorationHint()) {
    top_border_    = 0;
    bottom_border_ = 3;
    left_border_   = 3;
    right_border_  = 3;
  }
  else {
    top_border_    = 0;
    bottom_border_ = 0;
    left_border_   = 0;
    right_border_  = 0;
  }

  outside_edge_width_ = 1;
  inside_edge_width_  = 0;

  button_width_ = 18;

  if (window->getTitleDecorationHint()) {
    title_height_ = window->getFontHeight() + 2;

    if (title_height_ < button_width_)
      title_height_ = button_width_;
  }
  else
    title_height_ = 0;

  if (title_height_ > 0)
    resize_height_ = title_height_;
  else
    resize_height_ = 6;

  resize_splitter_ = false;
}

CwmDecoration::
~CwmDecoration()
{
  delete container_child_;

  delete n_side_child_;
  delete s_side_child_;
  delete w_side_child_;
  delete e_side_child_;

  delete nw_corner_child_;
  delete ne_corner_child_;
  delete sw_corner_child_;
  delete se_corner_child_;

  delete maximize_child_;
  delete minimize_child_;
  delete close_child_;

  delete menu_child_;

  delete title_child_;
}

bool
CwmDecoration::
processExpose(XExposeEvent *event)
{
  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  if (xwindow == 0)
    return false;

  if      (isMenu(xwindow))
    getMenu()->draw();
  else if (isTitle(xwindow))
    getTitle()->draw();
  else if (isMinimize(xwindow))
    getMinimize()->draw();
  else if (isMaximize(xwindow))
    getMaximize()->draw();
  else if (isClose(xwindow))
    getClose()->draw();
  else if (isNSide(xwindow))
    getNSide()->draw();
  else if (isSSide(xwindow))
    getSSide()->draw();
  else if (isWSide(xwindow))
    getWSide()->draw();
  else if (isESide(xwindow))
    getESide()->draw();
  else if (isNWCorner(xwindow))
    getNWCorner()->draw();
  else if (isNECorner(xwindow))
    getNECorner()->draw();
  else if (isSWCorner(xwindow))
    getSWCorner()->draw();
  else if (isSECorner(xwindow))
    getSECorner()->draw();

  return true;
}

bool
CwmDecoration::
processButtonPress(XButtonPressedEvent *event)
{
  CwmWMWindow *window = CwmMachineInst->getWindowWMWindow(event->window);

  if (window == 0)
    return false;

  CwmWindow *xwindow = CwmMachineInst->getWindowCwmXWindow(event->window);

  bool flag = true;

  if      (event->button == 1) {
    if      (isMenu(xwindow)) {
      menu_child_->select();

      window_->processWindowMenuAtPosition(0, menu_child_->getHeight());

      menu_child_->deselect();
    }
    else if (isTitle(xwindow)) {
      title_child_->select();

      window->raise();

      CwmMoveWindowInst->moveDelayed(window);

      title_child_->deselect();
    }
    else if (isMinimize(xwindow)) {
      minimize_child_->select();

      Window xwin =
        CwmEventMgrInst->waitForButtonRelease
         (minimize_child_->getXWindow(), CWM_LEFT_BUTTON);

      if (xwin == minimize_child_->getXWindow()->getXWin())
        window->iconise();

      minimize_child_->deselect();
    }
    else if (isMaximize(xwindow)) {
      maximize_child_->select();

      Window xwin =
        CwmEventMgrInst->waitForButtonRelease
         (maximize_child_->getXWindow(), CWM_LEFT_BUTTON);

      if (xwin == maximize_child_->getXWindow()->getXWin())
        window->maximize();

      maximize_child_->deselect();
    }
    else if (isClose(xwindow)) {
      window->close();
    }
    else if (isNSide(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP);
    }
    else if (isSSide(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM);
    }
    else if (isWSide(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_LEFT);
    }
    else if (isESide(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_RIGHT);
    }
    else if (isNWCorner(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP_LEFT);
    }
    else if (isNECorner(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP_RIGHT);
    }
    else if (isSWCorner(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM_LEFT);
    }
    else if (isSECorner(xwindow)) {
      window->raise();

      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM_RIGHT);
    }
    else
      flag = false;
  }
  else if (event->button == 2) {
    if      (isMenu(xwindow)) {
      CwmWindowEventFunctionMgrInst->
        processEventFunction(window, CWM_WINDOW_MENU_AREA, (XEvent *) event);
    }
    else if (isTitle(xwindow)) {
      title_child_->select();

      CwmMoveWindowInst->moveDelayed(window);

      title_child_->deselect();
    }
    else if (isMinimize(xwindow)) {
      minimize_child_->select();

      Window xwin =
        CwmEventMgrInst->waitForButtonRelease
         (minimize_child_->getXWindow(), CWM_MIDDLE_BUTTON);

      if (xwin == minimize_child_->getXWindow()->getXWin())
        window->iconise();

      minimize_child_->deselect();
    }
    else if (isMaximize(xwindow)) {
      maximize_child_->select();

      Window xwin =
        CwmEventMgrInst->waitForButtonRelease
         (maximize_child_->getXWindow(), CWM_MIDDLE_BUTTON);

      if (xwin == maximize_child_->getXWindow()->getXWin())
        window->minimize();

      maximize_child_->deselect();
    }
    else if (isClose(xwindow)) {
    }
    else if (isNSide(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP);
    else if (isSSide(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM);
    else if (isWSide(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_LEFT);
    else if (isESide(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_RIGHT);
    else if (isNWCorner(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP_LEFT);
    else if (isNECorner(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_TOP_RIGHT);
    else if (isSWCorner(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM_LEFT);
    else if (isSECorner(xwindow))
      CwmResizeWindowInst->resizeDelayed(window, CWM_RESIZE_BOTTOM_RIGHT);
    else
      flag = false;
  }
  else if (event->button == 3) {
    window->processWindowMenu();
  }

  return flag;
}

bool
CwmDecoration::
isMenu(CwmWindow *xwindow)
{
  return (menu_child_ != 0 && xwindow == menu_child_->getXWindow());
}

bool
CwmDecoration::
isTitle(CwmWindow *xwindow)
{
  return (title_child_ != 0 && xwindow == title_child_->getXWindow());
}

bool
CwmDecoration::
isMinimize(CwmWindow *xwindow)
{
  return (minimize_child_ != 0 && xwindow == minimize_child_->getXWindow());
}

bool
CwmDecoration::
isMaximize(CwmWindow *xwindow)
{
  return (maximize_child_ != 0 && xwindow == maximize_child_->getXWindow());
}

bool
CwmDecoration::
isClose(CwmWindow *xwindow)
{
  return (close_child_ != 0 && xwindow == close_child_->getXWindow());
}

bool
CwmDecoration::
isNSide(CwmWindow *xwindow)
{
  return (n_side_child_ != 0 && xwindow == n_side_child_->getXWindow());
}

bool
CwmDecoration::
isSSide(CwmWindow *xwindow)
{
  return (s_side_child_ != 0 && xwindow == s_side_child_->getXWindow());
}

bool
CwmDecoration::
isWSide(CwmWindow *xwindow)
{
  return (w_side_child_ != 0 && xwindow == w_side_child_->getXWindow());
}

bool
CwmDecoration::
isESide(CwmWindow *xwindow)
{
  return (e_side_child_ != 0 && xwindow == e_side_child_->getXWindow());
}

bool
CwmDecoration::
isNWCorner(CwmWindow *xwindow)
{
  return (nw_corner_child_ != 0 && xwindow == nw_corner_child_->getXWindow());
}

bool
CwmDecoration::
isNECorner(CwmWindow *xwindow)
{
  return (ne_corner_child_ != 0 && xwindow == ne_corner_child_->getXWindow());
}

bool
CwmDecoration::
isSWCorner(CwmWindow *xwindow)
{
  return (sw_corner_child_ != 0 && xwindow == sw_corner_child_->getXWindow());
}

bool
CwmDecoration::
isSECorner(CwmWindow *xwindow)
{
  return (se_corner_child_ != 0 && xwindow == se_corner_child_->getXWindow());
}

void
CwmDecoration::
print()
{
  CwmMachineInst->logf("  Container %x\n",
                       container_child_ ? container_child_->getXWindow() : 0);
  CwmMachineInst->logf("   Menu      %x\n",
                       menu_child_      ? menu_child_     ->getXWindow() : 0);
  CwmMachineInst->logf("   Title     %x\n",
                       title_child_     ? title_child_    ->getXWindow() : 0);
  CwmMachineInst->logf("   Minimize  %x\n",
                       minimize_child_  ? minimize_child_ ->getXWindow() : 0);
  CwmMachineInst->logf("   Maximize  %x\n",
                       maximize_child_  ? maximize_child_ ->getXWindow() : 0);
  CwmMachineInst->logf("   Close     %x\n",
                       close_child_     ? close_child_    ->getXWindow() : 0);
  CwmMachineInst->logf("   N Side    %x\n",
                       n_side_child_    ? n_side_child_   ->getXWindow() : 0);
  CwmMachineInst->logf("   S Side    %x\n",
                       s_side_child_    ? s_side_child_   ->getXWindow() : 0);
  CwmMachineInst->logf("   W Side    %x\n",
                       w_side_child_    ? w_side_child_   ->getXWindow() : 0);
  CwmMachineInst->logf("   E Side    %x\n",
                       e_side_child_    ? e_side_child_   ->getXWindow() : 0);
  CwmMachineInst->logf("   NW Corner %x\n",
                       nw_corner_child_ ? nw_corner_child_->getXWindow() : 0);
  CwmMachineInst->logf("   NE Corner %x\n",
                       ne_corner_child_ ? ne_corner_child_->getXWindow() : 0);
  CwmMachineInst->logf("   SW Corner %x\n",
                       sw_corner_child_ ? sw_corner_child_->getXWindow() : 0);
  CwmMachineInst->logf("   SE Corner %x\n",
                       se_corner_child_ ? se_corner_child_->getXWindow() : 0);
}

void
CwmDecoration::
focusIn(CwmGraphics *graphics)
{
  CwmFocusHighlightType highlight_type =
    CwmResourceDataInst->getFocusHighlightType();

  if      (highlight_type == CWM_FOCUS_HIGHLIGHT_TITLE)
    title_child_->setGraphics(graphics);
  else if (highlight_type == CWM_FOCUS_HIGHLIGHT_ALL) {
    container_child_->setGraphics(graphics);

    n_side_child_   ->setGraphics(graphics);
    s_side_child_   ->setGraphics(graphics);
    w_side_child_   ->setGraphics(graphics);
    e_side_child_   ->setGraphics(graphics);

    nw_corner_child_->setGraphics(graphics);
    ne_corner_child_->setGraphics(graphics);
    sw_corner_child_->setGraphics(graphics);
    se_corner_child_->setGraphics(graphics);

    menu_child_     ->setGraphics(graphics);
    title_child_    ->setGraphics(graphics);
    minimize_child_ ->setGraphics(graphics);
    maximize_child_ ->setGraphics(graphics);
    close_child_    ->setGraphics(graphics);
  }

  has_focus_ = true;
}

void
CwmDecoration::
focusOut(CwmGraphics *graphics)
{
  CwmFocusHighlightType highlight_type =
    CwmResourceDataInst->getFocusHighlightType();

  if      (highlight_type == CWM_FOCUS_HIGHLIGHT_TITLE)
    title_child_->setGraphics(graphics);
  else if (highlight_type == CWM_FOCUS_HIGHLIGHT_ALL) {
    container_child_->setGraphics(graphics);

    n_side_child_   ->setGraphics(graphics);
    s_side_child_   ->setGraphics(graphics);
    w_side_child_   ->setGraphics(graphics);
    e_side_child_   ->setGraphics(graphics);

    nw_corner_child_->setGraphics(graphics);
    ne_corner_child_->setGraphics(graphics);
    sw_corner_child_->setGraphics(graphics);
    se_corner_child_->setGraphics(graphics);

    menu_child_     ->setGraphics(graphics);
    title_child_    ->setGraphics(graphics);
    minimize_child_ ->setGraphics(graphics);
    maximize_child_ ->setGraphics(graphics);
    close_child_    ->setGraphics(graphics);
  }

  has_focus_ = false;
}

void
CwmDecoration::
rollUp()
{
  if (window_->getResizeHandleDecorationHint()) {
    n_side_child_   ->unmap();
    s_side_child_   ->unmap();
    w_side_child_   ->unmap();
    e_side_child_   ->unmap();

    nw_corner_child_->unmap();
    ne_corner_child_->unmap();
    sw_corner_child_->unmap();
    se_corner_child_->unmap();
  }

  menu_child_    ->unmap();
  minimize_child_->unmap();
  maximize_child_->unmap();
  close_child_   ->unmap();

  container_child_->unmap();

  rolled_up_ = true;
}

void
CwmDecoration::
rollDown()
{
  container_child_->map();

  menu_child_    ->map();
  minimize_child_->map();
  maximize_child_->map();
  close_child_   ->map();

  if (window_->getResizeHandleDecorationHint()) {
    n_side_child_->map();
    s_side_child_->map();
    w_side_child_->map();
    e_side_child_->map();

    nw_corner_child_->map();
    ne_corner_child_->map();
    sw_corner_child_->map();
    se_corner_child_->map();
  }

  rolled_up_ = false;
}

void
CwmDecoration::
decorate()
{
  if (window_->getResizeHandleDecorationHint()) {
    n_side_child_->map();
    s_side_child_->map();
    w_side_child_->map();
    e_side_child_->map();

    nw_corner_child_->map();
    ne_corner_child_->map();
    sw_corner_child_->map();
    se_corner_child_->map();
  }

  if (window_->getCloseDecorationHint())
    close_child_->map();

  if (window_->getMaximizeDecorationHint())
    maximize_child_->map();

  if (window_->getMinimizeDecorationHint())
    minimize_child_->map();

  if (window_->getMenuDecorationHint())
    menu_child_->map();

  if (window_->getTitleDecorationHint())
    title_child_->map();

  container_child_->map();

  CwmWindow *xwindow = menu_child_->getXWindow();

  xwindow->addCallback(CWM_CALLBACK_DOUBLE_CLICK_1,
                       &CwmDecoration::closeDoubleClickProc,
                       (CwmData) window_);
}

void
CwmDecoration::
redecorate()
{
  if (! rolled_up_) {
    if (window_->getResizeHandleDecorationHint()) {
      n_side_child_->update();
      s_side_child_->update();
      w_side_child_->update();
      e_side_child_->update();

      nw_corner_child_->update();
      ne_corner_child_->update();
      sw_corner_child_->update();
      se_corner_child_->update();
    }

    if (window_->getCloseDecorationHint())
      close_child_->update();

    if (window_->getMaximizeDecorationHint())
      maximize_child_->update();

    if (window_->getMinimizeDecorationHint())
      minimize_child_->update();

    if (window_->getMenuDecorationHint())
      menu_child_->update();

    if (window_->getTitleDecorationHint())
      title_child_->update();

    container_child_->update();
  }
  else
    title_child_->update();
}

void
CwmDecoration::
reparent(CwmWindow *user_xwindow)
{
  user_xwindow->reparent(container_child_->getXWindow(), 0, 0);
}

void
CwmDecoration::
redraw()
{
  menu_child_     ->draw();
  title_child_    ->draw();
  minimize_child_ ->draw();
  maximize_child_ ->draw();
  close_child_    ->draw();

  n_side_child_   ->draw();
  s_side_child_   ->draw();
  w_side_child_   ->draw();
  e_side_child_   ->draw();

  nw_corner_child_->draw();
  ne_corner_child_->draw();
  sw_corner_child_->draw();
  se_corner_child_->draw();
}

void
CwmDecoration::
closeDoubleClickProc(CwmWindow *, CwmData data, CwmData)
{
  CwmWMWindow *window = (CwmWMWindow *) data;

  window->close();
}

int
CwmDecoration::
getTopBorder() const
{
  return top_border_;
}

int
CwmDecoration::
getBottomBorder() const
{
  return bottom_border_;
}

int
CwmDecoration::
getLeftBorder() const
{
  return left_border_;
}

int
CwmDecoration::
getRightBorder() const
{
  return right_border_;
}

int
CwmDecoration::
getOutsideEdgeWidth() const
{
  return outside_edge_width_;
}

int
CwmDecoration::
getInsideEdgeWidth() const
{
  return inside_edge_width_;
}

int
CwmDecoration::
getButtonWidth () const
{
  return button_width_;
}

int
CwmDecoration::
getTitleHeight () const
{
  return title_height_;
}

int
CwmDecoration::
getResizeHeight() const
{
  return resize_height_;
}

bool
CwmDecoration::
hasResizeSplitter() const
{
  return resize_splitter_;
}
