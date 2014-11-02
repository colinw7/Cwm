#include <CXCursor.h>

class CwmXCursor {
 public:
  CwmXCursor(CwmScreen &screen, CCursorType type);
 ~CwmXCursor();

  void recolor(CRGBA &bg, CRGBA &fg);

  CXCursor *getCursor() const { return xcursor_; }

 private:
  CXCursor *xcursor_;
};

class CwmCursorMgr {
 public:
  CwmCursorMgr(CwmScreen &screen);
 ~CwmCursorMgr();

  Cursor getCursor(CwmCursorType type) const;

 private:
  typedef std::vector<CwmXCursor *>             CursorList;
  typedef std::map<CwmCursorType, CwmXCursor *> CursorMap;

  CwmScreen  &screen_;
  CursorList  cursor_list_;
  CursorMap   cursor_map_;
};
