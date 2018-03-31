#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <string>
#include "rt3dObjLoader.h"
#include "rt3d.h"

using namespace glm;
using namespace std;

class Terrain {
private:
	vec3 position;
	string filename;

	// Mesh properties
	GLuint meshObject;
	GLuint modelTexture;
	GLuint objIndexCount;

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;

	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};

public:
	Terrain(string filename, vec3 position, rt3d::materialStruct& material) : filename(filename), position(position), material(material) {

	}
	virtual ~Terrain() {}

	void init(GLuint texture);
	void update();
	void draw(mat4 matrix, GLuint shaderProgram);
};