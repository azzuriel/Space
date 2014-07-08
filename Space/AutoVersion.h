#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "0.1-3-g4c5516b";
#define Time "1404755319";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__   
