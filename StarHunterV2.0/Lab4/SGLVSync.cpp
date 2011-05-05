// SGLVSync.cpp: Implementierung der Klasse SGLVSync.
//
// Copyright by André Stein
// E-Mail: stonemaster@steinsoft.net, andre_stein@web.de
// http://www.steinsoft.net
//////////////////////////////////////////////////////////////////////

#include "SGLVSync.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>


//Function typdefs
typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC)	(int);
typedef int  (*PFNWGLEXTGETSWAPINTERVALPROC)    (void);

PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT	    = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT  = NULL;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

SGLVSync::SGLVSync()
{
	isOK = false;
}

SGLVSync::~SGLVSync()
{

}

//////////////////////////////////////////////////////////////////////
// Funktionen
//////////////////////////////////////////////////////////////////////

bool SGLVSync::isEnabled()
{
	if (isOK)
		return (wglGetSwapIntervalEXT() > 0);
	else
		return false;
}

bool SGLVSync::isDisabled()
{
	if (isOK)
		return (!wglGetSwapIntervalEXT());
	else
		return false;
}

void SGLVSync::enable()
{
	if (isOK)
		wglSwapIntervalEXT(1);
}

void SGLVSync::disable()
{
	if (isOK)
		wglSwapIntervalEXT(0);
}

bool SGLVSync::init()
{
	isOK = false;
	const char*ext = 0;
	std::string s;
	s.append((char*)glGetStringi(GL_EXTENSIONS, 221));
	ext = s.c_str();

	if(ext != NULL){
		if (strstr(ext,"WGL_EXT_swap_control"))
		{
			wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

			isOK = true;
		}
	}
	return isOK;
}
