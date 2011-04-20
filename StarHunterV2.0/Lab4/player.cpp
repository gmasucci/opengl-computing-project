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
	
	//	me=new md2model("tris.md2",sm);
	//me = new DA::DA_MD5Model();
	//me->loadMesh("hellknight.md5mesh");
	//me->loadAnim("idle2.md5anim");
	//me->loadAnim("walk7.md5anim");
	//me->setAnim(0);

	this->kp.w=false;
	this->kp.a=false;
	this->kp.d=false;
	this->kp.s=false;
	this->kp.k=false;
	this->kp.l=false;
	this->rev =false;
	velocity=0.154;
	this->scaleValue=0.005f;
	this->radius=0.3;
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
	if(key=='w'){kp.w=true;this->setAnim(1);}
	if(key=='a')kp.a=true;
	if(key=='s'){
		kp.s=true;
		this->setAnim(1);
		rev=true;
	}
	if(key=='d')kp.d=true;
	if(key=='k')kp.k=true;
	if(key=='l')kp.l=true;
}

void Player::keyboardUpdater(){
	
	if(kp.w){myFrame->MoveForward(velocity);}
	if(kp.s){myFrame->MoveForward(-velocity);}
	if(kp.d){myFrame->MoveRight(-velocity);}
	if(kp.a){myFrame->MoveRight(velocity);}
	if(kp.k){myFrame->RotateWorld(0.03,0.0,1.0,0.0);}
	if(kp.l){myFrame->RotateWorld(-0.03,0.0,1.0,0.0);}
	



	//myFrame->RotateLocalX(0.01);

//	m3dCrossProduct3(






	static float interp = 0.0;
	static double current_time = 0;
	static double last_time = 0;
	float dt;

	last_time = current_time;
	current_time = (double)glutGet(GLUT_ELAPSED_TIME) / 100.0;

	/* Animate model from frames 0 to num_frames-1 */
	interp += (current_time - last_time);
	dt = (current_time - last_time);
	
    //if ( me->countAnims() > 0 )
	//	me->animate(dt);
}

void Player::keysUp(unsigned char key){
	if(key=='w'){
		kp.w=false;
		//this->setAnim(0);
	}
	if(key=='a')kp.a=false;
	if(key=='s'){
		kp.s=false;
		//this->setAnim(0);
		rev=false;
	}
	if(key=='d')kp.d=false;
	if(key=='k')kp.k=false;
	if(key=='l')kp.l=false;
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
	M3DVector
	bool retVal = false;
	//collisions.

	//check how far away we are.
	M3DVector3f Apos,Aang,Bpos;
	this->getPos(Apos,Aang);
	m->getPos(Bpos);
	Vec3 max,min;
	m->getColInfo(&max,&min); 
	//get the max, min points in space.
	//Make sure its WORLD coords (i.e add the position to the max/min)
	
	//TODO: do more detailed collisions only if close.
	// this so far may be quite cpu hungry.
	if(	Apos[0] < max.x && Apos[2] < max.z){	
	if( Apos[0] > min.x && Apos[2] > min.z){
			//youre inside the box. Get out!
			retVal=true;
		}
	}
	return retVal; 	
}