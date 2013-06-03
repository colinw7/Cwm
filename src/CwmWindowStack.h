#ifndef CWM_WINDOW_STACK_H
#define CWM_WINDOW_STACK_H

class CwmScreenWindowGroups;
class CwmCirculateWindowStack;

class CwmWindowGroup {
 private:
  typedef vector<CwmWMWindow *> WMWindowList;

  WMWindowList window_stack_;

 public:
  CwmWindowGroup(CwmWMWindow *window);
 ~CwmWindowGroup();

  void restack();
  void map();
  void unmap();

  bool contains(CwmWMWindow *window);

  int size() const;

  CwmWMWindow &getTopWindow   () const;
  CwmWMWindow &getBottomWindow() const;

  CwmWMWindow &operator[](int i) const;

 private:
  void stackChildren(CwmWMWindow *window);
  void stackParents(CwmWMWindow *window);
};

class CwmCirculateWindowStack {
 private:
  typedef vector<CwmWindowGroup *> WindowGroupList;

  CwmScreen             &screen_;
  CwmScreenWindowGroups *screen_window_groups_;
  WindowGroupList        window_groups_;

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

  int size() const;

  CwmWindowGroup &getTopGroup   () const;
  CwmWindowGroup &getBottomGroup() const;

  CwmWindowGroup &operator[](int i) const;

  bool isTop(CwmWMWindow *window);
};

class CwmScreenWindowGroups {
 private:
  typedef vector<CwmWindowGroup *> WindowGroupList;

  WindowGroupList window_groups_;

 public:
  CwmScreenWindowGroups(CwmScreen &screen);
 ~CwmScreenWindowGroups();

  int size() const;

  CwmWindowGroup &getTopGroup   () const;
  CwmWindowGroup &getBottomGroup() const;

  CwmWindowGroup &operator[](int i) const;
};

#endif
