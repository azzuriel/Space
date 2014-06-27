#include "ColladaRaw.h"
#include <rapidxml.hpp>
#include <sstream>
#include <fstream>
#include <glog\logging.h>

ColladaRaw::ColladaRaw(void)
{
}


ColladaRaw::~ColladaRaw(void)
{
	delete[] xml;
	delete data;
}

ColladaRaw::ColladaRaw(std::string FileName)
{
	std::stringstream ss;
	std::ifstream file(FileName.c_str());
	std::string line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			ss << line << std::endl;
		}
		file.close();
	} else {
		LOG(ERROR) << "Failed to open file " << FileName;
		return;
	}
	auto str = ss.str();
	xml = new char[str.size() + 1];
	std::copy(str.begin(), str.end(), xml);
	xml[str.size()] = '\0';
	

	data = new rapidxml::xml_document<>();
	data->parse<0>(xml);
}
