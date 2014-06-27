#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <memory>
#include <string>
#include <stdarg.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
 
 inline bool inLimsV(glm::vec2 param1, glm::vec2 wpos, glm::vec2 size)
 {
 	return param1.x >= wpos.x && param1.y >= wpos.y && param1.x <= size.x && param1.y <= size.y;
 }
 
 template<class T>
 inline bool inLims(T param1, T wpos, T size)
 {
 	return param1 >= wpos && param1 <= size;
 }
 
 template<class T>
 inline T atCenter(T size, T zoneBegin, T zoneSize){
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
		 return string_format("{{%g, %g, %g}, {%g, %g, %g}, {%g, %g, %g}}", a[0][0], a[1][0], a[2][0], a[0][1], a[1][1], a[2][1], a[0][2], a[1][2], a[2][2]);
	 }
 }