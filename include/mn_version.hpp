#ifndef _MINLIB_VERSION_H_
#define _MINLIB_VERSION_H_

#include <string>

/**
 * Version wrapper for this library
 */ 
class libmnVersion {
private:
  	libmnVersion();
public:
	~libmnVersion();

	/**
	 * Get the major version
	 * @return The major version
	 */ 
	int get_major_version() const								{ return m_major; }
	/**
	 * Get the minor version
	 * @return The minor version
	 */ 
	int get_minor_version() const								{ return m_minor; }
	/**
	 * Get the debug version
	 * @return The debug version
	 */ 
	int get_debug_version() const								{ return m_debug; }
	/**
	 * Get the license string
	 * @return The license string
	 */ 
  	std::string get_license() const								{ return m_license; }

	/**
	 * Get the version string with extra informations
	 * @return The version string with extra informations
	 */ 
	std::string to_string() const;

	/**
	 * Get the singleton instances from this class 
	 * @return The singleton instance
	 */
	static libmnVersion& instance() {
		if(m_pInstance == NULL) m_pInstance = new libmnVersion();
		return *m_pInstance;
	}
private:
	/**
	 * The Sigleton Object holder
	 */ 
	static libmnVersion* m_pInstance;
private:
	/**
	 * The license string holder
	 */
	std::string m_license;
	/**
	 * The Version holders
	 */
	int m_major, m_minor, m_debug;
	/**
	 * The extra versions infomations string holder
	 */
	std::string m_extras;
};

using mnversion_t = libmnVersion;

#endif
