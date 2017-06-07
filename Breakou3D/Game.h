#pragma once

#include <list>

#include "Ball.h"
#include "Ground.h"
#include "Camera.h"
#include "Brick.h"
#include "Light.h"
#include "BrickRing.h"

// TODO:
// kolizie - ball x bats, ball x bricks
// getDistance
// materialy
// attenuation svetla?
// shader na per-fragment lighting

class Game
{
public:
	static Game* instance() { static Game inst; return &inst; }

	void draw();
	void keyboardCallback(unsigned char, int, int);
	void passiveMouseCallback(int, int);
	void activeMouseCallback(int, int);
	void mouseWheelCallback(int, int, int, int);
	void resizeCallback(int, int);

	void tick();
	void start(int, int);
private:
	Game();
	~Game();

	void initLevel();
	void destroyLevel();

	void initRendering();

	void loseLife();
	void resetBall();

	Ground* ground;
	Ball* ball;
	Camera* camera;
	std::list<Brick*> bats;
	std::list<Light*> lights;
	std::list<BrickRing*> rings;

	int prevX;
	int prevY;
	long prevT;
	int level_radius;
	bool isReady;

	const double BALL_RADIUS = 1.0;
	const double BRICK_THICKNESS = 2.0;
	const double BRICK_HEIGHT = 2.0;
	double OUTER_PADDING;
	double INNER_PADDING;
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
};

