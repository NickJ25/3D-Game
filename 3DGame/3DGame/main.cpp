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
#include <vector>
#include "rt3d.h"
#include "md2model.h"
#include "Camera.h"
#include "Skybox.h"
#include "Player.h"
#include "Coin.h"
#include "Terrain.h"
#include "AABB.h" // temp
#include "Entity.h"
#include "Car.h"


#define DEG_TO_RADIAN 0.017453293

using namespace std;

// Globals
// Data would normally be read from files

#pragma region Old Cube
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

GLfloat cubeTexCoords[] = { 0.0f, 0.0f,
							0.0f, 1.0f,
							1.0f, 1.0f,
							1.0f, 0.0f,
							1.0f, 1.0f,
							1.0f, 0.0f,
							0.0f, 0.0f,
							0.0f, 1.0f	};

GLuint cubeIndexCount = 36;
GLuint cubeIndices[] = { 0,1,2, 0,2,3, // back  
						1,0,5, 0,4,5, // left					
						6,3,2, 3,6,7, // right
						1,5,6, 1,6,2, // top
						0,3,4, 3,7,4, // bottom
						6,5,4, 7,6,4 }; // front
#pragma endregion


rt3d::lightStruct light0 = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
	{ 0.7f, 0.7f, 0.7f, 1.0f }, // diffuse
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
	{ 0.0f, 0.0f, 1.0f, 1.0f }  // position
};

rt3d::lightStruct light1 = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
{ 0.7f, 0.7f, 0.7f, 1.0f }, // diffuse
{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
{ 10.0f, 0.0f, 1.5f, 1.0f }  // position
};



#pragma region Materials
rt3d::materialStruct material0 = {
	{0.4f, 0.2f, 0.2f, 1.0f}, // ambient
	{0.8f, 0.5f, 0.5f, 1.0f}, // diffuse
	{1.0f, 0.8f, 0.8f, 1.0f}, // specular
	2.0f // shininess
};

rt3d::materialStruct material1 = {
	{0.4f, 0.2f, 0.2f, 0.3f}, // ambient
	{0.8f, 0.5f, 0.5f, 0.3f}, // diffuse
	{1.0f, 0.8f, 0.8f, 0.3f}, // specular
	2.0f // shininess
};

rt3d::materialStruct material2 = { // Metal Material
	{ 0.9f, 0.9f, 0.9f, 1.0f }, // ambient
	{ 0.5f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
4.0f // shininess
};
#pragma endregion

// Screen Size
GLfloat screenWidth = 1280, screenHeight = 720;

// glm perspective settings
GLfloat fov = float(60.0f*DEG_TO_RADIAN), aspect = ((float)screenWidth / (float)screenHeight), near = 1.0f, far = 50.0f;

// Stack for storing modelview matrix when dealing with multiple matrixes
stack<glm::mat4> mvStack;
vector<Entity*> gameEntities;

GLuint meshObjects[6]; // Array with X amount of Unique Objects
GLuint textures[5]; // Array with X amount of Unique Textures
GLuint skybox[5]; // Array with X amount of Unique Skyboxes



Skybox skyBox;

GLuint mvpShaderProgram;
GLuint skyboxProgram;
bool lightMode = false;

// Object Settings
GLfloat dx = 0.0f, dy = 0.0f, dz = 0.0f, r = 0.0f, scalar = 1.0f;

// Light Settings
GLfloat dxl = 0.0f, dyl = 0.0f, lscalar = 1.0f, lr = 0.0f;

// Player Settings
glm::vec3 playerPos(0.0f, 0.5f, 1.0f);
Player player("tris.MD2", glm::vec3(0.0f,0.5f,1.0f), material0);
glm::vec3 carPos(0.0f, -1.0f, -10.0f);
GLfloat tempVel = 0;//30;



// Terrain Settings / test
Terrain terrain("Road.obj", vec3(0,0,0), material0); //RoadTerrain.obj

AABB testAABB(playerPos, 0.5, 0.5, 0.5);

// Camera Settings

Camera camera(player.getPosition(), vec3(0.0f,1.0f,0.0f), 270.0f);

glm::vec3 eye(0.0f, 1.0f, 4.0f);
glm::vec3 at(playerPos);
glm::vec3 up(0.0f, 1.0f, 0.0f);
GLfloat cameraDistance = 5.0f;

//string stringtest = "coin.md2";

Coin* coinTest = new Coin("coin.md2", glm::vec3(playerPos.x+5, 0, 0), material2); // delete these
Car* carTest = new Car("policecar.md2", vec3(0,-1,0), vec3(-0.1,0,0), material0);


//MD2 Stuff
GLuint md2VertCount = 0, md2VertCount2 = 0;
md2model tmpModel, tmpModel2;
int currentAnim = 0;

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

	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering

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

//A simple texture loading function (lots of room for improvement
GLuint loadBitmap(char *fname)
{
	GLuint texID;
	glGenTextures(1, &texID); // generate texture ID

	//load file - using core SDL library
	SDL_Surface *tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	if (!tmpSurface)
	{
		std::cout << "Error loading bitmap: " << fname << endl;
	}
	else {
		std::cout << fname << " loaded" << endl;
	}

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_PixelFormat *format = tmpSurface->format;
	GLuint externalFormat, internalFormat;
	if (format->Amask) {
		internalFormat = GL_RGBA;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
	}
	else {
		internalFormat = GL_RGB;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
	return texID;	// return value of texture ID

}

void init(void) {
	// For this simple example we'll be using the most basic of shader programs
	skyboxProgram = rt3d::initShaders("textured.vert", "textured.frag");
	mvpShaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	//rt3d::setLight(mvpShaderProgram, light0);
	//rt3d::setLight(mvpShaderProgram, light1);
	rt3d::setMaterial(mvpShaderProgram, material0);

	gameEntities.push_back(coinTest);
	gameEntities.push_back(carTest);

	textures[0] = loadBitmap("cobble.bmp");
	textures[1] = loadBitmap("studdedmetal.bmp");
	textures[2] = loadBitmap("fabric.bmp");
	textures[3] = loadBitmap("hobgoblin2.bmp");
	textures[4] = loadBitmap("car.bmp");
	textures[5] = loadBitmap("coin.bmp");

	meshObjects[3] = tmpModel.ReadMD2Model("tris.MD2");
	meshObjects[4] = tmpModel2.ReadMD2Model("policecar.md2");
	md2VertCount = tmpModel.getVertDataCount();
	md2VertCount2 = tmpModel2.getVertDataCount();


	player.init(textures[3]);

	skybox[0] = loadBitmap("violentdays_bk.bmp");
	skybox[1] = loadBitmap("violentdays_rt.bmp");
	skybox[2] = loadBitmap("violentdays_ft.bmp");
    skybox[3] = loadBitmap("violentdays_lf.bmp");
	skybox[4] = loadBitmap("violentdays_up.bmp");

	
	
	coinTest->init(textures[5]);
	carTest->init(textures[4]);
	terrain.init(textures[0]); // without terrain, skybox messes up?
	skyBox.init(skybox[0]);
	

 
	// Going to create our mesh objects here
	meshObjects[0] = rt3d::createMesh(cubeVertCount, cubeVerts, nullptr, cubeVerts, cubeTexCoords, cubeIndexCount, cubeIndices);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

//glm::vec3 moveVert(glm::vec3 cam, GLfloat angle, GLfloat d) {
//	return glm::vec3(cam.x + d * std::sin(angle*DEG_TO_RADIAN), 
//		cam.y, cam.z - d * std::cos(angle*DEG_TO_RADIAN));
//}
//
//glm::vec3 moveHori(glm::vec3 pos, GLfloat angle, GLfloat d) {
//	return glm::vec3(pos.x + d * std::cos(angle*DEG_TO_RADIAN), 
//		pos.y, pos.z + d * std::sin(angle*DEG_TO_RADIAN));
//}

bool TestAABBAABB(AABB* a, AABB b) // Checks all axis to see if there was an intersection (Base code taken from Real Time Collision Detection by 
{
	if (abs(a->getPosition().x - b.getPosition().x) > (a->getSize().x + b.getSize().x)) return false;
	if (abs(a->getPosition().y - b.getPosition().y) > (a->getSize().y + b.getSize().y)) return false;
	if (abs(a->getPosition().z - b.getPosition().z) > (a->getSize().z + b.getSize().z)) return false;
	return true;
}

void update(void) {

	// Keyboard inputs

	//currentAnim = 0; // set player animation to idle
	player.setCurrentAnim(0);
	player.setRotation(camera.getRotation());

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_L]) lightMode = !lightMode;
	if (!lightMode) {
		if (keys[SDL_SCANCODE_W]) {
			//playerPos = moveVert(playerPos, r, 0.1f);
			player.moveVert(0.1f);
			//currentAnim = 1; // change player animation to walking
			player.setCurrentAnim(1);
		}
		if (keys[SDL_SCANCODE_S]) {
			//playerPos = moveVert(playerPos, r, -0.1f);
			player.moveVert(-0.1f);
			//currentAnim = 1; // change player animation to walking
			player.setCurrentAnim(1);
		}
		if (keys[SDL_SCANCODE_D]) {
			//r += 2.0f; // increase rotation
			camera.setRotation(camera.getRotation() + 2.0f);
		}
		if (keys[SDL_SCANCODE_A]) {
			//r -= 2.0f; // decrease rotation
			camera.setRotation(camera.getRotation() - 2.0f);
			
		}
		player.update();
		//if (keys[SDL_SCANCODE_R]) eye.y += 0.1;
		//if (keys[SDL_SCANCODE_F]) eye.y -= 0.1f;
		if (keys[SDL_SCANCODE_COMMA]) r -= 1.0f;
		if (keys[SDL_SCANCODE_PERIOD]) r += 1.0f;

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

	for (vector<Entity*>::iterator it = gameEntities.begin(); it < gameEntities.end(); it++)
	{
		(*it)->update();
	}
	//eye = glm::vec3(playerPos.x, playerPos.y + 2, playerPos.z + 3);
	//at = playerPos;
	tempVel;// -= 0.1;
	carPos = glm::vec3(tempVel, carPos.y, carPos.z);

	// Collision Testing

	testAABB.setPosition(playerPos);
	if (TestAABBAABB(coinTest->getCollision(), testAABB) == true) {
		cout << "COLLISION BOI" << endl;
		delete coinTest;
	}

	//coinTest.update();

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

#pragma region Camera

	mvStack.push(modelview);
	mvStack.top() = camera.draw(player.getPosition());

	//// rotation amount (r = rotation amount), - 270 so player starts facing forward.
	//GLfloat r_cam = r - 270;

	//// calculate unit vector [x, y] on unit circle using cos, sin of angle
	//vec3 offset(cameraDistance*cos(r_cam*DEG_TO_RADIAN), cameraDistance *0.5, cameraDistance* sin(r_cam*DEG_TO_RADIAN));

	//at = playerPos; // Look at the player's position

	//// eye (Camera's position) + the rotation
	//eye = playerPos + offset;
	//mvStack.top() = glm::lookAt(eye, at, up);

#pragma endregion

	skyBox.draw(mvStack.top(), projection, mvpShaderProgram);


	//Light Object
	mvStack.push(mvStack.top());
	rt3d::setLight(mvpShaderProgram, light0);
	
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(dxl, dyl, 0));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(lscalar, lscalar, lscalar));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));

	lightpos = glm::vec4(dxl, dyl, 0, 1.0f);
	glm::vec4 tmp = mvStack.top()*lightpos;
	rt3d::setLightPos(mvpShaderProgram, glm::value_ptr(tmp));

	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	mvStack.pop();

	rt3d::setLight(mvpShaderProgram, light1);

	player.draw(mvStack.top(), mvpShaderProgram);

	//Player
	//mvStack.push(mvStack.top());
	//mvStack.top() = glm::translate(mvStack.top(), glm::vec3(playerPos));
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.0f, 3.0f, 1.0f));
	//mvStack.top() = glm::rotate(mvStack.top(), float(-r*DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	//glBindTexture(GL_TEXTURE_2D, textures[1]);
	//rt3d::setMaterial(mvpShaderProgram, material0);
	//rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	//mvStack.pop();

#pragma region MD2 Model Draw

	// Animate the md2 model, and update the mesh with new vertex data
	tmpModel.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObjects[3], RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	// draw the hobgoblin
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	rt3d::materialStruct tmpMaterial = material0;
	rt3d::setMaterial(mvpShaderProgram, tmpMaterial);
	mvStack.push(mvStack.top());
	
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(playerPos));
	//
	mvStack.top() = glm::rotate(mvStack.top(), float(-r*DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0*DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1 * 0.05, 1 * 0.05, 1 * 0.05));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawMesh(meshObjects[3], md2VertCount, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);

	// draw terrain
	terrain.draw(mvStack.top(), mvpShaderProgram);

	// draw car
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	//rt3d::materialStruct tmpMaterial = material0;
	rt3d::setMaterial(mvpShaderProgram, tmpMaterial);
	mvStack.push(mvStack.top());

	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(carPos));
	//mvStack.top() = glm::rotate(mvStack.top(), float(-r * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0*DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1 * 0.1, 1 * 0.1, 1 * 0.1));
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawMesh(meshObjects[4], md2VertCount2, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);
#pragma endregion

	for (vector<Entity*>::iterator it = gameEntities.begin(); it < gameEntities.end(); it++)
	{
		(*it)->draw(mvStack.top(), mvpShaderProgram);
	}

	//coinTest.draw(mvStack.top(), mvpShaderProgram);

	//Objects
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -10.0f));
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	rt3d::setMaterial(mvpShaderProgram, material0);
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	mvStack.pop();

	//mvStack.push(mvStack.top()); // Old Terrain
	//mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0, -1, -10.0f));
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0f, 0.1f, 20.0f));
	//glBindTexture(GL_TEXTURE_2D, textures[0]);
	//rt3d::setMaterial(mvpShaderProgram, material0);
	//rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	//mvStack.pop();

	glDepthMask(GL_FALSE);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.2, 0, -2.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5, 0.5, 0.5));
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	rt3d::setMaterial(mvpShaderProgram, material1);
	rt3d::setUniformMatrix4fv(mvpShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], cubeIndexCount, GL_TRIANGLES);
	glDepthMask(GL_TRUE);
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



