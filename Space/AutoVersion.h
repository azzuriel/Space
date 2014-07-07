#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "0.1-1-gb830d92";
#define Time "1404427681";
class AutoVersion { 
public: 
    static std::string GetTitle();
private:
    static std::string title_;
    static bool ready_;
};
#endif // AutoVersion_h__   
