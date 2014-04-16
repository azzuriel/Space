#include "Mesh.h"
#include <vector>
#include "VertexPositionTexture.h"
#include <glew.h>
#include <glog\logging.h>
#include <..\..\Sansbox\libs\release\glog\glog\logging.h>

#define OPENGL_CHECK_ERRORS() \
	while( unsigned int openGLError = glGetError()) \
{ \
	LOG(ERROR) << "OpenGL Error " << openGLError<< " -- " << glewGetErrorString(openGLError); \
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

void Mesh::Create(std::vector<VertexPositionNormalTexture> v, std::vector<GLuint> i)
{
	Verteces.assign(v.begin(), v.end());
	Indeces.assign(i.begin(), i.end());
}

bool Mesh::loadOBJ(std::string path)
{
	std::vector< GLuint > vertexIndices, uvIndices, normalIndices;
	std::vector< Vector3 > temp_vertices;
	std::vector< Vector2 > temp_uvs;
	std::vector< Vector3 > temp_normals;

	FILE * file = fopen(path.c_str(), "r");
	if( file == NULL ){    
		printf("Impossible to open the file !\n");    
		return false;
	}

	while( 1 ){
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				LOG(FATAL) << "Model ruined";
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	Verteces.clear();
	Verteces.resize(vertexIndices.size());
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		Verteces[i] = VertexPositionNormalTexture(temp_normals[normalIndices[i]-1], temp_vertices[vertexIndices[i]-1], temp_uvs[uvIndices[i]-1]);
	}
	/*for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		Vector3 vertex = temp_vertices[ vertexIndex-1 ];
		unsigned int normalIndex = normalIndices[i];
		Vector3 normal = temp_normals[ normalIndex-1 ];
		unsigned int uvIndex = uvIndices[i];
		Vector2 uv = temp_uvs[ uvIndex-1 ];
		Verteces[i] = VertexPositionTexture(vertex, normal, uv);
	}*/
	Indeces.clear();
	Indeces.resize(vertexIndices.size());
	for(int i=0;i<vertexIndices.size();i++){
		Indeces[i] = i;
	}
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
	GLuint stride = sizeof(VertexPositionNormalTexture);
	GLuint offset = 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionNormalTexture)*Verteces.size(), &Verteces[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(Vector3);
	glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
	glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));  offset += sizeof(Vector2);
	glEnableVertexAttribArray(BUFFER_TYPE_NORMALE);
	glVertexAttribPointer(BUFFER_TYPE_NORMALE, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

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
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, Indeces.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::Combine()
{

}
