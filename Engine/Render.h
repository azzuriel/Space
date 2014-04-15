#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>
#include "GameMath.h"
#include <vector>

class Render
{
private:
	mat4 orthoProjection;

	unsigned int width;
	unsigned int height;

public:
	Render(void);
	~Render(void);

	bool Initialize();

	void SetWindowSize(unsigned int width, unsigned int height);

	mat4 GetOrthoProjection();

};


#endif // Render_h__