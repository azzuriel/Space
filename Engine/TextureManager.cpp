#include "TextureManager.h"
#include <glew.h>
#include <glfw3.h>
#include <fstream>

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

Texture::Texture()
{
	textureId = -1;
}

Texture::~Texture()
{

}

void Texture::Load(std::string a)
{
	name = a;

	Bitmap* b = new Bitmap();
	b->Load(a);
	textureId = GenerateOpenglBitmap(*b, false, false);
	height = b->GetHeight();
	width = b->GetWidth();
	b->Free();
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
