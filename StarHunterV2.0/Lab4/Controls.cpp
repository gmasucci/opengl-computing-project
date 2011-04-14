#include"controls.h"


//the constructor sets all keystates and mouse buttons to false.
Controls::Controls()
{
	for(int i = 0; i< 255; i++)
	{
		keys[i] = false;
	}
	mouse_active = false;
	mouse_right_active = false;
}

Controls::~Controls()
{
}

//registers a mouse click and sets a bool variable to true to store it or if no click registered resets both buttons to false
void Controls::mouseClick(int button, int state, int x, int y)
{
	if(!state)
	{
		switch(button)
		{
			case GLUT_LEFT_BUTTON:
				mouse_active=true;
				break;
			case GLUT_RIGHT_BUTTON:
				mouse_right_active=true;
				break;
		}
	}
	else
	{
		mouse_active = false;
		mouse_right_active = false;
	}
}