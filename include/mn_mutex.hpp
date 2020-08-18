#ifndef MINLIB_ESP32_MUTEX_
#define MINLIB_ESP32_MUTEX_

#include "mn_error.hpp"

class basic_mutex {
public:
  basic_mutex();
  ~basic_mutex();

  int create();
	int lock();
	int unlock();

  bool try_lock();

  bool is_initialized() const 						{ return m_bisinitialized; }
private:
	void* m_pmutex;
	bool m_bisinitialized;
};

using mutex_t = basic_mutex;



#endif
