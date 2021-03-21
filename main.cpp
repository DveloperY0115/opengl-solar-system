// C++ standarad libraries
#include <iostream>

// OpenGL libraries
#include <GL/glew.h>
#include <GL/glut.h>

#include "ShaderState.h"

// Utilities
#include "glsupport.h"
#include "ppm.h"

void initGLUT(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL Solar System");
	glutDisplayFunc(display);
}

void initGLState() {
	glClearColor(128. / 255, 200. / 255, 1, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return;
}

int main(int argc, char** argv) {
	initGLUT(argc, argv);
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("GLEW init failed: %s!n", glewGetErrorString(err));
		exit(1);
	}
	else {
		printf("GLEW init success!\n");
	}

	initGLState();
	glutMainLoop();

	return 0;
}