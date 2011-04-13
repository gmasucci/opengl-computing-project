//custom yellow refection shader

#version 130

out vec4 vFragColor;

uniform samplerCube cubeMap;
smooth in vec3 vVaryingTexCoord;

void main(void)
    { 
	vec4 yellow;
	yellow.rgba += 0.2f;
	yellow.b=0.0f;
    vFragColor = yellow + texture(cubeMap, vVaryingTexCoord.stp);
    }
    