#include "Model.h"
#include <easylogging++.h>
#include <string>
#include <iostream>

_INITIALIZE_EASYLOGGINGPP 

int main(int argc, char *argn[]) {

	LOG(INFO) << "//////////////////////////////////////////////////////////////////////////";

	if(argc > 1){
		for (int i=1; i< argc; i++)
		{
			auto name = std::string(argn[i]);
			Model* m = new Model(name);
			std::string subname = name.substr(0, name.rfind("."));
			m->SaveBinary(subname + ".m");

			delete m;
		}
	} else {
		LOG(ERROR) << "Write names in arguments";
	}

	return 0;
}