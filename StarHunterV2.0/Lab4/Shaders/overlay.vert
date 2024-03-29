// Shader adapted from 
// OpenGL SuperBible by Richard S. Wright Jr.
#version 140

in vec4 vVertex;
in vec2 vTexCoord;
uniform mat4 mvpMatrix;
smooth out vec2 vVaryingTexCoord;

void main(void) 
{ 
	vVaryingTexCoord = vTexCoord;
	gl_Position = mvpMatrix * vVertex;
}
