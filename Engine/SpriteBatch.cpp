#include "SpriteBatch.h"
#include "JRectangle.h"
#include "ColorExtender.h"
#include "TextureManager.h"
#include "Font.h"
#include <utf8\unchecked.h>
#include <utf8\checked.h>
#include "TextGeometry.h"
#include <vector>

Batched::Batched()
{
    uv = new glm::vec2[1000*4];
    vertex = new glm::vec3[1000*4];
    color = new glm::vec4[1000*4];
    index = new GLuint[1000*6];

    lines_2d_vertex = new VertexPositionColor[1000*4];
    lines_2d_index = new GLuint[1000*6];

    m_lineVertex = new glm::vec3[1000*4];
    m_lineIndex = new GLuint[1000*6];
    m_lineColor = new glm::vec4[1000*4];
    m_textureBuffer = m_vertexBuffer = m_indecesBuffer = m_lineColorBuffer = m_lineVertexBuffer = m_lvao = m_vao = curn = lcurn = m_colorBuffer = lines_2d_curn = 0;
    dcurn = 0;
    dvao = 0; dvbo = nullptr;
    lines_2d_vao = 0; lines_2d_vbo = nullptr;
    dvertex = new VertexPositionColor[1000*4];
    dindex = new GLuint[1000*4];

    curz = -1;
    m_blankTex = new Texture();
    m_blankTex->Load("wp.png");
    m_currentTex = m_blankTex;
}

Batched::~Batched()
{
    delete[] uv;
    delete[] vertex;
    delete[] index;

    delete[] m_lineColor;
    delete[] m_lineVertex;
    delete[] m_lineIndex;

    delete[] dindex;
    delete[] dvertex;

    delete[] lines_2d_vbo;

    delete m_blankTex;

    glDeleteBuffers(1, &m_indecesBuffer);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_colorBuffer);
    glDeleteBuffers(1, &m_textureBuffer);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);

    glDeleteBuffers(1, &m_lineVertexBuffer);
    glDeleteBuffers(1, &m_lineIndecesBuffer);
    glDeleteBuffers(1, &m_lineColorBuffer);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_lvao);

    glDeleteBuffers(2, dvbo);
    glBindVertexArray(dvao);
    glDeleteVertexArrays(1, &dvao);

    glDeleteBuffers(2, lines_2d_vbo);
    glBindVertexArray(lines_2d_vao);
    glDeleteVertexArrays(1, &lines_2d_vao);
}

void Batched::Initialize(const JargShader* tex, const JargShader* col){
    m_texturedShader = tex;
    m_coloredShader = col;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_colorBuffer);
    glGenBuffers(1, &m_indecesBuffer);
    glGenBuffers(1, &m_textureBuffer);

    glGenVertexArrays(1, &m_lvao);
    glBindVertexArray(m_lvao);

    glGenBuffers(1, &m_lineVertexBuffer);
    glGenBuffers(1, &m_lineIndecesBuffer);
    glGenBuffers(1, &m_lineColorBuffer);

    glGenVertexArrays(1, &lines_2d_vao);
    glBindVertexArray(lines_2d_vao);
    lines_2d_vbo = new GLuint[2];
    glGenBuffers(2, lines_2d_vbo);

    glGenVertexArrays(1, &dvao);
    glBindVertexArray(dvao);
    dvbo = new GLuint[2];
    glGenBuffers(2, dvbo);
}

void Batched::DrawLines3d(std::vector<glm::vec3> a, glm::vec4 col){
    for (unsigned int i = 0; i < a.size(); i+=2)
    {
        DrawLine3d(a[i], a[i+1], col);
    }
}

void Batched::DrawStored(glm::vec2 pos, const Texture& tex, glm::vec3 *l_vertex, glm::vec2 *l_uv, glm::vec4 *l_color, GLuint *l_index, int &size) {
    if(curn > 1000 - size - 1){
        Render();
    }
    if(tex.textureId != m_currentTex->textureId){
        Render();
        m_currentTex = &tex;
    }
    auto pos3 = vec3(pos, 0);
    for(int i = 0; i < size; i++)
    {
        vertex[4*curn+0] = l_vertex[4*i+0] + pos3;
        vertex[4*curn+1] = l_vertex[4*i+1] + pos3;
        vertex[4*curn+2] = l_vertex[4*i+2] + pos3;
        vertex[4*curn+3] = l_vertex[4*i+3] + pos3;

        uv[4*curn+0] = l_uv[4*i+0];
        uv[4*curn+1] = l_uv[4*i+1];
        uv[4*curn+2] = l_uv[4*i+2];
        uv[4*curn+3] = l_uv[4*i+3];

        color[4*curn+0] = l_color[4*i+0];
        color[4*curn+1] = l_color[4*i+1];
        color[4*curn+2] = l_color[4*i+2];
        color[4*curn+3] = l_color[4*i+3];

        index[6*curn+0] = 4*curn+2;
        index[6*curn+1] = 4*curn+3;
        index[6*curn+2] = 4*curn+0;
        index[6*curn+3] = 4*curn+0;
        index[6*curn+4] = 4*curn+1;
        index[6*curn+5] = 4*curn+2;
        curn++;
        curz+=0.001f;
    }
}


//************************************
// Method:    GetStringData
// FullName:  Batched::GetStringData
// Access:    public 
// Returns:   glm::vec2 -- Text render size
// Qualifier:
// Parameter: glm::vec2 pos -- position of text, basically (0, 0)
// Parameter: std::string text
// Parameter: vec4 col
// Parameter: const Font & font
// Parameter: glm::vec3 * vertex -- vertex buffer to fill; minimum size = text.lenght*4
// Parameter: glm::vec2 * uv -- uv buffer to fill; minimum size = text.lenght*4
// Parameter: glm::vec4 * color -- color buffer to fill; minimum size = text.lenght*4
// Parameter: GLuint * index -- index buffer to fill; minimum size = text.lenght*6
// Parameter: int & size -- current buffer offset in quads (vert*4 or index*6)
//************************************
inline vec2 Batched::GetStringData(glm::vec2 pos, std::string text, vec4 col, const Font& font, glm::vec3 *vertex, glm::vec2 *uv, glm::vec4 *color, GLuint *index, int &size) {
    return GetStringData(pos, text, col, 1000, font, vertex, uv, color, index, size);
}



//************************************
// Method:    GetStringData
// FullName:  Batched::GetStringData
// Access:    public 
// Returns:   glm::vec2 -- Text render size
// Qualifier:
// Parameter: glm::vec2 pos -- position of text, basically (0, 0)
// Parameter: std::string text
// Parameter: vec4 col
// Parameter: int fixer -- maximum width of text
// Parameter: const Font & font
// Parameter: glm::vec3 * vertex -- vertex buffer to fill; minimum size = text.lenght*4
// Parameter: glm::vec2 * uv -- uv buffer to fill; minimum size = text.lenght*4
// Parameter: glm::vec4 * color -- color buffer to fill; minimum size = text.lenght*4
// Parameter: GLuint * index -- index buffer to fill; minimum size = text.lenght*6
// Parameter: int & size -- current buffer offset in quads (vert*4 or index*6)
//************************************
glm::vec2 Batched::GetStringData(glm::vec2 pos, std::string text, vec4 col, int fixer, const Font& font, glm::vec3 *vertex, glm::vec2 *uv, glm::vec4 *color, GLuint *index, int &size)
{
    std::vector<std::uint32_t> utf32text;
    utf8::utf8to32(text.begin(), text.end(), std::back_inserter(utf32text));
    m_currentFont = &font;
    FontTexture fontTexture;
    float glyphX = pos.x;
    float glyphY = pos.y;
    float maxx = 0;
    float stringHeight = font.GetGlyphTexture(0).height;
    float stringWidth = font.GetGlyphTexture(0).width;
    for(unsigned int i = 0; i < utf32text.size(); i++)
    {
        fontTexture = font.GetGlyphTexture(utf32text[i]);
        fontTexture.width = stringWidth;
        if(utf32text[i] == 32){
            glyphX += stringWidth/2;
            continue;
        }
        if(utf32text[i] == 10){
            glyphY += fontTexture.height+3;
            glyphX = pos.x;
            continue;
        }
        if(glyphX + fontTexture.width + 1 + stringWidth > fixer){
            glyphY += fontTexture.height+3;
            glyphX = pos.x;
        }
        float ypos = glyphY - fontTexture.height - fontTexture.offsetDown + stringHeight;
        //innerDraw( vec2(glyphX, ypos), vec2((float)fontTexture.width, (float)fontTexture.height), 0, *font.tex, Rect(fontTexture.texture.u1, fontTexture.texture.v1, fontTexture.texture.u2 - fontTexture.texture.u1, fontTexture.texture.v2 - fontTexture.texture.v1));
        vertex[4*size+0] = glm::vec3(glyphX, ypos, curz);
        vertex[4*size+1] = glm::vec3(glyphX, ypos + (float)fontTexture.height, curz);
        vertex[4*size+2] = glm::vec3(glyphX + (float)fontTexture.width, ypos + (float)fontTexture.height, curz);
        vertex[4*size+3] = glm::vec3(glyphX + (float)fontTexture.width, ypos, curz);
        uv[4*size+0] = glm::vec2(fontTexture.texture.u1, fontTexture.texture.v2);
        uv[4*size+1] = glm::vec2(fontTexture.texture.u1, fontTexture.texture.v1);
        uv[4*size+2] = glm::vec2(fontTexture.texture.u2, fontTexture.texture.v1);
        uv[4*size+3] = glm::vec2(fontTexture.texture.u2, fontTexture.texture.v2);
        color[4*size+0] = col;
        color[4*size+1] = col;
        color[4*size+2] = col;
        color[4*size+3] = col;
        index[6*size+0] = 4*size+2;
        index[6*size+1] = 4*size+3;
        index[6*size+2] = 4*size+0;
        index[6*size+3] = 4*size+0;
        index[6*size+4] = 4*size+1;
        index[6*size+5] = 4*size+2;
        size++;

        glyphX += fontTexture.width + 1;
        if (glyphX > maxx){
            maxx = glyphX;
        }
    }
    return glm::vec2(maxx, glyphY+stringHeight);
}

void Batched::DrawString(glm::vec2 pos, std::string text, const Font& font){
    if(curn > 1000 - text.length()*4 - 1){
        Render();
    }
    if(font.tex->textureId != m_currentTex->textureId){
        Render();
        m_currentTex = font.tex;
    }

    GetStringData(pos, text, vec4(1,1,1,1), font, vertex, uv, color, index, curn);

    curz+=0.001f;
}

void Batched::DrawString(glm::vec2 pos, std::string text, vec3 col, const Font& font){
    if(curn > 1000 - text.length()*4 - 1){
        Render();
    }
    if(font.tex->textureId != m_currentTex->textureId){
        Render();
        m_currentTex = font.tex;
    }

    GetStringData(pos, text, vec4(col, 1), font, vertex, uv, color, index, curn);

    curz+=0.001f;
}

void Batched::DrawString(glm::vec2 pos, std::string text, vec4 col, const Font& font){
    if(curn > 1000 - text.length()*4 - 1){
        Render();
    }
    if(font.tex->textureId != m_currentTex->textureId){
        Render();
        m_currentTex = font.tex;
    }

    GetStringData(pos, text, col, font, vertex, uv, color, index, curn);

    curz+=0.001f;
}

inline void Batched::innerDraw(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, Rect sub){
    if(curn >= 1000 - 1){
        Render();
    }
    if(tex.textureId != m_currentTex->textureId){
        Render();
        m_currentTex = &tex;
    }
    vertex[4*curn+0] = glm::vec3(pos.x, pos.y, curz);
    vertex[4*curn+1] = glm::vec3(pos.x + size.x, pos.y, curz);
    vertex[4*curn+2] = glm::vec3(pos.x, pos.y + size.y, curz);
    vertex[4*curn+3] = glm::vec3(pos.x + size.x, pos.y + size.y, curz);
    uv[4*curn+3] = glm::vec2(sub.x + sub.w, sub.y);
    uv[4*curn+2] = glm::vec2(sub.x, sub.y);
    uv[4*curn+1] = glm::vec2(sub.x + sub.w, sub.y + sub.h);
    uv[4*curn+0] = glm::vec2(sub.x, sub.y + sub.h);
    color[4*curn+0] = Colors::White;
    color[4*curn+1] = Colors::White;
    color[4*curn+2] = Colors::White;
    color[4*curn+3] = Colors::White;
    index[6*curn+0] = 4*curn+0;
    index[6*curn+1] = 4*curn+2;
    index[6*curn+2] = 4*curn+1;
    index[6*curn+3] = 4*curn+1;
    index[6*curn+4] = 4*curn+2;
    index[6*curn+5] = 4*curn+3;
    curn++;
    curz+=0.001f;
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, Rect sub)
{
    innerDraw(pos, size, rotation, (const Texture) tex, sub);
}	

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex, int atl)
{
    int i = atl%64;
    int j = atl/64;
    Rect aa(i/64.0, (j*32.0)/tex.height, 1/64.0, 32.0/tex.height);
    innerDraw(pos, size, rotation, (const Texture) tex, aa);
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, const Texture& tex)
{
    innerDraw(pos, size, rotation, (const Texture) tex, Rect(0,0,1,1));
}

void Batched::DrawQuad(glm::vec2 pos, glm::vec2 size, const Texture& tex)
{
    innerDraw(pos, size, 0, (const Texture) tex, Rect(0,0,1,1));
}

void Batched::DrawLine(glm::vec2 from, glm::vec2 to, float w, glm::vec4 col){
    if(curn >= 1000){
        Render();
    }

    vertex[4*curn+0] = glm::vec3(from.x - 1, from.y + 1, curz);
    vertex[4*curn+1] = glm::vec3(from.x + 1, from.y - 1, curz);
    vertex[4*curn+2] = glm::vec3(to.x - 1, to.y + 1, curz);
    vertex[4*curn+3] = glm::vec3(to.x + 1, to.y - 1, curz);
    uv[4*curn+3] = glm::vec2(0, 0);
    uv[4*curn+2] = glm::vec2(0, 0);
    uv[4*curn+1] = glm::vec2(0, 0);
    uv[4*curn+0] = glm::vec2(0, 0);
    color[4*curn+0] = col;
    color[4*curn+1] = col;
    color[4*curn+2] = col;
    color[4*curn+3] = col;
    index[6*curn+0] = 4*curn+0;
    index[6*curn+1] = 4*curn+2;
    index[6*curn+2] = 4*curn+1;
    index[6*curn+3] = 4*curn+1;
    index[6*curn+4] = 4*curn+2;
    index[6*curn+5] = 4*curn+3;
    curn++;
    curz+=0.001f;
}

void Batched::DrawLine3d(glm::vec3 from, glm::vec3 to, glm::vec4 col){
    if(dcurn >= 1000){
        line3dRender();
    }
    auto t = 2*dcurn;
    dvertex[t++].pos = from;
    dvertex[t--].pos = to;
    dvertex[t++].col = col;
    dvertex[t--].col = col;
    dindex[t] = t++;
    dindex[t] = t;
    dcurn++;
}

void Batched::DrawLine2d(glm::vec2 from, glm::vec2 to, glm::vec4 col){
    if(dcurn >= 1000){
        line2dRender();
    }
    auto t = 2*lines_2d_curn;
    lines_2d_vertex[t++].pos = vec3(from, curz);
    lines_2d_vertex[t--].pos = vec3(to, curz);
    lines_2d_vertex[t++].col = col;
    lines_2d_vertex[t--].col = col;
    lines_2d_index[t] = t++;
    lines_2d_index[t] = t;
    lines_2d_curn++;
    curz+=0.001f;
}

void Batched::DrawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 col){
    if(curn >= 1000 - 1){
        Render();
    }
    // 	if(Batched::m_blankTex->textureId != m_currentTex->textureId){
    // 		Render();
    // 		m_currentTex = (const Texture *)Batched::m_blankTex;
    // 	}
    vertex[4*curn+0] = glm::vec3(pos.x, pos.y, curz);
    vertex[4*curn+1] = glm::vec3(pos.x + size.x, pos.y, curz);
    vertex[4*curn+2] = glm::vec3(pos.x, pos.y + size.y, curz);
    vertex[4*curn+3] = glm::vec3(pos.x + size.x, pos.y + size.y, curz);
    uv[4*curn+3] = glm::vec2(0, 0);
    uv[4*curn+2] = glm::vec2(0, 0);
    uv[4*curn+1] = glm::vec2(0, 0);
    uv[4*curn+0] = glm::vec2(0, 0);
    color[4*curn+0] = col;
    color[4*curn+1] = col;
    color[4*curn+2] = col;
    color[4*curn+3] = col;
    index[6*curn+0] = 4*curn+0;
    index[6*curn+1] = 4*curn+2;
    index[6*curn+2] = 4*curn+1;
    index[6*curn+3] = 4*curn+1;
    index[6*curn+4] = 4*curn+2;
    index[6*curn+5] = 4*curn+3;
    curn++;
    curz+=0.001f;
}

//world space render
int Batched::RenderFinallyWorld()
{
    line3dRender();
    dcurn = 0;
    return dc;
}

//ui space render
int Batched::RenderFinally()
{
    lineRender();
    line2dRender();
    Render();

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
    glBindVertexArray(m_vao);
    m_texturedShader->BindProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_currentTex->textureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Batched::m_blankTex->textureId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*curn*4, vertex, GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
    glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*curn*4, uv, GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
    glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*curn*4, color, GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
    glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indecesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*curn*6, index, GL_STREAM_DRAW);


    glDrawElements(GL_TRIANGLES, curn*6, GL_UNSIGNED_INT, NULL);
    curn = 0;
    dc++;
}

void Batched::lineRender()
{
    if(lcurn == 0) {
        return;
    }
    glBindVertexArray(m_lvao);
    m_coloredShader->BindProgram();

    glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*lcurn*4, m_lineVertex, GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
    glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_lineColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*lcurn*4, m_lineColor, GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
    glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIndecesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lcurn*6, m_lineIndex, GL_STREAM_DRAW);


    glDrawElements(GL_TRIANGLES, lcurn*6, GL_UNSIGNED_INT, NULL);
    dc++;
    lcurn = 0;
}

void Batched::line3dRender()
{
    glBindVertexArray(dvao);
    m_coloredShader->BindProgram();

    GLuint stride = sizeof(VertexPositionColor);
    GLuint offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, dvbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor)*dcurn*2, &dvertex[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
    glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
    glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dvbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*dcurn*2, &dindex[0], GL_STREAM_DRAW);

    glPointSize(3);
    glDrawElements(GL_LINES, dcurn*2, GL_UNSIGNED_INT, NULL);
    //glDrawElements(GL_POINTS, dcurn*2, GL_UNSIGNED_INT, NULL);
    dc++; //dc++;
    dcurn = 0;
}

void Batched::line2dRender()
{
    glBindVertexArray(dvao);
    m_texturedShader->BindProgram();

    GLuint stride = sizeof(VertexPositionColor);
    GLuint offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, lines_2d_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor)*lines_2d_curn*2, &lines_2d_vertex[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
    glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
    glVertexAttribPointer(BUFFER_TYPE_COLOR, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_2d_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lines_2d_curn*2, &lines_2d_index[0], GL_STREAM_DRAW);

    glPointSize(3);
    glDrawElements(GL_LINES, lines_2d_curn*2, GL_UNSIGNED_INT, NULL);
    //glDrawElements(GL_POINTS, dcurn*2, GL_UNSIGNED_INT, NULL);
    dc++; //dc++;
    lines_2d_curn = 0;
}
