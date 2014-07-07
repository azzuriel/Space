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
    Texture();
    Texture(GLuint id);
    ~Texture();

    void Load(std::string a);
    void Empty(glm::vec2 size);
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
