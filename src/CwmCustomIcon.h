#define CwmCustomIconMgrInst CwmCustomIconMgr::getInstance()

class CwmCustomIconMgr {
 public:
  static CwmCustomIconMgr *getInstance();

  CwmCustomIconMgr();
 ~CwmCustomIconMgr();

  void addCustomIcon(const std::string &icon, const std::string &label, const std::string &command,
                     const std::string &bg, const std::string &fg, int x, int y);

  void addCustomIcons(CwmScreen &screen);

 private:
  typedef std::list<CwmCustomIcon *> CustomIconList;

  CustomIconList custom_icons_;
};

class CwmCustomIcon {
 public:
  CwmCustomIcon(const std::string &icon, const std::string &label, const std::string &command,
                const std::string &bg, const std::string &fg, int x, int y);
 ~CwmCustomIcon();

  std::string getIcon   () const;
  std::string getLabel  () const;
  std::string getCommand() const;
  std::string getBg     () const;
  std::string getFg     () const;
  int         getX      () const;
  int         getY      () const;

  CwmImage    *getImage(CwmScreen &screen) const;
  Pixel        getBgPixel(CwmScreen &screen) const;
  Pixel        getFgPixel(CwmScreen &screen) const;
  CwmColor    *getColor(CwmScreen &screen) const;
  CwmFont     *getFont(CwmScreen &screen) const;
  CwmGraphics *getGraphics(CwmScreen &screen) const;

 private:
  std::string icon_;
  std::string label_;
  std::string command_;
  std::string bg_;
  std::string fg_;
  int         x_;
  int         y_;
};
