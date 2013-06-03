class CwmRootImage {
 private:
  CwmScreen &screen_;
  CwmImage  *image_;
  int        x_;
  int        y_;
  Pixel      bg_;
  Pixel      fg_;
  int        bgindex_;

 public:
  CwmRootImage(CwmScreen &screen, const std::string &spec);
 ~CwmRootImage();

  void draw();
};

#define CwmRootEventFunctionMgrInst CwmRootEventFunctionMgr::getInstance()

class CwmRootEventFunction;

class CwmRootEventFunctionMgr {
 private:
  typedef vector<CwmRootEventFunction *> RootEventFunctionList;

  RootEventFunctionList functions_;

 public:
  static CwmRootEventFunctionMgr *getInstance();

  void add(CXNamedEvent *event, CwmFunctionDef *def, CwmData data);

  void process(CwmScreen &screen, XEvent *event);

  void deleteAll();

 private:
  CwmRootEventFunctionMgr();

  void addFunctions();
};

class CwmRootEventFunction {
 private:
  CXNamedEvent    *event_;
  CwmFunctionDef  *function_;
  CwmData          data_;

 public:
  CwmRootEventFunction(CXNamedEvent *event, CwmFunctionDef *function,
                       CwmData data);
 ~CwmRootEventFunction();

  CXNamedEvent *getEvent() const { return event_; }

  void setFunction(CwmFunctionDef *function, CwmData data) {
    function_ = function;
    data_     = data;
  }

  void process(CwmScreen &screen) {
    function_->processRoot(screen, data_);
  }
};
