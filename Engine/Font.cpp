#include "Font.h"
//#include <json/writer.h>
//#include <json/reader.h>
#include <fstream>
#include <utf8.h>
#include <math.h>
#include <algorithm>
#include "JRectangle.h"
#include <glog/logging.h>

Font::Font()
{
	library = nullptr;
	face = nullptr;
}


Font::~Font()
{
	if(tex) {
		delete tex;
	}
}

bool Font::Initialize()
{
	if (FT_Init_FreeType( &library ))
	{
		LOG(ERROR) << "FT_Init_FreeType ERROR";
	    return false;
	}
	if(!GenerateEmptyGlyph())
	{
		LOG(ERROR) << "GenerateEmptyGlyph ERROR";
		return false;
	}

	return true;
}

bool Font::Create(std::string configFileName)
{
	bool processing = true;
	if(!CreateFromConfig(configFileName))
	{
		processing = false;
	}

	if(!GenerateOpenglGlyphs(configFileName))
	{
		processing = false;
	}

	if(library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}

	return processing;
}

void Font::Remove()
{
	glyphsTextureMap.clear();
}

bool Font::GenerateEmptyGlyph()
{
	unsigned int key = 0;
	GlyphBitmap glyphBitmap;
	glyphBitmap.bitmap = new Bitmap();
	glyphBitmap.bitmap->Generate(Bitmap::FORMAT_RGBA, 6, 8, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_RGBA);

	byte *glyphImageData = glyphBitmap.bitmap->GetData();

	unsigned int w = glyphBitmap.bitmap->GetWidth();
	unsigned int h = glyphBitmap.bitmap->GetHeight();


	for(unsigned int i = 0; i < w; i++)
	{
		glyphImageData[i * channelCount + channelCount - 1] = 255;
		glyphImageData[(h - 1) * w * channelCount + i * channelCount + channelCount - 1] = 255;
	}
	for(unsigned int j = 0; j < h; j++)
	{
		glyphImageData[j * w * channelCount + channelCount - 1] = 255;
		glyphImageData[j * w * channelCount + (w - 1) * channelCount + channelCount - 1] = 255;
	}
	glyphBitmap.offsetDown = 0;

	glyphBitmap.key = key;
	glyphsBitmapList.push_back(glyphBitmap);

	return true;
}


bool Font::CreateFromConfig( std::string configFileName )
{
// 	std::ifstream configFile(configFileName);
// 	name = configFileName;
// 
// 	if (!configFile.is_open())
// 	{
// 		//LOG_WARNING("
// 		return false;
// 	}
// 
// 
// 	Json::Value root;
// 	Json::Reader reader;
// 
// 	bool parsingSuccessful = reader.parse( configFile, root );
// 	if ( !parsingSuccessful )
// 	{
// 		configFile.close();
// 		//LOG(FATAL) << configFile << " font config file corrupted.";
// 		return false;
// 	}
// 
// 	const Json::Value fontList = root["FontList"];
// 	for ( unsigned int i = 0; i < fontList.size(); i++ )
// 	{
// 		std::string fontFileName = fontList[i].get("FileName", "").asString();
// 		unsigned int fontSize = fontList[i].get("Size", 14).asUInt();
// 		const Json::Value glyphs = fontList[i]["Glyphs"];
// 
// 		if (FT_New_Face( library, fontFileName.c_str(), 0, &face ))
// 		{
// 			configFile.close();
// 			//LOG(FATAL) << fontFileName << " not opened. Font cant be build.";
// 			return false;
// 		}
// 
// 		FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, 96, 96);
// 
// 		for ( unsigned int j = 0; j < glyphs.size(); j++ )
// 		{
// 			std::string glyphList = glyphs[j].asString();
// 			GenerateGlyphsList(glyphList);
// 		}
// 
// 		if(face)
// 		{
// 			FT_Done_Face(face);
// 			face = nullptr;
// 		}
// 	}
// 
// 	configFile.close();

	FT_New_Face( library, "fonts\\arial.ttf", 0, &face);
	FT_Set_Char_Size( face, 14 << 6, 14 << 6, 96, 96);

	std::string glyphList = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890 ";
	GenerateGlyphsList(glyphList);

	if(face)
	{
		FT_Done_Face(face);
		face = nullptr;
	}

	return true;
}

bool Font::GenerateGlyphsList( std::string glyphList )
{
	GlyphBitmap glyphBitmap;

	std::vector<int> glyphsUTF32;
	utf8::utf8to32(glyphList.c_str(), glyphList.c_str() + glyphList.length(), std::back_inserter(glyphsUTF32));

	for(unsigned int i = 0; i < glyphsUTF32.size(); i++)
	{	
		int key = glyphsUTF32[i];
		if( std::find_if( glyphsBitmapList.begin(), glyphsBitmapList.end(),
			[&key](GlyphBitmap n) { return n.key == key; }
		) != glyphsBitmapList.end() )
		{
			continue;
		}

		glyphBitmap.bitmap = new Bitmap();
		if( GenerateGlyph(key, glyphBitmap) )
		{
			//glyphsBitmapList[glyphsUTF32[i]] = glyphBitmap;
			glyphBitmap.key = key;
			glyphsBitmapList.push_back(glyphBitmap);
		}
		else
		{
			//LOG_WARNING("
			glyphBitmap.bitmap->Free();
			delete glyphBitmap.bitmap;
			glyphBitmap.bitmap = nullptr;
		}

	}

	return true;
}

bool Font::GenerateGlyph( unsigned int glyphNumber, GlyphBitmap &glyphBitmap)
{

	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, glyphNumber ), FT_LOAD_DEFAULT ))
	{
		
		return false;
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
	
		return false;
	}

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap &bitmap=bitmap_glyph->bitmap;


	glyphBitmap.bitmap->Generate(Bitmap::FORMAT_RGBA, bitmap.width, bitmap.rows, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_RGBA);

	byte *glyphImageData = glyphBitmap.bitmap->GetData();

	for(int j = 0; j < bitmap.rows; j++)
	{
		for(int i=0; i < bitmap.width; i++)
		{
			glyphImageData[( bitmap.width * j + i ) * channelCount + channelCount - 1]
			= bitmap.buffer[bitmap.width * ( bitmap.rows - j - 1) + i];
		}
	}

	glyphBitmap.offsetDown = bitmap_glyph->top - bitmap.rows;

	return true;
}

bool Font::GenerateOpenglGlyphs( std::string configFileName)
{
	unsigned int area = 0;
	Bitmap *bitmap;


	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		bitmap = (*i).bitmap;
		area += bitmap->GetHeight() * bitmap->GetWidth();
	}

	unsigned int sideAtlas = (unsigned int)(sqrt( float(area) ));
	sideAtlas = next_p2( sideAtlas );

	float diff = float(sideAtlas * sideAtlas) / float(area);
	if( diff < 1.8f )
		sideAtlas <<= 1;

	if(!glyphAtlas.Create(Bitmap::FORMAT_RGBA, sideAtlas, sideAtlas))
	{
		//LOG_ERROR(
		return false;
	}

	glyphsBitmapList.sort();

	iRect rect;
	Bitmap *atlasBitmap = glyphAtlas.GetAtlas();

	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		if( !glyphAtlas.InsertImage( (*i).bitmap, rect ) )
		{
			LOG(WARNING) << "font atlass ERROR";
			delete (*i).bitmap;
			(*i).bitmap = nullptr;
			continue;
		}

		FontTexture fontTexture;

		fontTexture.width = (*i).bitmap->GetWidth();
		fontTexture.height = (*i).bitmap->GetHeight();
		fontTexture.offsetDown = (*i).offsetDown;

		GenerateTextCoord(atlasBitmap, &rect, fontTexture.texture);

		glyphsTextureMap[(*i).key] = fontTexture;

		delete (*i).bitmap;
		(*i).bitmap = nullptr;
	}

	// CreateAtlasOpenglTexture
	unsigned int ogltexture = GenerateOpenglBitmap(*atlasBitmap, false, false);
	tex = new Texture();
	tex->textureId = ogltexture;
	tex->name = configFileName;
	tex->height = atlasBitmap->GetHeight();
	tex->width = atlasBitmap->GetWidth();
	for (auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		(*i).second.texture.textureId = ogltexture;
		
	}

	glyphsBitmapList.clear();
	glyphAtlas.GetAtlas()->Save(configFileName+".png");

	glyphAtlas.Remove();
	return true;
}

const FontTexture Font::GetGlyphTexture( unsigned int utf32glyph ) const
{
	if(glyphsTextureMap.count(utf32glyph) == 0)
	{
		return glyphsTextureMap.at(0);
	} 
	return glyphsTextureMap.at(utf32glyph);
}