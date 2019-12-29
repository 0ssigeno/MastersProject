#pragma once
#include "GameObject.h"
#include <chrono>

/*
	Un semplice blocco che si disegna e possiede una hitBox.
*/
class Block : public GameObject, public Solid
{
public:
	Block(float width, float height, color c);
	void draw();
	float getWidth();
	float getHeight();
	/*
	Restituisce una hitBox in coordinate del mondo
	*/
	hitBox getHitBox();

	void drawDebugHitbox();
	~Block();
protected:
	int width, height;
	color c;
};
/*
	Una evoluzione del blocco che aggiunge una animazione di caduta.
*/
class FallingBlock : public Block, public Animated{
public:
	FallingBlock(float width, float height, color c);
	void updateAnimation(double delta_time);
	void setStopAnimation(bool value);
	/*
		Salva l'istante in cui la caduta è iniziata
	*/
	void registerContact();
private:
	clock_t last_time_anim_paused;
	clock_t last_registered_contact;
	float lastFallSpeed = 0.0f;
};
/*
	Un blocco ancora più evoluto, che accetta input 
	per modificare la sua animazione in maniera interattiva.
*/
class PlayerBlock : public FallingBlock {
public:
	PlayerBlock(float width, float height, color c, float jumpSpeed);
	void updateAnimation(double delta_time);
	bool moveUP = false;
	bool moveDown = false;
	bool moveRight = false;
	bool moveLeft = false;
private:
	double movementSpeed;
};


/*
Consiglio:
In C++ le classi possono estendere un qualsiasi numero di classi quindi...

Mai creare dei diamanti!

Ovvero non derivare da 2 o più classi se queste condividono una o più classi antenate.
*/