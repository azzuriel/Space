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
    // Когда картинка будет увеличиваться(нет большей Мипмапы), используем LINEAR фильтрацию
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smoothing ? GL_LINEAR : GL_NEAREST);

    if(mipmap)
    {
        // Когда минимизируем — берем две ближних мипмапы и лиейно смешиваем цвета
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
            //LOG(LOG_WARNING, "Generate GLBitmap. Не поддерживаемый тип цвета.");
            break;
        }
    }

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, colorType, bitmap.GetWidth(), bitmap.GetHeight(), 0, colorType, GL_UNSIGNED_BYTE, bitmap.GetData());
    //OPENGL_CHECK_ERRORS();

    if(mipmap)
    {
        // Создаем сами мипмапы.
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
    width(0),
    zsize(0)
{
}

Texture::Texture(GLuint id) :
    textureId(id),
    height(0),
    width(0),
    zsize(0)
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

void Texture::Empty(glm::vec2 size, GLuint dim /*= GL_TEXTURE_2D*/, GLuint format /*= GL_RGBA*/)
{
    width = size.x;
    height = size.y;
    name = "empty_texture";

    glGenTextures(1, &textureId);
    glBindTexture(dim, textureId);
    glTexImage2D(dim, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(dim, 0);
}

void Texture::EmptyFloatSpace(glm::vec3 size, GLuint dim /*= GL_TEXTURE_2D*/, GLuint color /* = GL_RGB16F*/)
{
    width = size.x;
    height = size.y;
    zsize = size.z;

    name = "empty_float_space_texture";

    glGenTextures(1, &textureId);
    glBindTexture(dim, textureId);

    glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(dim, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    if(dim == GL_TEXTURE_2D || dim == GL_TEXTURE_3D)
        glTexParameteri(dim, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if(dim == GL_TEXTURE_3D)
            glTexParameteri(dim, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    if(dim == GL_TEXTURE_2D)
    glTexImage2D(GL_TEXTURE_2D, 0, color, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    else if(dim == GL_TEXTURE_3D)
        glTexImage3D(GL_TEXTURE_3D, 0, color, width, height, zsize, 0, GL_RGB, GL_FLOAT, NULL);
        else if(dim == GL_TEXTURE_1D)
            glTexImage1D(GL_TEXTURE_1D, 0, color, width, 0, GL_RGB, GL_FLOAT, NULL);
}

void Texture::CreateDepth(glm::vec2 size)
{
    width = size.x;
    height = size.y;
    name = "empty_depth";

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
