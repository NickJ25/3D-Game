#pragma once
#include "Entity.h"

class Coin : Entity{
private:
	vec3 position;
	GLfloat spinSpeed = 5;
	GLfloat rotation;
	AABB* collisionBox;
	string filename;

	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};

	GLuint meshObject;
	GLuint modelTexture;
	GLuint md2VertCount = 0;
	md2model model;
	int currentAnim = 0;
public:
	Coin(string filename, vec3 position, rt3d::materialStruct material) : filename(filename), position(position), material(material) {
	}

	virtual ~Coin() {
		delete collisionBox;
	}

	vec3 getPosition();
	void setPosition(vec3 pos);

	AABB* getCollision();

	void init(GLuint texture);
	void update();
	void draw(mat4 matrix, GLuint shaderProgram);
};