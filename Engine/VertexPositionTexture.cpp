#include "VertexPositionTexture.h"
VertexPositionTexture::VertexPositionTexture(Vector3 v, Vector3 p , Vector2 u) : Position(p), Uv(u)
{
}

VertexPositionTexture::VertexPositionTexture() : Position(0), Uv(0)
{
}

VertexPositionTexture::~VertexPositionTexture()
{
}

VertexPositionNormalTexture::VertexPositionNormalTexture(Vector3 norm, Vector3 pos , Vector2 uv) : Position(pos), Normal(norm), Uv(uv)
{
}

VertexPositionNormalTexture::VertexPositionNormalTexture() : Position(), Normal(), Uv() 
{
}

VertexPositionNormalTexture::~VertexPositionNormalTexture()
{
}
