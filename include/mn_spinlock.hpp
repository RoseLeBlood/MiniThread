#ifndef MINLIB_ESP32_SPINLOCK_
#define MINLIB_ESP32_SPINLOCK_

#include "mn_error.hpp"


class basic_spinlock {
public:
  basic_spinlock(int count = 1, int maxcount = 0x7fffffff);
  virtual ~basic_spinlock();

  virtual int create();
  virtual int destroy();

	virtual int lock(unsigned int timeout = (unsigned int) 0xffffffffUL);
	virtual int unlock();

  virtual bool try_lock();

  bool is_initialized() const { return m_bisinitialized; }
  int get_count() const;

protected:
	void* m_pSpinlock;
  int m_uiCount;
  int m_uiMaxCount;
	bool m_bisinitialized;
};

class binary_semaphore : public basic_spinlock {
public:
  binary_semaphore();

  virtual int create();
};



using spinlock_t = basic_spinlock;
using semaphore_t = binary_semaphore;

#endif
