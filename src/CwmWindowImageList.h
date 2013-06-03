class CwmWindowImage;

class CwmWindowImageList {
 private:
  typedef vector<CwmWindowImage *> WindowImageList;

  CwmWMWindow     *window_;
  WindowImageList  images_;

 public:
  CwmWindowImageList(CwmWMWindow *window);
 ~CwmWindowImageList();

  CwmImage *getImage(int width, int height);
  CwmImage *getImage();

 private:
  void addImage(CwmImage *image, const std::string &name);
};

class CwmWindowImage {
 private:
  CwmImage    *image_;
  std::string  name_;

 public:
  CwmWindowImage(CwmImage *image, const std::string &name);
 ~CwmWindowImage();

  int getWidth() const;
  int getHeight() const;

  CwmImage    *getImage() { return image_; }
  std::string  getName () { return name_ ; }
};
