#ifndef SpriteBatch_h__
#define SpriteBatch_h__
#include <glew.h>
#include "Vector.h"
#include "TextureManager.h"
#include "JargShader.h"
#include "Font.h"
#include "VertexPositionTexture.h"

class Batched{
private:
	GLuint m_vertexBuffer, m_textureBuffer, m_indecesBuffer, vao;
	Vector2* uv;
	Vector3* vertex;
	GLuint* index;

	GLuint l_vertexBuffer, l_colorBuffer, l_indecesBuffer, lvao;
	Vector3* lvertex;
	Color4* lcolor;
	GLuint* lindex;

	GLuint dvao;
	GLuint* dvbo;
	VertexPositionColor* dvertex;
	GLuint* dindex;

	float curz;
	int curn, lcurn, dcurn;
	int dc;
	Texture* currentTex;
	Texture* blankTex;
	Font* currentFont;
	JargShader* textured;
	JargShader* colored;
public:
	Batched();
	~Batched();

	Texture* atlasTex;

	void DrawString(Vector2 pos, std::string text, Font& font);
	void Initialize(JargShader* tex, JargShader* col);
	void DrawQuad(Vector2 pos, Vector2 size, float rotation, Texture& tex, Rect sub);
	void DrawQuad(Vector2 pos, Vector2 size, float rotation, Texture& tex, int atl);
	void DrawQuad(Vector2 pos, Vector2 size, float rotation, Texture& tex);
	void DrawQuad(Vector2 pos, Vector2 size, Texture& tex);
	int RenderFinally();
	void DrawLine(Vector2 from, Vector2 to, float w, Color4 col);
	void DrawRectangle(Vector2 from, Vector2 to, Color4 col);
	void DrawLine3d(Vector3 from, Vector3 to, Color4 col);
		int RenderFinallyWorld();
private:
	inline void innerDraw(Vector2 pos, Vector2 size, float rotation, Texture& tex, Rect sub);
	void LRender();
	void Render();
	void DRender();

};
#endif // SpriteBatch_h__
