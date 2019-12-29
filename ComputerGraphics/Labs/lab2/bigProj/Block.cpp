#include "Block.h"
#include <cmath>


Block::Block(float width, float height, color c)
{
	this->width = width;
	this->height = height;
	this->c = c;
}

void Block::draw()
{
	glPushMatrix();
	glTranslatef(this->currentInWorldPosition.x, this->currentInWorldPosition.y, 0.0);
	glScalef(scale,scale,1.0);

	glColor4f(c.r,c.g,c.b,c.a);
	glBegin(GL_POLYGON);
	glVertex2f(-width / 2.0, height / 2.0);
	glVertex2f(-width / 2.0, -height / 2.0);
	glVertex2f(width / 2.0, -height / 2.0);
	glVertex2f(width / 2.0, height / 2.0);
	glEnd();
	glPopMatrix();
}

float Block::getWidth()
{
	return this->width;
}

float Block::getHeight()
{
	return this->height;
}

hitBox Block::getHitBox()
{
	hitBox box;
	float scaled_w = this->width / 2.0 * scale;
	float scaled_h = this->height / 2.0 * scale;
	box.upLeft.x = -scaled_w + this->currentInWorldPosition.x;
	box.upLeft.y = scaled_h + this->currentInWorldPosition.y;
	box.bottomRight.x = scaled_w + this->currentInWorldPosition.x;
	box.bottomRight.y = -scaled_h + this->currentInWorldPosition.y;

	return box;
}

void Block::drawDebugHitbox()
{
	hitBox xbox = this->getHitBox();

	glLineWidth(1.0);
	glColor4f(1.0, 0.0, 0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xbox.upLeft.x,xbox.upLeft.y);
	glVertex2f(xbox.upLeft.x, xbox.bottomRight.y);
	glVertex2f(xbox.bottomRight.x, xbox.bottomRight.y);
	glVertex2f(xbox.bottomRight.x, xbox.upLeft.y);
	glEnd();
}

Block::~Block()
{
}

FallingBlock::FallingBlock(float width, float height, color c)
	: Block(width, height, c)
{
}

void FallingBlock::updateAnimation(double delta_time)
{
	if (anim_stopped) return;
	double time = clock();
	double fall_delta_time = time - last_registered_contact;
	fall_delta_time /= 1000.0; //from milliseconds to seconds

	float current_fall_speed = - ACC_GRAV * fall_delta_time;
	float medium_fall_speed = current_fall_speed + lastFallSpeed / 2.0f;
	this->currentInWorldPosition.y += medium_fall_speed * delta_time * scale;
	lastFallSpeed = current_fall_speed;
}

void FallingBlock::setStopAnimation(bool value)
{
	this->anim_stopped = value;
	//I must save the instant in which the animation was stopped
	if (value) {
		last_time_anim_paused = clock();
	}
	//When the game wants to unpause the animation, just add the time that has passed
	else {
		last_registered_contact += clock() -last_time_anim_paused;
	}
}

void FallingBlock::registerContact()
{
	last_registered_contact = clock();
	lastFallSpeed = 0.0;
}

PlayerBlock::PlayerBlock(float width, float height, color c, float speed)
	: FallingBlock(width, height, c)
{
	this->movementSpeed = speed;
}
/*
	Nota: vogliamo che il blocco controllato da giocatore cada anche,
	per fare questo deleghiamo il compito al metodo della classe padre,
	richiamando FallingBlock::updateAnimation(delta_time);
*/
void PlayerBlock::updateAnimation(double delta_time)
{
	FallingBlock::updateAnimation(delta_time);
	if (anim_stopped) {
		delta_time = 0.0;
	}
	if (this->moveUP) {
		this->currentInWorldPosition.y += movementSpeed * delta_time * scale;
	}
	if (this->moveDown) {
		this->currentInWorldPosition.y -= movementSpeed * delta_time * scale;
	}
	if (this->moveLeft) {
		this->currentInWorldPosition.x -= movementSpeed * delta_time * scale;

	}
	if (this->moveRight) {
		this->currentInWorldPosition.x += movementSpeed * delta_time * scale;
	}
}
