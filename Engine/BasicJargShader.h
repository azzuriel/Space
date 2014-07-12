#pragma once
#include "jargshader.h"
#include <glew.h>
class BasicJargShader :
    public JargShader
{
public:
    BasicJargShader(void);
    ~BasicJargShader(void);
    GLuint ambient_location, diffuse_location, specular_location, emission_location, shininess_location, texture_location, normal_location;
    void UpdateUniforms();
};

