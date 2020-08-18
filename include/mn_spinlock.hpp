#ifndef MINLIB_ESP32_SPINLOCK_
#define MINLIB_ESP32_SPINLOCK_

#include "mn_error.hpp"


class basic_spinlock {
public:
  basic_spinlock(int count = 1);
  ~basic_spinlock();

  int create();
	int lock();
	int unlock();

  bool try_lock();

  bool is_initialized() const 						{ return m_bisinitialized; }
  int get_count() const;
private:
	void* m_pSpinlock;
  int m_uiCount;
	bool m_bisinitialized;
};

using spinlock_t = basic_spinlock;


#endif
