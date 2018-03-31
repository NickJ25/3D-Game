#pragma once
#include "Entity.h"

class Coin : public Entity{
private:
	vec3 position;
	GLfloat spinSpeed = 5;
	GLfloat rotation = 0;
	AABB* collisionBox;
	std::string filename;

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
	Coin(string& fileName, vec3& position, rt3d::materialStruct& material) : filename(fileName), position(position), material(material) {
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