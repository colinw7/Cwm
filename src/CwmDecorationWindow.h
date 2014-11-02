class CwmWindowChild {
 public:
  CwmWindowChild(CwmDecoration *decoration, CwmWMWindow *window,
                 uint event_mask, CwmCursorType cursor);

  virtual ~CwmWindowChild();

  CwmDecoration *getDecoration() const { return decoration_; }
  CwmWMWindow   *getWindow    () const { return window_; }
  CwmWindow     *getXWindow   () const { return xwindow_; }

  std::string getInfoString() const { return info_string_; }

  CwmScreen &getScreen() const;

  void setGraphics(CwmGraphics *graphics) { graphics_ = graphics; }

  void update();

  void draw();
  void drawBorder();
  void drawBorderIn();
  void drawBorderOut();

  void drawSides(int sides);
  void drawSidesIn(int sides);
  void drawSidesOut(int sides);

  void drawText(const std::string &text);

  void map();
  void unmap();

  void select();
  void deselect();

  bool getMapped() const;

  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;

  virtual void calcSize(int *x, int *y, int *width, int *height) = 0;
  virtual void detail() = 0;

  static void idleProc(CwmWindow *xwindow, CwmData data, CwmData detail);

 protected:
  CwmDecoration *decoration_;
  CwmWMWindow   *window_;
  CwmWindow     *xwindow_;
  CwmGraphics   *graphics_;
  bool           selected_;
  std::string    info_string_;
};
