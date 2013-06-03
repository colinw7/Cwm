class CwmNWCornerChild : public CwmWindowChild {
 public:
  CwmNWCornerChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
