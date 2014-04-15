#include "JargShader.h"
#include <glew.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <math.h>

JargShader::JargShader()
{
	program = 0;
}

JargShader::~JargShader(void)
{
	if(!program) {
		glDeleteProgram(program);
	}
}

void JargShader::BindProgram()
{

	glUseProgram(program);
}

GLint JargShader::LocateVars(std::string s)
{
	GLint a = glGetUniformLocation(program, s.c_str());
	vars.push_back(a);
	return a;
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

