#include <CwmI.h>
#include <CEnv.h>
#include <CXScreen.h>

unsigned char
CwmNamedImage::image_data_[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xa1, 0xa5, 0xfc, 0xff, 0xff,
  0xff, 0x7f, 0x48, 0xad, 0xb5, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0x28, 0xad,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xa1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0x85, 0xfc, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0x85, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xfd, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xdd, 0xfc, 0xff, 0xff, 0xff, 0x01, 0x00, 0xa0,
  0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xed, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa1,
  0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xed, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xdd, 0xfc, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xa0,
  0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xed, 0x0c, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xdd, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0xed, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0x0c, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xed, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xdd, 0xfc, 0xff, 0xf1, 0x3f, 0xfc, 0x07, 0xa0,
  0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0xfc, 0xff, 0xf1,
  0x3f, 0xfc, 0x07, 0xa0, 0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0xdd, 0xfc, 0xff, 0xf1, 0x3f, 0xfc, 0x07, 0xa0, 0xed, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xdd, 0xfc, 0xff, 0xf1, 0x3f, 0xfc, 0x07, 0xa0,
  0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0xdd, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xfd, 0x30, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0x85, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0xfc, 0xff, 0xff,
  0xff, 0x3f, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0xfc, 0xff, 0x03,
  0x00, 0x00, 0x00, 0xa0, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xa0, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0,
  0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xfd, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xbf, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

CwmImageMgr *
CwmImageMgr::
getInstance()
{
  static CwmImageMgr *instance;

  if (! instance)
    instance = new CwmImageMgr();

  return instance;
}

CwmImageMgr::
CwmImageMgr()
{
}

CwmImageMgr::
~CwmImageMgr()
{
  for (auto &image : images_)
    delete image;
}

CwmImage *
CwmImageMgr::
getImage(CwmScreen &screen, const std::string &name, int width, int height)
{
  CwmNamedImage *named_image = lookup(name);

  if (! named_image) {
    if (name != "") {
      if (name[0] == ' ')
        return 0;

      std::string pathname = getPathName(name);

      if (pathname == "") {
        CwmMachineInst->logf("File %s not found on Path\n", name.c_str());
        return 0;
      }

      named_image = new CwmNamedImage(name, pathname);
    }
    else
      named_image = new CwmNamedImage("", "");

    images_.push_back(named_image);
  }

  return named_image->getImage(screen, width, height);
}

CwmImage *
CwmImageMgr::
getTiledImage(CwmScreen &screen, const std::string &name, int width, int height,
              int halign, int valign)
{
  CwmNamedImage *named_image = lookup(name);

  if (! named_image) {
    if (name != "") {
      if (name[0] == ' ')
        return 0;

      std::string pathname = getPathName(name);

      if (pathname == "") {
        CwmMachineInst->logf("File %s not found on Path\n", name.c_str());
        return 0;
      }

      named_image = new CwmNamedImage(name, pathname);
    }
    else
      named_image = new CwmNamedImage("", "");

    images_.push_back(named_image);
  }

  return named_image->getTiledImage(screen, width, height, halign, valign);
}

CwmNamedImage *
CwmImageMgr::
addImage(const std::string &name, const CImagePtr &image)
{
  CwmNamedImage *named_image = new CwmNamedImage(name, image);

  images_.push_back(named_image);

  return named_image;
}

CwmNamedImage *
CwmImageMgr::
lookup(const std::string &name)
{
  NamedImageList::const_iterator pimage1 = images_.begin();
  NamedImageList::const_iterator pimage2 = images_.end  ();

  for ( ; pimage1 != pimage2; ++pimage1)
    if ((*pimage1)->getName() == name)
      return *pimage1;

  return 0;
}

std::string
CwmImageMgr::
getPathName(const std::string &name)
{
  if (name == "")
    return "";

  std::string image_path_list = CEnvInst.get("CWM_IMAGE_PATH");

  std::string image_path = "";

  if (image_path_list != "") {
    image_path = searchPathList(image_path_list, name);

    if (image_path == "")
      CwmMachineInst->logf("Failed to find file %s in path %s\n",
                           name.c_str(), image_path_list.c_str());
  }

  if (image_path == "") {
    image_path_list = "/usr/include/X11/pixmaps:/usr/include/X11/bitmaps";

    image_path = searchPathList(image_path_list, name);

    if (image_path == "")
      CwmMachineInst->logf("Failed to find file %s in path %s\n",
                           name.c_str(), image_path_list.c_str());
  }

  return image_path;
}

std::string
CwmImageMgr::
searchPathList(const std::string &path_list, const std::string &name)
{
  CStrWords paths = CStrUtil::toFields(path_list, ":");

  auto num_paths = paths.size();

  for (int i = 0; i < int(num_paths); i++) {
    std::string filename = paths[i].getWord() + "/" + name;

    CFile file(filename);

    if (file.exists() && file.isRegular())
      return filename;
  }

  return "";
}

CwmNamedImage::
CwmNamedImage(const std::string &name, const std::string &pathname) :
 name_(name)
{
  if (name_ != "") {
    CFile file(pathname);

    CImageFileSrc src(pathname);

    image_ = CImageMgrInst->createImage(src);

    image_->read(&file);

    if (! image_)
      CwmMachineInst->logf("Failed to read image file %s\n", pathname.c_str());
  }
  else {
    CImageNameSrc src("Cwm/default_image");

    image_ = CImageMgrInst->createImage(src);

    image_->readXBM(image_data_, 64, 64);

    if (! image_)
      CwmMachineInst->log("Failed to read image data\n");
  }
}

CwmNamedImage::
CwmNamedImage(const std::string &name, const CImagePtr &image) :
 name_(name), image_(image)
{
}

CwmNamedImage::
~CwmNamedImage()
{
  for (auto &image : images_)
    delete image;
}

CwmImage *
CwmNamedImage::
getImage(CwmScreen &screen, int width, int height)
{
  CwmScreenImage *screen_image = lookup(screen);

  if (! screen_image) {
    screen_image = new CwmScreenImage(screen, image_);

    images_.push_back(screen_image);
  }

  return screen_image->getImage(width, height);
}

CwmImage *
CwmNamedImage::
getTiledImage(CwmScreen &screen, int width, int height, int halign, int valign)
{
  CwmScreenImage *screen_image = lookup(screen);

  if (! screen_image) {
    screen_image = new CwmScreenImage(screen, image_);

    images_.push_back(screen_image);
  }

  return screen_image->getTiledImage(width, height, halign, valign);
}

CwmScreenImage *
CwmNamedImage::
lookup(CwmScreen &screen)
{
  ScreenImageList::const_iterator pimage1 = images_.begin();
  ScreenImageList::const_iterator pimage2 = images_.end  ();

  for ( ; pimage1 != pimage2; ++pimage1)
    if (&(*pimage1)->getScreen() == &screen)
      return *pimage1;

  return 0;
}

CwmScreenImage::
CwmScreenImage(CwmScreen &screen, const CImagePtr &image) :
 screen_(screen), image_(image)
{
  CwmImage *image2 = new CwmImage(screen_, image_);

  images_.push_back(image2);
}

CwmScreenImage::
~CwmScreenImage()
{
  for (auto &image : images_)
    delete image;
}

CwmImage *
CwmScreenImage::
getImage(int width, int height)
{
  if (width <= 0 || height <= 0)
    return images_[0];

  CwmImage *image1 = lookup(width, height);

  if (! image1) {
    image1 = lookupBest(width, height);

    image1 = new CwmImage(screen_, image1->getImage(), width, height);

    images_.push_back(image1);
  }

  return image1;
}

CwmImage *
CwmScreenImage::
getTiledImage(int width, int height, int halign, int valign)
{
  CwmTiledImage *timage = lookupTiled(width, height, halign, valign);

  if (! timage) {
    timage = new CwmTiledImage(screen_, images_[0], width, height, halign, valign);

    tiled_images_.push_back(timage);
  }

  return timage->getImage();
}

CwmImage *
CwmScreenImage::
lookup(int width, int height)
{
  ImageList::const_iterator pimage1 = images_.begin();
  ImageList::const_iterator pimage2 = images_.end  ();

  for ( ; pimage1 != pimage2; ++pimage1)
    if ((*pimage1)->getWidth () == width &&
        (*pimage1)->getHeight() == height)
      return *pimage1;

  return 0;
}

CwmImage *
CwmScreenImage::
lookupBest(int width, int height)
{
  ImageList::const_iterator pimage1 = images_.begin();
  ImageList::const_iterator pimage2 = images_.end  ();

  if (pimage1 == pimage2)
    return 0;

  int dx = abs((*pimage1)->getWidth () - width );
  int dy = abs((*pimage1)->getHeight() - height);

  int       best_dxy   = dx*dx + dy*dy;
  CwmImage *best_image = *pimage1;

  while (pimage1 != pimage2) {
    dx = abs((*pimage1)->getWidth () - width );
    dy = abs((*pimage1)->getHeight() - height);

    int dxy = dx*dx + dy*dy;

    if (dxy < best_dxy) {
      best_dxy   = dxy;
      best_image = *pimage1;
    }

    ++pimage1;
  }

  return best_image;
}

CwmTiledImage *
CwmScreenImage::
lookupTiled(int width, int height, int halign, int valign)
{
  TiledImageList::const_iterator ptimage1 = tiled_images_.begin();
  TiledImageList::const_iterator ptimage2 = tiled_images_.end  ();

  for ( ; ptimage1 != ptimage2; ++ptimage1)
    if ((*ptimage1)->getWidth () == width  &&
        (*ptimage1)->getHeight() == height &&
        (*ptimage1)->getHAlign() == halign &&
        (*ptimage1)->getVAlign() == valign)
      return *ptimage1;

  return 0;
}

CwmTiledImage::
CwmTiledImage(CwmScreen &screen, CwmImage *image, int width, int height, int halign, int valign) :
 image_(image), width_(width), height_(height), halign_(halign), valign_(valign)
{
  CImagePtr cimage = image_->getImage();

  CHAlignType halign2;
  CVAlignType valign2;

  switch (halign) {
    case CHALIGN_TYPE_LEFT:
      halign2 = CHALIGN_TYPE_LEFT;
      break;
    case CHALIGN_TYPE_CENTER:
      halign2 = CHALIGN_TYPE_CENTER;
      break;
    default:
      halign2 = CHALIGN_TYPE_RIGHT;
      break;
  }

  switch (halign) {
    case CVALIGN_TYPE_TOP:
      valign2 = CVALIGN_TYPE_TOP;
      break;
    case CVALIGN_TYPE_CENTER:
      valign2 = CVALIGN_TYPE_CENTER;
      break;
    default:
      valign2 = CVALIGN_TYPE_BOTTOM;
      break;
  }

  CImageTileData tile(halign2, valign2);

  CImagePtr cimage1 = cimage->tile(width, height, tile);

  image_ = new CwmImage(screen, cimage1);
}

CwmTiledImage::
~CwmTiledImage()
{
  delete image_;
}

//-----------------

CwmImage::
CwmImage(CwmScreen &screen, const CImagePtr &image) :
 screen_(screen), image_(image)
{
  width_  = int(image_->getWidth ());
  height_ = int(image_->getHeight());

  init();
}

CwmImage::
CwmImage(CwmScreen &screen, const CImagePtr &image, int width, int height) :
 screen_(screen), width_(width), height_(height)
{
  image_ = image->resize(width_, height_);

  init();
}

void
CwmImage::
init()
{
  pixmap_      = 0;
  pixmap_mask_ = 0;
}

CwmImage::
~CwmImage()
{
  delete pixmap_;
  delete pixmap_mask_;
}

void
CwmImage::
draw(CwmWindow *xwindow, CwmGraphics *graphics, int x, int y)
{
  if (! pixmap_)
    createPixmap(graphics);

  if (pixmap_mask_ != 0)
    graphics->startClip(pixmap_mask_, x, y);

  graphics->copyArea(pixmap_, xwindow, 0, 0, width_, height_, x, y);

  if (pixmap_mask_ != 0)
    graphics->endClip();
}

void
CwmImage::
draw(CwmXPixmap *xpixmap, CwmGraphics *graphics, int x, int y)
{
  if (! pixmap_)
    createPixmap(graphics);

  if (pixmap_mask_ != 0)
    graphics->startClip(pixmap_mask_, x, y);

  graphics->copyArea(pixmap_, xpixmap, 0, 0, width_, height_, x, y);

  if (pixmap_mask_ != 0)
    graphics->endClip();
}

bool
CwmImage::
isTransparent()
{
  return image_->isTransparent();
}

void
CwmImage::
createPixmap(CwmGraphics *graphics)
{
  pixmap_ = new CwmXPixmap(screen_, width_, height_);

  graphics->fillRectangle(pixmap_, 0, 0, width_, height_);

  CXScreen *cxscreen = screen_.getCXScreen();

  CXImage *ximage = image_.cast<CXImage>();

  if (ximage)
    ximage->draw(cxscreen, pixmap_->getXPixmap(), graphics->getGC(), 0, 0);

  pixmap_mask_ = createMask();
}

CwmXPixmap *
CwmImage::
createMask()
{
  CXScreen *cxscreen = screen_.getCXScreen();

  Pixmap mask_pixmap = cxscreen->createMask(image_);

  CwmXPixmap *xpixmap = new CwmXPixmap(screen_, mask_pixmap, width_, height_, 1);

  return xpixmap;
}
