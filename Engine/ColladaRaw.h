#pragma once
#include <string>
#include <../libs/rapidxml-1.13/rapidxml.hpp>

class ColladaRaw
{
public:
	ColladaRaw(void);
	ColladaRaw(std::string name);
	~ColladaRaw(void);
	rapidxml::xml_document<>* data;
	char* xml;
	int size;
};


