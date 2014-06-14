#include "JargShader.h"
#include <glew.h>
#include <algorithm>
#include <vector>
#include <ostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>

#define printLog(obj){int infologLength = 0; \
	char infoLog[1024]; \
	if (glIsShader(obj)) \
	glGetShaderInfoLog(obj, 1024, &infologLength, infoLog); \
else \
	glGetProgramInfoLog(obj, 1024, &infologLength, infoLog); \
	if (infologLength > 0) { \
	LOG(INFO) << infoLog; \
	} else { \
	LOG(INFO) << "     no errors"; \
	} }

JargShader::JargShader()
{
	 program = glCreateProgram();
}

JargShader::~JargShader(void)
{
	while(!shaders_.empty()) {
		glDeleteShader(shaders_.back());
		LOG(INFO) << "Deleting shader " << std::to_string(shaders_.back());
		shaders_.pop_back();
	}
	glDeleteProgram(program);
	LOG(INFO) << "Deleting program " << std::to_string(program);
}

void JargShader::BindProgram() const
{
	glUseProgram(program);
}

GLint JargShader::LocateVars(std::string s)
{
	GLint a = glGetUniformLocation(program, s.c_str());
	vars.push_back(a);
	return a;
}

void JargShader::loadShaderFromSource(GLenum type, std::string source) {

	std::stringstream ss;
	std::string name;
	ss << "#version 330 core" << std::endl;
	if(type == GL_FRAGMENT_SHADER) {
		name = "#define _FRAGMENT_";
		ss << name << std::endl;
	}
	else if(type == GL_VERTEX_SHADER) {
		name = "#define _VERTEX_";
		ss << name << std::endl;
	}
	else if(type == GL_GEOMETRY_SHADER) {
		name = "#define _GEOMETRY_";
		ss << name << std::endl;
	}
	else if(type == GL_TESS_EVALUATION_SHADER) {
		name = "#define _TESSEVAL_";
		ss << name << std::endl;
	}
	else if(type == GL_TESS_CONTROL_SHADER) {
		name = "#define _TESSCONTROL_";
		ss << name << std::endl;
	}
	std::ifstream file(source.c_str());
	std::string line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			ss << line << std::endl;
		}
		file.close();
	} else {
		LOG(ERROR) << "Failed to open file " << source << std::endl;
		return;
	}
	std::string str = ss.str();
	int length = str.length();
	const char *data = str.c_str();
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, (const char **)&data, &length);
	glCompileShader(id);
    LOG(INFO) << source << " file " << name << "PART";
	printLog(id);
	glAttachShader(program, id);
	shaders_.push_back(id);
}

bool JargShader::Link() {
	glLinkProgram(program);
	LOG(INFO) << "Program " << std::to_string(program) << " linking";
	printLog(program);
	LOG(INFO) << "--------------------";
	return true;
}


void JargShader::LoadFromFile(std::string fragment_file_path, std::string vertex_file_path)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	LOG(INFO) << "Compiling shader: \"" << vertex_file_path << "\".";
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
	if(VertexShaderErrorMessage.size() == 1) 
	{
		LOG(INFO) << "   successful";
	} else {
		LOG(INFO) << &VertexShaderErrorMessage[0];
	}

	// Compile Fragment Shader
	LOG(INFO) << "Compiling shader: \"" << fragment_file_path << "\".";
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
	if(FragmentShaderErrorMessage.size() == 1) 
	{
		LOG(INFO) << "   successful";
	} else {
		LOG(INFO) << &FragmentShaderErrorMessage[0];
	}

	// Link the program
	LOG(INFO) << "Linking program";
	program = glCreateProgram();
	glAttachShader(program, VertexShaderID);
	glAttachShader(program, FragmentShaderID);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( InfoLogLength > 1 ? InfoLogLength : 1 );
	glGetProgramInfoLog(program, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
	if(ProgramErrorMessage.size() == 1) 
	{
		LOG(INFO) << "   successful";
	} else {
		LOG(INFO) << &ProgramErrorMessage[0];
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

