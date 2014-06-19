#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "";
#define Time "1403167570";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__   
