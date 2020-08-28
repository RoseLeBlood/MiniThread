#ifndef _MINLIB_AUTOLOCK_H_
#define _MINLIB_AUTOLOCK_H_

#include "mn_config.hpp"
#include "mn_mutex.hpp"
#include "mn_spinlock.hpp"
#include "mn_critical.hpp"


template <class LOCK = basic_mutex>
class  basic_autolock
{
public:
	basic_autolock(LOCK &m) : m_ref_lock(m) {
    m_ref_lock.lock();
  }
	~basic_autolock() {
     m_ref_lock.unlock();
   }
   basic_autolock(const basic_autolock&) = delete;
private:
	LOCK &m_ref_lock;
};

using autospin_t = basic_autolock<spinlock_t>;
using automutx_t = basic_autolock<mutex_t>;
using autosemp_t = basic_autolock<semaphore_t>;

using auto_critical_t = basic_autolock<basic_critical_lock>;
using auto_interrupt_t = basic_autolock<interrupts_lock_t>;
using auto_shedular_t = basic_autolock<sheduler_lock_t>;

#if (configUSE_RECURSIVE_MUTEXES == MN_THREAD_CONFIG_YES)
#include "mn_recursive_mutex.hpp"
using autoremutx_t = basic_autolock<remutex_t>;
#endif

#if MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_MUTEX
  using autolock_t = basic_autolock<mutex_t>;
#elif MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_SPINLOCK
  using autolock_t = basic_autolock<spinlock_t>;
#endif

#endif
