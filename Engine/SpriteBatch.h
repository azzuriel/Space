#ifndef SpriteBatch_h__
#define SpriteBatch_h__
#include <glew.h>
#include "ColorExtender.h"
#include "TextureManager.h"
#include "JargShader.h"
#include "Font.h"
#include "VertexPositionTexture.h"
#include <glm.hpp>
#include <vector>

class Batched{
private:
    GLuint m_vertexBuffer, m_textureBuffer, m_indecesBuffer, m_colorBuffer, m_vao;
    glm::vec2* uv;
    glm::vec3* vertex;
    glm::vec4* color;
    GLuint* index;

    GLuint m_lineVertexBuffer, m_lineColorBuffer, m_lineIndecesBuffer, m_lvao;
    glm::vec3* m_lineVertex;
    glm::vec4* m_lineColor;
    GLuint* m_lineIndex;


    GLuint lines_2d_vao;
    GLuint* lines_2d_vbo;
    VertexPositionColor* lines_2d_vertex;
    GLuint* lines_2d_index;

    GLuint dvao;
    GLuint* dvbo;
    VertexPositionColor* dvertex;
    GLuint* dindex;

    float curz;
    int curn, lcurn, dcurn, lines_2d_curn;
    int dc;

    const Texture* m_currentTex;
    Texture* m_blankTex;
    const Font* m_currentFont;
    const JargShader* m_texturedShader;
    const JargShader* m_coloredShader;
public:
    Batched();
    ~Batched();

    Texture* atlasTexture;

    void Initialize(const JargShader* tex, const JargShader* col);

    void DrawString(glm::vec2 pos, std::string text, const Font& font);
    void DrawString(glm::vec2 pos, std::string text, vec3 col, const Font& font);
    void DrawString(glm::vec2 pos, std::string text, vec4 col, const Font& font);
    void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, Rect sub);
    void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, int atl);
    void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex);
    void DrawQuad(glm::vec2 pos, glm::vec2 size, const Texture& tex);
    void DrawLine(glm::vec2 from, glm::vec2 to, float w, glm::vec4 col);
    void DrawRectangle(glm::vec2 from, glm::vec2 to, glm::vec4 col);
    void DrawLine2d(glm::vec2 from, glm::vec2 to, glm::vec4 col);
    void DrawLine3d(glm::vec3 from, glm::vec3 to, glm::vec4 col);
    void DrawLines3d(std::vector<glm::vec3> a, glm::vec4 col);
    vec2 GetStringData(glm::vec2 pos, std::string text, vec4 col, const Font& font, glm::vec3 *vertex, glm::vec2 *uv, glm::vec4 *color, GLuint *index, int &size);
    vec2 GetStringData(glm::vec2 pos, std::string text, vec4 col, int fixer, const Font& font, glm::vec3 *vertex, glm::vec2 *uv, glm::vec4 *color, GLuint *index, int &size);
    void DrawStored(glm::vec2 pos, const Texture& tex, glm::vec3 *vertex, glm::vec2 *uv, glm::vec4 *color, GLuint *index, int &size);
    int RenderFinally();
    int RenderFinallyWorld();
    void DrawCube3d(glm::vec3 maxCoord, glm::vec3 minCoord, glm::vec4 color);

private:
    inline void innerDraw(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, Rect sub);
    void lineRender();
    void Render();
    void line3dRender();
    void line2dRender();
    
};
#endif // SpriteBatch_h__
