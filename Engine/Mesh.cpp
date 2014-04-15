#include "Mesh.h"
#include <vector>
#include "VertexPositionTexture.h"
#include <glew.h>
#include <glog\logging.h>

#define OPENGL_CHECK_ERRORS() \
	while( unsigned int openGLError = glGetError() ) \
{ \
	LOG(ERROR) << "OpenGL Error "; \
};

Mesh::Mesh(void)
{
	m_vao = -1;
	m_vbo = nullptr;
	Texture = nullptr;
	Shader = nullptr;
}


Mesh::~Mesh(void)
{
	if(m_vao == -1) {
		return;
	}
	glDeleteBuffers(2, m_vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);

	delete m_vbo;
	m_vbo = nullptr;
}

void Mesh::Create(std::vector<VertexPositionTexture> v, std::vector<GLuint> i)
{
	Verteces.assign(v.begin(), v.end());
	Indeces.assign(i.begin(), i.end());
}

void Mesh::Bind()
{
	if(m_vao == -1){
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_vbo = new GLuint[2];
		glGenBuffers(2, m_vbo);
	} else {
		glBindVertexArray(m_vao);
	}
	GLuint stride = sizeof(VertexPositionTexture);
	GLuint offset = 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionTexture)*Verteces.size(), &Verteces[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(Vector3);
	glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
	glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Indeces.size(), &Indeces[0], GL_STATIC_DRAW);

	OPENGL_CHECK_ERRORS();
}

void Mesh::Render()
{
	if(Verteces.size() == 0){
		return;
	}
	if(Shader != nullptr) {
		Shader->BindProgram();
		glUniformMatrix4fv(Shader->vars[1], 1, GL_FALSE, &World[0][0]);
	}
	if(Texture != nullptr) {
		glBindTexture(GL_TEXTURE_2D, Texture->textureId);
	}
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, Indeces.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::Combine()
{

}
