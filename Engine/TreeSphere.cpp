#include "TreeSphere.h"
#include "Cube.h"


QuadLod::QuadLod(void)
{
	m = new Mesh();
	root = nullptr;
}


QuadLod::~QuadLod(void)
{
	delete m;
	if(root){
		delete root;
	}
}

void Generate(int level, QuadLodPart* node, glm::vec3 cam){
	if(level < 1) {
		return;
	}
	node->NE = new QuadLodPart();
	node->NE->start = vec3((node->start.x + node->end.x)/2.0F, node->start.y, node->start.z);
	node->NE->end = vec3(node->end.x, node->end.y, (node->start.z + node->end.z)/2.0F);
	node->NE->initialSize = node->initialSize;

	node->NW = new QuadLodPart();
	node->NW->start = vec3(node->start.x, node->start.y, node->start.z);
	node->NW->end = vec3((node->start.x + node->end.x)/2.0F, node->end.y, (node->start.z + node->end.z)/2.0F);
	node->NW->initialSize = node->initialSize;


	node->SE = new QuadLodPart();
	node->SE->start = vec3((node->start.x + node->end.x)/2.0F, node->start.y, (node->start.z + node->end.z)/2.0F);
	node->SE->end = vec3(node->end.x, node->end.y, node->end.z);
	node->SE->initialSize = node->initialSize;

	node->SW = new QuadLodPart();
	node->SW->start = vec3(node->start.x, node->start.y,  (node->start.z + node->end.z)/2.0F);
	node->SW->end = vec3((node->start.x + node->end.x)/2.0F, node->end.y, node->end.z);
	node->SW->initialSize = node->initialSize;

	float size =  abs((float)node->start.x - (float)node->end.x)*10.0F;
	float d = glm::distance(cam, glm::vec3((node->start.x + node->end.x)/2.0F,(node->start.y + node->end.y)/2.0F,(node->start.z + node->end.z)/2.0F));
	size /= d /50;

	if( level > 1){
		float d = glm::distance(cam, glm::vec3((node->NE->start.x + node->NE->end.x)/2.0F,(node->NE->start.y + node->NE->end.y)/2.0F,(node->NE->start.z + node->NE->end.z)/2.0F));
		int m = max(1, (int)(d/size));
		Generate(level - m, node->NE, cam);
		
		d = glm::distance(cam, glm::vec3((node->SE->start.x + node->SE->end.x)/2.0F,(node->SE->start.y + node->SE->end.y)/2.0F,(node->SE->start.z + node->SE->end.z)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->SE, cam);
		
		d = glm::distance(cam, glm::vec3((node->NW->start.x + node->NW->end.x)/2.0F,(node->NW->start.y + node->NW->end.y)/2.0F,(node->NW->start.z + node->NW->end.z)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->NW, cam);
		
		d = glm::distance(cam, glm::vec3((node->SW->start.x + node->SW->end.x)/2.0F,(node->SW->start.y + node->SW->end.y)/2.0F,(node->SW->start.z + node->SW->end.z)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->SW, cam);
	}
}

inline float Noise2D__(float x, float y) {
	int n = (int) (x + y*57);
	n = (n << 13) ^ n;
	double value = (1.0f - ((n*(n*n*15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0f);
	return abs(value);
}

inline float Noise2D(float x, float y){
	return Noise2D__(x,y);
}

inline float SmoothedNoise2D(float x, float y) {
	float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) +
		Noise2D(x + 1, y + 1))/16;
	float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1))/8;
	float center = Noise2D(x, y)/4;
	return -4;//(corners + sides + center)*1000;
}

void BuildGeometry(QuadLodPart* node){
	if(node->NE){
		BuildGeometry(node->NE);
		BuildGeometry(node->NW);
		BuildGeometry(node->SE);
		BuildGeometry(node->SW);
	} else {
		node->m = new Mesh();
		//node->start = glm::normalize(node->start);
		//node->start *= node->initialSize;
		//node->end = glm::normalize(node->end);
		//node->end *= node->initialSize;
		auto mesh = node->m;
		float size = node->initialSize;
		VertexPositionNormalTexture a00;
		a00.Position = glm::vec3(node->start.x, node->start.y, node->start.z);
		//a00.Position.Normalize();
		//a00.Position *= size;
		a00.Normal = glm::vec3(0,1,0);
		a00.Uv = glm::vec2(0,0);
		VertexPositionNormalTexture a01;
		a01.Position = glm::vec3(node->end.x,  node->start.y, node->start.z);
		//a01.Position.Normalize();
		//a01.Position *= size;
		a01.Normal = glm::vec3(0,1,0);
		a01.Uv = glm::vec2(0,1);
		VertexPositionNormalTexture a10;
		a10.Position = glm::vec3(node->start.x,  node->start.y, node->end.z);
		//a10.Position.Normalize();
		//a10.Position *= size;
		a10.Normal = glm::vec3(0,1,0);
		a10.Uv = glm::vec2(1,0);
		VertexPositionNormalTexture a11;
		a11.Position = glm::vec3(node->end.x,  node->start.y, node->end.z);
		//a11.Position.Normalize();
		//a11.Position *= size;
		a11.Normal = glm::vec3(0,1,0);
		a11.Uv = glm::vec2(1,1);
		mesh->Verteces.push_back(a00);
		mesh->Verteces.push_back(a01);
		mesh->Verteces.push_back(a10);
		mesh->Verteces.push_back(a11);

		mesh->Indeces.push_back(0);
		mesh->Indeces.push_back(1);
		mesh->Indeces.push_back(2);
		mesh->Indeces.push_back(1);
		mesh->Indeces.push_back(3);
		mesh->Indeces.push_back(2);
	}
}

void CollectGeometry(QuadLodPart* node, Mesh* rootmesh){
	if(node->NE){
		CollectGeometry(node->NE, rootmesh);
		CollectGeometry(node->NW, rootmesh);
		CollectGeometry(node->SE, rootmesh);
		CollectGeometry(node->SW, rootmesh);
	}
	if(node->m) {
		rootmesh->Combine(node->m);
	}
}

void QuadLod::GenerateFrom(vec3 cam)
{
	if(!root){
		root = new QuadLodPart();
		root->initialSize = 3000;
		root->start = vec3(-1500,0,-1500);
		root->end = vec3(1500,0,1500);
		Generate(8, root, glm::vec3(cam.x,cam.y,cam.z));
		root->m = new Mesh();
		BuildGeometry(root);
		CollectGeometry(root, m);
	}

	
}

void QuadLod::Bind()
{
	m->Bind();
}

void QuadLod::Render()
{
	m->Render();
}

QuadLodPart::QuadLodPart()
{
		SW = SE = NE = NW = nullptr;
		m = nullptr;
}

QuadLodPart::~QuadLodPart()
{
	if(m){
		delete m;
		int a = 0;
	}
	if(SE){
		delete SE;
	}
	if(SW){
		delete SW;
	}
	if(NW){
		delete NW;
	}
	if(NE){
		delete NE;
	}
	SE = SW = NW = NW = nullptr;
	m = nullptr;
}
