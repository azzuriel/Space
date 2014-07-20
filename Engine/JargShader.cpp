#include "JargShader.h"
#include <glew.h>
#include "JHelpers_inl.h"
#include <string>
#include <fstream>
#include <sstream>


#define printLog(obj){int infologLength = 0; \
    char infoLog[1024]; \
    if (glIsShader(obj)) \
    glGetShaderInfoLog(obj, 1024, &infologLength, infoLog); \
else \
    glGetProgramInfoLog(obj, 1024, &infologLength, infoLog); \
    if (infologLength > 0) { \
    LOG(info) << infoLog; \
    } else { \
    LOG(info) << "     no errors"; \
    } }

JargShader::JargShader() :
    has_header(false)
{
    program = glCreateProgram();
}

JargShader::~JargShader(void)
{
    while(!shaders_.empty()) {
        glDeleteShader(shaders_.back());
        LOG(info) << "Deleting shader " << std::to_string(shaders_.back());
        shaders_.pop_back();
    }
    glDeleteProgram(program);
    LOG(info) << string_format("Deleting program %i", program);
}

void JargShader::Use() const
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
    name = source;
    std::string part_name;
    if(!has_header){
        ss << "#version 330 core" << std::endl;
    }
    if(type == GL_FRAGMENT_SHADER) {
        part_name = "#define _FRAGMENT_";
        ss << part_name << std::endl;
    }
    else if(type == GL_VERTEX_SHADER) {
        part_name = "#define _VERTEX_";
        ss << part_name << std::endl;
    }
    else if(type == GL_GEOMETRY_SHADER) {
        part_name = "#define _GEOMETRY_";
        ss << part_name << std::endl;
    }
    else if(type == GL_TESS_EVALUATION_SHADER) {
        part_name = "#define _TESSEVAL_";
        ss << part_name << std::endl;
    }
    else if(type == GL_TESS_CONTROL_SHADER) {
        part_name = "#define _TESSCONTROL_";
        ss << part_name << std::endl;
    }
    if(has_header) {
        ss << global_header;
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
        LOG(error) << string_format("%s %s", "Failed to open file ", source.c_str());
        return;
    }
    std::string str = ss.str();
    int length = str.length();
    const char *data = str.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);
    LOG(info) << source << " file " << part_name << "PART";
    printLog(id);
    glAttachShader(program, id);
    shaders_.push_back(id);
}

bool JargShader::Link() {
    glLinkProgram(program);
    LOG(info) << "Program " << std::to_string(program) << " linking";
    printLog(program);
    LOG(info) << "--------------------";
    return true;
}

void JargShader::PushGlobalHeader(std::string source)
{
    std::stringstream ss;
    name = source;
    std::string part_name;
    if(!has_header) {
        ss << "#version 330 core" << std::endl;
    } else {
        ss << global_header;
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
        LOG(error) << string_format("%s %s", "Failed to open file ", source.c_str());
        return;
    }
    global_header = ss.str();
    has_header = true;
}
