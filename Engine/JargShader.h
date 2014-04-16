#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <glog/logging.h>
#include <glew.h>
#pragma deprecated(LoadFromFile)

class JargShader{
public:
	JargShader();
	~JargShader(void);
	std::vector<int> vars;
	void LoadFromFile(std::string fragment, std::string vertex);
	void BindProgram();
	int LocateVars(std::string s);
	void loadShaderFromSource(GLenum type, std::string source);
	bool link();
	int program;
	std::vector<GLuint> shaders_;
};
#endif // JargShader_h__
