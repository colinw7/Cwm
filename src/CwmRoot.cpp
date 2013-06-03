#include <CwmI.h>
#include <sstream>
#include <iomanip>

struct CwmRootImageSpec {
  friend class CwmRootImage;

 private:
  string image_file_;
  string bg_color_;
  string fg_color_;
  int    bgindex_;
  bool   resize_image_;
  bool   tile_image_;
  bool   aspect_;
  int    halign_;
  int    valign_;

 public:
  CwmRootImageSpec(string spec) :
   image_file_(""), bg_color_("white"), fg_color_("black"), bgindex_(-1), resize_image_(false),
   tile_image_(false), aspect_(false), halign_(CHALIGN_TYPE_CENTER), valign_(CVALIGN_TYPE_CENTER) {
    decode(spec);
  }

 private:
  bool decode(string spec);
};

CwmRootEventFunctionMgr *
CwmRootEventFunctionMgr::
getInstance()
{
  static CwmRootEventFunctionMgr *instance;

  if (instance == NULL) {
    instance = new CwmRootEventFunctionMgr();

    instance->addFunctions();
  }

  return instance;
}

CwmRootEventFunctionMgr::
CwmRootEventFunctionMgr()
{
}

void
CwmRootEventFunctionMgr::
addFunctions()
{
  CXNamedEvent   *event    = new CXNamedEvent("<Button3>");
  CwmFunctionDef *function = new CwmFunctionDef("f.menu");

  string *data = new string("Root Functions");

  add(event, function, data);
}

void
CwmRootEventFunctionMgr::
add(CXNamedEvent *event, CwmFunctionDef *function, CwmData data)
{
  if (event == NULL)
    return;

  int num_functions = functions_.size();

  for (int i = 0; i < num_functions; i++) {
    CXNamedEvent *event1 = functions_[i]->getEvent();

    if (event1 != NULL && event1->matchEvent(event)) {
      functions_[i]->setFunction(function, data);

      return;
    }
  }

  CwmRootEventFunction *root_event_function =
    new CwmRootEventFunction(event, function, data);

  functions_.push_back(root_event_function);
}

void
CwmRootEventFunctionMgr::
process(CwmScreen &screen, XEvent *event)
{
  int num_functions = functions_.size();

  for (int i = 0; i < num_functions; i++) {
    CXNamedEvent *event1 = functions_[i]->getEvent();

    if (event1->matchEvent(event)) {
      functions_[i]->process(screen);
      return;
    }
  }
}

void
CwmRootEventFunctionMgr::
deleteAll()
{
  int num_functions = functions_.size();

  for (int i = 0; i < num_functions; i++)
    delete functions_[i];

  functions_.clear();
}

CwmRootEventFunction::
CwmRootEventFunction(CXNamedEvent *event, CwmFunctionDef *function, CwmData data) :
 event_(event), function_(function), data_(data)
{
}

CwmRootEventFunction::
~CwmRootEventFunction()
{
  delete event_;
}

CwmRootImage::
CwmRootImage(CwmScreen &screen, const string &spec) :
 screen_(screen), image_(NULL), x_(0), y_(0), bg_(0), fg_(0), bgindex_(-1)
{
  CwmRootImageSpec image_spec(spec);

  // TODO: use aspect ratio flag for resize

  bgindex_ = image_spec.bgindex_;

  bg_ = screen_.getPixel(image_spec.bg_color_, screen_.getWhitePixel());
  fg_ = screen_.getPixel(image_spec.fg_color_, screen_.getBlackPixel());

  CwmColor *color = screen_.getColor(fg_, bg_);

  CRGBA fg_rgba = color->getFgRGBA();
  CRGBA bg_rgba = color->getBgRGBA();

  x_ = 0;
  y_ = 0;

  image_ = NULL;

  if (image_spec.image_file_.size() == 0)
    return;

  if      (image_spec.resize_image_) {
    image_ = CwmImageMgrInst->getImage(screen_, image_spec.image_file_, screen_.getWidth(),
                                       screen_.getHeight());
  }
  else if (image_spec.tile_image_) {
    image_ = CwmImageMgrInst->getTiledImage(screen_, image_spec.image_file_,
                                            screen_.getWidth (), screen_.getHeight(),
                                            image_spec.halign_, image_spec.valign_);
  }
  else {
    image_ = CwmImageMgrInst->getImage(screen_, image_spec.image_file_);

    if (image_ != NULL) {
      if (image_spec.halign_ == CHALIGN_TYPE_CENTER)
        x_ = (screen_.getWidth() - image_->getWidth())/2;
      else
        x_ = screen_.getWidth() - image_->getWidth();

      if (x_ < 0)
        x_ = 0;

      if (image_spec.valign_ == CVALIGN_TYPE_CENTER)
        y_ = (screen_.getHeight() - image_->getHeight())/2;
      else
        y_ = screen_.getHeight() - image_->getHeight();

      if (y_ < 0)
        y_ = 0;
    }
  }

  if (image_ == NULL)
    return;

  CImagePtr cimage = image_->getImage();

  if (cimage->getNumColors() == 2) {
    cimage->setColor(0, bg_rgba);
    cimage->setColor(1, fg_rgba);
  }
}

CwmRootImage::
~CwmRootImage()
{
}

void
CwmRootImage::
draw()
{
  if (image_ == NULL)
    return;

  if (! screen_.getRoot()->grabPointer(0, CWM_CURSOR_BUSY))
    return;

  CImagePtr image1 = image_->getImage();

  CwmColor *color = NULL;

  if (bgindex_ != -1 && image1->hasColormap()) {
    double r, g, b, a;

    image1->getColorRGBA(bgindex_, &r, &g, &b, &a);

    std::ostringstream ostr;

    ostr << "#" << std::setw(2) << std::setfill('0') << (int) (r*255) <<
                   std::setw(2) << std::setfill('0') << (int) (g*255) <<
                   std::setw(2) << std::setfill('0') << (int) (b*255);

    Pixel pixel = screen_.getPixel(ostr.str(), screen_.getWhitePixel());

    color = screen_.getColor(pixel, pixel);
  }

  CwmGraphics *graphics = new CwmGraphics(screen_, NULL, color);

  CwmXPixmap *xpixmap =
    new CwmXPixmap(screen_, screen_.getWidth(), screen_.getHeight());

  graphics->fillRectangle(xpixmap, 0, 0, screen_.getWidth(), screen_.getHeight());

  image_->draw(screen_.getRoot(), graphics, 0, 0);

  image_->draw(xpixmap, graphics, x_, y_);

  screen_.getRoot()->setBackgroundPixmap(xpixmap);

  screen_.getRoot()->clear();

  delete xpixmap;

  delete graphics;

  CwmMachineInst->flushEvents();

  const CXAtom &prop = CwmMachineInst->getXSetRootIdAtom();

  Pixmap pixmap;

  if (CwmMachineInst->getPixmapProperty(screen_.getRoot()->getXWin(), prop, &pixmap))
    CwmMachineInst->killClient(pixmap);

  CwmMachineInst->deleteProperty(screen_.getRoot()->getXWin(), prop);

  CwmMachineInst->flushEvents();

  screen_.refresh();

  screen_.getRoot()->ungrabPointer();
}

bool
CwmRootImageSpec::
decode(string spec)
{
  CStrWords words = CStrUtil::toWords(spec, NULL);

  if (words.size() == 0)
    return false;

  image_file_ = words[0].getWord();

  for (int i = 1; i < words.size(); i++) {
    string word = words[i].getWord();

    if      (CStrUtil::casecmp(word, "resize") == 0)
      resize_image_ = true;
    else if (CStrUtil::casecmp(word, "tile") == 0)
      tile_image_ = true;
    else if (CStrUtil::casecmp(word, "bg") == 0) {
      if (i < words.size() - 1) {
        word = words[++i].getWord();

        bg_color_ = word;
      }
      else
        CwmMachineInst->log("Missing color for bg\n");
    }
    else if (CStrUtil::casecmp(word, "fg") == 0) {
      if (i < words.size() - 1) {
        word = words[++i].getWord();

        fg_color_ = word;
      }
      else
        CwmMachineInst->log("Missing color for fg\n");
    }
    else if (CStrUtil::casecmp(word, "halign") == 0) {
      if (i < words.size() - 1) {
        i++;

        word = words[i].getWord();

        if      (CStrUtil::casecmp(word, "left") == 0)
          halign_ = CHALIGN_TYPE_LEFT;
        else if (CStrUtil::casecmp(word, "right") == 0)
          halign_ = CHALIGN_TYPE_RIGHT;
        else if (CStrUtil::casecmp(word, "center") == 0 ||
                 CStrUtil::casecmp(word, "center") == 0)
          halign_ = CHALIGN_TYPE_CENTER;
        else
          CwmMachineInst->logf("Missing value for halign - %s\n", word.c_str());
      }
      else
        CwmMachineInst->log("Missing value for halign\n");
    }
    else if (CStrUtil::casecmp(word, "valign") == 0) {
      if (i < words.size() - 1) {
        i++;

        word = words[i].getWord();

        if      (CStrUtil::casecmp(word, "top") == 0)
          valign_ = CVALIGN_TYPE_TOP;
        else if (CStrUtil::casecmp(word, "bottom") == 0)
          valign_ = CVALIGN_TYPE_BOTTOM;
        else if (CStrUtil::casecmp(word, "center") == 0 ||
                 CStrUtil::casecmp(word, "center") == 0)
          valign_ = CVALIGN_TYPE_CENTER;
        else
          CwmMachineInst->logf("Missing value for valign - %s\n", word.c_str());
      }
      else
        CwmMachineInst->log("Missing value for valign\n");
    }
    else if (CStrUtil::casecmp(word, "bgindex") == 0) {
      if (i < words.size() - 1) {
        word = words[++i].getWord();

        bgindex_ = CStrUtil::toInteger(word);
      }
      else
        CwmMachineInst->log("Missing value for bgindex\n");
    }
    else if (CStrUtil::casecmp(word, "aspect") == 0)
      aspect_ = true;
    else
      CwmMachineInst->logf("Invalid root image option %s\n", word.c_str());
  }

  return true;
}
