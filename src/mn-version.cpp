#include "mn-config.h"
#include "mn-version.h"

#include <stdio.h>

libmnVersion libmn_version;

libmnVersion::libmnVersion()
{
	m_major = MN_THREAD_MAJOR_VERSION;
	m_minor = MN_THREAD_MINOR_VERSION;
	m_debug = MN_THREAD_DEBUG_VERSION;
	m_license = std::string("LGPL");
}

libmnVersion::~libmnVersion() { }

std::string libmnVersion::to_string() const {
	char str[16];
	snprintf(str, 16, "%d.%d.%d", m_major, m_minor, m_debug);
	return std::string(str);
}
