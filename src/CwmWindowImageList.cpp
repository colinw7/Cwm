#include "CwmI.h"
#include "CXScreen.h"

CwmWindowImageList::
CwmWindowImageList(CwmWMWindow *window) :
 window_(window)
{
  CwmScreen &screen = window->getScreen();

  string image_file = CwmCustomDeskIconMgrInst->getIcon(window);

  if (image_file != "") {
    CwmImage *image = CwmImageMgrInst->getImage(screen, image_file);

    if (image != NULL)
      addImage(image, image_file);
  }

  image_file = CwmCustomDeskIconMgrInst->getIconSmall(window);

  if (image_file != "") {
    CwmImage *image = CwmImageMgrInst->getImage(screen, image_file);

    if (image != NULL)
      addImage(image, image_file);
  }

  image_file = CwmResourceDataInst->getIconImage();

  if (image_file != "") {
    CwmImage *image = CwmImageMgrInst->getImage(screen, image_file);

    if (image != NULL)
      addImage(image, image_file);
  }

  Window xwin = window->getWMIconWindowHint();

  if (! CwmMachineInst->isValidWindow(xwin))
    xwin = None;

  if (xwin != None) {
    string name = " " + window->getNameHint() + "_icon_window";

    CwmImage *image = CwmImageMgrInst->getImage(screen, name);

    if (image == NULL) {
      CXScreen *cxscreen = screen.getCXScreen();

      CImagePtr cimage;

      cxscreen->windowToImage(xwin, cimage);

      CwmImageMgrInst->addImage(name, cimage);

      image = CwmImageMgrInst->getImage(screen, name);
    }

    addImage(image, name);
  }

  Pixmap pixmap = window->getIconPixmapHint();

  if (pixmap != None) {
    string name = "Cwm/" + window->getNameHint() + "_icon_pixmap";

    CwmImage *image = CwmImageMgrInst->getImage(screen, name);

    if (image == NULL) {
      CXScreen *cxscreen = screen.getCXScreen();

      CImagePtr cimage;

      cxscreen->windowToImage(pixmap, cimage);

      Pixmap mask_pixmap = window->getIconMaskHint();

      if (mask_pixmap != None) {
        CImagePtr mask_cimage;

        cxscreen->windowToImage(mask_pixmap, mask_cimage);

        mask_cimage->setAlphaByGray(true);

        CImageNameSrc src(name);

        CImagePtr mask_image = CImageMgrInst->createImage(src);

        mask_image = mask_cimage;

        cimage->alphaMask(mask_image);
      }

      CwmImageMgrInst->addImage(name, cimage);

      image = CwmImageMgrInst->getImage(screen, name);
    }

    addImage(image, name);
  }
}

CwmWindowImageList::
~CwmWindowImageList()
{
  std::for_each(images_.begin(), images_.end(), CDeletePointer());
}

void
CwmWindowImageList::
addImage(CwmImage *image, const string &name)
{
  CwmWindowImage *window_image = new CwmWindowImage(image, name);

  images_.push_back(window_image);
}

CwmImage *
CwmWindowImageList::
getImage(int width, int height)
{
  CwmWindowImage *image = NULL;
  int             dxy   = 0;

  WindowImageList::const_iterator pimage1 = images_.begin();
  WindowImageList::const_iterator pimage2 = images_.end  ();

  for ( ; pimage1 != pimage2; ++pimage1) {
    int dx = abs((*pimage1)->getWidth () - width );
    int dy = abs((*pimage1)->getHeight() - height);

    int dxy1 = dx*dx + dy*dy;

    if (image == NULL || dxy1 < dxy) {
      image = *pimage1;

      dxy = dxy1;
    }
  }

  if (image == NULL)
    return NULL;

  int dx = width  - image->getWidth ();
  int dy = height - image->getHeight();

  if (dx < 0 || dy < 0) {
    CwmImage *image1 =
      CwmImageMgrInst->getImage(window_->getScreen(), image->getName(),
                                width, height);

    addImage(image1, image->getName());

    return image1;
  }
  else
    return image->getImage();
}

CwmImage *
CwmWindowImageList::
getImage()
{
  if (images_.size() == 0)
    return NULL;

  return images_[0]->getImage();
}

CwmWindowImage::
CwmWindowImage(CwmImage *image, const string &name) :
 image_(image), name_(name)
{
}

CwmWindowImage::
~CwmWindowImage()
{
}

int
CwmWindowImage::
getWidth() const
{
  return image_->getWidth();
}

int
CwmWindowImage::
getHeight() const
{
  return image_->getHeight();
}
