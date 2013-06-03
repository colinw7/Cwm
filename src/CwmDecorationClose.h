class CwmCloseChild : public CwmWindowChild {
 private:
  int sides_;

 public:
  CwmCloseChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
