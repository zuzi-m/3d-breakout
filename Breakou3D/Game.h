#pragma once

#include <list>

#include "glew.h"
#include "Ball.h"
#include "Ground.h"
#include "Camera.h"
#include "Brick.h"
#include "Light.h"
#include "BrickRing.h"
#include "MenuScreen.h"

// TODO:
// kolizie - rohy, kraje brickov
// getDistance
// materialy
// attenuation svetla?
// shader na per-fragment lighting

/**
 * @class	Game
 *
 * @brief	A singleton design pattern class controlling the whole 3D Breakout game.
 * 			The game is built on OpenGL, so it provides necessary event callbacks.
 */
class Game
{
public:
	/**
	 * @fn	static Game* Game::instance()
	 *
	 * @brief	Gets the instance of the Game singleton class.
	 *
	 * @return	The game singleton instance.
	 */
	static Game* instance() { static Game inst; return &inst; }

	// GLUT callbacks for events
	void draw();
	void keyboardCallback(unsigned char, int, int);
	void specialFuncCallback(int, int, int);
	void passiveMouseCallback(int, int);
	void activeMouseCallback(int, int);
	void mouseWheelCallback(int, int, int, int);
	void resizeCallback(int, int);

	/**
	 * @fn	void Game::tick();
	 *
	 * @brief	Callback for performing the simulation step. The function
	 * 			will find how much time has passed since it was last updates
	 * 			and updates the positions of objects, handles the collision
	 * 			detection and reacts to the state of the game.
	 * 			Has no effect if the game is not loaded yet or is in main menu.
	 */
	void tick();

	/**
	 * @fn	bool Game::load(int w, int h);
	 *
	 * @brief	Loads the game resources and initializes its state to make it ready.
	 * 			If some loading fails during the process, the function returns false
	 * 			and should not be started.
	 *
	 * @param	w	The initial width of the game window.
	 * @param	h	The initial height of the game window.
	 *
	 * @return	True if it succeeds, false if it fails.
	 */
	bool load(int w, int h);

	// public functions to trigger some events of the game from outside
	void showMainMenu();
	void showInstructions();
	void showHighscores();
	void exitGame();
	void newGame();
private:
	Game();
	~Game();

	void initLevel();
	void destroyLevel();

	void initRendering();
	void drawText(double x, double y, std::string str, const Color&);

	void menuKeypress(unsigned int key, bool isSpecial);
	void gameKeypress(unsigned int key, bool isSpecial);

	// events affecting the life-cycle of the game
	void startGame();
	void loseLife();
	void completeLevel();
	void restartGame();
	void finishGame();

	void resetBall();
	void rotateBats(double amount);

	// objects making up one level:
	Ground* ground; // circular ground
	Ball* ball; // ball
	Camera* camera; // camera of the scene
	std::list<Brick*> bats; // bats controlled by user
	std::list<Light*> lights; // lights in the scene
	std::list<BrickRing*> rings; // rings of bricks

	// variables holding the state of things:
	int prevX; // X mouse position
	int prevY; // Y mouse position
	long prevT; // time of simulation
	bool isLoaded = false; // whether the game was loaded properly
	bool isPaused = false;
	bool isPlaying = true;
	int level; // current level
	int score; // current score
	int lives; // current lives left

	// variables initialized once on loading the program
	Texture* tex_concrete;
	Texture* tex_wall;
	Texture* tex_background;
	MenuScreen* currentScreen;
	MenuScreen* scr_main;
	MenuScreen* scr_instructions;
	MenuScreen* scr_scores;

	// variables and constants related to the sizes of things
	// NOTE: most of them are recomputed at some points (e.g. at level initialization)
	int level_radius;
	const double BALL_RADIUS = 1.0;
	const double BRICK_THICKNESS = 2.0;
	const double BRICK_HEIGHT = 2.0;
	double OUTER_PADDING;
	double INNER_PADDING;
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
};

