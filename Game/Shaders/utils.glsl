/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#ifndef UTILS_GLSL_
#define UTILS_GLSL_

/*----------------------------------------------------------------------------*/
float log10(in float x) {
	return log2(x) / log2(10);
}

/*----------------------------------------------------------------------------*/
vec2 powv2(in vec2 b, in float e) {
	return vec2(pow(b.x, e), pow(b.y, e));
}
/*----------------------------------------------------------------------------*/
vec3 powv3(in vec3 b, in float e) {
	return vec3(pow(b.x, e), pow(b.y, e), pow(b.z, e));
}
/*----------------------------------------------------------------------------*/
vec4 powv4(in vec4 b, in float e) {
	return vec4(pow(b.x, e), pow(b.y, e), pow(b.z, e), pow(b.w, e));
}

/*----------------------------------------------------------------------------*/
float luminance(in vec3 rgb) {
	const vec3 kLum = vec3(0.2126, 0.7152, 0.0722);
	return max(dot(rgb, kLum), 0.0001); // prevent zero result
}

/*----------------------------------------------------------------------------*/
float linearizeDepth(in float depth, in mat4 projMatrix) {
	return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

#endif // UTILS_GLSL_