#include "VertexPositionTexture.h"
VertexPositionTexture::VertexPositionTexture(glm::vec3 v, glm::vec3 p , glm::vec2 u) : Position(p), Uv(u)
{
}

VertexPositionTexture::VertexPositionTexture() : Position(0), Uv(0)
{
}

VertexPositionTexture::~VertexPositionTexture()
{
}

VertexPositionNormalTexture::VertexPositionNormalTexture(glm::vec3 norm, glm::vec3 pos , glm::vec2 uv) : Position(pos), Normal(norm), Uv(uv)
{
}

VertexPositionNormalTexture::VertexPositionNormalTexture() : Position(), Normal(), Uv() 
{
}

VertexPositionNormalTexture::~VertexPositionNormalTexture()
{
}
