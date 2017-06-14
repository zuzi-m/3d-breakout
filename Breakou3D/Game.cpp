#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <algorithm>

#include "Ray.h"
#include "CollisionDetector.h"

using namespace MathLib;

std::string readFile(const char* filename) {
	std::ifstream file(filename);
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

Game::Game()
{
	isLoaded = false;
}


Game::~Game()
{
	// destroy the level if needed
	destroyLevel();
	// destroy things that were initialized once
	delete(tex_concrete);
	delete(tex_background);
	delete(tex_wall);
	delete(scr_main);
	delete(scr_instructions);
	delete(scr_scores);
}

void Game::draw()
{
	if (!isLoaded) {
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	if (isPlaying) {
		// render the 3D environment:

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

		// draw text
		Color white(1.0);
		std::ostringstream txts;
		txts << "Level: " << level << "  " << "Lives: " << lives << "  " << "Score: " << score;
		drawText(10, 20, txts.str(), white);
	}
	else {
		// draw the menu
		if (currentScreen != NULL) {
			currentScreen->draw();
		}
	}

	glPopMatrix();
	glutSwapBuffers();
}

void Game::keyboardCallback(unsigned char key, int a, int b)
{
	if (!isLoaded) {
		return;
	}

	if (isPlaying) {
		gameKeypress(key, false);
	}
	else {
		menuKeypress(key, false);
	}
}

void Game::specialFuncCallback(int key, int, int)
{
	if (!isLoaded) {
		return;
	}

	if (isPlaying) {
		gameKeypress(key, true);
	}
	else {
		menuKeypress(key, true);
	}
}

void Game::activeMouseCallback(int newX, int newY)
{
	if (!isLoaded || !isPlaying) {
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
	if (!isLoaded || !isPlaying) {
		return;
	}
	if (!isPaused && prevX != -1 && prevY != -1) {
		double amount = prevX - newX;
		rotateBats(amount);
	}
	prevX = newX;
	prevY = newY;
}

void Game::mouseWheelCallback(int wheel, int direction, int x, int y)
{
	if (!isLoaded || !isPlaying) {
		return;
	}

	camera->zoom(-direction * 2.0);
}

void Game::resizeCallback(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

	scr_main->resize(w, h);
	scr_instructions->resize(w, h);
	scr_scores->resize(w, h);

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isPlaying) {
		gluPerspective(40.0, ((double)WINDOW_WIDTH) / ((double)WINDOW_HEIGHT), 1.0, 500.0);
	} else {
		glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1.0, 1.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

void Game::tick()
{
	if (!isLoaded || !isPlaying) {
		// dont simulate if the game is not ready
		return;
	}
	long currT = clock();
	long dt_milis = currT - prevT;
	double dt = dt_milis / 1000.0;
	prevT = currT;
	if (isPaused) {
		// dont simulate if the game is paused
		// however, let the time still pass by
		return;
	}

	// get the initial ball position, also its radius because it is helpful
	Point ballPosition = ball->getPosition();
	double ballRadius = sqrt((ballPosition[0] * ballPosition[0]) + (ballPosition[2] * ballPosition[2]));

	// detect collisions with bats if ball is near their radius
	if (ballRadius > level_radius - OUTER_PADDING) {
		for (auto it = bats.begin(); it != bats.end(); it++) {
			Brick* bat = *it;
			double timeLeft = CollisionDetector::getCollision(ball, bat, dt);
			if (timeLeft > 0.0) {
				dt = timeLeft;
				prevT = currT - (long)floor(timeLeft * 1000.0);
				return;
			}
		}
	}

	// let the bricks simulate themselves and do their things,
	// check for collisions and if there was one, react to it properly
	int bricksLeft = 0;
	for (auto it = rings.begin(); it != rings.end(); it++) {
		BrickRing* ring = *it;
		bricksLeft += ring->bricksLeft();
		double timeLeft = ring->tick(dt, ball);
		if (timeLeft > 0.0) {
			dt = timeLeft;
			prevT = currT - (long)floor(timeLeft * 1000.0);
			score += 100;
			return;
		}
	}
	// if no bricks are left, the level is finished
	if (bricksLeft <= 0) {
		return completeLevel();
	}

	// move the ball
	ball->setPosition(ball->getPosition().add(ball->getVelocity().multiply(dt)));
	// if the ball got out, lose a life
	if (ballRadius > level_radius) {
		loseLife();
	}
}

// ///////////////////////////////////////////////////////
// //////// CALLBACKS TO BE SENT TO BUTTONS: /////////////
// ///////////////////////////////////////////////////////

void callback_start() {
	Game::instance()->newGame();
}

void callback_exit() {
	Game::instance()->exitGame();
}

void callback_show_main() {
	Game::instance()->showMainMenu();
}

void callback_show_inst() {
	Game::instance()->showInstructions();
}

void callback_show_score() {
	Game::instance()->showHighscores();
}

// ///////////////////////////////////////////////////////

bool Game::load(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

	// do one-off opengl settings:
	// set default clear color & depth
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	// smooth shading model
	glShadeModel(GL_SMOOTH);
	// enable textures
	glEnable(GL_TEXTURE_2D);

	// load textures
	try {
		tex_concrete = new Texture("concrete.jpg");
		tex_wall = new Texture("wall.png");
		tex_background = new Texture("back.png");
	}
	catch (const char* str) {
		std::cout << "Texture load error: " << str << std::endl;
		return false;
	}

	// load menu
	scr_main = new MenuScreen("Breakou3D Game", tex_background);
	scr_instructions = new MenuScreen("Instructions & About", tex_background);
	scr_scores = new MenuScreen("High Scores", tex_background);

	// add buttons to screens
	scr_main->addButton("Start Game", callback_start);
	scr_main->addButton("Instructions & About", callback_show_inst);
	//scr_main->addButton("Highscores", callback_show_score); TODO not finished
	scr_main->addButton("Exit Game", callback_exit);
	scr_instructions->addButton("Back", callback_show_main);
	scr_scores->addButton("Back", callback_show_main);

	// add content to screens
	scr_instructions->addContent("The 3D version of the Breakout game.");
	scr_instructions->addContent("Instructions:");
	scr_instructions->addContent("W,A,S,D, active mouse move - move camera");
	scr_instructions->addContent("left, right, passive mouse move - move bats");
	scr_instructions->addContent("+, -, mouse wheel - zoom in/out");
	scr_instructions->addContent("r - restart the game");
	scr_instructions->addContent("p - toggle pause");
	scr_instructions->addContent("f/g - increase/decrease ball speed");
	scr_instructions->addContent("e - CHEAT: remove random brick from the innermost circle");
	scr_instructions->addContent("t - CHEAT: finish current level");
	scr_instructions->addContent("esc - quit game");
	scr_instructions->addContent("About:");
	scr_instructions->addContent("This simple game was created as a project for the PA199 course.");
	scr_instructions->addContent("Marek Zuzi, FI MUNI Brno, June 2017");

	currentScreen = scr_main;
	isPlaying = false;
	initRendering();

	showMainMenu();
	isLoaded = true;

	return true;
}

void Game::showMainMenu()
{
	if (!isLoaded) {
		return;
	}
	if (isPlaying) {
		finishGame();
	}
	currentScreen = scr_main;
}

void Game::showInstructions()
{
	if (!isLoaded) {
		return;
	}
	if (isPlaying) {
		finishGame();
	}
	currentScreen = scr_instructions;
}

void Game::showHighscores()
{
	if (!isLoaded) {
		return;
	}
	if (isPlaying) {
		finishGame();
	}
	currentScreen = scr_scores;
}

void Game::exitGame()
{
	std::exit(0);
}

void Game::newGame()
{
	if (!isLoaded) {
		return;
	}
	if (isPlaying) {
		restartGame();
	}
	else {
		startGame();
	}
}

double difficultyMultiplier(int level) {
	return (std::min(level, 7) / 7.0);
}

void Game::initLevel()
{
	// the higher the level, the smaller the radius
	level_radius = /*(rand() % 36) + 25;*/40;
	// higher level = more rings, bricks and levels
	int brick_rings = std::min(level, 4); // 1-4 rings
	int brick_count = 9 + std::min(level, 3)*2; // 9-15 bricks
	int brick_levels = 3 + std::min(level, 3); // 3 - 6 levels

	OUTER_PADDING = level_radius * 0.3;
	INNER_PADDING = level_radius * 0.06;

	// set up essential game objects
	ground = new Ground(level_radius + INNER_PADDING);
	ground->texture = tex_wall;
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
		bat->setColor(Color(1.0));
		bat->trans(Matrix::rotation(1, i * 2.0 * PI / 3.0));
		bat->setTexture(tex_concrete);
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
}

void Game::destroyLevel()
{
	isLoaded = false;
	// destroy basic objects
	if (ground != NULL) {
		delete(ground);
		ground = NULL;
	}
	if (camera != NULL) {
		delete(camera);
		camera = NULL;
	}
	if (ball != NULL) {
		delete(ball);
		ball = NULL;
	}
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
	if (isPlaying) {
		// init lighting, add default ambient light
		glEnable(GL_LIGHTING);
		float vals[3]{ 0.05f, 0.05f, 0.05f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vals);
		// init depth testing
		glEnable(GL_DEPTH_TEST);
		// set up proper backface culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	// setup the viewport
	resizeCallback(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::drawText(double x, double y, std::string str, const Color& color) {
	glMatrixMode(GL_PROJECTION);
	// save current projection
	glPushMatrix();
	// set orthographic projection for 2D rendering
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	// save current modelview transformations and load identity
	glPushMatrix();
	glLoadIdentity();
	// save current state of some settings that may interfere with text rendering
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	// set color and raster position
	glColor3d(color[0], color[1], color[2]);
	glRasterPos2d(x, y);
	// draw text one character at a time
	for (unsigned int i = 0; i<str.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
	// restore saved attributes of rendering to continue without a change
	glPopAttrib();
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Game::menuKeypress(unsigned int key, bool isSpecial)
{
	if (isSpecial) {
		// special keys - arrows
		switch (key)
		{
		case GLUT_KEY_UP:
			currentScreen->up();
			break;
		case GLUT_KEY_DOWN:
			currentScreen->down();
			break;
		}
	}
	else {
		// normal keys - enter and escape
		switch (key)
		{
		case 13:
			currentScreen->enter();
			break;
		case 27:
			if (currentScreen == scr_main) {
				exitGame();
			}
			else {
				showMainMenu();
			}
			break;
		}
	}
}

void Game::gameKeypress(unsigned int key, bool isSpecial)
{
	if (isSpecial) {
		// special keys - arrows moving bat
		switch (key)
		{
		case GLUT_KEY_LEFT:
			rotateBats(-5.0);
			break;
		case GLUT_KEY_RIGHT:
			rotateBats(5.0);
			break;
		}
	}
	else {
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
		case 'p':
			isPaused = !isPaused;
			break;
		case '+':
			camera->zoom(-1.0);
			break;
		case '-':
			camera->zoom(1.0);
			break;
		case 'e':
			if (rings.size() > 0) {
				// remove random brick from the first brick ring that is non-empty
				for (auto it = rings.begin(); it != rings.end(); it++) {
					br = *it;
					if (br->bricksLeft() > 0) {
						br->removeRandom();
						break;
					}
				}
			}
			break;
		case 'r':
			restartGame();
			break;
		case 't':
			completeLevel();
			break;
		case 'f':
			if (isPlaying && isLoaded) {
				ball->setVelocity(ball->getVelocity().multiply(1.25));
			}
			break;
		case 'g':
			if (isPlaying && isLoaded) {
				ball->setVelocity(ball->getVelocity().multiply(0.85));
			}
			break;
		case 27:
			finishGame();
			break;
		}
	}
}

void Game::startGame()
{
	// start with clean score and some lives
	level = 1;
	score = 0;
	lives = 5;

	// initialize a new level
	initLevel();
	
	// set the flags
	isPaused = false;
	isPlaying = true;
	initRendering();
	isLoaded = true;
}

void Game::loseLife()
{
	// remove one life
	lives--;
	if (lives < 0) {
		// if there are no lives left, the game is lost
		finishGame();
	} else {
		// if there are still some lives, just reset the ball
		resetBall();
		score -= 200;
	}
}

void Game::completeLevel()
{
	// destroy the old level
	destroyLevel();
	// initialize a new level
	level++;
	initLevel();
	// award the player with some score and lives
	score += 300;
	lives += 3;
	isLoaded = true;
}

void Game::restartGame()
{
	// destroy level and start over
	destroyLevel();
	startGame();
}

void Game::finishGame()
{
	// return to the menu:
	isPlaying = false;
	destroyLevel();
	initRendering();
	isLoaded = true;
}

void Game::resetBall()
{
	// move ball to some reasonable position and give it some V:
	// position the ball to initial position
	ball->setPosition(Point(level_radius - (OUTER_PADDING / 2.0) - 2 * BALL_RADIUS, BALL_RADIUS, 0.0));

	// come up with some random vector pointing towards the middle
	Vector v(-50.0, 0.0, (rand() % 100) - 50);
	// normalize the vector and give it some random length to get specific speed
	double speed = 3.0 + difficultyMultiplier(level)*5.0;
	ball->setVelocity(v.normalize().multiply(speed));
}

void Game::rotateBats(double amount)
{
	// get the rotation matrix - around Y axis, by given amount
	Matrix batRotation = Matrix::rotation(1, amount * 2 * PI / 360.0);

	// rotate all bats
	for (auto it = bats.begin(); it != bats.end(); it++) {
		(*it)->trans(batRotation);
	}
}
