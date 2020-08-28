#ifndef MINLIB_ESP32_TASKLETS_
#define MINLIB_ESP32_TASKLETS_



#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"
#include "mn_convar.hpp"

class basic_tasklet {
public:

    virtual int create(uint32_t parameter, TickType_t timeout = portMAX_DELAY);
    virtual int destroy();
protected:
    virtual int on_tasklet(uint32_t arg);
protected:
  static void runtaskletstub(void* parm, uint32_t parameter);
protected:
    semaphore_t     m_ssLock;
};

using tasklet_t = basic_tasklet;

#endif