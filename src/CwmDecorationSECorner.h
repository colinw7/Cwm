class CwmSECornerChild : public CwmWindowChild {
 public:
  CwmSECornerChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height) override;

  void detail() override;
};
