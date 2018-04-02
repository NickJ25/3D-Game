#pragma once

#include <glm\glm.hpp>
#include <GL/glew.h>
#include <string>
#include "rt3d.h"

class Light {
private:
	rt3d::lightStruct light0 = {
	{ 0.2f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.7f, 0.7f, 0.7f, 1.0f }, // diffuse
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
	{ 0.0f, 0.0f, 1.0f, 1.0f }  // position
	};

public:
};