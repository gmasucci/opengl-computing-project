#pragma once
#include "game.h"

#include "Toolbox.h"
#include <GLFrame.h>
#include "loaders.h"



Game::Game(int argc,char* argv[]){

	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800	,	600);
	glutCreateWindow("Relic Hunter - V1.0");

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
	Loaders::overlay("about.tga",&textures[4]);
	about = new Overlay(&textures[4],800,600,0,600);
	loading = new Overlay(&textures[9],800,600,0,600);
	menu = new Overlay(&textures[11],800,600,0,600);
	
	sndMan->play(MUSIC_MENU,1);
	


	CURRENT_STATE = MENU;
}

void Game::init(){
	
	//Spare textures
	Loaders::tex("skin.tga",&textures[0]);
	Loaders::tex("star.tga" ,&textures[1]);
	Loaders::tex("grass.tga",&textures[3]);
	
	//overlays
	Loaders::overlay("pause.tga",&textures[5]);
	Loaders::overlay("font.tga",&textures[6]);
	Loaders::overlay("hint.tga",&textures[7]);
	Loaders::overlay("overlay.tga",&textures[8]);
	Loaders::overlay("start help.tga",&textures[10]);
	Loaders::overlay("about.tga",&textures[4]);

	glGenTextures(2,treetex);
	Loaders::tex("trunk.tga",&treetex[0]);
	Loaders::tex("leaf.tga",&treetex[1]);
	
	glGenTextures(3,house);
	Loaders::tex("brick.tga",&house[0]);
	Loaders::tex("roof.tga",&house[1]);
	Loaders::tex("door.tga",&house[2]);

	glGenTextures(1,stumpTex);
	Loaders::tex("stump.tga",&stumpTex[1]);
	/*already loaded so copy uint so GL uses that texture*/
	stumpTex[0] = treetex[0];	

	glGenTextures(3,hedgeTex);
	Loaders::tex("hedge3.tga",&hedgeTex[0]);
	Loaders::tex("hedge1.tga",&hedgeTex[1]);
	Loaders::tex("hedge2.tga",&hedgeTex[2]);

	hud = new Overlay(&textures[8],800,600,0,600);
	eHint = new Overlay(&textures[7],200,100,600,200);
	startHelp = new Overlay(&textures[10],800,600,0,600);
	menu = new Overlay(&textures[11],800,600,0,600);
	counters = new Numbers(&textures[6]);
	pause = new Overlay(&textures[5],800,600,0,600);
	

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
		&textures[1],
		stumpTex,
		hedgeTex,
		myCam,
		theRealCam,
		&input,
		eHint,
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
		if(input.getMouseActive()){

			if(tempx > 682 && tempx < 775){
				if(tempy > 370 && tempy < 396){
					loading->render();
					glutPostRedisplay();
					glutSwapBuffers();
					sndMan->play(FX_MENU_SELECT);
					this->init();

					sndMan->stop(MUSIC_MENU);
					grabMouse = true;
					glutSetCursor(GLUT_CURSOR_NONE);
					glutWarpPointer((int)xcentre, (int)ycentre);
					CURRENT_STATE = QUICKHELP;
					PREV_STATE = MENU;
					
				}
			}
			
			if(tempx > 676 && tempx < 782){
				if(tempy > 458 && tempy < 488){
					sndMan->play(FX_MENU_SELECT);
					CURRENT_STATE = ABOUT;
					PREV_STATE = MENU;
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
			PREV_STATE = MENU;
			glutWarpPointer((int)xcentre, (int)ycentre);
			grabMouse=true;
			glutSetCursor(GLUT_CURSOR_NONE);
			sndMan->play(FX_GAME_AMBIENT,1);
			sndMan->play(MUSIC_GAME,1);
		}
		break;
	case PLAYING:

			mySM->updateAllObjects();
			M3DVector3f v,fv;
			theRealCam->update();
			theRealCam->getFrame().GetOrigin(v);
			theRealCam->getFrame().GetForwardVector(fv);
			v[1] = hm->getHeightAt(v[0],v[2]) + 1.3;
			theRealCam->setOrigin(v);
			hayden->setPos(v);
			hayden->setAng(fv);
			myCam->update(v,fv);

			if(input.getKeyState(27)){
				CURRENT_STATE = PAUSED;
				PREV_STATE = PLAYING;
				grabMouse=false;
				glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
				
			}
			if(input.getKeyState('q')){
				//grabMouse=false;
				//glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
				M3DVector3f p,a;
				hayden->getPos(p,a);
				std::cout << "pos = " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
			}

			if(input.getKeyState('w')){theRealCam->moveForward();sndMan->play(FX_WALK);}
			if(input.getKeyState('a')){theRealCam->strafeLeft();sndMan->play(FX_WALK);}
			if(input.getKeyState('s')){theRealCam->moveBackward();sndMan->play(FX_WALK);}
			if(input.getKeyState('d')){theRealCam->strafeRight();sndMan->play(FX_WALK);}
			if(input.getKeyState('e')){mySM->ePressed();}


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
		if(input.getMouseActive()){
			if(tempx > 0 && tempx < 80){
				if(tempy > 0 && tempy < 40){
					CURRENT_STATE = PREV_STATE;
					PREV_STATE = ABOUT;
				}
			}
		}
		break;

	case PAUSED:
	

		if(input.getMouseActive()){

			if(tempx > 30 && tempx < 58){
				if(tempy > 33 && tempy < 72){
					sndMan->play(FX_MENU_SELECT);
					CURRENT_STATE = QUICKHELP;
					PREV_STATE = PAUSED;
					glutWarpPointer((int)xcentre, (int)ycentre);
					grabMouse=true;
					glutSetCursor(GLUT_CURSOR_NONE);
				}
			}

			if(tempx > 317 && tempx < 480){
				if(tempy > 169 && tempy < 204){
					sndMan->play(FX_MENU_SELECT);
					CURRENT_STATE = PLAYING;
					PREV_STATE = PAUSED;
					glutWarpPointer((int)xcentre, (int)ycentre);
					grabMouse=true;
					glutSetCursor(GLUT_CURSOR_NONE);
				}
			}
			
			if(tempx > 333 && tempx < 469){
				if(tempy > 271 && tempy < 315){
					sndMan->play(FX_MENU_SELECT);
					CURRENT_STATE = ABOUT;
					PREV_STATE = PAUSED;
				}
			}
			if(tempx > 355 && tempx < 448){
				if(tempy > 377 && tempy < 417){
					CURRENT_STATE = MENU;
					PREV_STATE = PLAYING;
					sndMan->stop(FX_GAME_AMBIENT);
					sndMan->stop(MUSIC_GAME);
					sndMan->play(MUSIC_MENU,1);
				}
			}

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
	counters->render(hayden->getNumKeys(),-110,-410);

		break;
	case ABOUT:
		about->render();
		break;
	case PAUSED:
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
		counters->render(hayden->getNumKeys(),-110,-410);

		pause->render();	
		break;
	}

	glutPostRedisplay();
	glutSwapBuffers();

}


void Game::reshape(int w,int h){
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(60.0f, float(w) / float(h), 0.1f, 5000.0f);
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