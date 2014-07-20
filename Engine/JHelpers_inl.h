#pragma once
#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <memory>
#include <string>
#include <stdarg.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <math.h>

#include <boost/log/trivial.hpp>


#define LOG(level) BOOST_LOG_TRIVIAL(level)

#define G 6.671919999999999999999e-11
#define Pc 3.0856776e+16
#define Ae 149597870.7
#define Ss 299792458
#define Sy 9.46073047+15

inline double getExcentricity(double a, double b){
    return sqrt(1 - ((b*b)/(a*a)));
}

inline double getFocalLength(double a, double b){
    return sqrt(1 - ((b*b)/(a*a)))*a;
}

inline double getSpaceSpeed1st(double mass, double radius)	{
    return sqrt(G*(mass/radius));
}

inline double getSpaceSpeed2st(double mass, double radius)	{
    return sqrt(2*G*(mass/radius));
}

inline glm::vec3 SphToDec(float phi, float theta, float r){
    return glm::vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
}

inline double RotTime(double mass, double majorAxis){
    return pow(2*M_PI*majorAxis, 3.0/2.0)*(G*mass)-1/2;
}

inline bool inLimsV(glm::vec2 param1, glm::vec2 wpos, glm::vec2 size)
{
    return param1.x >= wpos.x && param1.y >= wpos.y && param1.x <= size.x && param1.y <= size.y;
}

template<class _Ty>
inline bool inLims(_Ty param1, _Ty wpos, _Ty size)
{
    return param1 >= wpos && param1 <= size;
}

template<class _Ty>
inline _Ty kmToParsec(_Ty a){
    return (_Ty)(a / (3.0856776e+13)); 
}

template<class _Ty>
inline _Ty mToParsec(_Ty a){
    return (_Ty)(a / (3.0856776e+16));
}

template<class _Ty>
inline _Ty kmToAE(_Ty a){
    return (_Ty)(a / 149597870.7);
}

template<class _Ty>
inline _Ty mToAE(_Ty a){
    return (_Ty)(a / 149597870700.0);
}

template<class _Ty>
inline _Ty atCenter(_Ty size, _Ty zoneBegin, _Ty zoneSize){
    return zoneBegin + zoneSize/2.0f - (size)/2.0f;
}

inline char keyToChar(int key, bool shift){
    int r = 0;
    if(inLims(key, 48, 57)){
        r = '0'+key-48;
    }
    if(inLims(key, 65, 90)){
        r = 'a'+key-65;
    }

    if(shift)
        return toupper(r);
    return r;
}

inline std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; 
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); 
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

inline std::string to_string_helper(double speed, bool is_speed)
{
    if(speed > Pc/100) {
        return string_format(is_speed ? "%f pc/s (FTL)" : "%f pc", speed/Pc);
    }
    if(speed > Ae/100) {
        return string_format(is_speed ?  speed > Ss ? "%f ae/s (FTL)" : "%f ae/s" : "%f ae", speed/Ae);
    }
    if(speed > 1000) {
        return string_format(is_speed ? "%f Km/s" : "%f Km", speed/1000.0);
    }
    return string_format(is_speed ? "%f m/s" : "%f m", speed);
}


inline std::string MetersSpeedString(double speed){
    return to_string_helper(speed, true);
}

inline std::string to_traf_string(double traf){
    if (traf > 1024*1024*1024) {
        return string_format("%2f GiB", traf/(1024.0*1024.0*1024.0));
    }
    if (traf > 1024 * 1024)
    {
        return string_format("%2f MiB", traf / (1024.0 * 1024.0));
    }
    if (traf > 1024)
    {
        return string_format("%2f KiB", traf / 1024.0);
    }
    return string_format("%i B", (int)traf);
}

namespace std
{
    inline std::string to_string(const glm::vec2& a){
        return string_format("{%g, %g, %g}", a.x, a.y);
    }
    inline std::string to_string(const glm::vec3& a){
        return string_format("{%g, %g, %g}", a.x, a.y, a.z);
    }
    inline std::string to_string(const glm::vec4& a){
        return string_format("{%g, %g, %g, %g}", a.x, a.y, a.z, a.w);
    }

    inline std::string to_string(const glm::quat& a){
        return string_format("{%g, %g, %g, %g}", a.x, a.y, a.z, a.w);
    }

    inline std::string to_string(const btVector3& a){
        return string_format("{%g, %g, %g}", a.getX(), a.getY(), a.getZ());
    }


    inline std::string to_string(const btVector4& a){
        return string_format("{%g, %g, %g, %g}", a.getX(), a.getY(), a.getZ(), a.getW());
    }

    inline std::string to_string(const btQuaternion& a){
        return string_format("{%g, %g, %g, %g}", a.getX(), a.getY(), a.getZ(), a.getW());
    }

    inline std::string to_string(const glm::mat3& a){
        return string_format("{%g, %g, %g}\n{%g, %g, %g}\n{%g, %g, %g}", a[0][0], a[1][0], a[2][0], a[0][1], a[1][1], a[2][1], a[0][2], a[1][2], a[2][2]);
    }
    inline std::string to_string(const glm::mat4& a){
        return string_format("{%g, %g, %g, %g}\n{%g, %g, %g, %g}\n{%g, %g, %g, %g}", a[0][0], a[1][0], a[2][0], a[3][0], a[0][1], a[1][1], a[2][1], a[3][1], a[0][2], a[1][2], a[2][2], a[3][2], a[0][3], a[1][3], a[2][3], a[3][3]);
    }
}