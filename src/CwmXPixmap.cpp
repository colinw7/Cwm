#include <CwmI.h>

CwmXPixmapMgr *
CwmXPixmapMgr::
getInstance()
{
  static CwmXPixmapMgr *instance;

  if (! instance)
    instance = new CwmXPixmapMgr();

  return instance;
}

CwmXPixmapMgr::
CwmXPixmapMgr()
{
}

void
CwmXPixmapMgr::
add(CwmXPixmap *xpixmap)
{
  xpixmaps_.push_back(xpixmap);
}

void
CwmXPixmapMgr::
remove(CwmXPixmap *xpixmap)
{
  xpixmaps_.remove(xpixmap);
}

CwmXPixmap::
CwmXPixmap(CwmScreen &screen, int width, int height, int depth) :
 screen_(screen), xpixmap_(None), width_(width), height_(height), depth_(depth)
{
  if (depth_ <= 0)
    depth_ = screen.getDepth();

  if (width_ <= 0)
    width_ = -1;

  if (height_ <= 0)
    height_ = -1;

  xpixmap_ = CwmMachineInst->createPixmap(uint(width_), uint(height_), uint(depth_));

  owner_ = true;

  CwmXPixmapMgrInst->add(this);
}

CwmXPixmap::
CwmXPixmap(CwmScreen &screen, Pixmap xpixmap, int width, int height, int depth) :
 screen_(screen), xpixmap_(xpixmap), width_(width), height_(height), depth_(depth)
{
  if (depth_ <= 0)
    depth_ = screen_.getDepth();

  if (width_ <= 0)
    width_ = -1;

  if (height_ <= 0)
    height_ = -1;

  owner_ = false;

  CwmXPixmapMgrInst->add(this);
}

CwmXPixmap::
~CwmXPixmap()
{
  CwmXPixmapMgrInst->remove(this);

  if (owner_ && xpixmap_ != None)
    CwmMachineInst->freePixmap(xpixmap_);
}

GC
CwmXPixmap::
createGC()
{
  GC gc = CwmMachineInst->createGC(xpixmap_, 0, 1);

  return gc;
}
