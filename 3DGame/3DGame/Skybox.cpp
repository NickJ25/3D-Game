#include "Skybox.h"

void Skybox::init(GLuint skyboxT)
{
	skyboxProgram = rt3d::initShaders("textured.vert", "textured.frag");
	skyboxTex = skyboxT;

	GLuint meshObjects = rt3d::createMesh(cubeVertCount, cubeVerts, nullptr, cubeVerts, cubeTexCoords, cubeIndexCount, cubeIndices);
}

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

	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(4.0f, 4.0f, 4.0f)); // you may need to scale it beyond the near clipping plane
	glm::mat4 view = mvStack.top();
	mvStack.top() = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
		//rt3d::setMaterial(shaderProgram, materialsky);
	glm::vec4 vec4_skyboxlight(1.0);
		//rt3d::setLightPos(shaderProgram, glm::value_ptr(vec4_skyboxlight));
	rt3d::drawIndexedMesh(meshObjects, 36, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);
	mvStack.pop();

	glDepthMask(GL_TRUE);
	glUseProgram(mvpProgram);
}
