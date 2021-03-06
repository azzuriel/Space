#pragma once
#ifndef Model_h__
#define Model_h__
#define COLLADA_MODEL 1

#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>
#include "Frustum.h"
class Model
{
public:
    Model(void);
    Model(std::string name, int model_type = COLLADA_MODEL);
    ~Model(void);
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;
    mat4 World;
    void Bind();
    void Render(const Frustum &frust) const;
    void Render() const;
    std::shared_ptr<Mesh> findMeshById(const char* str);
    std::shared_ptr<Material> findMaterialById(char* str);
    void SaveBinary(std::string name);
    void LoadBinary(std::string name);
    void BuildBounding();
    void RenderBounding(Batched &sb);
    std::shared_ptr<Material> ErrorMaterial;
};
#endif // Model_h__

