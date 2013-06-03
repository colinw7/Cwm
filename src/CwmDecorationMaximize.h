class CwmMaximizeChild : public CwmWindowChild {
 private:
  int sides_;

 public:
  CwmMaximizeChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
