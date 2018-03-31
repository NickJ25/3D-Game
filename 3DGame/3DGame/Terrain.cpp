#include "Terrain.h"

void Terrain::init(GLuint texture)
{
	modelTexture = texture;
	rt3d::loadObj(filename.c_str(), verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	objIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	cout << "Terrain: " << meshObject << endl;
}

void Terrain::draw(mat4 matrix, GLuint shaderProgram)
{
	glBindTexture(GL_TEXTURE_2D, modelTexture);
	matrix = glm::translate(matrix, glm::vec3(0.0f, -0.1f, 0.0f));
	matrix = glm::scale(matrix, glm::vec3(1.0f, 1.0f, 1.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(matrix));
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, objIndexCount, GL_TRIANGLES); //GL_TRIANGLES
}
