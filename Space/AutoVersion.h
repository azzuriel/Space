#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "0.1-17-gccd9db8";
#define Time "1405472702";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__   
