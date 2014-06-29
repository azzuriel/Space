#include "Mesh.h"
#include <vector>
#include "VertexPositionTexture.h"
#include <glew.h>
#include <easylogging++.h>

#define OPENGL_CHECK_ERRORS() \
	while( unsigned int openGLError = glGetError()) \
{ \
	LOG(ERROR) << "OpenGL Error " << openGLError<< " -- " << glewGetErrorString(openGLError); \
};

Mesh::Mesh(void) :
	World(mat4(1.0f))
{
	m_vao = 0;
	m_vbo = nullptr;
	material = nullptr;
	shader = nullptr;
}


Mesh::~Mesh(void)
{
	if(m_vao == 0) {
		return;
	}
	glDeleteBuffers(2, m_vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);

	delete m_vbo;
	m_vbo = nullptr;
}

void Mesh::Unindex()
{
	Verteces.resize(Indeces.size());
	auto temp = std::vector<VertexPositionNormalTexture>(Verteces);
	for(int i=0; i<Indeces.size();i++){
		Verteces[i] = temp[Indeces[i]];
		Indeces[i] = i;
	}
} 


//************************************
// needs unindexed model
//************************************
void Mesh::computeNormal()
{
	for(int i=0; i<Verteces.size();i+=3){
		glm::vec3 const & a = Verteces[i].Position;
		glm::vec3 const & b = Verteces[i+1].Position;
		glm::vec3 const & c = Verteces[i+2].Position;
		auto t = glm::normalize(glm::cross(c - a, b - a));
		Verteces[i].Normal = Verteces[i+1].Normal = Verteces[i+2].Normal = t;
	}
} 

void Mesh::MergeVerteces()
{
	for (int i=0;i<Verteces.size();i++)
	{
		auto n = Verteces[i].Normal;
		float nn = 1.0f;
		std::vector<int> same;
		for (int j=i;j<Verteces.size();j++)
		{
			if(Verteces[i].Position == Verteces[j].Position){
				n += Verteces[j].Normal;
				nn++;
				same.push_back(j);
			}
		}
		n /= nn;
		Verteces[i].Normal = n;
		if(same.size() > 0)
		for(int j=0; j<same.size(); j++){
			Verteces[same[j]].Normal = n;
		}
	}
}

void Mesh::Create(std::vector<VertexPositionNormalTexture> v, std::vector<GLuint> i)
{
	Verteces.assign(v.begin(), v.end());
	Indeces.assign(i.begin(), i.end());
}

bool Mesh::loadOBJ(std::string path)
{
	std::vector< GLuint > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

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
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
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
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		Verteces[i] = VertexPositionTexture(vertex, normal, uv);
	}*/
	Indeces.clear();
	Indeces.resize(vertexIndices.size());
	for(int i=0;i<vertexIndices.size();i++){
		Indeces[i] = i;
	}
}

void Mesh::Bind(int type /* = 0 */)
{
	auto bindtype = type == 0 ? GL_STATIC_DRAW : GL_STREAM_DRAW;

	if(m_vbo) {
		glBindVertexArray(0);
		glDisableVertexAttribArray(BUFFER_TYPE_VERTEX);
		glDisableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
		glDisableVertexAttribArray(BUFFER_TYPE_NORMALE);

		glDeleteBuffers(2, m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}

	if(m_vao == 0){
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionNormalTexture)*Verteces.size(), &Verteces[0], bindtype);
	glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
	glVertexAttribPointer(BUFFER_TYPE_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
	glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
	glVertexAttribPointer(BUFFER_TYPE_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));  offset += sizeof(glm::vec2);
	glEnableVertexAttribArray(BUFFER_TYPE_NORMALE);
	glVertexAttribPointer(BUFFER_TYPE_NORMALE, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Indeces.size(), &Indeces[0], bindtype);

	OPENGL_CHECK_ERRORS();
}

void Mesh::Render()
{
	Render(mat4(1));
}

inline void Mesh::Render(mat4 Model)
{
	if(Verteces.size() == 0){
		return;
	}
	if(shader != nullptr) {
		shader->BindProgram();
		auto mult = Model*World;
		glUniformMatrix4fv(shader->vars[1], 1, GL_FALSE, &mult[0][0]);
		mat3 normal = transpose(mat3(inverse(mult)));
		glUniformMatrix3fv(shader->vars[2], 1, GL_FALSE, &normal[0][0]);

		glUniform4fv(glGetUniformLocation(shader->program, "material.ambient"),   1, &material->ambient[0]);
		glUniform4fv(glGetUniformLocation(shader->program, "material.diffuse"),   1, &material->diffuse[0]);
		glUniform4fv(glGetUniformLocation(shader->program, "material.specular"),  1, &material->specular[0]);
		glUniform4fv(glGetUniformLocation(shader->program, "material.emission"),  1, &material->emission[0]);																			  
		glUniform1fv(glGetUniformLocation(shader->program, "material.shininess"), 1, &material->shininess);

		glUniform1i(glGetUniformLocation(shader->program, "material.texture"), 0);
		glUniform1i(glGetUniformLocation(shader->program, "material.normal"), 1);
	}
	if(material != nullptr) {
		if(material->texture != nullptr) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
		}
		if(material->normal != nullptr) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->normal->textureId);
		}
	}
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, Indeces.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::Combine(Mesh* com)
{	
	GLuint lastIndex = Verteces.size();

	int t = Verteces.size();
	Verteces.resize(Verteces.size() + com->Verteces.size());
	for (int i =0; i<com->Verteces.size();i++)
	{
		Verteces[t] = com->Verteces[i];
		t++;
	}

	t = Indeces.size();
	Indeces.resize(Indeces.size() + com->Indeces.size());
	for (int i =0; i<com->Indeces.size();i++)
	{
		Indeces[t] = com->Indeces[i] + lastIndex;
		t++;
	}
}
