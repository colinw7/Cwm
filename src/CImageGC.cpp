#include <CImageGC.h>

CImageGC::
CImageGC()
{
}

void
CImageGC::
setForeground(const CRGBA &rgba)
{
  fg_rgba_ = rgba;
}

void
CImageGC::
setForeground(int ind)
{
  fg_ind_ = ind;
}

void
CImageGC::
setBackground(const CRGBA &rgba)
{
  bg_rgba_ = rgba;
}

void
CImageGC::
setBackground(int ind)
{
  bg_ind_ = ind;
}
