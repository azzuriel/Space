#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "0.1-16-g53e23fa";
#define Time "1405398920";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__   
