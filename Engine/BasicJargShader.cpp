#include "BasicJargShader.h"


BasicJargShader::BasicJargShader(void) :
    ambient_location(-1),
    diffuse_location(-1),
    specular_location(-1),
    emission_location(-1),
    texture_location(-1),
    shininess_location(-1),
    normal_location(-1)
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
