#pragma once
#include <string>
#include <rapidxml.hpp>

class ColladaRaw
{
public:
	ColladaRaw(void);
	ColladaRaw(std::string name);
	~ColladaRaw(void);
	rapidxml::xml_document<>* data;
	char* xml;
};


