class CwmNECornerChild : public CwmWindowChild {
 public:
  CwmNECornerChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height) override;

  void detail() override;
};
