#ifndef CWM_WINDOW_STACK_H
#define CWM_WINDOW_STACK_H

#include <CwmTypes.h>

class CwmScreenWindowGroups;
class CwmCirculateWindowStack;

class CwmWindowGroup {
 public:
  CwmWindowGroup(CwmWMWindow *window);
 ~CwmWindowGroup();

  void restack();
  void map();
  void unmap();

  bool contains(CwmWMWindow *window);

  uint size() const;

  CwmWMWindow &getTopWindow   () const;
  CwmWMWindow &getBottomWindow() const;

  CwmWMWindow &operator[](uint i) const;

 private:
  void stackChildren(CwmWMWindow *window);
  void stackParents(CwmWMWindow *window);

 private:
  typedef std::vector<CwmWMWindow *> WMWindowList;

  WMWindowList window_stack_;
};

class CwmCirculateWindowStack {
 public:
  CwmCirculateWindowStack(CwmScreen &screen);
 ~CwmCirculateWindowStack();

  void update();

  void circulateUp();
  void circulateDown();

  void lower(CwmWMWindow *window);
  void raise(CwmWMWindow *window);

  CwmWindowGroup *getWindowGroup(CwmWMWindow *window);

  void restack();

  uint size() const;

  CwmWindowGroup &getTopGroup   () const;
  CwmWindowGroup &getBottomGroup() const;

  CwmWindowGroup &operator[](uint i) const;

  bool isTop(CwmWMWindow *window);

 private:
  typedef std::vector<CwmWindowGroup *> WindowGroupList;

  CwmScreen             &screen_;
  CwmScreenWindowGroups *screen_window_groups_;
  WindowGroupList        window_groups_;
};

class CwmScreenWindowGroups {
 public:
  CwmScreenWindowGroups(CwmScreen &screen);
 ~CwmScreenWindowGroups();

  uint size() const;

  CwmWindowGroup &getTopGroup   () const;
  CwmWindowGroup &getBottomGroup() const;

  CwmWindowGroup &operator[](uint i) const;

 private:
  typedef std::vector<CwmWindowGroup *> WindowGroupList;

  WindowGroupList window_groups_;
};

#endif
