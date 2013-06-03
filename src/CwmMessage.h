#define CwmMessageInst CwmMessage::getInstance()

class CwmMessage {
 private:
  Window xwin_;
  int    change_count_;

 public:
  static CwmMessage *getInstance();

  void init();

  bool processClientMessage(XClientMessageEvent *event);

  void updateChangeCount();

 private:
  CwmMessage();
 ~CwmMessage();

  CwmWMWindow *stringToWindow(const std::string &str);
};
