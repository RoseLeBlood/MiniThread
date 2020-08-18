#ifndef _MINLIB_VERSION_H_
#define _MINLIB_VERSION_H_

#include <string>

class libmnVersion {
public:
  libmnVersion();
	~libmnVersion();


	int get_major_version() const								{ return m_major; }
	int get_minor_version() const								{ return m_minor; }
	int get_debug_version() const								{ return m_debug; }
  	std::string get_license() const							{ return m_license; }

	std::string to_string() const;
private:
	std::string m_license;
	int m_major, m_minor, m_debug;
	std::string m_extras;
};

extern libmnVersion libmn_version;

#endif
