#include "Game.h"

#include <gl\freeglut.h>

#include <iostream>
#include <ctime>

#include "Ray.h"

using namespace MathLib;

Game::Game()
{
	isReady = false;
}


Game::~Game()
{
	if (isReady) {
		destroyLevel();
	}
}

void Game::draw()
{
	if (!isReady) {
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	// set camera first
	camera->setCamera();

	// initialize lights
	for (auto it = lights.begin(); it != lights.end(); it++) {
		(*it)->bind();
	}

	// draw all
	ground->draw();
	ball->draw();
	for (auto it = rings.begin(); it != rings.end(); it++) {
		(*it)->draw();
	}
	for (auto it = bats.begin(); it != bats.end(); it++) {
		(*it)->draw();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void Game::keyboardCallback(unsigned char key, int a, int b)
{
	if (!isReady) {
		return;
	}
	BrickRing* br;
	switch (key) {
	case 'w':
		camera->moveCamera(0.0, 15.0);
		break;
	case 'a':
		camera->moveCamera(-15.0, 0.0);
		break;
	case 's':
		camera->moveCamera(0.0, -15.0);
		break;
	case 'd':
		camera->moveCamera(15.0, 0.0);
		break;
	case '+':
		camera->zoom(-1.0);
		break;
	case '-':
		camera->zoom(1.0);
		break;
	case 'e':
		br = *(rings.begin());
		br->removeRandom();
		break;
	case 'r':
		destroyLevel();
		initLevel();
		break;
	case 27:
		std::exit(0);
		break;
	}
}

void Game::activeMouseCallback(int newX, int newY)
{
	if (!isReady) {
		return;
	}
	if (prevX != -1 && prevY != -1) {
		camera->moveCamera((prevX - newX) / 2.0, (newY - prevY) / 2.0);
	}
	prevX = newX;
	prevY = newY;
}

void Game::passiveMouseCallback(int newX, int newY)
{
	if (!isReady) {
		return;
	}
	if (prevX != -1 && prevY != -1) {
		double amount = prevX - newX;
		Matrix batRotation = Matrix::rotation(1, amount * 2 * PI / 360.0);

		for (auto it = bats.begin(); it != bats.end(); it++) {
			(*it)->trans(batRotation);
		}
	}
	prevX = newX;
	prevY = newY;
}

void Game::mouseWheelCallback(int wheel, int direction, int x, int y)
{
	if (!isReady) {
		return;
	}
	camera->zoom(-direction * 2.0);
}

void Game::resizeCallback(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(40.0, ((double)WINDOW_WIDTH) / ((double)WINDOW_HEIGHT), 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void Game::tick()
{
	if (!isReady) {
		return;
	}
	long currT = clock();
	long dt = currT - prevT;
	prevT = currT;

	ball->move(dt);

	// bricks falling down
	for (auto it = rings.begin(); it != rings.end(); it++) {
		BrickRing* br = *it;
		br->tick(dt, ball);
	}

	// get ball position
	Point pos = ball->getPosition();
	double ballR = sqrt((pos[0] * pos[0]) + (pos[2] * pos[2]));
	double ballFi = atan2(pos[2], pos[0]);
	std::cout << "ballR: " << ballR;
	if (ballR > level_radius) {
		loseLife();
	} else if(ballR > (level_radius - OUTER_PADDING)) {
		// check for bat-ball collision only if the ball is near bats
		Brick* bat = *(bats.begin());
		double ballDist = bat->getDistance(ball);
		std::cout << " bat " << bat << " = " << ballDist;
	}
	std::cout << std::endl;

	// trigger rendering
	glutPostRedisplay();
}

void Game::start(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	initRendering();
	initLevel();
}

void Game::initLevel()
{
	// DEBUG
	Quad kvad(Point(-0.5,-0.5,0.5), Point(0.5, -0.5, 0.5), Point(0.5, 0.5, 0.5), Point(-0.5, 0.5, 0.5));
	//Ray rej(Point(1.0,0.0,0.0), Vector(0.0,0.0,0.5));
	Ball bal(1.0);
	bal.setPosition(Point(0.0, 0.0, 0.0));
	bal.setVelocity(Vector(0.0, 0.0, 0.5));
	std::cout << "dist = " << kvad.getDistance(&bal) << std::endl;

	level_radius = (rand() % 36) + 25;
	int brick_rings = rand() % 3 + 1;
	int brick_count = 12;
	int brick_levels = 5;

	OUTER_PADDING = level_radius * 0.3;
	INNER_PADDING = level_radius * 0.06;

	// set up essential game objects
	ground = new Ground(level_radius + INNER_PADDING);
	camera = new Camera(level_radius * 2.0);
	ball = new Ball(BALL_RADIUS);
	
	// set up bricks
	int hue = rand() % 360;
	int hueStep = rand() % 60 + 60;
	for (int ring = 0; ring < brick_rings; ring++) {
		// get radius - the space between padding divided to "brick_layers" parts
		double L = level_radius - OUTER_PADDING - INNER_PADDING;
		double step = L / brick_rings;
		double ring_radius = INNER_PADDING + (step * ring) + (step / 2.0);

		BrickRing* br = new BrickRing(brick_count, brick_levels, ring_radius, BRICK_THICKNESS, BRICK_HEIGHT, hue);
		rings.push_back(br);
		// each ring has one less level and 3 more bricks
		brick_levels -= 1;
		brick_count += 3;
		// shift the color of each ring
		if (ring == 0) {
			hue += 180;
			hue -= hueStep / 2;
		} else {
			hue += hueStep;
		}
	}
	// set up bats
	double bats_radius = level_radius - (OUTER_PADDING * 0.5);
	for (int i = 0; i < 3; i++) {
		Brick* bat = Brick::unit(brick_count, bats_radius, BRICK_THICKNESS*1.5, BRICK_HEIGHT*2.0);
		bat->color = Color(1.0);
		if (i == 0) {
			bat->color = Color(0.5);
		}
		bat->trans(Matrix::rotation(1, i * 2.0 * PI / 3.0));
		bats.push_back(bat);
	}

	// set up lights
	Light* l = new Light(GL_LIGHT0);
	l->light_type = LIGHT_SPOT;
	l->position = Point(0.0, level_radius, 0.0);
	l->direction = Vector(0.0, -1.0, 0.0);
	l->spot_cutoff = 45;
	l->spot_exponent = 1.0;
	lights.push_back(l);

	// reset ball to give it right position and some velocity
	resetBall();

	// reset some control variables
	prevX = -1;
	prevY = -1;
	prevT = clock();
	isReady = true;
}

void Game::destroyLevel()
{
	isReady = false;
	// destroy basic objects
	delete(ground);
	delete(camera);
	delete(ball);
	// destroy rings
	for (auto it = rings.begin(); it != rings.end(); it++) {
		delete(*it);
	}
	rings.clear();
	// destroy bats
	for (auto it = bats.begin(); it != bats.end(); it++) {
		delete(*it);
	}
	bats.clear();
	// destroy lights
	for (auto it = lights.begin(); it != lights.end(); it++) {
		delete(*it);
	}
	lights.clear();
}

void Game::initRendering()
{
	// set default clear color & depth
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	// smooth shading model
	glShadeModel(GL_SMOOTH);
	// init lighting, add default ambient light
	glEnable(GL_LIGHTING);
	float vals[3]{ 0.05f, 0.05f, 0.05f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vals);
	// init depth testing
	glEnable(GL_DEPTH_TEST);
	// set up proper backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// enable textures
	glEnable(GL_TEXTURE_2D);

	// setup the viewport
	resizeCallback(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::loseLife()
{
	resetBall();
}

void Game::resetBall()
{
	// move ball to some reasonable position and give it some V

	// position the ball to initial position
	ball->setPosition(Point(level_radius - (OUTER_PADDING / 2.0) - 2 * BALL_RADIUS, BALL_RADIUS, 0.0));

	// come up with some random vector pointing towards the middle
	Vector v(-50.0, 0.0, (rand() % 100) - 50);
	// normalize the vector and give it some random length to get specific speed
	ball->setVelocity(v.normalize().multiply(5.0));
}
