#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"
#include <bitset>
#include <glew.h>

typedef std::vector<float>	VertexBuffer;
typedef std::vector<GLuint>	IndexBuffer;

class BufferArray
{
private:
	std::bitset<4>	activeBuffers;

	unsigned int stride;

	unsigned int	VAO;
	unsigned int	videoVertexBuffer;
	unsigned int	videoindexBuffer;

public:
	VertexBuffer	vertexBuffer;
	IndexBuffer	indexBuffer;

	unsigned int vbSize;
	unsigned int ibSize;

public:
	BufferArray();
	~BufferArray();

	void Create(bool color, bool textcoord, bool normale, unsigned int sizeVertex = 0, unsigned int sizeIndex = 0);

	void Reset();

	void CreateVideoBuffer();
	void DeleteVideoBuffer();

	void PushBack( BufferArray &bufferArray);

	void Clear();

	void Draw();
};


#endif // BufferArray_h__