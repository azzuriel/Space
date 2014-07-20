#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <glew.h>

class JargShader{
public:
    JargShader();
    ~JargShader(void);
    std::string name;
    std::vector<int> vars;
    void Use() const;
    GLint LocateVars(std::string s);
    void PushGlobalHeader(std::string s);
    void loadShaderFromSource(GLenum type, std::string source);
    bool Link();
    GLint program;
    bool has_header;
    std::vector<GLint> shaders_;
    std::string global_header;
};
#endif // JargShader_h__
