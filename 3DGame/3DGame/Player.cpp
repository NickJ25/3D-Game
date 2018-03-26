#include "Player.h"

vec3 Player::getPosition()
{
	return position;
}

vec3 Player::setPosition(vec3 pos)
{
	return vec3();
}

void Player::init()
{
	meshObject = model.ReadMD2Model(filename);
	md2VertCount = model.getVertDataCount();
}

void Player::update()
{
}

void Player::draw(mat4 matrix, GLuint shaderProgram)
{
	model.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, model.getAnimVerts(), model.getVertDataSize());
	glm::mat4 modelview(1.0);
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, texture);
	rt3d::materialStruct tmpMaterial = material;
	rt3d::setMaterial(shaderProgram, tmpMaterial);

	matrix = glm::translate(matrix, glm::vec3(position));
	//
	matrix = glm::rotate(matrix, float(90 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, float(90.0*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, float(90.0*DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, glm::vec3(1 * 0.05, 1 * 0.05, 1 * 0.05));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(matrix));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);



	//glDepthMask(GL_FALSE);
	//matrix = glm::translate(matrix, glm::vec3(0.2, 0, -2.0f));
	//matrix = glm::scale(matrix, glm::vec3(0.5, 0.5, 0.5));
	//glBindTexture(GL_TEXTURE_2D, texture);
	//rt3d::setMaterial(shaderProgram, material);
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(matrix));
	//rt3d::drawIndexedMesh(meshObject, md2VertCount, GL_TRIANGLES);
	//glDepthMask(GL_TRUE);

	//return matrix;
}
