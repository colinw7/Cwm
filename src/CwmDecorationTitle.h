class CGradient;

class CwmTitleChild : public CwmWindowChild {
 private:
  int          sides_;
  CGradient   *gradient1_;
  CGradient   *gradient2_;
  CGradient   *gradient3_;
  CGradient   *gradient4_;
  int          gradient_width_;
  int          gradient_height_;
  CwmGraphics *focus_graphics_;
  CwmGraphics *nofocus_graphics_;

 public:
  CwmTitleChild(CwmDecoration *decoration, CwmWMWindow *window);
 ~CwmTitleChild();

  void calcSize(int *x, int *y, int *width, int *height) override;

  void detail() override;

 private:
  void detailGradient();
  void detailSolid();
};
