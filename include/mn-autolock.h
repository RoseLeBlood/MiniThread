#ifndef _MINLIB_AUTOLOCK_H_
#define _MINLIB_AUTOLOCK_H_

#include "mn-config.h"
#include "mn-mutex.h"
#include "mn-spinlock.h"

template <class MUTEX = basic_mutex>
class  basic_autolock
{
public:
	basic_autolock(MUTEX &m) : m_ref_mutex(m) {
    m_ref_mutex.lock();
  }
	~basic_autolock() {
     m_ref_mutex.unlock();
   }
private:
	MUTEX &m_ref_mutex;
};

using autospin_t = basic_autolock<spinlock_t>;
using automutx_t = basic_autolock<mutex_t>;

#if MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_MUTEX
using autolock_t = basic_autolock<mutex_t>;
#elif MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_SPINLOCK
using autolock_t = basic_autolock<spinlock_t>;
#endif

#endif
