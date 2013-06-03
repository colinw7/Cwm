class CwmNSideChild : public CwmWindowChild {
 public:
  CwmNSideChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
