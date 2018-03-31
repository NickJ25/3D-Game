#pragma once
#include "rt3d.h"
#include <stack>

using namespace std;
using namespace glm;

class Skybox {
private:

	stack<glm::mat4> mvStack;
	GLuint skyboxProgram;
	GLuint skyboxTex;

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

	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};


	GLuint meshObjects;


public:
	Skybox(){



	}

	void init(GLuint skyboxT);
	void update();
	void draw(glm::mat4 mvp, glm::mat4 projection, GLuint mvpProgram);
};