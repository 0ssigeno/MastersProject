#pragma once
#include <queue>
#include "Input.h"
#include "Block.h"

/*
Il gioco presenta 3 stati che ne disciplinano il comportamento.
*/
typedef enum {
	PRE_START,// il gioco aspetta l'utente
	RUNNING,// tutte le animazioni eseguono
	PAUSED// il gioco blocca tutte le animazioni 
} GameState;
/*
	Una classe che definisce la scena e gestisce la logica della simulazione.
*/
class Game
{
public:
	Game();
	void setupNewGame();
	/*
	Innesca l'avvio di tutti gli oggetti animati
	*/
	void start();
	/*
	Aggiunge un evento alla coda.
	*/
	void addInputEvent(InputEvent event);
	/*
	Consuma tutti gli eventi nella coda in ordine cronologico.
	*/
	void processInput();
	void updateScene();
	void drawScene();
	~Game();
	bool hitBoxDebug = false;
private:
	GameState status = GameState::PRE_START;
	std::queue<InputEvent> inputEvents;
	clock_t last_frame;
//	GameObjects
	std::vector<Block*> static_blocks;
	std::vector<FallingBlock*> falling_blocks;
	PlayerBlock* player;
};

