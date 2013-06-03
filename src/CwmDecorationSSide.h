class CwmSSideChild : public CwmWindowChild {
 public:
  CwmSSideChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
