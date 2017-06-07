#include <iostream>

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
	glutTimerFunc(100, framerateTrigger, 1);
	Game::instance()->tick();
}

int main(int argc, char** argv) {
	// basic GLUT initialization
	glutInit(&argc, argv);
	// random seed based on time
	srand(static_cast<unsigned int>(time(NULL)));

	// initialize GLUT window
	int window_w = 800;
	int window_h = 600;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Breakou3D Game");

	// hook the callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyPress);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(passiveMouseMove);
	glutMouseWheelFunc(mouseWheel);

	// start the game - will set up everything in game instance
	Game::instance()->start(window_w, window_h);

	// set up the framerate
	glutTimerFunc(20, framerateTrigger, 1);

	// start the GLUT main loo
	glutMainLoop();

	return 0;
}