#include "stdafx.h"


extern struct shader_program sp;

superchunk::superchunk() {
	memset(c, 0, sizeof c);
}

superchunk::~superchunk() {
	for (int x = 0; x < SCX; x++)
		for (int y = 0; y < SCX; y++)
			for (int z = 0; z < SCX; z++)
				delete c[x][y][z];
}

GLubyte superchunk::get(int x, int y, int z) const {
	int cx = x / CX;
	int cy = y / CY;
	int cz = z / CZ;

	x %= CX;
	y %= CY;
	z %= CZ;

	if (!c[cx][cy][cz])
		return 0;
	else
		return c[cx][cy][cz]->get(x, y, z);
}

void superchunk::set(int x, int y, int z, GLubyte type) {
	int cx = x / CX;
	int cy = y / CY;
	int cz = z / CZ;

	x %= CX;
	y %= CY;
	z %= CZ;

	if (!c[cx][cy][cz])
		c[cx][cy][cz] = new chunk();

	c[cx][cy][cz]->set(x, y, z, type);
}

void superchunk::render() {

	struct vec3 fudgedCamera;
	fudgedCamera.x = globals.cameraPosition.x;
	fudgedCamera.y = globals.cameraPosition.y;
	float len = sqrtf((globals.lookAt.x - globals.cameraPosition.x)*(globals.lookAt.x - globals.cameraPosition.x) +
		(globals.lookAt.y - globals.cameraPosition.y)*(globals.lookAt.y - globals.cameraPosition.y));
	fudgedCamera.x -= CX * (globals.lookAt.x - globals.cameraPosition.x) / len;
	fudgedCamera.y -= CY * (globals.lookAt.y - globals.cameraPosition.y) / len;

	if (glewGetExtension("GL_NV_vertex_buffer_unified_memory"))
	{
		glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void *)12);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	for (int x = 0; x < SCX; x++)
		for (int y = 0; y < SCY; y++)
			for (int z = 0; z < SCZ; z++)
				if (c[x][y][z]
					&& (x* CX - globals.cameraPosition.x)* (x* CX - globals.cameraPosition.x)
					  + (y*CY - globals.cameraPosition.y)* (y*CY - globals.cameraPosition.y) < 300*300
					&& ((x * CX - fudgedCamera.x)* (globals.lookAt.x - fudgedCamera.x) +
					(y * CY - fudgedCamera.y)* (globals.lookAt.y - fudgedCamera.y)) > 0) {
					//glm::translate(glm::mat4(1), glm::vec3(x * CX, y * CY, z * CZ));
					//lPushMatrix();
					//glLoadIdentity();
					glVertexAttrib3f(2, x * CX, y * CY, z * CZ);
					//glTranslatef(x * CX, y * CY, z * CZ);
					c[x][y][z]->render();
					//glPopMatrix();
				}

	if (glewGetExtension("GL_NV_vertex_buffer_unified_memory"))
	{
		glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
	}
}

void superchunk::update() {
	for (int x = 0; x < SCX; x++)
		for (int y = 0; y < SCY; y++)
			for (int z = 0; z < SCZ; z++)
				if (c[x][y][z])
					c[x][y][z]->update();
}