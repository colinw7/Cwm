#ifndef CWM_H
#define CWM_H

#define CwmInst Cwm::getInstance()

class Cwm {
 public:
  static Cwm *getInstance();

 ~Cwm();

  void init(int argc, char **argv);
  void term(int retCode);

  void restart(CwmScreen &screen, const std::string &command);
  void quit();

  void setRootImage(const std::string &filename);

  void getArgs(int *argc, char ***argv) { *argc = argc_; *argv = argv_; }

  std::string getResource(const std::string &name, const std::string &fallback);

  std::string getRootImage() { return root_image_; }

  bool getPedantic() { return pedantic_; }

  bool getPrintHints() { return print_hints_; }

  bool getGnome() { return gnome_; }

 private:
  Cwm();

  void saveArgs(int argc, char **argv);
  void parseArgs(int *argc, char **argv);
  void setHomeDir();
  void setSignals();
  void controlScreens();
  void processWindows();
  void processScreenWindows(CwmScreen &screen);
  void resetIconWindows(int i, Window *children, int num_children);
  void drawRootImages();
  void addCustomIcons();
  void initClickToFocus();
  void processScreenStartup();
  void printScreenWindows(CwmScreen &screen);
  void cleanup();

  static void signalHandler(int sig);

 private:
  int           argc_;
  char        **argv_;
  bool          pedantic_;
  bool          synchronize_;
  std::string   root_image_;
  std::string   home_dir_;
  bool          print_hints_;
  std::string   display_name_;
  bool          gnome_;
};

#endif
