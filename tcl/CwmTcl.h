class CwmTclCanvas;

class CwmTcl : public CTclApp {
 private:
  Window        server_xwin_;
  Window        client_xwin_;
  CwmTclCanvas *canvas_;
  int           change_count_;

 public:
  CwmTcl(int argc, char **argv);
 ~CwmTcl();

  void addCanvas(CwmTclCanvas *canvas);

  string sendClientMessage(const string &str, int argc, const char **argv);
  string sendClientMessage(const string &str);

  void redraw();

  void mousePress(int button, int x, int y);
  void keyPress(const string &key);

  void checkUpdate();

 protected:
  void startup();

  void createWindow();

  void addCommands();

  string getTclStr();
};
