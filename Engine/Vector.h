#ifndef Vector_h__
#define Vector_h__

struct Color4 {
public:
	float r,g,b,a;
	static Color4 White;
	static Color4 Red;
	Color4();
	Color4(float r_, float g_, float b_, float a_);
	Color4(int r_, int g_, int b_, int a_);
	~Color4();
private:
	inline int clamp(int a, int min, int max);
};
struct Vector2 {
public:
	float x, y;
	Vector2();
	Vector2(float a, float b);
	Vector2(float a);
	~Vector2();
	Vector2 operator + (const Vector2& a){return Vector2(x + a.x, y + a.y);}
	void operator += (const Vector2& a){x += a.x; y += a.y;}
	Vector2 operator - (Vector2 a){return Vector2(x - a.x, y - a.y);}
	Vector2 operator - (){return Vector2(-x, -y);}
	void operator -= (Vector2 a){x -= a.x; y -= a.y;}
	Vector2 operator * (float a){return Vector2(x * a, y * a);}
	Vector2 operator / (float a){return Vector2(x / a, y / a);}
	void operator *= (float a){x = x * a; y = y * a;}
	bool operator >  (const Vector2& a){return (x > a.x && y > a.y);}
	bool operator >= (const Vector2& a){return (x >= a.x && y >= a.y);}
	bool operator <  (const Vector2& a){return (x < a.x && y < a.y);}
	bool operator <= (const Vector2& a){return (x <= a.x && y <= a.y);}
	bool operator == (const Vector2& a){return (x == a.x && y == a.y);}
	bool operator != (const Vector2& a){return (x != a.x && y != a.y);}
};

struct Vector3 {
	float x, y, z;
	Vector3();
	Vector3(float a, float b, float c);
	Vector3(float a);
	void Normalize();
	~Vector3();
	Vector3 operator + (const Vector3& a){return Vector3(x + a.x, y + a.y, z + a.z);}
	void operator += (Vector3 a){x += a.x; y += a.y; z += a.z; }
	Vector3 operator - (Vector3 a){return Vector3(x - a.x, y - a.y, z - a.z);}
	Vector3 operator - (){return Vector3(-x, -y, -z);}
	void operator -= (Vector3 a){x -= a.x; y -= a.y;}
	Vector3 operator * (float a){return Vector3(x * a, y * a, z * a);}
	Vector3 operator / (float a){return Vector3(x / a, y / a, z / a);}
	void operator *= (float a){x = x * a; y = y * a; z = z*a;}
	bool operator >  (const Vector3& a){return (x > a.x && y > a.y && z > a.z);}
	bool operator >= (const Vector3& a){return (x >= a.x && y >= a.y && z >= a.z);}
	bool operator <  (const Vector3& a){return (x < a.x && y < a.y && z < a.z);}
	bool operator <= (const Vector3& a){return (x <= a.x && y <= a.y && z <= a.z);}
	bool operator == (const Vector3& a){return (x == a.x && y == a.y && z == a.z);}
	bool operator != (const Vector3& a){return (x != a.x && y != a.y && z != a.z);}
};
#endif // Vector_h__
