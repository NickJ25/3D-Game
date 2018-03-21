#pragma once
#include "Entity.h"
#include "md2model.h"


using namespace glm;

class Player
{
private:
	vec2 position;

	// Basic material
	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};

	GLuint texture;

	// Model and Animation variables
	GLuint meshObject;
	GLuint md2VertCount = 0;
	md2model model;
	int currentAnim = 0;
public:
	Player(const char *filename, vec3 position, rt3d::materialStruct material, GLuint texture) : position(position), material(material), texture(texture) {
		meshObject = model.ReadMD2Model("tris.MD2");
		md2VertCount = model.getVertDataCount();
	}

	virtual ~Player() {}

	vec3 getPosition();
	vec3 setPosition(vec3 pos);

	void update();
	mat4 draw(mat4 matrix, GLuint shaderProgram);
};