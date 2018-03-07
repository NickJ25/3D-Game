//test
#if _DEBUG // Opens console if on Debug mode
#pragma comment (linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include <SDL.h>
#include <iostream>

using namespace std;

void exitFatalError(char *message) //Error function - prints error message and quit program
{
	cout << message << " " << endl;
	cout << SDL_GetError();
	SDL_Quit();
	exit(1);
}

bool handleSDLEvent(SDL_Event const &sdlEvent) // Exit via escape key and close icon
{
	if (sdlEvent.type == SDL_QUIT)
		return false;
	if (sdlEvent.type == SDL_KEYDOWN)
	{
		// Can extend this to handle a wider range of keys
		switch (sdlEvent.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return false;
		default:
			break;
		}
	}
	return true;
}

int main(int argc, char *argv[]) // Program entry point - SDL manages the actual WinMain entry point for us
{
	SDL_GLContext glContext; // OpenGL context handle
	SDL_Window * hWindow; // window handle

	SDL_Init(SDL_INIT_VIDEO); // initialize SDL

							  // Create 800x600 window
	hWindow = SDL_CreateWindow("3D Games Assessment",
		100, 100, 800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	glContext = SDL_GL_CreateContext(hWindow);
	SDL_Event sdlEvent;
	bool running = true;
	while (running) // the event loop
	{
		SDL_PollEvent(&sdlEvent);
		running = handleSDLEvent(sdlEvent);
		//update(); // not used yet!
		//draw(hWindow); //not used yet!
	}

	SDL_DestroyWindow(hWindow);
	SDL_Quit();
	return 0;
}



