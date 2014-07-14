import sys
from OpenGL.GL import *
from OpenGL.GLUT import *
from glew import *

# MUST have a GL context before initializing GLEW
glutInit(sys.argv)
glutInitWindowSize(512, 512)
glutCreateWindow('GLEW Testing')

# Now initialize GLEW
err = glewInit()
if (err != GLEW_OK):
   print('Error loading glew:', glewGetErrorString(err))
   sys.exit()
else:
   print('GLEW Version:', glewGetString(GLEW_VERSION))

# Now you can query extension availability in two ways.
if (glewGetExtension('GL_ARB_shading_language_100') == GL_TRUE):
   print('You have OpenGL Shading Language Support')

# The faster method
# Note this is a function call, unlike GLEW's value check
if GLEW_ARB_shading_language_100(): 
   print('You have OpenGL Shading Language Support')