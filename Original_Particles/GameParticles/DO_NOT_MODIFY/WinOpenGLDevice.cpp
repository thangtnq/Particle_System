// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifdef WIN32

#include "OpenGLInterface.h"
#include "WinEventHandler.h"


OpenGLDevice::OpenGLDevice()
:	screenSize(),
	running(true)
{		
	screenSize.width = 800;
	screenSize.height = 600;
}

OpenGLDevice::~OpenGLDevice()
{
}
	
void OpenGLDevice::StartDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();							// Reset The Current Modelview Matrix
	glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10000.0f);
	glClearColor(0.20f, 0.20f, 0.50f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLDevice::EndDraw()
{
	Instance().endDraw();
}

bool OpenGLDevice::InitWindow()
{
	return Instance().initWindow();
}

bool OpenGLDevice::KillWindow()
{
	return Instance().killWindow();
}

bool OpenGLDevice::IsRunning()
{
	return Instance().isRunning();
}

void OpenGLDevice::Quit()
{
	Instance().quit();
}

void OpenGLDevice::SetHInstance(HINSTANCE hInst)
{
	Instance().setHInstance(hInst);
}

OpenGLDevice& OpenGLDevice::Instance()
{
	static OpenGLDevice device;
	return device;
}

bool OpenGLDevice::initWindow()
{
	int		PixelFormat;						// Holds The Results After Searching For A Match
	WNDCLASS	wc;									// Windows Class Structure
	DWORD		dwExStyle;							// Window Extended Style
	DWORD		dwStyle;							// Window Style
	RECT WindowRect;								// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left		=(long)0;					// Set Left Value To 0
	WindowRect.right	=(long)screenSize.width;	// Set Right Value To Requested Width
	WindowRect.top		=(long)0;					// Set Top Value To 0
	WindowRect.bottom	=(long)screenSize.height;	// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "Particles";//title.getCString();					// Set The Class Name
	
	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Exit And Return FALSE
	}

	if (fullscreen)								// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;					// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= (DWORD) screenSize.width;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= (DWORD)screenSize.height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= 16;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;				// Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;					// Exit And Return FALSE
			}
		}
	}
	if (fullscreen)								// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;						// Windows Style
		ShowCursor(FALSE);						// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	hWnd=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
					"Particles",//title.getCString(),				// Class Name
					"Particles",//title.getCString(),					// Window Title
					WS_CLIPSIBLINGS |			// Required Window Style
					WS_CLIPCHILDREN |			// Required Window Style
					dwStyle,				// Selected Window Style
					0, 0,					// Window Position
					WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
					WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
					NULL,					// No Parent Window
					NULL,					// No Menu
					hInstance,				// Instance
					NULL);					// Don't Pass Anything To WM_CREATE
	if( !hWnd )
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,						// Must Support Double Buffering
		PFD_TYPE_RGBA,							// Request An RGBA Format
		16,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,						// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,							// Main Drawing Layer
		0,								// Reserved
		0, 0, 0								// Layer Masks Ignored
	};

	hDC=GetDC(hWnd);							// Did We Get A Device Context?
	if( !hDC )
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	PixelFormat=ChoosePixelFormat(hDC,&pfd);				// Did Windows Find A Matching Pixel Format?
	if( !PixelFormat )
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	hRC=wglCreateContext(hDC);					// Are We Able To Get A Rendering Context?
	if( !hRC )
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}
	if(!wglMakeCurrent(hDC,hRC))						// Try To Activate The Rendering Context
	{
		killWindow();							// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}
	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);								// Sets Keyboard Focus To The Window

	//glShadeModel(GL_SMOOTH);						// Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background
	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective 
	return TRUE;								// Initialization Went OK
}

bool OpenGLDevice::killWindow()
{
	if (fullscreen)								// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);						// Show Mouse Pointer
	}

	if (hRC)								// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))					// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;							// Set RC To NULL
	}
	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;							// Set DC To NULL
	}
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// Set hWnd To NULL
	}
	if (!UnregisterClass("Particles",hInstance))	
	//if (!UnregisterClass(title.getCString(),hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// Set hInstance To NULL
	}
	return true;
}

void OpenGLDevice::endDraw()
{
	//glFlush();
	//glEnd();
	SwapBuffers(hDC);			// Swap Buffers (Double Buffering)
}

bool OpenGLDevice::isRunning()
{
	return running;
}

void OpenGLDevice::quit()
{
	killWindow();
	running = false;
}

void OpenGLDevice::setHInstance(HINSTANCE hInst)
{
	hInstance = hInst;
}

//void OpenGLDevice::GetApplicationDir(char* dest, int size)
//{
//	OpenGLDevice::Instance().getApplicationDir(dest, size);
//}

//void OpenGLDevice::getApplicationDir(char* dest, int size)
//{
//	char tmp_buffer[256];
//	const char* dirApp = "\\Resources";
//	GetCurrentDirectory(256, reinterpret_cast<LPSTR>(&tmp_buffer));
//	CString::concat(dest, size, &tmp_buffer[0], dirApp);
//}

#endif
