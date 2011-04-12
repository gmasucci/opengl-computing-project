// Shader adapted from 
// OpenGL SuperBible by Richard S. Wright Jr.
#version 140

out vec4 vFragColor;

uniform sampler2DRect  rectangleImage;

smooth in vec2 vVaryingTexCoord;

void main(void)
{ 
	vFragColor = texture(rectangleImage, vVaryingTexCoord);
}
    