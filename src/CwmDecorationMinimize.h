class CwmMinimizeChild : public CwmWindowChild {
 private:
  int sides_;

 public:
  CwmMinimizeChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height) override;

  void detail() override;
};
