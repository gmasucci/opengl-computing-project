// Shader adapted from 
// OpenGL SuperBible by Richard S. Wright Jr.
#version 130

in vec4 vVertex;
uniform mat4   mvpMatrix;
varying vec3 vVaryingTexCoord;

void main(void)
{
		vVaryingTexCoord = normalize(vVertex.xyz);
		gl_Position = mvpMatrix * vVertex;
}
