#ifndef __EXAMPLE_MNTHREAD_HALLOWORLD_H__
#define __EXAMPLE_MNTHREAD_HALLOWORLD_H__

class helloWorld_task : public mthread {
public:
  helloWorld_task();

  virtual void* on_thread();
private:
  int k;
};

#endif