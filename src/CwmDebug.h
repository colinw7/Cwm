class CwmDebug {
 private:
  bool debug_;

 public:
  CwmDebug() { debug_ = false; }

  bool getDebug() { return debug_; }
  void setDebug(bool debug) { debug_ = debug; }

  void print(const std::string &msg);
};
