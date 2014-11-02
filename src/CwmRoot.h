class CwmRootImage {
 public:
  CwmRootImage(CwmScreen &screen, const std::string &spec);
 ~CwmRootImage();

  void draw();

 private:
  CwmScreen &screen_;
  CwmImage  *image_;
  int        x_;
  int        y_;
  Pixel      bg_;
  Pixel      fg_;
  int        bgindex_;
};

#define CwmRootEventFunctionMgrInst CwmRootEventFunctionMgr::getInstance()

class CwmRootEventFunction;

class CwmRootEventFunctionMgr {
 public:
  static CwmRootEventFunctionMgr *getInstance();

  void add(CXNamedEvent *event, CwmFunctionDef *def, CwmData data);

  void process(CwmScreen &screen, XEvent *event);

  void deleteAll();

 private:
  CwmRootEventFunctionMgr();

  void addFunctions();

 private:
  typedef std::vector<CwmRootEventFunction *> RootEventFunctionList;

  RootEventFunctionList functions_;
};

class CwmRootEventFunction {
 public:
  CwmRootEventFunction(CXNamedEvent *event, CwmFunctionDef *function, CwmData data);
 ~CwmRootEventFunction();

  CXNamedEvent *getEvent() const { return event_; }

  void setFunction(CwmFunctionDef *function, CwmData data) {
    function_ = function;
    data_     = data;
  }

  void process(CwmScreen &screen) {
    function_->processRoot(screen, data_);
  }

 private:
  CXNamedEvent    *event_;
  CwmFunctionDef  *function_;
  CwmData          data_;
};
