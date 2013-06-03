class CwmLog {
 private:
  std::string  dir_;
  std::string  fileName_;
  CFile       *file_;

 public:
  CwmLog();
 ~CwmLog();

  void print(const std::string &msg);

 private:
  void setDir();

  bool open();
};
