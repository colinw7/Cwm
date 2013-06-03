#include "CXCursor.h"

class CwmXCursor {
 private:
  CXCursor *xcursor_;

 public:
  CwmXCursor(CwmScreen &screen, CCursorType type);
 ~CwmXCursor();

  void recolor(CRGBA &bg, CRGBA &fg);

  CXCursor *getCursor() const { return xcursor_; }
};

class CwmCursorMgr {
 private:
  typedef std::vector<CwmXCursor *>             CursorList;
  typedef std::map<CwmCursorType, CwmXCursor *> CursorMap;

  CwmScreen  &screen_;
  CursorList  cursor_list_;
  CursorMap   cursor_map_;

 public:
  CwmCursorMgr(CwmScreen &screen);
 ~CwmCursorMgr();

  Cursor getCursor(CwmCursorType type) const;
};
