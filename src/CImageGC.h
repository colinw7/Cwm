#ifndef CIMAGE_GC_H
#define CIMAGE_GC_H

#include <CRGBA.h>

class CImageGC {
 private:
  CRGBA fg_rgba_;
  int   fg_ind_;
  CRGBA bg_rgba_;
  int   bg_ind_;

 public:
  CImageGC();
 ~CImageGC() { }

  void getForeground(CRGBA &rgba) const { rgba = fg_rgba_; }
  void getForeground(int   &ind ) const { ind  = fg_ind_ ; }

  void setForeground(const CRGBA &rgba);
  void setForeground(int          ind);

  void getBackground(CRGBA &rgba) const { rgba = bg_rgba_; }
  void getBackground(int   &ind ) const { ind  = bg_ind_ ; }

  void setBackground(const CRGBA &rgba);
  void setBackground(int          ind );
};

#endif
