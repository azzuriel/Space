#include "Model.h"
#include <ColladaRaw.h>
#include <vector>
#include "VertexPositionTexture.h"
#include <JHelpers_inl.h>
#include <io.h>
#include <fstream>
#include <sstream>

Model::Model(void) :
    World(1),
    ErrorMaterial()
{
}

//valid string "1 2 3 4"
void parseVec4(char *str, vec4 &target){
    if(str == nullptr){
        return;
    }
    sscanf(str, "%f %f %f %f", &target.x, &target.y, &target.z, &target.w);
}

//valid string "1 2 3"
void parseVec3(char *str, vec3 &target){
    if(str == nullptr){
        return;
    }
    sscanf(str, "%f %f %f", &target.x, &target.y, &target.z);
}

//valid string "1.0e-1"
void parseFloat(char *str, float &target){
    if(str == nullptr){
        return;
    }
    sscanf(str, "%f", &target);
}

//valid string {"1.0 "}xn
void parseFloatArray(char *str, std::vector<float> &target){
    if(str == nullptr){
        return;
    }
    char *data = str;
    int offset = 0;
    float n = 0;
    while (sscanf(data, "%f %n", &n, &offset) == 1) {
        data += offset;
        target.push_back(n);
    }
}

//valid string {"1 "}xn
void parseGLuintArray(char *str, std::vector<GLuint> &target){
    if(str == nullptr){
        return;
    }
    char *data = str;
    int offset = 0;
    GLuint n = 0;
    while (sscanf(data, "%u %n", &n, &offset) == 1) {
        data += offset;
        target.push_back(n);
    }
}

//valid string {"1 2.0 3.0e-2 "}xn
void parseVec3Array(char *str, std::vector<vec3> &target){
    if(str == nullptr){
        return;
    }
    char *data = str;
    int offset3 = 0;
    float n1, n2, n3 = 0;
    while (sscanf(data, "%f %f %f %n", &n1, &n2, &n3, &offset3) == 3) {
        data += offset3;
        target.push_back(glm::vec3(n1, n2, n3));
    }
}

//valid string {"1 2.0 "}xn
void parseVec2Array(char *str, std::vector<vec2> &target){
    if(str == nullptr){
        return;
    }
    char *data = str;
    int offset3 = 0;
    float n1, n2 = 0;
    while (sscanf(data, "%f %f %n", &n1, &n2, &offset3) == 2) {
        data += offset3;
        target.push_back(glm::vec2(n1, n2));
    }
}

//valid string "1"
void parseGLuint(char *str, GLuint &target){
    if(str == nullptr){
        return;
    }
    sscanf(str, "%u", &target);
}

//valid string "1.0 2.0e-2 3 4 1 2 3 4 1 2 3 4 1 2 3 4"
void parseMat4(char *str, mat4 &target){
    if(str == nullptr){
        return;
    }
    sscanf(str, "%f %f %f %f %f %f %f %f %f %f %f %f", &target[0][0], &target[1][0], &target[2][0], &target[3][0], &target[0][1], &target[1][1], &target[2][1], &target[3][1], &target[0][2], &target[1][2], &target[2][2], &target[3][2], &target[0][3], &target[1][3], &target[2][3], &target[3][3]);
}

std::shared_ptr<Mesh> Model::findMeshById(const char* str){
    for (int i=0;i<meshes.size();i++)
    {
        if(strcmp(meshes[i]->id.c_str(), str) == 0){
            return meshes[i];
        }
    }
    return nullptr;
}

void Model::SaveBinary(std::string name){
    std::ofstream file(name.c_str(), std::ios::out | std::ios::binary);
    if (file.is_open()) {

        //40 head
        char* head = "jmod ver 1                              ";
        file.write(head, 40);

        unsigned int efnum = (unsigned int)materials.size();
        file.write((char*)&efnum, sizeof(unsigned int));

        for (int i=0;i<efnum;i++)
        {
            file.write((char*)&materials[i]->emission, sizeof(glm::vec4));
            file.write((char*)&materials[i]->ambient, sizeof(glm::vec4));
            file.write((char*)&materials[i]->diffuse, sizeof(glm::vec4));
            file.write((char*)&materials[i]->specular, sizeof(glm::vec4));
            file.write((char*)&materials[i]->shininess, sizeof(float));
            file.write((char*)&materials[i]->index_of_refraction, sizeof(float));

            unsigned int slen = materials[i]->id.length() + 1;
            file.write((char*)&slen, sizeof(unsigned int));
            file.write(materials[i]->id.c_str(), slen);
        }

        unsigned int menum = (unsigned int)meshes.size();
        file.write((char*)&menum, sizeof(unsigned int));
        for (int i=0;i<menum;i++)
        {
            unsigned int vcount = (unsigned int) meshes[i]->Verteces.size();
            file.write((char*)&vcount, sizeof(unsigned int));
            for (int j = 0; j < vcount; j++)
            {
                file.write((char*)&(meshes[i]->Verteces[j]), sizeof(VertexPositionNormalTexture));
            }

            unsigned int icount = (unsigned int) meshes[i]->Indeces.size();
            file.write((char*)&icount, sizeof(unsigned int));
            for (int j = 0; j < icount; j++)
            {
                file.write((char*)&(meshes[i]->Indeces[j]), sizeof(GLuint));
            }

            unsigned int slen;
            if(meshes[i]->material != nullptr && meshes[i]->material != ErrorMaterial) {
                slen = meshes[i]->material->id.length() + 1;
                file.write((char*)&slen, sizeof(unsigned int));
                file.write((char*)meshes[i]->material->id.c_str(), slen);
            } else {
                slen = 13;
                file.write((char*)&slen, sizeof(unsigned int));
                file.write((char*)("errormaterial\0"), slen);
            }

            file.write((char*)&meshes[i]->World, sizeof(mat4));

            slen = meshes[i]->id.length() + 1;
            file.write((char*)&slen, sizeof(unsigned int));
            file.write((char*)meshes[i]->id.c_str(), slen);
        }

        file.close();
    } else {
        //LOG(ERROR) << "Failed to open file " << name;
        return;
    }
}

void Model::LoadBinary(std::string name){
    materials.clear();
    meshes.clear();

    //LOG(INFO) << name << " loading begin";

    std::ifstream file(name.c_str(), std::ios::in | std::ios::binary);
    auto begin = file.tellg();
    if (file.is_open()) {

        //40 head
        char head[40];
        file.read(head, 40);

        unsigned int efnum;
        file.read((char*)&efnum, sizeof(unsigned int));

        for (int i=0;i<efnum;i++)
        {
            materials.push_back(std::shared_ptr<Material>(new Material()));
            file.read((char*)&materials[i]->emission, sizeof(glm::vec4));
            file.read((char*)&materials[i]->ambient, sizeof(glm::vec4));
            file.read((char*)&materials[i]->diffuse, sizeof(glm::vec4));
            file.read((char*)&materials[i]->specular, sizeof(glm::vec4));
            file.read((char*)&materials[i]->shininess, sizeof(float));
            file.read((char*)&materials[i]->index_of_refraction, sizeof(float));

            unsigned int slen;
            file.read((char*)&slen, sizeof(unsigned int));
            char* sbuf = new char[slen];
            file.read((char*)sbuf, slen);
            materials[i]->id = std::string(sbuf);
            delete[] sbuf;
        }

        unsigned int menum;
        file.read((char*)&menum, sizeof(unsigned int));
        for (int i=0;i<menum;i++)
        {
            meshes.push_back(std::shared_ptr<Mesh>(new Mesh()));
            unsigned int vcount;
            file.read((char*)&vcount, sizeof(unsigned int));
            meshes[i]->Verteces.resize(vcount);
            for (int j = 0; j < vcount; j++)
            {
                file.read((char*)&(meshes[i]->Verteces[j]), sizeof(VertexPositionNormalTexture));
            }

            unsigned int icount;
            file.read((char*)&icount, sizeof(unsigned int));
            meshes[i]->Indeces.resize(icount);
            for (int j = 0; j < icount; j++)
            {
                file.read((char*)&(meshes[i]->Indeces[j]), sizeof(GLuint));
            }

            unsigned int slen;
            file.read((char*)&slen, sizeof(unsigned int));
            char* sbuf = new char[slen];
            file.read((char*)sbuf, slen);
            meshes[i]->material = std::shared_ptr<Material>(findMaterialById(sbuf));
            if(meshes[i]->material == nullptr){
                meshes[i]->material = ErrorMaterial;
            }
            delete[] sbuf;



            file.read((char*)&meshes[i]->World, sizeof(mat4));

            file.read((char*)&slen, sizeof(unsigned int));
            sbuf = new char[slen];
            file.read((char*)sbuf, slen);
            meshes[i]->id = std::string(sbuf);
            delete[] sbuf;
        }

        file.close();
    } else {
        //LOG(ERROR) << "Failed to open file " << name;
        return;
    }

    //LOG(INFO) << string_format("     %i meshes, %i materials (%s)", meshes.size(), materials.size(), to_traf_string(file.tellg() - begin).c_str());
    //LOG(INFO) << name << " loading end";
}

std::shared_ptr<Material> Model::findMaterialById(char* str){
    for (int i=0;i<materials.size();i++)
    {
        if(strcmp(materials[i]->id.c_str(), str) == 0){
            return materials[i];
        }
    }
    return nullptr;
}

Model::Model(std::string name, int model_type /*= COLLADA_MODEL*/) :
    World(1),
    ErrorMaterial()
{
    auto c = new ColladaRaw(name);
    //LOG(INFO) << name << " parsing begin";

    //////////////////////////////////////////////////////////////////////////
    // Effects part
    //////////////////////////////////////////////////////////////////////////
    auto COLLADA = c->data->first_node("COLLADA");
    auto effects = COLLADA->first_node("library_effects");
    auto effect = effects->first_node("effect");
    for (auto alleffects = effect; alleffects; alleffects = alleffects->next_sibling())
    {
        auto m = std::shared_ptr<Material>(new Material());
        m->id = alleffects->first_attribute("id")->value();

        auto technique = alleffects->first_node("profile_COMMON")->first_node("technique");
        rapidxml::xml_node<>* extr = technique->first_node("lambert");
        if(extr == nullptr){
            extr = technique->first_node("phong");
        }
        if(extr == nullptr){
            extr = technique->first_node("fresnel");
        }

        if(extr->first_node("emission") != nullptr)
            parseVec4(extr->first_node("emission")->first_node("color")->value(), m->emission);
        if(extr->first_node("ambient") != nullptr)
            parseVec4(extr->first_node("ambient")->first_node("color")->value(), m->ambient);
        if(extr->first_node("diffuse") != nullptr) {
            auto temp = extr->first_node("diffuse")->first_node("color");
            if(temp != nullptr) {
                parseVec4(temp->value(), m->diffuse);
            } else {
                //текстура
            }
        }
        if(extr->first_node("specular") != nullptr)
            parseVec4(extr->first_node("specular")->first_node("color")->value(), m->specular);
        if(extr->first_node("shininess") != nullptr)
            parseFloat(extr->first_node("shininess")->first_node("float")->value(), m->shininess);
        if(extr->first_node("index_of_refraction") != nullptr)
            parseFloat(extr->first_node("index_of_refraction")->first_node("float")->value(), m->index_of_refraction);

        materials.push_back(m);
        int i = 1;
    }

    //////////////////////////////////////////////////////////////////////////
    // Materials part
    //////////////////////////////////////////////////////////////////////////
    auto library_materials = COLLADA->first_node("library_materials");
    auto material = library_materials->first_node("material");
    for (auto allmaterials = material; allmaterials; allmaterials = allmaterials->next_sibling())
    {
        auto mat_id = allmaterials->first_attribute("id")->value();
        //instance_effect url starts with # therefore ++ pointer
        auto insteff =  allmaterials->first_node("instance_effect")->first_attribute("url")->value();
        insteff++;

        auto finded = findMaterialById(insteff);
        if(finded){
            finded->id = mat_id;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Geometries part
    //////////////////////////////////////////////////////////////////////////
    auto geometries = COLLADA->first_node("library_geometries");
    auto geometry = geometries->first_node("geometry");
    for (auto allgeomerty = geometry; allgeomerty; allgeomerty = allgeomerty->next_sibling())
    {
        char* geomid = allgeomerty->first_attribute("id")->value();

        auto extr = allgeomerty->first_node("mesh");
        std::vector<vec3> positions;
        parseVec3Array(extr->first_node("source")->first_node("float_array")->value(), positions);
        std::vector<vec3> normals;
        if(extr->first_node("source")->next_sibling() != nullptr) {
            parseVec3Array(extr->first_node("source")->next_sibling()->first_node("float_array")->value(), normals);
        }
        std::vector<vec2> uvs;
        if(extr->first_node("source")->next_sibling()->next_sibling() != nullptr) {
            auto temp = extr->first_node("source")->next_sibling()->next_sibling();
            if(strcmp(temp->name(), "source") == 0) {
                parseVec2Array(temp->first_node("float_array")->value(), uvs);
            }
        }

        auto polylist = extr->first_node("polylist");
        for (auto allpolylists = polylist; allpolylists; allpolylists = allpolylists->next_sibling())
        {
            if(strcmp(allpolylists->name(), "polylist") == 0) {
                std::vector<GLuint> indexes;
                auto mesh = std::shared_ptr<Mesh>(new Mesh());
                auto datacounter = allpolylists->first_node("input");
                int datacount = 0;
                for (auto allcounter = datacounter; allcounter; allcounter = allcounter->next_sibling())
                {
                    if(strcmp(allcounter->name(), "input") == 0) {
                        datacount++;
                    }
                }
                auto polymat = allpolylists->first_attribute("material")->value();
                parseGLuintArray(allpolylists->first_node("p")->value(), indexes);
                mesh->Verteces.resize(indexes.size()/datacount);
                mesh->Indeces.resize(indexes.size()/datacount);
                for (int i=0; i<indexes.size()-datacount+1; i+=datacount)
                {
                    switch(datacount){
                    case 1:
                        mesh->Verteces[i/datacount] = VertexPositionNormalTexture(glm::vec3(1), positions[indexes[i]], glm::vec2(1));
                        break;
                    case 2:
                        mesh->Verteces[i/datacount] = VertexPositionNormalTexture(normals[indexes[i+1]], positions[indexes[i]], glm::vec2(1));
                        break;
                    case 3:
                        mesh->Verteces[i/datacount] = VertexPositionNormalTexture(normals[indexes[i+1]], positions[indexes[i]], uvs[indexes[i+2]]);//);
                        break;
                    }
                    mesh->Indeces[i/datacount] = i/datacount;
                }
                mesh->id = geomid;
                mesh->material = std::shared_ptr<Material>(findMaterialById(polymat));
                meshes.push_back(mesh);
            }
        }
    }
    //LOG(INFO) << string_format("     %i meshes, %i materials (%s)", meshes.size(), materials.size(), to_traf_string(c->size).c_str());

    //////////////////////////////////////////////////////////////////////////
    // Scenes part
    //////////////////////////////////////////////////////////////////////////
    auto scenes = COLLADA->first_node("library_visual_scenes");
    auto scene = scenes->first_node("visual_scene");
    for (auto allscenes = scene; allscenes; allscenes = allscenes->next_sibling())
    {
        auto node = allscenes->first_node("node");
        for (auto allnodes = node; allnodes; allnodes = allnodes->next_sibling())
        {
            mat4 t;
            parseMat4(allnodes->first_node("matrix")->value(), t);

            //geometry_url starts with # therefore ++ pointer
            auto geometry_node = allnodes->first_node("instance_geometry");
            if(geometry_node != nullptr) {
                auto geometry_url = geometry_node->first_attribute("url")->value();
                geometry_url++;

                auto url_ref = findMeshById(geometry_url);
                if(url_ref != nullptr){
                    url_ref->World = t;
                }
            }
        }
    }

    delete c;

    //LOG(INFO) << name << " parsing end";
}




Model::~Model(void)
{
    meshes.clear();
    materials.clear();
}

void Model::Bind()
{
    for (int i=0;i<meshes.size();i++)
    {
        meshes[i]->Bind();
    }
}

void Model::Render() const
{
    for (int i=0;i<meshes.size();i++)
    {
        meshes[i]->Render(World);
    }
}
