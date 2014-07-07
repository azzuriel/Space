#include <Material.h>

Material::Material(void) :
    emission(0),
    ambient(0),
    diffuse(0),
    shininess(0),
    specular(0),
    index_of_refraction(0)
{
    texture = nullptr;
    normal = nullptr;
}


PointLight::PointLight(void) :
    ambient(0),
    diffuse(0),
    specular(0),
    position(0),
    attenuation(0)
{

}
