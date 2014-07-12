#include "BasicJargShader.h"


BasicJargShader::BasicJargShader(void)
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
