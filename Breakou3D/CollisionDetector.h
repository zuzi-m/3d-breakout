#pragma once

#include "Ball.h"
#include "Brick.h"

class CollisionDetector
{
public:
	static double getCollision(Ball* ball, Brick* brick, double dt);
};

