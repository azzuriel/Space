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
	glm::vec2* uv;
	glm::vec3* vertex;
	GLuint* index;

	GLuint l_vertexBuffer, l_colorBuffer, l_indecesBuffer, lvao;
	glm::vec3* lvertex;
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

	void DrawString(glm::vec2 pos, std::string text, Font& font);
	void Initialize(JargShader* tex, JargShader* col);
	void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, Rect sub);
	void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, int atl);
	void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex);
	void DrawQuad(glm::vec2 pos, glm::vec2 size, Texture& tex);
	int RenderFinally();
	void DrawLine(glm::vec2 from, glm::vec2 to, float w, Color4 col);
	void DrawRectangle(glm::vec2 from, glm::vec2 to, Color4 col);
	void DrawLine3d(glm::vec3 from, glm::vec3 to, Color4 col);
		int RenderFinallyWorld();
private:
	inline void innerDraw(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, Rect sub);
	void LRender();
	void Render();
	void DRender();

};
#endif // SpriteBatch_h__
