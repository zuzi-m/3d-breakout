#include "BrickRing.h"

#include "CollisionDetector.h"

BrickRing::BrickRing(int count, int levels, double radius, double thickness, double height, int hue)
{
	// set the attributes of the brick ring
	this->radius = radius;
	activeBricks = count * levels;
	brick_count = count;
	brick_levels = levels;
	brick_width = thickness;
	brick_height = height;

	// build the brick making up this ring
	// allocate "brick_levels" of brick arrays for all levels
	bricks = new Brick**[brick_levels];
	for (int level = 0; level < brick_levels; level++) {
		// allocate array of "brick_count" bricks in each level
		bricks[level] = new Brick*[brick_count];
		double level_height = level * height;
		Vector height_translation(0.0, level_height, 0.0);
		for (int i = 0; i < brick_count; i++) {
			// create each brick and position it to proper place
			Brick* b = Brick::unit(brick_count, radius, thickness, height);
			b->setColor(Color::hsv(hue, 0.5 - ((i + level % 2) % 2)*0.25, 0.5, 1.0)); // TODO simplify
			b->trans(Matrix::rotation(1, i * 2.0 * PI / brick_count));
			b->translate(height_translation);
			bricks[level][i] = b;
		}
	}
}

BrickRing::~BrickRing()
{
	for (int level = 0; level < brick_levels; level++) {
		for (int i = 0; i < brick_count; i++) {
			if (bricks[level][i] != NULL) {
				// delete each remaining brick
				delete(bricks[level][i]);
			}
		}
		// delete level array
		delete(bricks[level]);
	}
	// finally, delete the array for levels
	delete(bricks);
}

void BrickRing::draw() const
{
	for (int level = 0; level < brick_levels; level++) {
		for (int i = 0; i < brick_count; i++) {
			if (bricks[level][i] != NULL) {
				bricks[level][i]->draw();
			}
		}
	}
}

void BrickRing::removeRandom()
{
	// dont remove brick if there are no bricks
	if (activeBricks > 0) {
		// if it makes sense, guess random position and try to remove brick there
		int rl = rand() % brick_levels;
		int rb = rand() % brick_count;
		if (!removeBrick(rl, rb)) {
			// if not successful, try again until it succeeds (it should eventually
			// because there is at least one brick active
			return removeRandom();
		}
	}
}

double BrickRing::tick(double dt, Ball* ball)
{
	if (activeBricks < 1) {
		// nothing to do if no bricks are active any more
		return 0.0;
	}

	// go over brick levels from the top to bottom
	// skip the lowest layer - there is nowhere to fall from there
	for (int level = brick_levels - 1; level > 0; level--) {
		// for each brick, find if theres any brick below it
		for (int i = 0; i < brick_count; i++) {
			Brick* current = bricks[level][i];
			Brick* below = bricks[level - 1][i];
			if (current != NULL && below == NULL) {
				bricks[level - 1][i] = bricks[level][i];
				bricks[level][i] = NULL;
			}
		}
	}

	// now go trough all bricks again and let them fall
	// if they are higher than their level height
	for (int level = 0; level < brick_levels; level++) {
		// for each brick, find if theres any brick below it
		double level_height = level * brick_height;
		for (int i = 0; i < brick_count; i++) {
			Brick* current = bricks[level][i];
			if (current != NULL) {
				if (current->getBottom() > level_height) {
					// add G to brick's velocity v = g * t
					current->velocity = current->velocity.add(Vector(0.0, -1.0, 0.0).multiply(dt*10.0));
				} else {
					current->velocity = Vector(0.0);
				}
				current->translate(current->velocity.multiply(dt));
			}
		}
	}

	Point ballPos = ball->getPosition();
	double ballRadius = sqrt(ballPos[0] * ballPos[0] + ballPos[2] * ballPos[2]);
	if (abs(ballRadius - radius) < brick_width * 3) {
		for (int brickIdx = 0; brickIdx < brick_count; brickIdx++) {
			//int brickIdx = (ballSection + i) % brick_count;
			Brick* b = bricks[0][brickIdx];
			if (b == NULL) continue;

			double remainingTime = CollisionDetector::getCollision(ball, b, dt);
			if (remainingTime != 0.0) {
				removeBrick(0, brickIdx);
				return remainingTime;
			}
		}
	}
	return 0.0;
}

int BrickRing::bricksLeft() const
{
	return activeBricks;
}

bool BrickRing::removeBrick(int level, int position)
{
	if (bricks[level][position] != NULL) {
		delete(bricks[level][position]);
		bricks[level][position] = NULL;
		activeBricks--;
		return true;
	}
	else {
		return false;
	}
}
