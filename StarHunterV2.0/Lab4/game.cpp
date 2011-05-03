#pragma once
#include "game.h"
#include "Toolbox.h"
#include <GLFrame.h>
#include "loaders.h"



Game::Game(int argc,char* argv[]){
	vWhite[0]=1.0f;
	vWhite[1]=1.0f;
	vWhite[2]=1.0f;
	vWhite[3]=1.0f;

	vLightPos[0] = 128.0f;
	vLightPos[1] = 30.0f;
	vLightPos[2] = 0.0f;
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800	,	600);
	glutCreateWindow("Star Hunter - V1.2");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//failed, something blew up
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(9001); //This exit code is OVER 9000!!!!!!
	}


	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	this->sndMan = new SoundManager();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glGenTextures(15,textures);
	Loaders::overlay("start.tga",&textures[11]);
	Loaders::overlay("load.tga",&textures[9]);
	loading = new Overlay(&textures[9],800,600,0,600);
	
	sndMan->play(MUSIC_MENU,1);
	menu = new Overlay(&textures[11],800,600,0,600);

	CURRENT_STATE = MENU;
}

void Game::init(){
	
	Loaders::tex("skin.tga",&textures[0]);
	Loaders::tex("star.tga" ,&textures[1]);
	Loaders::tex("grass.tga",&textures[3]);
	Loaders::tex("beast.tga",&textures[4]);
	Loaders::tex("heart.tga",&textures[5]);
	Loaders::overlay("font.tga",&textures[6]);
	Loaders::overlay("hint.tga",&textures[7]);
	Loaders::overlay("overlay.tga",&textures[8]);
	Loaders::overlay("start help.tga",&textures[10]);
	
	

	glGenTextures(2,treetex);
	Loaders::tex("trunk.tga",&treetex[0]);
	Loaders::tex("leaf.tga",&treetex[1]);
	
	glGenTextures(3,house);
	Loaders::tex("brick.tga",&house[0]);
	Loaders::tex("roof.tga",&house[1]);
	Loaders::tex("door.tga",&house[2]);

	glGenTextures(1,stumpTex);
	Loaders::tex("stump.tga",&stumpTex[1]);
	stumpTex[0] = treetex[0];//already loaded so copy uint so GL uses that texture


	hud = new Overlay(&textures[8],800,50,0,50);
	eHint = new Overlay(&textures[7],200,100,600,200);
	startHelp = new Overlay(&textures[10],800,600,0,600);
	menu = new Overlay(&textures[11],800,600,0,600);
	counters = new Numbers(&textures[6]);
	myCam = new Camera(&modelViewMatrix);
	theRealCam = new Camerak(&modelViewMatrix);
	hm = new Terrain ("h4.pgm",3,&textures[3],&transformPipeline);
	
	mySky = new Skybox();

	hayden = new Player(&textures[4],&transformPipeline,hm,myCam,sndMan);

	mySM = new ObjectManager(
		&transformPipeline,
		hayden,
		hm,
		treetex,
		house,
		&textures[4],
		stumpTex,
		myCam,
		theRealCam,
		&input,
		sndMan
	);
	
	
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f ); // white background
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	//glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)*0.5,glutGet(GLUT_WINDOW_HEIGHT)*0.5);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	modelViewMatrix.LoadIdentity();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}

void Game::update(){
	int xcentre = glutGet(GLUT_WINDOW_WIDTH) / 2;		// stores the current horizontal centre iof the screen
	int ycentre = glutGet(GLUT_WINDOW_HEIGHT) / 2;		// stores the current vertical centre of the screen
	
	float tempx = input.getMouseX();
	float tempy = input.getMouseY();
	float offset = 0;

	switch(CURRENT_STATE){
	case MENU:
		//check for clicks
		//get coords of clicks.
		//case 0,1,2
		//change gamemode
		// if start then load all shit: init();
		//			switch to quickhelp, not playing.
		// if about then display credits overlay (TBC)
		if(input.getMouseActive()){

			if(tempx > 682 && tempx < 775){
				if(tempy > 370 && tempy < 396){
					loading->render();
					glutPostRedisplay();
					glutSwapBuffers();

					this->init();
					sndMan->stop(MUSIC_MENU);
					grabMouse = true;
					glutSetCursor(GLUT_CURSOR_NONE);
					CURRENT_STATE = QUICKHELP;
					
				}
			}
			
			if(tempx > 676 && tempx < 782){
				if(tempy > 458 && tempy < 488){
					CURRENT_STATE = ABOUT;
				}
			}
			if(tempx > 688 && tempx < 770){
				if(tempy > 546 && tempy < 576){
					glutLeaveMainLoop();
				}
			}

		}

		break;
	case QUICKHELP:
		if(input.getKeyState('e')){
			CURRENT_STATE = PLAYING;
		}
		break;
	case PLAYING:

			mySM->updateAllObjects();
			M3DVector3f v,fv;
			theRealCam->getFrame().GetOrigin(v);
			theRealCam->getFrame().GetForwardVector(fv);
			v[1] = hm->getHeightAt(v[0],v[2]) + 1.3;
			theRealCam->setOrigin(v);
			hayden->setPos(v);
			hayden->setAng(fv);
			myCam->update(v,fv);

			if(input.getKeyState(27)){glutLeaveMainLoop();}
			if(input.getKeyState('q')){
				grabMouse=false;
				glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
				M3DVector3f p,a;
				hayden->getPos(p,a);
				std::cout << "pos = " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
			}

			if(input.getKeyState('w')){theRealCam->moveForward();}
			if(input.getKeyState('a')){theRealCam->strafeLeft();}
			if(input.getKeyState('s')){theRealCam->moveBackward();}
			if(input.getKeyState('d')){theRealCam->strafeRight();}
			if(input.getKeyState('e')){/*use button*/}


			if(tempx < xcentre - 1 || tempx > xcentre + 1)				// provides a dead zone in the centre of the screen
			{
				offset = (xcentre - tempx) * 0.003f;
				theRealCam->rotateAntiClockWise(offset);
			}
			if(tempy < ycentre - 1 || tempy > ycentre + 1)				// provides a dead zone in the centre of the screen
			{
				offset = (ycentre - tempy) * 0.003f;
				theRealCam->rotateUp(offset);
			}
			if(offset != 0 && grabMouse)
			{
				glutWarpPointer((int)xcentre, (int)ycentre);
			}

		break;
	case ABOUT:
		if(input.getMouseActive() || input.getMouseRightActive()){
			CURRENT_STATE = MENU;
		}
		break;
	}


}

void Game::display(){

	//sqRot++; 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	float t = this->updater.GetElapsedSeconds();
	if(t>0.0166666668){ //update 60 times/sec
		this->update();
		this->updater.Reset();
	}
	
	switch(CURRENT_STATE){
	case MENU:
		menu->render();
		break;
	case QUICKHELP:
		// Skybox
		//***********
		theRealCam->PushMatrix(true);	//true for skybox only
		mySky->draw(&transformPipeline);
		theRealCam->PopMatrix();	//pop off the rotation only transform for the camera
		//***********

		theRealCam->PushMatrix();//no params, means standard transforms (ie not rotation only)

			hm->drawMe(theRealCam);
			mySM->renderAllObjects(&modelViewMatrix);
	
		theRealCam->PopMatrix();		//last pop. for cam matrix

		hud->render();

		startHelp->render();
		eHint->render();
		break;
		//note: deliberate omission of a break here;
	case PLAYING:

	// Skybox
	//***********
	theRealCam->PushMatrix(true);	//true for skybox only
	mySky->draw(&transformPipeline);
	theRealCam->PopMatrix();	//pop off the rotation only transform for the camera
	//***********

	theRealCam->PushMatrix();//no params, means standard transforms (ie not rotation only)

		hm->drawMe(theRealCam);
		mySM->renderAllObjects(&modelViewMatrix);
	
	theRealCam->PopMatrix();		//last pop. for cam matrix

	hud->render();
	//render numbers(hayden->getKeys(),int,int)

		break;
	case ABOUT:
		break;
	}

	glutPostRedisplay();
	glutSwapBuffers();

}


void Game::reshape(int w,int h){
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(60.0f, float(w) / float(h), 0.001f, 1000.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
}
void Game::gameMain(){
	glutMainLoop();
}
Game::~Game(){
//for future deletions.
	//delete hud;
	//delete eHint;
	//delete startHelp;
	//delete menu;
	//delete counters;
	//delete myCam;
	//delete hm;
	//delete mySky;
	//delete hayden;
	//delete mySM;
	//delete sndMan;
	


}