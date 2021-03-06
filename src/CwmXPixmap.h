#define CwmXPixmapMgrInst CwmXPixmapMgr::getInstance()

class CwmXPixmapMgr {
 public:
  static CwmXPixmapMgr *getInstance();

  CwmXPixmapMgr();

  void add(CwmXPixmap *xpixmap);
  void remove(CwmXPixmap *xpixmap);

 private:
  typedef std::list<CwmXPixmap *> XPixmapList;

  XPixmapList xpixmaps_;
};

class CwmXPixmap {
 public:
  CwmXPixmap(CwmScreen &screen, int width, int height, int depth=-1);
  CwmXPixmap(CwmScreen &screen, Pixmap xpixmap, int width, int height, int depth=-1);
 ~CwmXPixmap();

  CwmScreen &getScreen () const { return screen_ ; }
  Pixmap     getXPixmap() const { return xpixmap_; }
  int        getWidth  () const { return width_  ; }
  int        getHeight () const { return height_ ; }
  int        getDepth  () const { return depth_  ; }

  GC createGC();

 private:
  CwmScreen &screen_;
  Pixmap     xpixmap_;
  int        width_;
  int        height_;
  int        depth_;
  bool       owner_;
};
