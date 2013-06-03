#ifndef CGRADIENT_H
#define CGRADIENT_H

#include <CRGBA.h>
#include <CImageLib.h>
#include <cassert>

enum CGradientDirEnum {
  CGRADIENT_DIR_VERTICAL,
  CGRADIENT_DIR_HORIZONTAL,
  CGRADIENT_DIR_LDIAGONAL,
  CGRADIENT_DIR_RDIAGONAL
};

class CGradientDir {
 private:
  CGradientDirEnum value_;

 public:
  CGradientDir(CGradientDirEnum value) : value_(value) { }

  std::string toString() const {
    switch (value_) {
      case CGRADIENT_DIR_VERTICAL  : return "vertical";
      case CGRADIENT_DIR_HORIZONTAL: return "horizontal";
      case CGRADIENT_DIR_LDIAGONAL : return "ldiagonal";
      case CGRADIENT_DIR_RDIAGONAL : return "rdiagonal";
      default                      : assert(false);
    }
  };

  bool operator==(CGradientDir value) const { return value_ == value.value_; }
  bool operator!=(CGradientDir value) const { return value_ != value.value_; }
};

class CGradientData;

class CGradient {
 private:
  CGradientData *gradient_data_;

 public:
  CGradient(const CRGBA &start, const CRGBA &end, int width, int height, CGradientDir direction);
 ~CGradient();

  CImagePtr getImage() const;

  const CRGBA &getStart() const;
  const CRGBA &getEnd  () const;

 private:
  CGradient(const CGradient &rhs);
  CGradient &operator=(const CGradient &rhs);
};

#endif
