#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <glog/logging.h>


class JargShader{
private:
	int program;
public:
	JargShader();
	~JargShader(void);
	std::vector<int> vars;
	void LoadFromFile(std::string fragment, std::string vertex);
	void BindProgram();
	int LocateVars(std::string s);
};
#endif // JargShader_h__
