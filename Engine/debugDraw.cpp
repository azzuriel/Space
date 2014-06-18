
#include "debugDraw.h"

#include <stdio.h> //printf debugging
#include <glew.h>
#include <btBulletDynamicsCommon.h>

GLDebugDrawer::GLDebugDrawer()
	:m_debugMode(0)
{

}

void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	bat->DrawLine3d(glm::vec3(from.x(), from.y(), from.z()), glm::vec3(to.x(), to.y(), to.z()), glm::vec4(color.x(), color.y(), color.z(), 1.0F));
}

void GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void    GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
	//glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	printf(warningString);
}

void    GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	{
	/*	btVector3 to=pointOnB+normalOnB*distance;
		const btVector3&from = pointOnB;
		glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

		GLDebugDrawer::drawLine(from, to, color);*/

		//glRasterPos3f(from.x(),  from.y(),  from.z());
		//char buf[12];
		//sprintf(buf," %d",lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
	}
}

void GLDebugDrawer::drawAabb(const btVector3& from,const btVector3& to,const btVector3& color)
{
	float tmp[ 18 ] = { from.getX(), from.getY(), from.getZ(),
					   from.getX(), to.getY(), from.getZ(),
					   from.getX(), from.getY(), from.getZ(),
					   to.getX(), from.getY(), from.getZ(),
					   from.getX(), from.getY(), from.getZ(),
					   from.getX(), from.getY(), to.getZ(),
					   };

	glPushMatrix();
	{         
		glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);         
		glVertexPointer( 3,
			GL_FLOAT,
			0,
			&tmp );

		//glPointSize( 5.0f );
		//glDrawArrays( GL_POINTS, 0, 2 );
		glDrawArrays( GL_LINES, 0, 18 );
	}
	glPopMatrix();   
}

void GLDebugDrawer::SetBatched(Batched* b)
{
	bat = b;
}
