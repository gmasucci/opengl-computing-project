#include "player.h"


Player::Player(){}

Player::Player(GLuint *tex,GLGeometryTransform *pGLGTin,Terrain *mapIn,Camera *camIn,SoundManager *smIn):GameObject(pGLGTin)
{
	myTex=tex;
	pGLGT = pGLGTin;
	map = mapIn;
	health = 5;
	stars = 0;
	this->theCam = camIn;
	sm=smIn;
	myFrame = new GLFrame();
	this->numKeys = 0;
	
	//	me=new md2model("tris.md2",sm);
	//me = new DA::DA_MD5Model();
	//me->loadMesh("hellknight.md5mesh");
	//me->loadAnim("idle2.md5anim");
	//me->loadAnim("walk7.md5anim");
	//me->setAnim(0);


	this->rev =false;
	velocity=0.164;
	this->scaleValue=0.005f;
	this->radius=0.5;
	pain = false;
	winner=false;
	
	myShader = gltLoadShaderPairWithAttributes("Shaders/phong.vert", "Shaders/phong.frag", 3, GLT_ATTRIBUTE_VERTEX, "vVertex",
			GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");

	locAmbient = glGetUniformLocation(myShader, "ambientColor");
	locDiffuse = glGetUniformLocation(myShader, "diffuseColor");
	locSpecular = glGetUniformLocation(myShader, "specularColor");
	locLight = glGetUniformLocation(myShader, "vLightPosition");
	locMVP = glGetUniformLocation(myShader, "mvpMatrix");
	locMV  = glGetUniformLocation(myShader, "mvMatrix");
	locNM  = glGetUniformLocation(myShader, "normalMatrix");
	locTexture = glGetUniformLocation(myShader, "colorMap");

}
void Player::setAnim(int n){
	//me->setAnim(n);
}
void Player::keys(unsigned char key){

}

void Player::keyboardUpdater(){
	
	//For giving the player an animated model.
	// First person view, so not using this.

	//static float interp = 0.0;
	//static double current_time = 0;
	//static double last_time = 0;
	//float dt;

	//last_time = current_time;
	//current_time = (double)glutGet(GLUT_ELAPSED_TIME) / 100.0;

	///* Animate model from frames 0 to num_frames-1 */
	//interp += (current_time - last_time);
	//dt = (current_time - last_time);
	//
	//if ( me->countAnims() > 0 )
	//	me->animate(dt);
}

void Player::keysUp(unsigned char key){
	
}
void Player::setPos(float*ppos){
	myFrame->SetOrigin(ppos[0],ppos[1]+0.46,ppos[2]);
}


void Player::setPos(float x, float y,float z){

	myFrame->SetOrigin(x,y+0.46,z);
}
void Player::setAng(float* ang){
	M3DVector3f t;
	t[0] = ang[0];
	t[2] = ang[2];
	t[1] = 0.0f;
	m3dNormalizeVector3(t);
	myFrame->SetForwardVector(t);

}
void Player::getPos(float*ret,float*ang){
	myFrame->GetOrigin(ret); 
	myFrame->GetForwardVector(ang);
}

void Player::render(GLMatrixStack *pMVM){

	M3DMatrix44f rotx;
	m3dRotationMatrix44(rotx,PI/2,-1.0f,0.0f,0.0f);
	
	M3DMatrix44f roty;
	m3dRotationMatrix44(roty,PI/2,0.0f,0.0f,-1.0f);
	
	pMVM->PushMatrix();
	pMVM->MultMatrix(*myFrame);

	pMVM->MultMatrix(rotx);
	pMVM->MultMatrix(roty);

	pMVM->Scale(scaleValue,scaleValue,scaleValue);
	
	M3DVector4f vEyeLight,vLight = { 128.0f, 50, 0 ,1.0f};
	M3DMatrix44f *mx;
	mx = theCam->getMx();
    m3dTransformVector4(vEyeLight, vLight, *mx);
		
	GLfloat vAmbientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };



	glBindTexture(GL_TEXTURE_2D,this->myTex[0]);
	glUseProgram(myShader);
	glUniform4fv(locAmbient, 1, vAmbientColor);
	glUniform4fv(locDiffuse, 1, vDiffuseColor);
	glUniform4fv(locSpecular, 1, vSpecularColor);
	glUniform3fv(locLight, 1, vEyeLight);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, pGLGT->GetModelViewProjectionMatrix());
	glUniformMatrix4fv(locMV, 1, GL_FALSE, pGLGT->GetModelViewMatrix());
	glUniformMatrix3fv(locNM, 1, GL_FALSE, pGLGT->GetNormalMatrix());
	glUniform1i(locTexture, 0);




	//me->render();
	//me->RenderFrameItp();
	pMVM->PopMatrix();

}

Player::~Player(){
	//
	//delete me;
}

bool Player::isColliding(StaticModel *m){

	bool retVal = false;
	//collisions.

	M3DVector3f Apos,Aang,Bpos;
	this->getPos(Apos,Aang);
	
	m->getPos(Bpos);
	Vec3 max,min;
	m->getColInfo(&max,&min); 
	//get the max, min points in space.
	//Make sure its WORLD coords (i.e add the position to the max/min)
	
	//TODO: do more detailed collisions only if close.
	// this so far may be quite cpu hungry.

	float r = this->radius;

	if(	Apos[0] - r < max.x && Apos[0] + r > min.x ){	
		if( Apos[2] - r < max.z && Apos[2] + r > min.z){
			retVal=true;
			//about to collide, prevent movement.

		}
	}
	return retVal; 	
}

bool Player::isColliding(int objects,Hedges *h){

	bool retVal = false;
	
	M3DVector3f Apos,Aang,Bpos;
	this->getPos(Apos,Aang);

	Vec3 max,min,pos;
	h->getColInfo(&max,&min);

	for(int i=0;i<objects;i++){
		
		pos = h->getColBox(i);
	
		float r = this->radius;

		if(	Apos[0] - r < (max.x+pos.x) && Apos[0] + r > (min.x+pos.x) ){	
			if( Apos[2] - r < (max.z+pos.z) && Apos[2] + r > (min.z+pos.z)){
				retVal=true;
				//about to collide, prevent movement.

			}
		}
	}
	return retVal; 	
}

bool Player::isColliding(int objects,House *h){

	bool retVal = false;
	
	M3DVector3f Apos,Aang,Bpos;
	this->getPos(Apos,Aang);

	Vec3 max,min,pos;
	h->getColInfo(&max,&min);

	for(int i=0;i<objects;i++){
		
		pos = h->getColBox(i);
	
		float r = this->radius;

		if(	Apos[0] - r < (max.x+pos.x) && Apos[0] + r > (min.x+pos.x) ){	
			if( Apos[2] - r < (max.z+pos.z) && Apos[2] + r > (min.z+pos.z)){
				retVal=true;
				//about to collide, prevent movement.

			}
		}
	}
	return retVal; 	
}