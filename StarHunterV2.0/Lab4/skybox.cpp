#include "skybox.h"


Skybox::Skybox()
{
	//initial operations
	gltMakeCube(skyCube, 20.0f);
	
	filenames[0]="Textures/Skybox/pos_x.tga";
	filenames[1]="Textures/Skybox/neg_x.tga";
	filenames[2]="Textures/Skybox/pos_y.tga";
	filenames[3]="Textures/Skybox/neg_y.tga";
	filenames[4]="Textures/Skybox/pos_z.tga";
	filenames[5]="Textures/Skybox/neg_z.tga";

	//load textures using loaders library.
	Loaders::cubemap(filenames,&skyTexture);

	//load&compile shaders
	skyboxShader = gltLoadShaderPairWithAttributes("Shaders/texonly.vert", "Shaders/texonly.frag", 2, 
                                                GLT_ATTRIBUTE_VERTEX, "vVertex",
                                                GLT_ATTRIBUTE_NORMAL, "vNormal");
	
	//get location of combined modelview/projecton matrix
	locMVPSkyBox = glGetUniformLocation(skyboxShader, "mvpMatrix");

}

void Skybox::draw(GLGeometryTransform *pGLGT){
	glDisable(GL_DEPTH_TEST);
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(locMVPSkyBox, 1, GL_FALSE, pGLGT->GetModelViewProjectionMatrix());
	skyCube.Draw(); 
	glEnable(GL_DEPTH_TEST);
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &skyTexture);
}
