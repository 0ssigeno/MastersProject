#include "Game.h"
#include "HUD_Logger.h"
#include <GL/glew.h>
#include <GL/freeglut.h>"

/*
	La scalatura comune applicata a tutti gli oggetti.
*/
#define COMMON_SCALE 10.0

extern float w_width, w_height;

void drawPauseOverlay();

Game::Game()
{
}

void Game::setupNewGame()
{
	Block* block;
	color c = { 0.3,1.0,0.5,1.0 };
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 50.0, 200.0};
	block->scale = COMMON_SCALE;
	static_blocks.push_back(block);
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 250.0, 200.0 };
	block->scale = COMMON_SCALE;	
	static_blocks.push_back(block); 
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 450.0, 200.0 };
	block->scale = COMMON_SCALE;
	static_blocks.push_back(block);
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 650.0, 200.0 };
	block->scale = COMMON_SCALE;
	static_blocks.push_back(block);
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 850.0, 200.0 };
	block->scale = COMMON_SCALE;
	static_blocks.push_back(block);
	block = new Block(10.0, 2.0, c);
	block->currentInWorldPosition = { 1050.0, 200.0 };
	block->scale = COMMON_SCALE;
	static_blocks.push_back(block);

	FallingBlock* fall_block;

	fall_block = new FallingBlock(3.0, 3.0, color{ 1.0,0.5,0.0,1.0 });
	fall_block->currentInWorldPosition = { w_width / 2, w_height / 2.0f + 50.0f };
	fall_block->scale = COMMON_SCALE;
	falling_blocks.push_back(fall_block);
	fall_block = new FallingBlock(3.0, 3.0, color{ 1.0,0.5,0.0,1.0 });
	fall_block->currentInWorldPosition = { w_width / 4, w_height / 2.0f + 300.0f };
	fall_block->scale = COMMON_SCALE;
	falling_blocks.push_back(fall_block);
	fall_block = new FallingBlock(3.0, 3.0, color{ 1.0,0.5,0.0,1.0 });
	fall_block->currentInWorldPosition = { w_width / 3, 250.0f };
	fall_block->scale = COMMON_SCALE;
	falling_blocks.push_back(fall_block);
	fall_block = new FallingBlock(2.0, 2.0, color{ 1.0,0.0,0.0,1.0 });
	fall_block->currentInWorldPosition = {250.0f, 768.0f };
	fall_block->scale = COMMON_SCALE;
	falling_blocks.push_back(fall_block);

	player = new PlayerBlock(3.0, 3.0, color{ 0.5,0.7,1.0,1.0 }, 30.0);
	player->currentInWorldPosition = { 50.0, w_height / 2.0f + 300.0f };
	player->scale = COMMON_SCALE;


}

void Game::start()
{
	this->last_frame = clock();

	for (auto falling : falling_blocks) {
		falling->setStopAnimation(false);
		falling->registerContact();
	}	
	player->setStopAnimation(false);
	player->registerContact();

}

void Game::addInputEvent(InputEvent event)
{
	this->inputEvents.push(event);
}

void Game::processInput()
{
	while (this->inputEvents.size() > 0) {
		InputEvent input = this->inputEvents.front();
		this->inputEvents.pop();

		switch (this->status) {
		case GameState::PRE_START:
			switch (input.getType()) {
			case InputType::JUMP:
				this->status = GameState::RUNNING;
				this->start();
				break;
			}
			break;
		case GameState::RUNNING:
			switch (input.getType()) {
			case InputType::MOVE_UP:
				player->moveUP = true;
				break;
			case InputType::STOP_MOVE_UP:
				player->moveUP = false;
				break;
			case InputType::MOVE_DOWN:
				player->moveDown = true;
				break;
			case InputType::STOP_MOVE_DOWN:
				player->moveDown = false;
				break;
			case InputType::MOVE_LEFT:
				player->moveLeft = true;
				break;
			case InputType::STOP_MOVE_LEFT:
				player->moveLeft = false;
				break;
			case InputType::MOVE_RIGHT:
				player->moveRight = true;
				break;
			case InputType::STOP_MOVE_RIGHT:
				player->moveRight = false;
				break;
			case InputType::PAUSE:
				this->status = GameState::PAUSED;
				player->setStopAnimation(true);
				for (auto falling : falling_blocks) {
					falling->setStopAnimation(true);
				}
				break;
			case InputType::MOUSE_POSITION:
				break;
			default:
				break;
			}
			break;
		case GameState::PAUSED:
			switch (input.getType()) {
			case InputType::PAUSE:
				this->status = GameState::RUNNING;
				player->setStopAnimation(false);
				for (auto falling : falling_blocks) {
					falling->setStopAnimation(false);
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Game::updateScene()
{
	double time = clock();
	double delta_time = time -last_frame;
	delta_time /= 1000.0; // conversion from milliseconds to seconds
	this->last_frame = time;
	/*
		Prima gestisco tutte le collisioni coi blocchi statici
	*/
	for (auto block : static_blocks) 
	{
		for (auto falling : falling_blocks) {
			if (findRectangularCollision(block->getHitBox(), falling->getHitBox()))
			{
				float height_correction = block->getHeight() / 2.0f + falling->getHeight() / 2.0f;
				falling->currentInWorldPosition.y = block->currentInWorldPosition.y + height_correction * COMMON_SCALE;
				falling->registerContact();
			}	
		}

		if (findRectangularCollision(block->getHitBox(), player->getHitBox()))
		{
			float height_correction = block->getHeight() / 2.0f + player->getHeight() / 2.0f;
			player->currentInWorldPosition.y = block->currentInWorldPosition.y + height_correction * COMMON_SCALE;
			player->registerContact();
		}
	}

	/*
		Collisioni extra tra i blocchi che cadono e il player
	*/
	for (auto falling : falling_blocks) {
		if (findRectangularCollision(player->getHitBox(), falling->getHitBox()))
		{
			float width_correction = player->getWidth() / 2.0f + falling->getWidth() / 2.0f;
			if (falling->currentInWorldPosition.x > player->currentInWorldPosition.x) {
				// sposto a destra
				falling->currentInWorldPosition.x = player->currentInWorldPosition.x + width_correction * COMMON_SCALE;
			}
			else {
				// sposto a sinistra
				falling->currentInWorldPosition.x = player->currentInWorldPosition.x - width_correction * COMMON_SCALE;
			}
		}
	}
	/*
		Poi aggiorno tutti gli oggetti
	*/
	for (auto falling : falling_blocks) {
		falling->updateAnimation(delta_time);
	}
	player->updateAnimation(delta_time);
}

void Game::drawScene()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto block : static_blocks) {
		block->draw();
	}
	for (auto falling : falling_blocks) {
		falling->draw();
	}
	player->draw();

	if (hitBoxDebug) {
		for (auto block : static_blocks) {
			block->drawDebugHitbox();
		}
		for (auto falling : falling_blocks) {
			falling->drawDebugHitbox();
		}
		player->drawDebugHitbox();
	}
	if (status == GameState::PAUSED || status == GameState::PRE_START) {
		drawPauseOverlay();
	}
	if (status == GameState::PRE_START) {
		HUD_Logger::get()->printMiddleScreenMsg(std::string("PRESS SPACE TO START"));
	}
	if (status == GameState::PAUSED) {
		HUD_Logger::get()->printMiddleScreenMsg(std::string("PAUSED"));
	}
}


Game::~Game()
{	
	for (auto block : static_blocks) {
		delete block;
	}
	for (auto falling : falling_blocks) {
		delete falling;
	}
	delete player;
}

void drawPauseOverlay()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	//upper half of the screen
	glColor4f(0.0, 0.0, 0.0, 0.0);
	glVertex2f(0.0, w_height);
	glVertex2f(w_width, w_height);

	glColor4f(0.0, 0.0, 0.0, 1.0);
	glVertex2f(w_width, w_height / 2);
	glVertex2f(0.0, w_height / 2);

	// lower half of the screen
	glColor4f(0.0, 0.0, 0.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(w_width, 0.0);

	glColor4f(0.0, 0.0, 0.0, 1.0);

	glVertex2f(w_width, w_height / 2);
	glVertex2f(0.0, w_height / 2);

	glEnd();
	glDisable(GL_BLEND);
}
