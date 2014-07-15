#include "BasicJargShader.h"


BasicJargShader::BasicJargShader(void) :
    ambient_location(GL_INVALID_VALUE),
    diffuse_location(GL_INVALID_VALUE),
    specular_location(GL_INVALID_VALUE),
    emission_location(GL_INVALID_VALUE),
    texture_location(GL_INVALID_VALUE),
    shininess_location(GL_INVALID_VALUE),
    normal_location(GL_INVALID_VALUE)
{
    
}


BasicJargShader::~BasicJargShader(void)
{
}

void BasicJargShader::UpdateUniforms()
{
    ambient_location = glGetUniformLocation(program, "material.ambient");  
    diffuse_location = glGetUniformLocation(program, "material.diffuse"); 
    specular_location = glGetUniformLocation(program, "material.specular"); 
    emission_location = glGetUniformLocation(program, "material.emission");
    shininess_location = glGetUniformLocation(program, "material.shininess");

    texture_location = glGetUniformLocation(program, "material.texture");
    normal_location = glGetUniformLocation(program, "material.normal");
}
