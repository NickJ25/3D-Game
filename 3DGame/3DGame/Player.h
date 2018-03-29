#pragma once
#include "Entity.h"


using namespace glm;

class Player : Entity
{
private:
	vec3 position;

	// Basic material
	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};


	// Model and Animation variables 
	GLuint meshObject;
	GLuint modelTexture;
	GLuint md2VertCount = 0;
	md2model model;
	int currentAnim = 0;
	string filename;

	AABB* collisionBox = new AABB(position, 0.5, 0.5, 0.5);


public:
	Player(string filename, vec3 position, rt3d::materialStruct material) : filename(filename), position(position), material(material) {
	}

	virtual ~Player() {
		delete collisionBox;
	}

	vec3 getPosition();
	void setPosition(vec3 pos);

	AABB* getCollision();

	void init(GLuint texture);
	void update();
	void draw(mat4 matrix, GLuint shaderProgram);
};