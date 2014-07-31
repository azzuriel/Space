#ifndef TextureManager_h__
#define TextureManager_h__

#include "Bitmap.h"
#include <glew.h>

struct TextureOld
{
    unsigned int textureId;
    float u1;
    float v1;
    float u2;
    float v2;
};

struct Texture {
    GLuint textureId;
    std::string name;
    unsigned int height;
    unsigned int width;
    unsigned int zsize;
    Texture();
    Texture(GLuint id);
    ~Texture();

    void Load(std::string a, bool smooth = false, bool mip = false);
    void Empty(glm::vec2 size, GLuint dim = GL_TEXTURE_2D, GLuint format = GL_RGBA);
    void CreateDepth(glm::vec2 size);
    void EmptyFloatSpace(glm::vec3 size, GLuint dim = GL_TEXTURE_2D,  GLuint color = GL_RGB16F);
};

struct Rect{
    float x,y;
    float h,w;
    Rect();
    Rect(float a,float b,float c,float d);
    ~Rect();
};

unsigned int GenerateOpenglBitmap(Bitmap &bitmap, bool smoothing, bool mipmap);

void GenerateTextCoord(Bitmap *bitmap, iRect *rect, TextureOld &texture);

class TextureManager
{
public:
    TextureManager(void);
    ~TextureManager(void);

};


#endif // TextureManager_h__
