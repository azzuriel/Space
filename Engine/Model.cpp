#include "Model.h"
#include <ColladaRaw.h>
#include <vector>
#include "VertexPositionTexture.h"
#include <JHelpers_inl.h>
#include <easylogging++.h>

Model::Model(void) :
	World(1)
{
}

void parseVec4(char *str, vec4 &target){
	if(str == nullptr){
		return;
	}
	sscanf(str, "%f %f %f %f", &target.x, &target.y, &target.z, &target.w);
}

void parseVec3(char *str, vec3 &target){
	if(str == nullptr){
		return;
	}
	sscanf(str, "%f %f %f", &target.x, &target.y, &target.z);
}

void parseFloat(char *str, float &target){
	if(str == nullptr){
		return;
	}
	sscanf(str, "%f", &target);
}

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

void parseGLuint(char *str, GLuint &target){
	if(str == nullptr){
		return;
	}
	sscanf(str, "%u", &target);
}

void parseMat4(char *str, mat4 &target){
	if(str == nullptr){
		return;
	}
	sscanf(str, "%f %f %f %f %f %f %f %f %f %f %f %f", &target[0][0], &target[1][0], &target[2][0], &target[3][0], &target[0][1], &target[1][1], &target[2][1], &target[3][1], &target[0][2], &target[1][2], &target[2][2], &target[3][2], &target[0][3], &target[1][3], &target[2][3], &target[3][3]);
}

Mesh* Model::findMeshById(char* str){
	for (int i=0;i<meshes.size();i++)
	{
		if(strcmp(meshes[i]->id.c_str(), str) == 0){
			return meshes[i];
		}
	}
	return nullptr;
}

Material* Model::findMaterialById(char* str){
	for (int i=0;i<materials.size();i++)
	{
		if(strcmp(materials[i]->id.c_str(), str) == 0){
			return materials[i];
		}
	}
	return nullptr;
}

Model::Model(std::string name, int model_type /*= COLLADA_MODEL*/) :
	World(1)
{
	auto c = new ColladaRaw(name);
	LOG(INFO) << name << " parsing begin";

	//////////////////////////////////////////////////////////////////////////
	// Effects part
	//////////////////////////////////////////////////////////////////////////
	auto COLLADA = c->data->first_node("COLLADA");
	auto effects = COLLADA->first_node("library_effects");
	auto effect = effects->first_node("effect");
	for (auto alleffects = effect; alleffects; alleffects = alleffects->next_sibling())
	{
		auto m = new Material();
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
				auto mesh = new Mesh();
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
				for (int i=0; i<indexes.size(); i+=datacount)
				{
					switch(datacount){
					case 1:
						mesh->Verteces[i/datacount] = VertexPositionNormalTexture(glm::vec3(1), positions[indexes[i]], glm::vec2(1));
						break;
					case 2:
						mesh->Verteces[i/datacount] = VertexPositionNormalTexture(normals[indexes[i+1]], positions[indexes[i]], glm::vec2(1));
						break;
					case 3:
						mesh->Verteces[i/datacount] = VertexPositionNormalTexture(normals[indexes[i+1]], positions[indexes[i]], glm::vec2(1));//uvs[indexes[i+1]]);
						break;
					}
					mesh->Indeces[i/datacount] = i/datacount;
				}
				mesh->id = geomid;
				mesh->material = findMaterialById(polymat);
				meshes.push_back(mesh);
			}
		}
	}
	LOG(INFO) << string_format("     %i meshes, %i materials (%s)", meshes.size(), materials.size(), to_traf_string(c->size).c_str());

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
				if(geometry_url != nullptr){
					url_ref->World = t;
				}
			}
		}
	}

	delete c;

	LOG(INFO) << name << " parsing end";
}




Model::~Model(void)
{
	if(materials.size() > 0){
		for(int i=0;i<materials.size();i++){
			delete materials[i];
		}
	}

	if(meshes.size() > 0){
		for(int i=0;i<meshes.size();i++){
			delete meshes[i];
		}
	}
}

void Model::Bind()
{
	for (int i=0;i<meshes.size();i++)
	{
		meshes[i]->Bind();
	}
}

void Model::Render()
{
	for (int i=0;i<meshes.size();i++)
	{
		meshes[i]->Render(World);
	}
}
