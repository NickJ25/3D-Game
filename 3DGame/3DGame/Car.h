#pragma once
#include "Entity.h"
#include "md2model.h"

class Car : public Entity {
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

	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};

public:
	Car(string filename, vec3& position, vec3& initialVelocity, rt3d::materialStruct& material) : filename(filename), position(position), velocity(initialVelocity),material(material) {

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
