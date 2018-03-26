#include "Skybox.h"

void Skybox::update()
{

}

void Skybox::draw(glm::mat4 mvp,glm::mat4 projection, GLuint mvpProgram)
{

	glUseProgram(skyboxProgram);
	rt3d::setUniformMatrix4fv(skyboxProgram, "projection", glm::value_ptr(projection));

	glDepthMask(GL_FALSE);
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvp);
	mvStack.push(glm::mat4(mvRotOnlyMat3));

	GLfloat indexProgram = 6;

	// front
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTex[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, indexProgram, GL_TRIANGLES);
	mvStack.pop();

	// back
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTex[1]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, indexProgram, GL_TRIANGLES);
	mvStack.pop();

	// left
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTex[2]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, indexProgram, GL_TRIANGLES);
	mvStack.pop();

	// right
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTex[3]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, indexProgram, GL_TRIANGLES);
	mvStack.pop();

	// top
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skyboxTex[4]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 2.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects, indexProgram, GL_TRIANGLES);
	mvStack.pop();

	mvStack.pop();
	glDepthMask(GL_TRUE);
	glUseProgram(mvpProgram);
}
