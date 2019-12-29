#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "HUD_Logger.h"
#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Keyboard:
//
// w  move up
// a  move left
// s  move down
// d  move right
// e pick object
// space jump
// p pause

// Dimensioni finestra
#define  W_WIDTH 1200.0
#define W_HEIGHT 768.0

float w_width = W_WIDTH;
float w_height = W_HEIGHT;




static Game* game;

void game_loop(int value)
{
	game->processInput();
	game->updateScene();
	game->drawScene();
	HUD_Logger::get()->print_FPS();
	glutSwapBuffers();
	glutTimerFunc(0,game_loop,0);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, W_WIDTH, 0.0, W_HEIGHT, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void drawScene() {
	game->drawScene();
}
void mouseClick(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			game->addInputEvent(InputEvent(InputType::MOUSE_L_CLICK, x, W_HEIGHT - y));
		}
		break;
	default:
		break;
	}
}

void mouseMotion(int x, int y)
{
	game->addInputEvent(InputEvent(InputType::MOUSE_POSITION, x, W_HEIGHT - y));
}

void manageControlsPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		game->addInputEvent(InputEvent(InputType::MOVE_UP));
		break;
	case 'a':
		game->addInputEvent(InputEvent(InputType::MOVE_LEFT));
		break;
	case 's':
		game->addInputEvent(InputEvent(InputType::MOVE_DOWN));
		break;
	case 'd':
		game->addInputEvent(InputEvent(InputType::MOVE_RIGHT));
		break;
	case 'e':
		game->addInputEvent(InputEvent(InputType::PICK_OBJECT));
		break;
	case 'p':
		game->addInputEvent(InputEvent(InputType::PAUSE));
		break;
	case ' ':
		game->addInputEvent(InputEvent(InputType::JUMP));
		break;
	case 'h':
		game->hitBoxDebug = true;
		break;
	case 13:
		delete game;
		game = new Game();
		game->setupNewGame();
		break;
	case 27:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void manageControlsReleased(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		game->addInputEvent(InputEvent(InputType::STOP_MOVE_UP));
		break;
	case 'a':
		game->addInputEvent(InputEvent(InputType::STOP_MOVE_LEFT));
		break;
	case 's':
		game->addInputEvent(InputEvent(InputType::STOP_MOVE_DOWN));
		break;
	case 'd':
		game->addInputEvent(InputEvent(InputType::STOP_MOVE_RIGHT));
		break;
	case 'h':
		game->hitBoxDebug = false;
		break;
	default:
		break;
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Game Loop Demo");
	glutDisplayFunc(drawScene);
	glutMouseFunc(mouseClick);
	glutReshapeFunc(resize);
	glutKeyboardFunc(manageControlsPressed);
	glutKeyboardUpFunc(manageControlsReleased);
	glutPassiveMotionFunc(mouseMotion);


	game = new Game();
	game->setupNewGame();

	glutTimerFunc(0, game_loop, 0);
	glutMainLoop();

	delete game;

	return 0;
}

