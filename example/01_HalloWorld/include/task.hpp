#ifndef __EXAMPLE_MNTHREAD_HALLOWORLD_H__
#define __EXAMPLE_MNTHREAD_HALLOWORLD_H__

#include <libmn.h>

class helloWorld_task : public basic_thread {
public:
  helloWorld_task();

  virtual void* on_thread();
private:
  int k;
};

#endif