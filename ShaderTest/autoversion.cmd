
@echo off
for /F "tokens=*" %%i in ('git describe --tags HEAD') do echo %%i && set VERSION=%%i
for /F "tokens=*" %%i in ('git show -s --format"=""%%ct"') do echo %%i && set VERSION2=%%i
>AutoVersion.h echo #ifndef AutoVersion_h__ 
>>AutoVersion.h echo #define AutoVersion_h__ 
>>AutoVersion.h echo #include ^<string^>
>>AutoVersion.h echo #define Ver "%VERSION%";
>>AutoVersion.h echo #define Time %VERSION2%";
>>AutoVersion.h echo class AutoVersion { 
>>AutoVersion.h echo public: 
>>AutoVersion.h echo 	static std::string GetTitle();
>>AutoVersion.h echo private:
>>AutoVersion.h echo 	static std::string title_;
>>AutoVersion.h echo	static bool ready_;
>>AutoVersion.h echo };
>>AutoVersion.h echo #endif // AutoVersion_h__   