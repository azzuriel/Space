#include "Model.h"
#include <easylogging++.h>
#include <string>
#include <iostream>
#include "sparse_vector.h"

_INITIALIZE_EASYLOGGINGPP 

int main(int argc, char *argn[]) {

	LOG(INFO) << "//////////////////////////////////////////////////////////////////////////";
    sparse_vector<int> a;
    a.push_back(1);a.push_back(2);a.push_back(3);a.push_back(4);a.push_back(5);
    for (int i = 0; i < a.size(); i++)
    {
        LOG(INFO) << a[i];
    }
    LOG(INFO) << "//////////////////////////////////////////////////////////////////////////";
    a.erase(a.begin()+2);
    a.pop_back();
    for (int i = 0; i < a.size(); i++)
    {
        LOG(INFO) << a[i];
    }
    LOG(INFO) << "//////////////////////////////////////////////////////////////////////////";
    a.push_back(789);
    a.push_back(987);
    for (int i = 0; i < a.size(); i++)
    {
        LOG(INFO) << a[i];
    }
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