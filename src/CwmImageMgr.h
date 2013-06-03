#ifndef CWM_IMAGE_MGR_H
#define CWM_IMAGE_MGR_H

#define CwmImageMgrInst CwmImageMgr::getInstance()

class CwmNamedImage;
class CwmScreenImage;
class CwmTiledImage;
class CwmImage;

class CwmImageMgr {
 private:
  typedef vector<CwmNamedImage *> NamedImageList;

  NamedImageList images_;

 public:
  static CwmImageMgr *getInstance();

  CwmImage *getImage(CwmScreen &screen, const std::string &name="",
                     int width=-1, int height=-1);

  CwmImage *getTiledImage(CwmScreen &screen, const std::string &name,
                          int width, int height, int halign, int valign);

  CwmNamedImage *addImage(const std::string &name, const CImagePtr &image);

 private:
  CwmImageMgr();
 ~CwmImageMgr();

  CwmNamedImage *lookup(const std::string &name);
  CwmNamedImage *lookup();

  std::string getPathName(const std::string &name);
  std::string searchPathList(const std::string &path_list, const std::string &name);
};

class CwmNamedImage {
 private:
  typedef vector<CwmScreenImage *> ScreenImageList;

  std::string     name_;
  CImagePtr       image_;
  ScreenImageList images_;

  static unsigned char image_data_[];

 public:
  CwmNamedImage(const std::string &name, const std::string &pathname);
  CwmNamedImage(const std::string &name, const CImagePtr &image);
 ~CwmNamedImage();

  CwmImage *getImage(CwmScreen &screen, int width=-1, int height=-1);

  CwmImage *getTiledImage(CwmScreen &screen, int width, int height,
                          int halign, int valign);

  std::string getName() const { return name_; }

 private:
  CwmScreenImage *lookup(CwmScreen &screen);
};

class CwmScreenImage {
 private:
  typedef vector<CwmImage *>      ImageList;
  typedef vector<CwmTiledImage *> TiledImageList;

  CwmScreen      &screen_;
  CImagePtr       image_;
  ImageList       images_;
  TiledImageList  tiled_images_;

 public:
  CwmScreenImage(CwmScreen &screen, const CImagePtr &image);
  CwmScreenImage(CwmScreen &screen);
 ~CwmScreenImage();

  CwmImage *getImage(int width=-1, int height=-1);
  CwmImage *getTiledImage(int width, int height, int halign, int valign);

  CwmScreen &getScreen() const { return screen_; }

 private:
  CwmImage *lookup(int width, int height);
  CwmImage *lookupBest(int width, int height);

  CwmTiledImage *lookupTiled(int width, int height, int halign, int valign);
};

class CwmTiledImage {
 private:
  CwmImage *image_;
  int       width_;
  int       height_;
  int       halign_;
  int       valign_;

 public:
  CwmTiledImage(CwmScreen &screen, CwmImage *image, int width, int height,
                int halign, int valign);
 ~CwmTiledImage();

  CwmImage *getImage () { return image_ ; }
  int       getWidth () { return width_ ; }
  int       getHeight() { return height_; }
  int       getHAlign() { return halign_; }
  int       getVAlign() { return valign_; }
};

class CwmImage {
 private:
  CwmScreen  &screen_;
  CImagePtr   image_;
  int         width_;
  int         height_;
  CwmXPixmap *pixmap_;
  CwmXPixmap *pixmap_mask_;

 public:
  CwmImage(CwmScreen &screen, const CImagePtr &image);
  CwmImage(CwmScreen &screen, const CImagePtr &image, int width, int height);
 ~CwmImage();

  CImagePtr getImage () const { return image_ ; }
  int       getWidth () const { return width_ ; }
  int       getHeight() const { return height_; }

  void draw(CwmWindow *xwindow, CwmGraphics *graphics, int x, int y);
  void draw(CwmXPixmap *xpixmap, CwmGraphics *graphics, int x, int y);

  bool isTransparent();

  CwmXPixmap *createMask();

 private:
  void init();

  void createPixmap(CwmGraphics *graphics);
};

#endif
