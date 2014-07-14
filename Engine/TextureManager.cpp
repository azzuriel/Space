#include "TextureManager.h"
#include <glew.h>
#include <glfw3.h>
#include <fstream>
#include "Heightmap.h"

//#define OPENGL_CHECK_ERRORS()							\
//	while( unsigned int openGLError = glGetError() )	\
//	{													\
//	LOG_ERROR("OpenGL Error 0x%X", openGLError);		\
//	};

unsigned int GenerateOpenglBitmap(Bitmap &bitmap, bool smoothing, bool mipmap)
{
    unsigned int glBitmap = 0;
    glGenTextures(1, &glBitmap);
    glBindTexture(GL_TEXTURE_2D, glBitmap);

    // ����� �������� ����� �������������(��� ������� �������), ���������� LINEAR ����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smoothing ? GL_LINEAR : GL_NEAREST);

    if(mipmap)
    {
        // ����� ������������ � ����� ��� ������� ������� � ������ ��������� �����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smoothing ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smoothing ? GL_LINEAR : GL_NEAREST);
    }

    unsigned int format = bitmap.GetFormat();
    unsigned int colorType = GL_RGB;
    switch (format)
    {
    case Bitmap::FORMAT_LUMINANCE:
        {
            colorType = GL_LUMINANCE;
            break;
        }

    case Bitmap::FORMAT_LUMINANCE_ALPHA:
        {
            colorType = GL_LUMINANCE_ALPHA;
            break;
        }

    case Bitmap::FORMAT_RGB:
        {
            colorType = GL_RGB;
            break;
        }

    case Bitmap::FORMAT_RGBA:
        {
            colorType = GL_RGBA;
            break;
        }

    default:
        {
            //LOG(LOG_WARNING, "Generate GLBitmap. �� �������������� ��� �����.");
            break;
        }
    }

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, colorType, bitmap.GetWidth(), bitmap.GetHeight(), 0, colorType, GL_UNSIGNED_BYTE, bitmap.GetData());
    //OPENGL_CHECK_ERRORS();

    if(mipmap)
    {
        // ������� ���� �������.
        glGenerateMipmap(GL_TEXTURE_2D);
        //OPENGL_CHECK_ERRORS();
    }

    return glBitmap;
}

void GenerateTextCoord( Bitmap *bitmap, iRect *rect, TextureOld &texture )
{
    unsigned int width = bitmap->GetWidth();
    unsigned int height = bitmap->GetHeight();

    if(rect == nullptr)
    {
        texture.u1 = 0;
        texture.v1 = 0;
        texture.u2 = 1;
        texture.v2 = 1;
        return;
    }

    float kx = 1.0f / float(width);
    float ky = 1.0f / float(height);

    texture.u1 = kx * float(rect->x);
    texture.v1 = ky * float(rect->y);
    texture.u2 = kx * float(rect->x + rect->w);
    texture.v2 = ky * float(rect->y + rect->h);

}

TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
}

Texture::Texture() :
    textureId(0),
    name("empty"),
    height(0),
    width(0)
{
}

Texture::Texture(GLuint id) :
    textureId(id),
    height(0),
    width(0)
{
    std::string s = "fromID ";
    name = s.append(std::to_string(id));
}

Texture::~Texture()
{
    if(textureId != -1){
        glDeleteTextures(1, &textureId);
        textureId = -1;
    }
}

void Texture::Empty(glm::vec2 size)
{
    width = size.x;
    height = size.y;
    name = "emptytexture";

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateDepth(glm::vec2 size)
{
    width = size.x;
    height = size.y;
    name = "emptydepth";

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Load(std::string a, bool smooth, bool mip)
{
    name = a;

    Bitmap* b = new Bitmap();
    b->Load(a);
    textureId = GenerateOpenglBitmap(*b, smooth, mip);
    height = b->GetHeight();
    width = b->GetWidth();
    delete b;
}

Rect::Rect()
{
    x = y = h = w = 0;
}

Rect::Rect(float a, float b, float c, float d)
{
    x = a; y = b; w = c; h = d;
}

Rect::~Rect()
{

}
