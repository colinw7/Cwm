#include <CwmDecorationI.h>
#include <CGradient.h>

static const uint event_mask =
  ButtonPressMask | ButtonReleaseMask |
  EnterWindowMask | LeaveWindowMask   |
  KeyPressMask    | ExposureMask;

CwmTitleChild::
CwmTitleChild(CwmDecoration *decoration, CwmWMWindow *window) :
 CwmWindowChild(decoration, window, event_mask, CWM_CURSOR_TITLE),
 gradient1_(0), gradient2_(0), gradient3_(0), gradient4_(0),
 gradient_width_(0), gradient_height_(0), focus_graphics_(0), nofocus_graphics_(0)
{
  sides_ = CSIDE_TYPE_HORIZONTAL;
}

CwmTitleChild::
~CwmTitleChild()
{
  delete gradient1_;
  delete gradient2_;
  delete gradient3_;
  delete gradient4_;
  delete focus_graphics_;
  delete nofocus_graphics_;
}

void
CwmTitleChild::
calcSize(int *x, int *y, int *width, int *height)
{
  CwmFrameWindow *frame    = window_    ->getFrame   ();
  CwmWindowChild *menu     = decoration_->getMenu    ();
  CwmWindowChild *close    = decoration_->getClose   ();
  CwmWindowChild *minimize = decoration_->getMinimize();
  CwmWindowChild *maximize = decoration_->getMaximize();

  if (! window_->getRolledUp()) {
    *x      = decoration_->getLeftBorder();
    *y      = decoration_->getTopBorder ();
    *width  = frame->getWidth() - *x - decoration_->getRightBorder();
    *height = decoration_->getTitleHeight();

    if (menu->getMapped()) {
      *x     += menu->getWidth();
      *width -= menu->getWidth();
    }

    if (close->getMapped())
      *width -= close->getWidth();

    if (minimize->getMapped())
      *width -= minimize->getWidth();

    if (maximize->getMapped())
      *width -= maximize->getWidth();
  }
  else {
    *x      = 0;
    *y      = 0;
    *width  = frame->getWidth();
    *height = decoration_->getTitleHeight();
  }
}

void
CwmTitleChild::
detail()
{
  CwmTitlePatternType type = CwmResourceDataInst->getTitlePatternType();

  if (type == CWM_TITLE_PATTERN_GRADIENT)
    detailGradient();
  else
    detailSolid();
}

void
CwmTitleChild::
detailGradient()
{
  std::string name = window_->getName();

  int width  = getWidth ();
  int height = getHeight();

  if (width <= 2 || height <= 2)
    return;

  if (width != gradient_width_ || height != gradient_height_) {
    delete gradient1_;
    delete gradient2_;
    delete gradient3_;
    delete gradient4_;

    gradient1_ = 0;
    gradient2_ = 0;
    gradient3_ = 0;
    gradient4_ = 0;
  }

  if (gradient1_ == 0) {
    CwmColor *color1 = CwmResourceDataInst->getTitleGradientColor(window_->getScreen());
    CwmColor *color2 = CwmResourceDataInst->getTitleGradientFocusColor(window_->getScreen());

    CRGBA start;
    CRGBA end;

    start = color2->getBgRGBA();
    end   = color2->getFgRGBA();

    gradient1_ = new CGradient(start, end, width - 2, height - 2, CGRADIENT_DIR_RDIAGONAL);

    start = color2->getFgRGBA();
    end   = color2->getBgRGBA();

    gradient2_ = new CGradient(start, end, width - 2, height - 2, CGRADIENT_DIR_RDIAGONAL);

    start = color1->getBgRGBA();
    end   = color1->getFgRGBA();

    gradient3_ = new CGradient(start, end, width - 2, height - 2, CGRADIENT_DIR_RDIAGONAL);

    start = color1->getFgRGBA();
    end   = color1->getBgRGBA();

    gradient4_ = new CGradient(start, end, width - 2, height - 2, CGRADIENT_DIR_RDIAGONAL);

    gradient_width_  = width;
    gradient_height_ = height;
  }

  CGradient *gradient = 0;

  if (decoration_->hasFocus()) {
    if (selected_)
      gradient = gradient1_;
    else
      gradient = gradient2_;
  }
  else {
    if (selected_)
      gradient = gradient3_;
    else
      gradient = gradient4_;
  }

  CImagePtr image1 = gradient->getImage();

  CwmImage *cimage = new CwmImage(window_->getScreen(), image1);

  cimage->draw(xwindow_, graphics_, 1, 1);

  delete cimage;

  graphics_->drawHLine(xwindow_, 0, width - 1,          0, gradient->getStart());
  graphics_->drawHLine(xwindow_, 0, width - 1, height - 1, gradient->getEnd  ());

  graphics_->drawVLine(xwindow_, 0        , 0, height - 1, gradient->getStart());
  graphics_->drawVLine(xwindow_, width - 1, 0, height - 1, gradient->getEnd  ());

  drawText(name);
}

void
CwmTitleChild::
detailSolid()
{
  std::string name = window_->getName();

  int width  = getWidth ();
  int height = getHeight();

  if (width <= 2 || height <= 2)
    return;

  if (focus_graphics_ == 0) {
    CwmFont  *font  = CwmResourceDataInst->getFocusFont(window_->getScreen());
    CwmColor *color = CwmResourceDataInst->getTitleFocusColor(window_->getScreen());

    focus_graphics_ = new CwmGraphics(window_->getScreen(), font, color);
  }

  if (nofocus_graphics_ == 0) {
    CwmFont  *font  = CwmResourceDataInst->getFont(window_->getScreen());
    CwmColor *color = CwmResourceDataInst->getTitleColor(window_->getScreen());

    nofocus_graphics_ = new CwmGraphics(window_->getScreen(), font, color);
  }

  if (decoration_->hasFocus())
    focus_graphics_  ->fillRectangle(xwindow_, 0, 0, width, height);
  else
    nofocus_graphics_->fillRectangle(xwindow_, 0, 0, width, height);

  drawText(name);
}
