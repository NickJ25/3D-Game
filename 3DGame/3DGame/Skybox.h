#pragma once
#include "rt3d.h"
#include <stack>

using namespace std;

class Skybox {
private:

	stack<glm::mat4> mvStack;
	GLuint skyboxProgram;
	GLuint skyboxTex[4];

	GLfloat cubeVertCount = 8;
	GLfloat cubeVerts[24] = { -0.5, -0.5f, -0.5f,
					-0.5, 0.5f, -0.5f,
		0.5, 0.5f, -0.5f,
		0.5, -0.5f, -0.5f,
		-0.5, -0.5f, 0.5f,
		-0.5, 0.5f, 0.5f,
		0.5, 0.5f, 0.5f,
		0.5, -0.5f, 0.5f };

	GLfloat cubeColours[24] = { 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f };

	GLfloat cubeTexCoords[16] = { 0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f };

	GLuint cubeIndexCount = 36;
	GLuint cubeIndices[36] = { 0,1,2, 0,2,3, // back  
							1,0,5, 0,4,5, // left					
							6,3,2, 3,6,7, // right
							1,5,6, 1,6,2, // top
							0,3,4, 3,7,4, // bottom
							6,5,4, 7,6,4 }; // front


	GLuint meshObjects = 5; //= rt3d::createMesh(cubeVertCount, cubeVerts, nullptr, cubeVerts, cubeTexCoords, cubeIndexCount, cubeIndices);


public:
	Skybox(){



	}

	void init(GLuint skyboxT[4]);
	void update();
	void draw(glm::mat4 mvp, glm::mat4 projection, GLuint mvpProgram);
};