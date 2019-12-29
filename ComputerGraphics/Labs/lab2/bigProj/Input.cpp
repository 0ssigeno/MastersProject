#include "Input.h"

InputEvent::InputEvent(InputType type)
{
	this->type = type;
}

InputEvent::InputEvent(InputType type, int x, int y) 
	: InputEvent(type)
{
	this->x = x;
	this->y = y;
}

InputType InputEvent::getType()
{
	return this->type;
}

int InputEvent::getX()
{
	return this->x;
}

int InputEvent::getY()
{
	return this->y;
}
