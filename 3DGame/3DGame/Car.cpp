#include "Car.h"

vec3 Car::getPosition()
{
	return position;
}

void Car::setPosition(vec3 pos)
{
	position = pos;
}

void Car::setVelocity(vec3 vel)
{
	velocity = vel;
}

AABB * Car::getCollision()
{
	return collisionBox;
}

void Car::init(GLuint texture)
{
	meshObject = model.ReadMD2Model(filename.c_str()); // Cast String to Char
	md2VertCount = model.getVertDataCount();
	modelTexture = texture;

	collisionBox = new AABB(position, 0.6, 0.6, 0.6);
}

void Car::update()
{
	position += velocity;
}

void Car::draw(mat4 matrix, GLuint shaderProgram)
{
	glm::mat4 modelview(1.0);
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, modelTexture);
	rt3d::materialStruct tmpMaterial = material;
	rt3d::setMaterial(shaderProgram, tmpMaterial);

	matrix = glm::translate(matrix, glm::vec3(position));
	matrix = glm::rotate(matrix, float(90.0*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, float(90.0*DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, glm::vec3(1 * 0.0005, 1 * 0.0005, 1 * 0.0005));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(matrix));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);
}
