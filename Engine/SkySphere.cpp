#include "SkySphere.h"
#include <glm.hpp>

SkySphere::SkySphere(void)
{
	m = new Mesh();
}

inline vec3 SphToDec(float phi, float theta, float r){
	return vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
}

int SkySphere::Generate(void *last, int argc, char **argv, char **azColName){
	float phi, theta, r;
	sscanf(argv[7], "%f", &phi);
	sscanf(argv[8], "%f", &theta);
	sscanf(argv[9], "%f", &r);
	r*=10;
	
	int i = *reinterpret_cast<int*>(last);
	m->Verteces[i+0] = VertexPositionNormalTexture(vec3(), SphToDec(phi, theta, r), vec2(0,0));
	m->Verteces[i+1] = VertexPositionNormalTexture(vec3(), SphToDec(phi+0.01f, theta, r), vec2(1,0));
	m->Verteces[i+2] = VertexPositionNormalTexture(vec3(), SphToDec(phi, theta+0.01f, r), vec2(0,1));
	m->Indeces[i+0] = i+0;
	m->Indeces[i+1] = i+1;
	m->Indeces[i+2] = i+2;
	return 0;
}

SkySphere::~SkySphere(void)
{
	if(m){
		delete m;
	}
}
