#ifndef CWM_IDLE_H
#define CWM_IDLE_H

#define CwmIdleInst CwmIdle::getInstance()

class CwmIdleTimer;

class CwmIdleState {
 private:
  bool active_;

 public:
  CwmIdleState(bool active) : active_(active) { }
 ~CwmIdleState() { }

  bool getActive() { return active_; }
};

class CwmIdle {
 private:
  bool          enabled_;
  CwmIdleTimer *timer_;
  Window        idle_xwin_;

 public:
  static CwmIdle *getInstance();

 ~CwmIdle();

  void enable();
  void disable();
  void start();
  void timeOut();

 private:
  CwmIdle();

  static Boolean idleWorkProc(CwmData data);
};

#endif
