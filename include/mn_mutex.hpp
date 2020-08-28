#ifndef MINLIB_ESP32_MUTEX_
#define MINLIB_ESP32_MUTEX_

#include "mn_config.hpp"
#include "mn_spinlock.hpp"
#include "mn_error.hpp"


class basic_mutex {
public:
  basic_mutex();
  virtual ~basic_mutex();

  virtual int create();
  virtual int destroy();

	virtual int lock(unsigned int timeout = (unsigned int) 0xffffffffUL);
	virtual int unlock();

  virtual bool try_lock();

  bool is_initialized() const 						{ return m_bisinitialized; }

  void* get_handle()                      { return m_pmutex; }
private:
	void* m_pmutex;
	bool m_bisinitialized;
};

using mutex_t = basic_mutex;



#endif
