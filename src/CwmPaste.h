class CwmPaste {
 public:
  static void pasteText(CwmWMWindow *window, const std::string &text);

 private:
  static void pasteChar(CwmWMWindow *window, int c);
};
