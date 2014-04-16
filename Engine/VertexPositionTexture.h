#pragma once
#ifndef VertexPositionTexture_h__
#define VertexPositionTexture_h__

#include "Vector.h"

enum BufferType
{
	BUFFER_TYPE_VERTEX,
	BUFFER_TYPE_TEXTCOORD,
	BUFFER_TYPE_NORMALE,
	BUFFER_TYPE_COLOR,
};

struct VertexPositionTexture{
public:
	Vector3 Position;
	Vector2 Uv;
	VertexPositionTexture(Vector3 v, Vector3 p , Vector2 u);
	VertexPositionTexture();
	~VertexPositionTexture();

	VertexPositionTexture operator + (VertexPositionTexture a){ VertexPositionTexture b; b.Position = Position + a.Position; b.Uv = a.Uv + Uv; return b; }
	VertexPositionTexture operator / (float a){ VertexPositionTexture b; b.Position = Position / a; b.Uv = Uv / a; return b; }
};

struct VertexPositionNormalTexture{
public:
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	VertexPositionNormalTexture(Vector3 norm, Vector3 pos, Vector2 uv);
	VertexPositionNormalTexture();
	~VertexPositionNormalTexture();

	VertexPositionNormalTexture operator + (VertexPositionNormalTexture a){ VertexPositionNormalTexture b; b.Position = Position + a.Position; b.Uv = a.Uv + Uv; b.Normal = Normal + a.Normal; return b; }
	VertexPositionNormalTexture operator / (float a){ VertexPositionNormalTexture b; b.Position = Position / a; b.Uv = Uv / a; b.Normal = Normal / 2.0F; return b; }
};

struct VertexPositionColor{
public:
	Vector3 pos;
	Color4 col;
};
#endif // VertexPositionTexture_h__
