// Based loosly on the first triangle OpenGL tutorial
// http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_%28C%2B%2B/Win%29
// This program will render two triangles
// Most of the OpenGL code for dealing with buffer objects, etc has been moved to a 
// utility library, to make creation and display of mesh objects as simple as possible

// Windows specific: Uncomment the following line to open a console window for debug output
#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif


#include <stack>
#include "rt3d.h"

#define DEG_TO_RADIAN 0.017453293

using namespace std;

// Globals
// Real programs don't use globals :-D
// Data would normally be read from files

GLfloat cubeVertCount = 8;
GLfloat cubeVerts[] = { -0.5, -0.5f, -0.5f,
						-0.5, 0.5f, -0.5f,
						0.5, 0.5f, -0.5f,
						0.5, -0.5f, -0.5f,
						-0.5, -0.5f, 0.5f,
						-0.5, 0.5f, 0.5f,
						0.5, 0.5f, 0.5f,
						0.5, -0.5f, 0.5f };

GLfloat cubeColours[] = { 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 0.0f, 1.0f };

GLuint cubeIndexCount = 36;
GLuint cubeIndices[] = { 0,1,2, 0,2,3, // back  
						1,0,5, 0,4,5, // left					
						6,3,2, 3,6,7, // right
						1,5,6, 1,6,2, // top
						0,3,4, 3,7,4, // bottom
						6,5,4, 7,6,4 }; // front

rt3d::lightStruct light0 = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
	{ 0.7f, 0.7f, 0.7f, 1.0f }, // diffuse
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
	{ 0.0f, 0.0f, 1.0f, 1.0f }  // position
};

rt3d::materialStruct material0 = {
	{0.4f, 0.2f, 0.2f, 1.0f}, // ambient
	{0.8f, 0.5f, 0.5f, 1.0f}, // diffuse
	{1.0f, 0.8f, 0.8f, 1.0f}, // specular
	2.0f // shininess
};

// Screen Size
GLfloat screenWidth = 800, screenHeight = 600;

// glm perspective settings
GLfloat fov = float(60.0f*DEG_TO_RADIAN), aspect = ((float)screenWidth / (float)screenHeight), near = 1.0f, far = 50.0f;

// Stack for storing modelview matrix when dealing with multiple matrixes
stack<glm::mat4> mvStack;

GLuint meshObjects[2];
GLuint mvpShaderProgram;
GLuint mvpShaderProgam2;
bool lightMode = false;

// Object Settings
GLfloat dx = 0.0f, dy = 0.0f, r = 0.0f, scalar = 1.0f;

// Light Settings
GLfloat dxl = 0.0f, dyl = 0.0f, lscalar = 1.0f, lr = 0.0f;

// Set up rendering context
SDL_Window * setupRC(SDL_GLContext &context) {
	SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		rt3d::exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.3 context.
	// Not able to use SDL to choose profile (yet), should default to core profile on 3.2 or later
	// If you request a context not supported by your drivers, no OpenGL context will be created

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

													   // Create 800x600 window
	window = SDL_CreateWindow("SDL/GLM/OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void init(void) {
	// For this simple example we'll be using the most basic of shader programs
	glEnable(GL_DEPTH_TEST);
	mvpShaderProgram = rt3d::initShaders("phong.vert", "phong.frag");//("mvp.vert", "minimal.frag");
	rt3d::setLight(mvpShaderProgram, light0);
	rt3d::setMaterial(mvpShaderProgram, material0);

	// Going to create our mesh objects here
	meshObjects[0] = rt3d::createMesh(cubeVertCount, cubeVerts, nullptr, cubeVerts, nullptr, cubeIndexCount, cubeIndices);

}

void update(void) {

	// Keyboard inputs
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_L]) lightMode = !lightMode;
	if (!lightMode) {
		if (keys[SDL_SCANCODE_W]) dy += 0.01f;
		if (keys[SDL_SCANCODE_S]) dy -= 0.01f;
		if (keys[SDL_SCANCODE_D]) dx += 0.01f;
		if (keys[SDL_SCANCODE_A]) dx -= 0.01f;

		if (keys[SDL_SCANCODE_LEFT]) r += 4.0f;
		if (keys[SDL_SCANCODE_RIGHT]) r -= 4.0f;
		if (keys[SDL_SCANCODE_UP]) scalar += 0.1f;
		if (keys[SDL_SCANCODE_DOWN]) scalar -= 0.1f;
	}
	else {
		if (keys[SDL_SCANCODE_W]) dyl += 0.1f;
		if (keys[SDL_SCANCODE_S]) dyl -= 0.1f;
		if (keys[SDL_SCANCODE_D]) dxl += 0.1f;
		if (keys[SDL_SCANCODE_A]) dxl -= 0.1f;

		if (keys[SDL_SCANCODE_LEFT]) lr += 0.1f;
		if (keys[SDL_SCANCODE_RIGHT]) lr -= 0.1f;
		if (keys[SDL_SCANCODE_UP]) lscalar += 0.1f;
		if (keys[SDL_SCANCODE_DOWN]) lscalar -= 0.1f;
	}
	//cout << light0.position[0] << " " <<
	//		light0.position[1] << " " << 
	//		light0.position[2] << " " <<
	//		light0.position[3] << " " << endl;

}

void draw(SDL_Window * window) {
	// clear the screen
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// resets projection matrix at the start before being put in
	glm::mat4 projection(1.0);

	// Light setup
	glm::mat4 lighttransform(1.0);
	glm::vec4 lightpos = glm::vec4(light0.position[0], light0.position[1], light0.position[2], light0.position[3]); // initalize light position

	// Object setup
	projection = glm::perspective(fov, aspect, near, far);
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "projection", glm::value_ptr(projection));

	glm::mat4 modelview(1.0);
	glm::mat4 identity(1.0);

	//Light Object
	mvStack.push(modelview);
	rt3d::setLight(mvpShaderProgram, light0);
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(dxl, dyl, 0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(lscalar, lscalar, lscalar));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));

	lightpos = glm::vec4(dxl, dyl, 0, 1.0f);
	cout << "light pos: " << lightpos[0] << " " <<
		lightpos[1] << " " <<
		lightpos[2] << " " <<
		lightpos[3] << endl;
	rt3d::setLightPos(mvpShaderProgram, glm::value_ptr(lightpos));

	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//Objects
	mvStack.push(modelview);
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(dx, dy, -4.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(scalar, scalar, scalar));
	mvStack.top() = glm::rotate(mvStack.top(), float(r*DEG_TO_RADIAN), glm::vec3(1.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	

	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(dx-2, dy-2, -4));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(scalar / 2, scalar / 2, scalar / 2));
	mvStack.top() = glm::rotate(mvStack.top(), float(r*DEG_TO_RADIAN), glm::vec3(1.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	mvStack.pop();
	mvStack.pop();

#pragma region Old Light Code
	////Light Transformations
	//rt3d::setLight(mvpShaderProgram, light0);
	//lighttransform = glm::translate(lighttransform, glm::vec3(dxl, dyl, 0));
	//lighttransform = glm::scale(lighttransform, glm::vec3(lscalar, lscalar, lscalar));
	//lighttransform = glm::rotate(lighttransform, lr, glm::vec3(0.0f, 1.0f, 0.0f));
	//lightpos = lighttransform * lightpos;
	//GLfloat lightpos2[4] = { lightpos[0],lightpos[1],lightpos[2],lightpos[3] };
	//rt3d::setLightPos(mvpShaderProgram, lightpos2);
#pragma endregion



	SDL_GL_SwapWindow(window); // swap buffers
}


// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[]) {
    SDL_Window * hWindow; // window handle
    SDL_GLContext glContext; // OpenGL context handle
    hWindow = setupRC(glContext); // Create window and render context 

	// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << endl;
		exit (1);
	}
	cout << glGetString(GL_VERSION) << endl;

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running)	{	// the event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(hWindow); // call the draw function
	}

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(hWindow);
    SDL_Quit();
    return 0;
}



