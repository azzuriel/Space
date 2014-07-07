#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>
#include "GameMath.h"

extern unsigned int GetChannelCount(unsigned int format);
extern unsigned int IsAvailableAlpha(unsigned int format);

typedef unsigned char byte;

class Bitmap
{
public:

    enum e_pixel_format
    {
        FORMAT_NULL,
        FORMAT_LUMINANCE,
        FORMAT_LUMINANCE_ALPHA,
        FORMAT_RGB,
        FORMAT_RGBA,
    };

private:
    byte *data;
    unsigned int width;
    unsigned int height;
    unsigned int format;

public:
    Bitmap(void);
    ~Bitmap(void);

    byte *GetData() const
    {
        return data;
    }

    void BlackToWhite();

    unsigned int GetWidth() const
    {
        return width;
    }
    unsigned int GetHeight() const
    {
        return height;
    }
    unsigned int GetFormat() const
    {
        return format;
    }

    void ConvertFormat(unsigned int format);

    bool Blit(i32vec2 *point, iRect *srcrect, Bitmap *bitmap);

    void Free();

    void Change(unsigned int format, unsigned int width, unsigned int height, byte *data);

    void Generate(unsigned int format, unsigned int width, unsigned int height, unsigned int color);

    bool Load(std::string fileName);
    bool Save(std::string fileName) const;
};


#endif // Bitmap_h__