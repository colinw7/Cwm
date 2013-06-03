class CwmContainerChild : public CwmWindowChild {
 public:
  CwmContainerChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
