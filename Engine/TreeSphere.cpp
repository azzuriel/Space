#include "TreeSphere.h"
#include "Cube.h"


TreeSphere::TreeSphere(void)
{
	m = new Mesh();
	root = nullptr;
}


TreeSphere::~TreeSphere(void)
{
	delete m;
	if(root){
		delete root;
	}
}

void Generate(int level, TreeSpherePart* node, Vector3 cam){
	if(level < 1) {
		return;
	}
	node->NE = new TreeSpherePart();
	float mo = (node->rect.z - node->rect.x)/3;
	node->NE->rect = vec4((node->rect.x + node->rect.z)/2.0F, node->rect.y, node->rect.z, (node->rect.y + node->rect.w)/2.0F);

	node->NW = new TreeSpherePart();
	node->NW->rect = vec4(node->rect.x, node->rect.y, (node->rect.x + node->rect.z)/2.0F, (node->rect.y + node->rect.w)/2.0F);


	node->SE = new TreeSpherePart();
	node->SE->rect = node->rect/2.0F;
	node->SE->rect = vec4((node->rect.x + node->rect.z)/2.0F, (node->rect.y + node->rect.w)/2.0F, node->rect.z, node->rect.w);

	node->SW = new TreeSpherePart();
	node->SW->rect = node->rect/2.0F;
	node->SW->rect = vec4(node->rect.x, (node->rect.y + node->rect.w)/2.0F, (node->rect.x + node->rect.z)/2.0F, node->rect.w);
	float size =  glm::log(abs((float)node->rect.x - (float)node->rect.z))*25;	if( level > 1){
		float d = Vector3::Distance(cam, Vector3((node->NE->rect.x + node->NE->rect.z)/2.0F,0,(node->NE->rect.y + node->NE->rect.w)/2.0F));
		int m = max(1, (int)(d/size));
		Generate(level - m, node->NE, cam);
		
		d = Vector3::Distance(cam, Vector3((node->SE->rect.x + node->SE->rect.z)/2.0F,0,(node->SE->rect.y + node->SE->rect.w)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->SE, cam);
		
		d = Vector3::Distance(cam, Vector3((node->NW->rect.x + node->NW->rect.z)/2.0F,0,(node->NW->rect.y + node->NW->rect.w)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->NW, cam);
		
		d = Vector3::Distance(cam, Vector3((node->SW->rect.x + node->SW->rect.z)/2.0F,0,(node->SW->rect.y + node->SW->rect.w)/2.0F));
		m = max(1, (int)(d/size));
		Generate(level - m, node->SW, cam);
	}
}


void BuildGeometry(TreeSpherePart* node){
	if(node->NE){
		BuildGeometry(node->NE);
		BuildGeometry(node->NW);
		BuildGeometry(node->SE);
		BuildGeometry(node->SW);
	} else {
		node->m = new Mesh();
		auto mesh = node->m;
		VertexPositionNormalTexture a00;
		a00.Position =Vector3(node->rect.x, -4, node->rect.y);
		a00.Normal = Vector3(0,1,0);
		a00.Uv = Vector2(0,0);
		VertexPositionNormalTexture a01;
		a01.Position = Vector3(node->rect.z, -4, node->rect.y);
		a01.Normal = Vector3(0,1,0);
		a01.Uv = Vector2(0,1);
		VertexPositionNormalTexture a10;
		a10.Position = Vector3(node->rect.x, -4, node->rect.w);
		a10.Normal = Vector3(0,1,0);
		a10.Uv = Vector2(1,0);
		VertexPositionNormalTexture a11;
		a11.Position = Vector3(node->rect.z, -4, node->rect.w);
		a11.Normal = Vector3(0,1,0);
		a11.Uv = Vector2(1,1);
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

void CollectGeometry(TreeSpherePart* node, Mesh* rootmesh){
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

void TreeSphere::GenerateFrom(vec4 cam)
{
	if(!root){
		root = new TreeSpherePart();
		root->rect = vec4(-1500,-1500,1500,1500);
		Generate(8, root, Vector3(cam.x,cam.y,cam.z));
		root->m = new Mesh();
		BuildGeometry(root);
		CollectGeometry(root, m);
	}

	
}

void TreeSphere::Bind()
{
	m->Bind();
}

void TreeSphere::Render()
{
	m->Render();
}

TreeSpherePart::TreeSpherePart()
{
		SW = SE = NE = NW = nullptr;
		m = nullptr;
}

TreeSpherePart::~TreeSpherePart()
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
