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
#endif // Vector_h__
