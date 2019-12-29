#pragma once

enum InputType {
	MOUSE_L_CLICK,
	MOUSE_R_CLICK,
	JUMP, ESCAPE,
	MOUSE_POSITION, 
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_DOWN,
	STOP_MOVE_RIGHT,
	STOP_MOVE_LEFT,
	STOP_MOVE_UP,
	STOP_MOVE_DOWN,
	PICK_OBJECT,
	PAUSE
};

/*
Semplice classe che incorpora un InputType,
se questo è un movimento del  mouse, getX() getY() danno le coordinate.
*/
class InputEvent {
public:
	InputEvent(InputType type);
	InputEvent(InputType type, int x, int y);
	InputType getType();
	int getX();
	int getY();
private:
	InputType type;
	int x;
	int y;
};