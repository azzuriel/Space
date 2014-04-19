#include "SpriteBatch.h"
#include "JRectangle.h"
#include "Vector.h"
#include "TextureManager.h"
#include "Font.h"
#include <utf8\unchecked.h>
#include <utf8\checked.h>

Batched::Batched()
{
	uv = new glm::vec2[1000*4];
	vertex = new glm::vec3[1000*4];
	index = new GLuint[1000*6];

	lvertex = new glm::vec3[1000*4];
	lindex = new GLuint[1000*6];
	lcolor = new Color4[1000*4];
	m_textureBuffer = m_vertexBuffer = m_indecesBuffer = l_colorBuffer = l_vertexBuffer = lvao = vao = curn = lcurn = 0;
	dcurn = 0;
	dvao = 0; dvbo = nullptr;
	dvertex = new VertexPositionColor[1000*4];
	dindex = new GLuint[1000*4];

	curz = -1;
	blankTex = new Texture();
	blankTex->name = "blank";
	blankTex->textureId = 0;
	currentTex = blankTex;
}

Batched::~Batched()
{
	delete[] uv;
	delete[] vertex;
	delete[] index;

	delete[] lcolor;
	delete[] lvertex;
	delete[] lindex;

	delete[] dindex;
	delete[] dvertex;

	delete blankTex;

	glDeleteBuffers(1, &m_indecesBuffer);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_textureBuffer);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &l_vertexBuffer);
	glDeleteBuffers(1, &l_indecesBuffer);
	glDeleteBuffers(1, &l_colorBuffer);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &lvao);

	glDeleteBuffers(2, dvbo);
	glBindVertexArray(dvao);
	glDeleteVertexArrays(1, &dvao);
}

void Batched::Initialize(JargShader* tex, JargShader* col){
	textured = tex;
	colored = col;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_indecesBuffer);
	glGenBuffers(1, &m_textureBuffer);

	glGenVertexArrays(1, &lvao);
	glBindVertexArray(lvao);

	glGenBuffers(1, &l_vertexBuffer);
	glGenBuffers(1, &l_indecesBuffer);
	glGenBuffers(1, &l_colorBuffer);

	glGenVertexArrays(1, &dvao);
	glBindVertexArray(dvao);
	dvbo = new GLuint[2];
	glGenBuffers(2, dvbo);
}

void Batched::DrawString(glm::vec2 pos, std::string text, Font& font){
	//std::vector<uint32_t> utf32text;
	//utf8::utf8to32(text.begin(), text.end(), std::back_inserter(utf32text));
	//currentFont = &font;
	//FontTexture fontTexture;
	//float glyphX = pos.x;
	//float glyphY = pos.y;
	//float stringHeight = 22.0f;
	//if(curn > 1000 - utf32text.size() - 1){
	//	Render();
	//}
	//if(font.tex->textureId != currentTex->textureId){
	//	Render();
	//	currentTex = font.tex;
	//}
	//for(unsigned int i = 0; i < utf32text.size(); i++)
	//{
	//	fontTexture = font.GetGlyphTexture(utf32text[i]);
	//	float ypos = glyphY + stringHeight - fontTexture.height - fontTexture.offsetDown;
	//	//innerDraw( vec2(glyphX, ypos), vec2((float)fontTexture.width, (float)fontTexture.height), 0, *font.tex, Rect(fontTexture.texture.u1, fontTexture.texture.v1, fontTexture.texture.u2 - fontTexture.texture.u1, fontTexture.texture.v2 - fontTexture.texture.v1));
	//	vertex[4*curn+0] = glm::vec3(glyphX, ypos, curz);
	//	vertex[4*curn+1] = glm::vec3(glyphX, ypos + (float)fontTexture.height, curz);
	//	vertex[4*curn+2] = glm::vec3(glyphX + (float)fontTexture.width, ypos + (float)fontTexture.height, curz);
	//	vertex[4*curn+3] = glm::vec3(glyphX + (float)fontTexture.width, ypos, curz);
	//	uv[4*curn+0] = glm::vec2(fontTexture.texture.u1, fontTexture.texture.v2);
	//	uv[4*curn+1] = glm::vec2(fontTexture.texture.u1, fontTexture.texture.v1);
	//	uv[4*curn+2] = glm::vec2(fontTexture.texture.u2, fontTexture.texture.v1);
	//	uv[4*curn+3] = glm::vec2(fontTexture.texture.u2, fontTexture.texture.v2);
	//	index[6*curn+0] = 4*curn+2;
	//	index[6*curn+1] = 4*curn+3;
	//	index[6*curn+2] = 4*curn+0;
	//	index[6*curn+3] = 4*curn+0;
	//	index[6*curn+4] = 4*curn+1;
	//	index[6*curn+5] = 4*curn+2;
	//	curn++;

	//	glyphX += fontTexture.width;
	//}
	//curz+=0.001f;
}

inline void Batched::innerDraw(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, Rect sub){
	if(curn >= 1000){
		Render();
	}
	if(tex.textureId != currentTex->textureId){
		Render();
		currentTex = &tex;
	}
	vertex[4*curn+0] = glm::vec3(pos.x, pos.y, curz);
	vertex[4*curn+1] = glm::vec3(pos.x + size.x, pos.y, curz);
	vertex[4*curn+2] = glm::vec3(pos.x, pos.y + size.y, curz);
	vertex[4*curn+3] = glm::vec3(pos.x + size.x, pos.y + size.y, curz);
	uv[4*curn+3] = glm::vec2(sub.x, sub.y);
	uv[4*curn+2] = glm::vec2(sub.x + sub.w, sub.y);
	uv[4*curn+1] = glm::vec2(sub.x, sub.y + sub.h);
	uv[4*curn+0] = glm::vec2(sub.x + sub.w, sub.y + sub.h);
	index[6*curn+0] = 4*curn+0;
	index[6*curn+1] = 4*curn+1;
	index[6*curn+2] = 4*curn+2;
	index[6*curn+3] = 4*curn+1;
	index[6*curn+4] = 4*curn+2;
	index[6*curn+5] = 4*curn+3;
	curn++;
	curz+=0.001f;
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, Rect sub)
{
	innerDraw(pos, size, rotation, tex, sub);
}	

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex, int atl)
{
	int i = atl%64;
	int j = atl/64;
	Rect aa(i/64.0, (j*32.0)/tex.height, 1/64.0, 32.0/tex.height);
	innerDraw(pos, size, rotation, tex, aa);
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Texture& tex)
{
	innerDraw(pos, size, rotation, tex, Rect(0,0,1,1));
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, Texture& tex)
{
	innerDraw(pos, size, 0, tex, Rect(0,0,1,1));
}

void Batched::DrawLine(glm::vec2 from, glm::vec2 to, float w, Color4 col){
	if(lcurn >= 1000){
		LRender();
	}
	lvertex[4*lcurn+0] = glm::vec3(from.x - 1, from.y + 1, curz);
	lvertex[4*lcurn+1] = glm::vec3(from.x + 1, from.y - 1, curz);
	lvertex[4*lcurn+2] = glm::vec3(to.x - 1, to.y + 1, curz);
	lvertex[4*lcurn+3] = glm::vec3(to.x + 1, to.y - 1, curz);
	lcolor[4*lcurn+3] = col;
	lcolor[4*lcurn+2] = col;
	lcolor[4*lcurn+1] = col;
	lcolor[4*lcurn+0] = col;
	lindex[6*lcurn+0] = 4*lcurn+0;
	lindex[6*lcurn+1] = 4*lcurn+1;
	lindex[6*lcurn+2] = 4*lcurn+2;
	lindex[6*lcurn+3] = 4*lcurn+1;
	lindex[6*lcurn+4] = 4*lcurn+2;
	lindex[6*lcurn+5] = 4*lcurn+3;
	lcurn++;
	curz+=0.001f;
}

void Batched::DrawLine3d(glm::vec3 from, glm::vec3 to, Color4 col){
	if(dcurn >= 1000){
		DRender();
	}
	dvertex[2*dcurn+0].pos = from;
	dvertex[2*dcurn+1].pos = to;
	dvertex[2*dcurn+0].col = col;
	dvertex[2*dcurn+1].col = col;
	dindex[2*dcurn+0] = 2*dcurn+0;
	dindex[2*dcurn+1] = 2*dcurn+1;
	dcurn++;
}

void Batched::DrawRectangle(glm::vec2 pos, glm::vec2 size, Color4 col){
	if(lcurn >= 1000){
		LRender();
	}
	lvertex[4*lcurn+0] = glm::vec3(pos.x, pos.y, curz);
	lvertex[4*lcurn+1] = glm::vec3(pos.x + size.x, pos.y, curz);
	lvertex[4*lcurn+2] = glm::vec3(pos.x, pos.y + size.y, curz);
	lvertex[4*lcurn+3] = glm::vec3(pos.x + size.x, pos.y + size.y, curz);
	lcolor[4*lcurn+3] = col;
	lcolor[4*lcurn+2] = col;
	lcolor[4*lcurn+1] = col;
	lcolor[4*lcurn+0] = col;
	lindex[6*lcurn+0] = 4*lcurn+0;
	lindex[6*lcurn+1] = 4*lcurn+1;
	lindex[6*lcurn+2] = 4*lcurn+2;
	lindex[6*lcurn+3] = 4*lcurn+1;
	lindex[6*lcurn+4] = 4*lcurn+2;
	lindex[6*lcurn+5] = 4*lcurn+3;
	lcurn++;
	curz+=0.001f;
}

int Batched::RenderFinallyWorld()
{
	if(dcurn != 0){
		glBindVertexArray(dvao);
		colored->BindProgram();

		GLuint stride = sizeof(VertexPositionColor);
		GLuint offset = 0;
		glBindBuffer(GL_ARRAY_BUFFER, dvbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor)*dcurn*2, &dvertex[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
		glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
		glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
		glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dvbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*dcurn*2, &dindex[0], GL_STATIC_DRAW);


		glPointSize(3);
		glDrawElements(GL_LINES, dcurn*2, GL_UNSIGNED_INT, NULL);
		glDrawElements(GL_POINTS, dcurn*2, GL_UNSIGNED_INT, NULL);
		dc++; dc++;
	}
 	dcurn = 0;
	return dc;
}

int Batched::RenderFinally()
{
	if(curn != 0) {
		glBindVertexArray(vao);
		textured->BindProgram();
		glBindTexture(GL_TEXTURE_2D, currentTex->textureId);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*curn*4, vertex, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
		glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*curn*4, uv, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
		glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indecesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*curn*6, index, GL_DYNAMIC_DRAW);


		glDrawElements(GL_TRIANGLES, curn*6, GL_UNSIGNED_INT, NULL);
		dc++;
	}
	if(lcurn != 0) {
		glBindVertexArray(lvao);
		colored->BindProgram();

		glBindBuffer(GL_ARRAY_BUFFER, l_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*lcurn*4, lvertex, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
		glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glBindBuffer(GL_ARRAY_BUFFER, l_colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Color4)*lcurn*4, lcolor, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
		glVertexAttribPointer(BUFFER_TYPE_COLOR, 2, GL_FLOAT, GL_FALSE, sizeof(Color4), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l_indecesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lcurn*6, lindex, GL_DYNAMIC_DRAW);


		glDrawElements(GL_TRIANGLES, lcurn*6, GL_UNSIGNED_INT, NULL);
		dc++;
	}

	curz = -90;
	curn = 0;
	lcurn = 0;
	int dcc = dc;
	dc = 0;
	return dcc;
}
			
void Batched::Render()
{
	if(curn == 0) {
		return;
	}
	glBindVertexArray(vao);
	textured->BindProgram();
	glBindTexture(GL_TEXTURE_2D, currentTex->textureId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*curn*4, vertex, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*curn*4, uv, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
	glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indecesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*curn*6, index, GL_DYNAMIC_DRAW);


	glDrawElements(GL_TRIANGLES, curn*6, GL_UNSIGNED_INT, NULL);
	curn = 0;
	dc++;
}

void Batched::LRender()
{
	if(lcurn == 0) {
		return;
	}
	glBindVertexArray(lvao);
	colored->BindProgram();

	glBindBuffer(GL_ARRAY_BUFFER, l_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*lcurn*4, lvertex, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, l_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color4)*lcurn*4, lcolor, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
	glVertexAttribPointer(BUFFER_TYPE_COLOR, 2, GL_FLOAT, GL_FALSE, sizeof(Color4), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l_indecesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lcurn*6, lindex, GL_STATIC_DRAW);


	glDrawElements(GL_TRIANGLES, lcurn*6, GL_UNSIGNED_INT, NULL);
	dc++;
	lcurn = 0;
}

void Batched::DRender()
{
	glBindVertexArray(dvao);
	colored->BindProgram();

	GLuint stride = sizeof(VertexPositionColor);
	GLuint offset = 0;
	glBindBuffer(GL_ARRAY_BUFFER, dvbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor)*dcurn*2, &dvertex[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
	glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
	glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dvbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*dcurn*2, &dindex[0], GL_DYNAMIC_DRAW);

	glPointSize(3);
	glDrawElements(GL_LINES, dcurn*2, GL_UNSIGNED_INT, NULL);
	glDrawElements(GL_POINTS, dcurn*2, GL_UNSIGNED_INT, NULL);
	dc++; dc++;
 	dcurn = 0;
}


