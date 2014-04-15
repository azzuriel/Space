#include "AutoVersion.h"
#include <boost\date_time\posix_time\conversion.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string AutoVersion::GetTitle()
{
	if(ready_){
		return title_;
	}
	char buff[100];
	std::string s1 = Time;
	long t = atol(s1.c_str());
	auto time = boost::posix_time::from_time_t(t);
	std::string s2 = Ver;
	std::string times = to_simple_string(time);
	sprintf(buff, "%s %s %s %s", "Sandbox", s2.c_str(), "from", times.c_str());
	title_ = buff;
	ready_ = true;
	return title_;
}

bool AutoVersion::ready_ = false;
std::string AutoVersion::title_;
