#ifndef CWM_UNIX_CMD_H
#define CWM_UNIX_CMD_H

class UnixCmd {
 public:
  UnixCmd(CwmScreen &screen, const std::string &command);

  void setAddDisplay(bool add_display) { add_display_ = add_display; }

  void run();

 private:
  CwmScreen   &screen_;
  std::string  command_;
  int          status_;
  bool         add_display_;
};

#endif
