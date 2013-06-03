#define CwmConfigInst CwmConfig::getInstance()

class CConfig;

class CwmConfig {
 private:
  CConfig *config_;

 public:
  static CwmConfig *getInstance();

 ~CwmConfig();

  void save();

  std::string getSectionValue(const std::string &path, const std::string &section,
                              const std::string &fallback);
  int         getSectionValue(const std::string &path, const std::string &section,
                              int fallback);
  void        setSectionValue(const std::string &path, const std::string &section,
                              const std::string &value);

  std::string getValue(const std::string &path, const std::string &fallback);
  int         getValue(const std::string &path, int fallback);
  void        setValue(const std::string &path, const std::string &value);

  void getGroups(const std::string &path, StringVectorT &groupNames,
                 StringVectorT &groupPaths);

  void getSections(const std::string &path, StringVectorT &sections);

 private:
  CwmConfig();
};
