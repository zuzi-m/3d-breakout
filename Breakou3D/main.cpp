#include <iostream>

#include "glew.h"
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>

#include "Game.h"

using namespace std;

void draw() {
	Game::instance()->draw();
}

void reshape(int w, int h) {
	Game::instance()->resizeCallback(w, h);
}

void keyPress(unsigned char key, int a, int b) {
	Game::instance()->keyboardCallback(key, a, b);
}

void specialPress(int key, int a, int b) {
	Game::instance()->specialFuncCallback(key, a, b);
}

void mouseMove(int x, int y) {
	Game::instance()->activeMouseCallback(x, y);
}

void passiveMouseMove(int x, int y) {
	Game::instance()->passiveMouseCallback(x, y);
}

void mouseWheel(int wheel, int direction, int x, int y) {
	Game::instance()->mouseWheelCallback(wheel, direction, x, y);
}

void framerateTrigger(int value) {
	glutTimerFunc(20, framerateTrigger, 1);
	Game::instance()->tick();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// basic GLUT initialization
	glutInit(&argc, argv);
	// random seed based on time
	srand(static_cast<unsigned int>(time(NULL)));

	// initialize GLUT window
	int window_w = 1024;
	int window_h = 768;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Breakou3D Game");

	// init the glew thing
	glewInit();

	// hook the callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyPress);
	glutSpecialFunc(specialPress);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(passiveMouseMove);
	glutMouseWheelFunc(mouseWheel);

	// load the game - will set up everything in game instance
	bool loadSuccess = Game::instance()->load(window_w, window_h);
	if (loadSuccess) {
		cout << "Game loaded successfully." << endl;
	}
	else {
		cout << "Game failed to load. Exiting." << endl;
		return -1;
	}

	// set up the framerate
	glutTimerFunc(20, framerateTrigger, 1);

	cout << "Vendor   : " << (const char *)glGetString(GL_VENDOR) << endl;
	cout << "Renderer : " << (const char *)glGetString(GL_RENDERER) << endl;
	cout << "Version  : " << (const char *)glGetString(GL_VERSION) << endl;
	cout << endl;

	// start the GLUT main loo
	glutMainLoop();

	return 0;
}