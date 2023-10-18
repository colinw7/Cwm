class CwmESideChild : public CwmWindowChild {
 public:
  CwmESideChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height) override;

  void detail() override;
};
