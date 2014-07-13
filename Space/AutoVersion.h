#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "0.1-13-ge19bb1e";
#define Time "1405175717";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__   
