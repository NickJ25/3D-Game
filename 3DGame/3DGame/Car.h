#pragma once
#include "Entity.h"
#include "md2model.h"

class Car : Entity {
private:
	vec3 position;
	vec3 velocity;
	AABB* collisionBox;
	string filename;

	// Mesh data
	GLuint meshObject;
	GLuint modelTexture;
	GLuint md2VertCount = 0;
	md2model model;

public:
	Car() {

	}
	virtual ~Car() {
		delete collisionBox;
	}

	vec3 getPosition();
	void setPosition(vec3 pos);

	void setVelocity(vec3 vel);

	AABB* getCollision();

	void init(GLuint texture);
	void update();
	void draw(mat4 matrix, GLuint shaderProgram);
};
