#include "QuadTreePlane.h"
#include <math.h>
#include "Quad.h"
#include "VertexPositionTexture.h"
#include "TextureGenerator.h"
#include "Generation.h"
#include <gtc\matrix_transform.inl>

QuadTreePlane::QuadTreePlane(void)
{
}


QuadTreePlane::~QuadTreePlane(void)
{
    delete root;
}

void QuadTreePlane::Update(glm::vec3 camera)
{
    root->Update(camera);
}

void QuadTreePlane::Init(std::shared_ptr<BasicJargShader> shader, const Camera &cam)
{
    root = new QuadTreeNode(vec3(0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5));
    basic = shader;
    K = cam.window_width/2*tan(cam.field_of_view/2);
    root->Init(shader, K, 1);
}

void QuadTreePlane::Draw()
{
    root->Draw();
}

QuadTreeNode::~QuadTreeNode(void)
{
   if(mesh){
       delete mesh;
   }
   if(NW){
       delete NW;
       delete SW;
       delete SE;
       delete NE;
   }
}

QuadTreeNode::QuadTreeNode(glm::vec3 _max, glm::vec3 _min) :
    NE(nullptr),
    SE(nullptr),
    NW(nullptr),
    SW(nullptr)
{
    max = _max;
    min = _min;
    mesh = new Mesh();
    mesh->Verteces.push_back(VertexPositionNormalTexture(glm::vec3(1,0,0), min, glm::vec2(0, 0)));
    mesh->Verteces.push_back(VertexPositionNormalTexture(glm::vec3(1,0,0), glm::vec3(max.x, min.y, min.z), glm::vec2(1, 0)));
    mesh->Verteces.push_back(VertexPositionNormalTexture(glm::vec3(1,0,0), max, glm::vec2(1, 1)));
    mesh->Verteces.push_back(VertexPositionNormalTexture(glm::vec3(1,0,0), glm::vec3(min.x, max.y, min.z), glm::vec2(0, 1)));
    mesh->Indeces.push_back(0);
    mesh->Indeces.push_back(1);
    mesh->Indeces.push_back(3);
    mesh->Indeces.push_back(2);
    mesh->Bind();
//     mesh->material = std::shared_ptr<Material>(new Material());
//     mesh->material->texture = std::shared_ptr<Texture>(new Texture());
//     mesh->material->texture->textureId = 4;
}

void QuadTreeNode::Init(std::shared_ptr<BasicJargShader> shader, float _K, float _level)
{
    mesh->shader = shader;
    K = _K;
    level = _level;

    auto tex = std::shared_ptr<Texture>(new Texture());
    tex->Empty(glm::vec2(128,128), GL_TEXTURE_2D, GL_RGB);
     
    mesh->World = glm::scale(glm::mat4(1), glm::vec3(100,100,100));

    Generation::tg.SetShader(Generation::cells);
    Generation::tg.Reset();
    Generation::tg.SetTextures(Generation::noise);
    Generation::tg.SetParams(min.x);
    Generation::tg.SetParams(max.y);
    Generation::tg.SetParams(max.x);
    Generation::tg.SetParams(min.y);
    Generation::tg.SetParams(10);
    Generation::tg.SetResultTexture(tex);
    Generation::tg.RenderOnTempFbo();
    //tex->Load("normal.png");
    mesh->material = std::shared_ptr<Material>(new Material()); 
    mesh->material->texture = tex;
}

void QuadTreeNode::Draw()
{
    if(NE){
        NE->Draw();
        NW->Draw();
        SE->Draw();
        SW->Draw();
    } else {
        mesh->Render(true);
    }
}

void QuadTreeNode::Update(glm::vec3 camera)
{
    auto w = mat3(inverse(mesh->World));
    auto tr = camera * transpose(w);
    float err = (1 / level) / distance(tr, max + min) * K;
    if(err > 100 && !NE && level < 5){
        auto cent = (max + min)/2.f;
        NE = new QuadTreeNode(glm::vec3(cent.x, min.y, min.z), glm::vec3(max.x, cent.y, min.z));
        NE->Init(mesh->shader, K, level + 1);

        NW = new QuadTreeNode(cent, max);
        NW->Init(mesh->shader, K, level + 1);

        SE = new QuadTreeNode(min, cent);
        SE->Init(mesh->shader, K, level + 1);

        SW = new QuadTreeNode(glm::vec3(min.x, cent.y, min.z), glm::vec3(cent.x, max.y, min.z));
        SW->Init(mesh->shader, K, level + 1);
    } else if (err < 90 && NE) {
        delete SW;
        delete SE;
        delete NW;
        delete NE;
        SW = NE = NW = SE = nullptr;
    }
    if(NE){
        NE->Update(camera);
        NW->Update(camera);
        SE->Update(camera);
        SW->Update(camera);
    }
}
