class CwmMenuChild : public CwmWindowChild {
 private:
  int sides_;

 public:
  CwmMenuChild(CwmDecoration *decoration, CwmWMWindow *window);

  void calcSize(int *x, int *y, int *width, int *height);
  void detail();
};
