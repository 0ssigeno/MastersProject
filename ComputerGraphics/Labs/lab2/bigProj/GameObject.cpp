#include "GameObject.h"

bool findRectangularCollision(hitBox A, hitBox B)
{
	if (A.upLeft.x < B.bottomRight.x &&
		A.bottomRight.x > B.upLeft.x &&
		A.upLeft.y > B.bottomRight.y &&
		A.bottomRight.y < B.upLeft.y)
	{
		return true;
	}
	else
		return false;
};
