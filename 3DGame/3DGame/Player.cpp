#include "Player.h"

vec3 Player::getPosition()
{
	return vec3();
}

vec3 Player::setPosition(vec3 pos)
{
	return vec3();
}

void Player::update()
{
}

mat4 Player::draw(mat4 matrix, GLuint shaderProgram)
{
	model.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, model.getAnimVerts(), model.getVertDataSize());

	glDepthMask(GL_FALSE);
	matrix = glm::translate(matrix, glm::vec3(0.2, 0, -2.0f));
	matrix = glm::scale(matrix, glm::vec3(0.5, 0.5, 0.5));
	glBindTexture(GL_TEXTURE_2D, texture);
	rt3d::setMaterial(shaderProgram, material);
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(matrix));
	rt3d::drawIndexedMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glDepthMask(GL_TRUE);

	return matrix;
}
