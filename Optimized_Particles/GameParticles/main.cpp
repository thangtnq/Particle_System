// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#include <assert.h>

// Offset to local directory
// REALLY useful trick to insure that you link to your code not some other library
#include "DO_NOT_MODIFY\Timer.h"
#include "DO_NOT_MODIFY\GlobalTimer.h"
#include "DO_NOT_MODIFY\EventHandler.h"
#include "DO_NOT_MODIFY\OpenGLInterface.h"
#include "DO_NOT_MODIFY\Trace.h"

#include "ParticleEmitter.h"

#define UNUSED_VAR(v) ((void *)v)

// WIN32 - prototype
int main(int argc, char * const argv[]);

// WinMain required by windows for all win32 applications.
// This is our Windows entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNUSED_VAR(nCmdShow);
	UNUSED_VAR(lpCmdLine);
	UNUSED_VAR(hPrevInstance);
	OpenGLDevice::SetHInstance(hInstance);
	main(__argc, __argv);
}

// main program
int main(int argc, char * const argv[])
{
	UNUSED_VAR(argc);
	UNUSED_VAR(argv);

	Trace::out("test\n");

	bool success = false;
	srand(1);

	// initialize timers:------------------------------
		// Initialize timer
	timer::initTimer();

	// Create a global Timer
	globalTimer::create();

	// Create a timer objects
	timer updateTimer;
	timer drawTimer;

	// create a window:---------------------------------
	success = OpenGLDevice::InitWindow();
	assert(success);

	// create an emitter:-------------------------------
	ParticleEmitter emitter;

	// Get the inverse Camera Matrix:-------------------

		// initialize the camera matrix
	Matrix CameraMatrix;
	CameraMatrix.setIdentMatrix();

	// setup the translation matrix
	Matrix TransMatrix;
	Vect4D Trans(0.0f, 10.0f, 15.0f);
	TransMatrix.setTransMatrix(&Trans);

	// multiply them together
	Matrix tmp;
	tmp = CameraMatrix * TransMatrix;

	Matrix inverseCameraMatrix;
	tmp.Inverse(inverseCameraMatrix);

	// counter for printing
	int i = 0;

	// main update loop... do this forever or until some breaks 
	while (OpenGLDevice::IsRunning())
	{
		// start update timer ---------------------------------------
		updateTimer.tic();

			// start draw... end draw (the draw updates)
			OpenGLDevice::StartDraw();

			// set matrix to Model View
			// push the inverseCameraMarix to stack
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(reinterpret_cast<float*>(&inverseCameraMatrix));
			glPushMatrix(); // push the camera matrix

			// update the emitter
			emitter.update();

		// stop update timer: -----------------------------------------
		updateTimer.toc();

		// start draw timer: ----------------------------------------
		drawTimer.tic();

			// draw particles
			emitter.draw();

			// pop matrix - needs to correspond to previous push
			glPopMatrix();

		// stop draw timer: -----------------------------------------
		drawTimer.toc();

		// finish draw update
		OpenGLDevice::EndDraw();

		// Love for Windows - allows the windows to behave to external events
		EventHandler::ProcessEvents();

		// update ouput every 50 times
		i++;
		if (i > 10)
		{
			i = 0;
			float updateTime = updateTimer.timeInSeconds();
			float drawTime = drawTimer.timeInSeconds();
			printf("LoopTime: update:%f ms  draw:%f ms  tot:%f\n", updateTime * 1000.0f, drawTime * 1000.0f, (updateTime + drawTime) *1000.0f);
		}
	}

	return 0;
}


// End of file