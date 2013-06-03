#include <CGradient.h>
#include <CImageGC.h>
#include <CImageDraw.h>
#include <CRGBA.h>

#define CGradientDataMgrInst CGradientDataMgr::getInstance()

class CGradientDataMgr {
 private:
  typedef std::list<CGradientData *> CGradientDataList;

  CGradientDataList gradient_data_list_;

 public:
  static CGradientDataMgr *getInstance();

  CGradientDataMgr();
 ~CGradientDataMgr();

  CGradientData *get(const CRGBA &start, const CRGBA &end,
                     int width, int height, CGradientDir direction);
  CGradientData *lookup(const CRGBA &start, const CRGBA &end,
                        int width, int height, CGradientDir direction);
  CGradientData *add(const CRGBA &start, const CRGBA &end,
                     int width, int height, CGradientDir direction);


  void incReference(CGradientData *gradient_data);
  void decReference(CGradientData *gradient_data);

  void remove(CGradientData *gradient_data);
};

class CGradientData {
 private:
  CImagePtr    image_;
  CRGBA        start_;
  CRGBA        end_;
  int          width_;
  int          height_;
  CGradientDir direction_;
  int          count_;

 public:
  CGradientData(const CRGBA &start, const CRGBA &end, int width, int height,
                CGradientDir direction);
 ~CGradientData();

  bool match(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction);

  void incReference();
  void decReference();

  int getNumReferences() const { return count_; }

  CImagePtr getImage() const { return image_; }

  const CRGBA &getStart() const { return start_; }
  const CRGBA &getEnd  () const { return end_  ; }
};

CGradient::
CGradient(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction) :
 gradient_data_(NULL)
{
  gradient_data_ = CGradientDataMgrInst->get(start, end, width, height, direction);

  CGradientDataMgrInst->incReference(gradient_data_);
}

CGradient::
~CGradient()
{
  CGradientDataMgrInst->decReference(gradient_data_);
}

CImagePtr
CGradient::
getImage() const
{
  return gradient_data_->getImage();
}

const CRGBA &
CGradient::
getStart() const
{
  return gradient_data_->getStart();
}

const CRGBA &
CGradient::
getEnd() const
{
  return gradient_data_->getEnd();
}

CGradientDataMgr *
CGradientDataMgr::
getInstance()
{
  static CGradientDataMgr *instance;

  if (! instance)
    instance = new CGradientDataMgr();

  return instance;
}

CGradientDataMgr::
CGradientDataMgr() :
 gradient_data_list_()
{
}

CGradientDataMgr::
~CGradientDataMgr()
{
  CGradientDataList::iterator p1 = gradient_data_list_.begin();
  CGradientDataList::iterator p2 = gradient_data_list_.end  ();

  for ( ; p1 != p2; ++p1)
    delete *p1;
}

CGradientData *
CGradientDataMgr::
get(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction)
{
  CGradientData *gradient_data = lookup(start, end, width, height, direction);

  if (gradient_data == NULL)
    gradient_data = add(start, end, width, height, direction);

  return gradient_data;
}

CGradientData *
CGradientDataMgr::
lookup(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction)
{
  CGradientDataList::iterator p1 = gradient_data_list_.begin();
  CGradientDataList::iterator p2 = gradient_data_list_.end  ();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->match(start, end, width, height, direction))
      return *p1;
  }

  return NULL;
}

CGradientData *
CGradientDataMgr::
add(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction)
{
  CGradientData *gradient_data = new CGradientData(start, end, width, height, direction);

  gradient_data_list_.push_back(gradient_data);

  return gradient_data;
}

void
CGradientDataMgr::
remove(CGradientData *gradient_data)
{
  gradient_data_list_.remove(gradient_data);
}

void
CGradientDataMgr::
incReference(CGradientData *gradient_data)
{
  gradient_data->incReference();
}

void
CGradientDataMgr::
decReference(CGradientData *gradient_data)
{
  gradient_data->decReference();

  if (gradient_data->getNumReferences() <= 0)
    remove(gradient_data);
}

CGradientData::
CGradientData(const CRGBA &start, const CRGBA &end, int width, int height,
              CGradientDir direction) :
 image_(), start_(start), end_(end), width_(width), height_(height),
 direction_(direction), count_(0)
{
  std::string fileName = "app://CGradientData?";

  fileName += "start="     + start.toString() + "&" +
              "end="       + end  .toString() + "&" +
              "width="     + CStrUtil::toString(width ) + "&"
              "height="    + CStrUtil::toString(height) + "&"
              "direction=" + CStrUtil::toString(height);

  CImageNameSrc src(fileName);

  image_ = CImageMgrInst->createImage(src);

  image_->setDataSize(width, height);

  CRGBA rgba(start);

  CImageDraw draw(image_);

  if      (direction == CGRADIENT_DIR_VERTICAL) {
    CRGBA drgba = (end - start)/(height - 1.0);

    CImageGC gc;

    for (int i = 0; i < height; i++) {
      gc.setForeground(rgba);

      draw.drawHLine(gc, 0, i, width - 1);

      rgba += drgba;
    }
  }
  else if (direction == CGRADIENT_DIR_HORIZONTAL) {
    CRGBA drgba = (end - start)/(width - 1.0);

    CImageGC gc;

    for (int i = 0; i < width; i++) {
      gc.setForeground(rgba);

      draw.drawVLine(gc, i, 0, height - 1);

      rgba += drgba;
    }
  }
  else if (direction == CGRADIENT_DIR_RDIAGONAL) {
    CRGBA drgba1 = (end - start)/(2.0*(width  - 1.0));
    CRGBA drgba2 = (end - start)/(2.0*(height - 1.0));

    CImageGC gc;

    for (int y1 = 0; y1 < height; y1++) {
      CRGBA rgba1 = rgba;

      for (int x1 = 0; x1 < width; x1++) {
        gc.setForeground(rgba);

        draw.drawPoint(gc, x1, y1);

        rgba += drgba1;
      }

      rgba = rgba1 + drgba2;
    }
  }
  else if (direction == CGRADIENT_DIR_LDIAGONAL) {
    CRGBA drgba1 = (end - start)/(2.0*(width  - 1.0));
    CRGBA drgba2 = (end - start)/(2.0*(height - 1.0));

    CImageGC gc;

    for (int y1 = height - 1; y1 >= 0; y1--) {
      CRGBA rgba1 = rgba;

      for (int x1 = width - 1; x1 >= 0; x1--) {
        gc.setForeground(rgba);

        draw.drawPoint(gc, x1, y1);

        rgba += drgba1;
      }

      rgba = rgba1 + drgba2;
    }
  }
}

bool
CGradientData::
match(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction)
{
  return (width == width_ && height == height_ && direction == direction_ &&
          start == start_ && end == end_);
}

CGradientData::
~CGradientData()
{
}

void
CGradientData::
incReference()
{
  ++count_;
}

void
CGradientData::
decReference()
{
  --count_;
}
