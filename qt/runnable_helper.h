#ifndef RUNNABLE_HELPER_H
#define RUNNABLE_HELPER_H

#include <QRunnable>

template <typename T>
class RunnableHelper : public QRunnable
{
public:
  typedef void (T::*CallbackFunc)();

  RunnableHelper(T *_dlg, CallbackFunc _func)
    : dlg(_dlg), func(_func) { }

private:
  void run() { (dlg->*func)(); }

private:
  T* dlg;
  CallbackFunc func;
};

#endif // RUNNABLE_HELPER_H
