#include "BrickRing.h"

BrickRing::BrickRing(int count, int levels, double radius, double thickness, double height, int hue)
{
	// set the attributes of the brick ring
	this->radius = radius;
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
			b->color = Color::hsv(hue, 0.5 - ((i + level % 2) % 2)*0.25, 0.5, 1.0);
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
	int rl = rand() % brick_levels;
	int rb = rand() % brick_count;
	if (bricks[rl][rb] != NULL) {
		delete(bricks[rl][rb]);
		bricks[rl][rb] = NULL;
	}
}

void BrickRing::tick(long milis, Ball* ball)
{
	double dt = milis / 1000.0;
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
					current->velocity = current->velocity.add(Vector(0.0, -1.0, 0.0).multiply(dt*1.0));
				} else {
					current->velocity = Vector(0.0);
				}
				current->translate(current->velocity.multiply(dt));
			}
		}
	}
}
