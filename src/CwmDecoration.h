class CwmDecoration {
 private:
  CwmWMWindow       *window_;

  CwmContainerChild *container_child_;

  CwmMenuChild      *menu_child_;
  CwmTitleChild     *title_child_;
  CwmMinimizeChild  *minimize_child_;
  CwmMaximizeChild  *maximize_child_;
  CwmCloseChild     *close_child_;
  CwmNSideChild     *n_side_child_;
  CwmSSideChild     *s_side_child_;
  CwmWSideChild     *w_side_child_;
  CwmESideChild     *e_side_child_;
  CwmNWCornerChild  *nw_corner_child_;
  CwmNECornerChild  *ne_corner_child_;
  CwmSWCornerChild  *sw_corner_child_;
  CwmSECornerChild  *se_corner_child_;

  int                top_border_;
  int                bottom_border_;
  int                left_border_;
  int                right_border_;

  int                outside_edge_width_;
  int                inside_edge_width_;
  int                button_width_;
  int                title_height_;
  int                resize_height_;
  int                resize_splitter_;

  bool               has_focus_;
  bool               rolled_up_;

 public:
  CwmDecoration(CwmWMWindow *window);
 ~CwmDecoration();

  bool processExpose(XExposeEvent *event);
  bool processButtonPress(XButtonPressedEvent *event);

  CwmWindowChild *getTitle() const {
   return (CwmWindowChild *) title_child_;
  }

  CwmWindowChild *getMenu() const {
   return (CwmWindowChild *) menu_child_;
  }

  CwmWindowChild *getMinimize() const {
   return (CwmWindowChild *) minimize_child_;
  }

  CwmWindowChild *getMaximize() const {
   return (CwmWindowChild *) maximize_child_;
  }

  CwmWindowChild *getClose() const {
   return (CwmWindowChild *) close_child_;
  }

  CwmWindowChild *getNSide() const {
   return (CwmWindowChild *) n_side_child_;
  }

  CwmWindowChild *getSSide() const {
   return (CwmWindowChild *) s_side_child_;
  }

  CwmWindowChild *getESide() const {
   return (CwmWindowChild *) e_side_child_;
  }

  CwmWindowChild *getWSide() const {
   return (CwmWindowChild *) w_side_child_;
  }

  CwmWindowChild *getNWCorner() const {
   return (CwmWindowChild *) nw_corner_child_;
  }

  CwmWindowChild *getNECorner() const {
   return (CwmWindowChild *) ne_corner_child_;
  }

  CwmWindowChild *getSWCorner() const {
   return (CwmWindowChild *) sw_corner_child_;
  }

  CwmWindowChild *getSECorner() const {
   return (CwmWindowChild *) se_corner_child_;
  }

  bool isMenu(CwmWindow *xwindow);
  bool isTitle(CwmWindow *xwindow);
  bool isMinimize(CwmWindow *xwindow);
  bool isMaximize(CwmWindow *xwindow);
  bool isClose(CwmWindow *xwindow);
  bool isNSide(CwmWindow *xwindow);
  bool isSSide(CwmWindow *xwindow);
  bool isWSide(CwmWindow *xwindow);
  bool isESide(CwmWindow *xwindow);
  bool isNWCorner(CwmWindow *xwindow);
  bool isNECorner(CwmWindow *xwindow);
  bool isSWCorner(CwmWindow *xwindow);
  bool isSECorner(CwmWindow *xwindow);

  void print();

  void focusIn(CwmGraphics *graphics);
  void focusOut(CwmGraphics *graphics);

  void rollUp();
  void rollDown();

  void decorate();
  void redecorate();

  void reparent(CwmWindow *user_xwindow);

  void redraw();

  int getTopBorder() const;
  int getBottomBorder() const;
  int getLeftBorder() const;
  int getRightBorder() const;

  int getOutsideEdgeWidth() const;
  int getInsideEdgeWidth() const;

  int getButtonWidth () const;
  int getTitleHeight () const;
  int getResizeHeight() const;

  bool hasResizeSplitter() const;

  bool hasFocus() const { return has_focus_; }

  static void closeDoubleClickProc(CwmWindow *xwindow, CwmData data,
                                   CwmData detail);
};
