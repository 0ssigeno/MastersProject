#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#define ACC_GRAV 9.81 * 10.0// costante di accellerazione

typedef struct {
	float x, y;
} point2D;

typedef struct {
	float r;
	float g;
	float b;
	float a;
} color;
/*
	Per individuare uno spazio rettangolare 2D bastano 2 punti.
*/
typedef struct {
	point2D upLeft;
	point2D bottomRight;
} hitBox;
/*
	Le seguenti sono classi astratte con solo metodi puramente virtuali.
	Ogni classe che le estende deve definirne una propria implementazione.
*/
class GameObject
{
public:
	virtual void draw() = 0; // = 0 significa che non è implementato
public:
	point2D currentInWorldPosition;
	float scale = 1.0; //default
};

class Animated {
public:
	virtual void updateAnimation(double delta_time) = 0;
	virtual void setStopAnimation(bool value) = 0;
protected:
	bool anim_stopped = true;
};

class Solid {
public:
	virtual hitBox getHitBox() = 0;
	virtual void drawDebugHitbox() = 0;
};


bool findRectangularCollision(hitBox A, hitBox B);