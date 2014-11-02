class CwmWindowImage;

class CwmWindowImageList {
 public:
  CwmWindowImageList(CwmWMWindow *window);
 ~CwmWindowImageList();

  CwmImage *getImage(int width, int height);
  CwmImage *getImage();

 private:
  void addImage(CwmImage *image, const std::string &name);

 private:
  typedef std::vector<CwmWindowImage *> WindowImageList;

  CwmWMWindow     *window_;
  WindowImageList  images_;
};

class CwmWindowImage {
 public:
  CwmWindowImage(CwmImage *image, const std::string &name);
 ~CwmWindowImage();

  int getWidth() const;
  int getHeight() const;

  CwmImage    *getImage() { return image_; }
  std::string  getName () { return name_ ; }

 private:
  CwmImage    *image_;
  std::string  name_;
};
