class CwmDecoration {
 public:
  CwmDecoration(CwmWMWindow *window);
 ~CwmDecoration();

  bool processExpose(XExposeEvent *event);
  bool processButtonPress(XButtonPressedEvent *event);

  CwmWindowChild *getTitle() const {
   return reinterpret_cast<CwmWindowChild *>(title_child_);
  }

  CwmWindowChild *getMenu() const {
   return reinterpret_cast<CwmWindowChild *>(menu_child_);
  }

  CwmWindowChild *getMinimize() const {
   return reinterpret_cast<CwmWindowChild *>(minimize_child_);
  }

  CwmWindowChild *getMaximize() const {
   return reinterpret_cast<CwmWindowChild *>(maximize_child_);
  }

  CwmWindowChild *getClose() const {
   return reinterpret_cast<CwmWindowChild *>(close_child_);
  }

  CwmWindowChild *getNSide() const {
   return reinterpret_cast<CwmWindowChild *>(n_side_child_);
  }

  CwmWindowChild *getSSide() const {
   return reinterpret_cast<CwmWindowChild *>(s_side_child_);
  }

  CwmWindowChild *getESide() const {
   return reinterpret_cast<CwmWindowChild *>(e_side_child_);
  }

  CwmWindowChild *getWSide() const {
   return reinterpret_cast<CwmWindowChild *>(w_side_child_);
  }

  CwmWindowChild *getNWCorner() const {
   return reinterpret_cast<CwmWindowChild *>(nw_corner_child_);
  }

  CwmWindowChild *getNECorner() const {
   return reinterpret_cast<CwmWindowChild *>(ne_corner_child_);
  }

  CwmWindowChild *getSWCorner() const {
   return reinterpret_cast<CwmWindowChild *>(sw_corner_child_);
  }

  CwmWindowChild *getSECorner() const {
   return reinterpret_cast<CwmWindowChild *>(se_corner_child_);
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

  static void closeDoubleClickProc(CwmWindow *xwindow, CwmData data, CwmData detail);

 private:
  CwmWMWindow *window_ { nullptr };

  CwmContainerChild *container_child_ { nullptr };

  CwmMenuChild      *menu_child_      { nullptr };
  CwmTitleChild     *title_child_     { nullptr };
  CwmMinimizeChild  *minimize_child_  { nullptr };
  CwmMaximizeChild  *maximize_child_  { nullptr };
  CwmCloseChild     *close_child_     { nullptr };
  CwmNSideChild     *n_side_child_    { nullptr };
  CwmSSideChild     *s_side_child_    { nullptr };
  CwmWSideChild     *w_side_child_    { nullptr };
  CwmESideChild     *e_side_child_    { nullptr };
  CwmNWCornerChild  *nw_corner_child_ { nullptr };
  CwmNECornerChild  *ne_corner_child_ { nullptr };
  CwmSWCornerChild  *sw_corner_child_ { nullptr };
  CwmSECornerChild  *se_corner_child_ { nullptr };

  int top_border_;
  int bottom_border_;
  int left_border_;
  int right_border_;

  int outside_edge_width_;
  int inside_edge_width_;
  int button_width_;
  int title_height_;
  int resize_height_;
  int resize_splitter_;

  bool has_focus_;
  bool rolled_up_;
};
