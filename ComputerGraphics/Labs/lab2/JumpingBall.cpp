//
// Created by simone on 20/03/19.
//


// JumpingBall.cpp
//
#include "stdio.h"
# include <GL/glew.h>
# include <GL/freeglut.h>
#include <cmath>
#define PI 3.14159265358979323846
#define nvertices 24

// Movimento palla controllato dai TASTI: A (left) D (right)

int width = 1280; // dimensioni iniziali della finestra
int height = 720;


// parametri della palla
float	delta = 15;
float	distacco_da_terra = 0; // distacco da terra
double	playerSpeed = 0; //velocita orizzontale (pixel per frame)

float  playerPositionY = height * 0.2;
double accelerazione = 1; // forza di accelerazione data dalla tastiera
float  playerPositionX = width / 2;  //coordinate sul piano della posizione iniziale della palla
typedef struct { double r, g, b, a; } ColorRGBA;

bool pressing_left = false;
bool pressing_right = false;

void keyboardPressedEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
            pressing_left = true;
            break;

        case 'd':
            pressing_right = true;
            break;

        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
            pressing_left = false;
            break;

        case 'd':
            pressing_right = false;
            break;

        default:
            break;
    }
}

double lerp(double a, double b, double amount) {
    //Interpolazione lineare tra a e b con coefficiente amount
    return (1 - amount)*a + amount * b;
}

void update(int a)
{
    bool moving = false;

    //Movimento della palla in orizzontale
    if (pressing_left)
    {
        playerSpeed -= accelerazione;
        moving = true;
    }
    if (pressing_right)
    {
        playerSpeed += accelerazione;
        moving = true;
    }

    //Movimento della palla sul posto
    if (!moving) {   // Se non mi sto muovendo con i tasti a e d
        // decremento o incremento la velocita' iniziale fino a portarla
        // a zero e la palla continua a rimbalzare sul posto

        if (playerSpeed > 0)
        {
            playerSpeed -= 1;
            if (playerSpeed < 0)
                playerSpeed = 0;
        }

        if (playerSpeed < 0)
        {
            playerSpeed += 1;
            if (playerSpeed > 0)
                playerSpeed = 0;
        }
    }

    //Aggioramento della posizione in x della pallina, che regola il movimento orizzontale

    playerPositionX += playerSpeed;

    // Se la palla assume una posizione in x minore di 0 o maggiore di width dello schermo
    // la palla rimbalza ai bordi dello schermo
    if (playerPositionX < 0) {
        playerPositionX = 0;
        playerSpeed = -playerSpeed * 0.8;
    }
    if (playerPositionX > width) {
        playerPositionX = width;
        playerSpeed = -playerSpeed * 0.8;
    }

    // Gestione del rimbalzo in relazione all'altezza da terra

    //Rimbalzo -20 < distacco_da_terra < 100
    distacco_da_terra += (delta*(1-(distacco_da_terra / 120)));
    if ((distacco_da_terra > 100) && (delta>0))
        delta = -delta;
    if ((distacco_da_terra < -20) && (delta<0))
        delta = -delta;

    glutPostRedisplay();
    glutTimerFunc(22, update, 0);
}

void disegna_montagne(float x, float y, int altezza_montagna, int larghezza_montagne, int numero_di_montagne, ColorRGBA color_top, ColorRGBA color_bot)
{

    int i = 0;
    int Numero_di_poligoni = 128;

    float base_poligono = larghezza_montagne / (float)Numero_di_poligoni;
    float frequenza = PI * numero_di_montagne;

    for (i = 0; i < Numero_di_poligoni; i++)
    {
        glBegin(GL_POLYGON);
        glColor4d(color_top.r , color_top.g , color_top.b , color_top.a );
        glVertex2f(x + i * base_poligono, y + altezza_montagna * abs(sin(i / (float)Numero_di_poligoni*frequenza)));
        glVertex2f(x + (i + 1)*base_poligono, y + altezza_montagna * abs(sin((i + 1) / (float)Numero_di_poligoni*frequenza)));
        glColor4d(color_bot.r , color_bot.g , color_bot.b , color_bot.a);
        glVertex2f(x + (i + 1)*base_poligono, y);
        glVertex2f(x + i * base_poligono, y);
        glEnd();
    }
}
void disegna_cerchio(float x, float y, float raggiox, float raggioy, ColorRGBA colore, ColorRGBA sfumatura) {
    int i;
    float stepA = (2 * PI) / nvertices;

    for (i = 0; i < nvertices; i++)
    {
        glBegin(GL_TRIANGLES);
        glColor4d(colore.r , colore.g , colore.b , colore.a );
        glVertex2f(x + cos((double)i *stepA)*raggiox, y + sin((double)i *stepA)*raggioy);
        glVertex2f(x + cos((double)(i + 1) *stepA)*raggiox, y + sin((double)(i + 1) *stepA)*raggioy);
        glColor4f(sfumatura.r, sfumatura.g, sfumatura.b, sfumatura.a);
        glVertex2f(x, y);

        glEnd();
    }
}
void disegna_palla(float x, float y, float ball_height) {
    ColorRGBA colore;
    ColorRGBA sfumatura;

    double shadow_scale = lerp(1, 0, (float)ball_height / 255);
    // larghezza effettiva in pixel della palla (80 diametro palla normale -- 100 palla dilatata)
    double bwidth = ball_height < 0 ? lerp(80, 100, (double)abs(ball_height) / 20) : 80;
    // altezza effettiva in pixel della palla
    double bheight = ball_height < 0 ? 80 + ball_height : 80;

    if (shadow_scale < 0)
        shadow_scale = 0;

    //palla
    colore = { 1.0, 0.0, 0, 1.0 };
    sfumatura = { 1.0, 0.95, 0, 1.0 };
    disegna_cerchio(x, y + ball_height, bwidth / 2, bheight / 2, colore, sfumatura);

    //ombra
    colore = { 0.5, 0.5, 0.5, 0.0 };
    sfumatura = { 0, 0, 0, lerp(0.25,1.0,shadow_scale) };
    disegna_cerchio(x, y - bheight / 2, (bwidth*shadow_scale), (50 * shadow_scale), colore, sfumatura);

}

void disegna_piano(float x, float y, float width, float height, ColorRGBA color_top, ColorRGBA color_bot) {

    glBegin(GL_POLYGON);
    glColor4d(color_top.r , color_top.g , color_top.b, color_top.a );
    glVertex2f(x, y);
    glVertex2f(x + width, y);

    glColor4d(color_bot.r , color_bot.g , color_bot.b, color_bot.a );
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}


// Drawing routine.

//SIMO statica creazione del bordo
void drawScene(void)
{
    ColorRGBA c_top;
    ColorRGBA c_bot;

    //Definisce il colore con cui verrà  presentato lo schermo
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Disegna cielo
    c_top = { 0.25,0.6 ,1.0, 1.0 };
    c_bot = { 0.0 ,0.55,0.7, 1.0 };
    disegna_piano(0, height / 2, width, height / 2, c_top, c_bot);
    //Disegna prato
    c_top = { 0.1, 0.2, 0.1, 1.0 };
    c_bot = { 0.7, 1.0, 0.1, 1.0 };
    disegna_piano(0, 0, width, height / 2, c_top, c_bot);

    //Disegna Sole
    c_top = { 1.0, 1.0, 1.0, 1.0 }; //white
    c_bot = { 1.0, 0.9, 0.0, 1.0 };
    disegna_cerchio(320, 640, 30, 30, c_top, c_bot);
    //Disegna Luce attorno sfumata
    c_top = { 1.0, 1.0, 1.0,0 };
    c_bot = { 1.0, 0.9, 0.0 ,1.0 };
    disegna_cerchio(320, 640, 80, 80, c_top, c_bot);


    //Disegna Montagne
    c_top = { 0.5, 0.2, 0.1, 1.0 };
    c_bot = { 1.0, 0.4, 0.05 ,1.0 };
    disegna_montagne(0, height / 2, 100, width, 5, c_top, c_bot);

    //Disegna palla
    disegna_palla(playerPositionX, playerPositionY, distacco_da_terra);

    //Forza il processo di rendering della scena
    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int  main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Animazione 2D");

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);

    //Evento tastiera tasti premuti
    glutKeyboardFunc(keyboardPressedEvent);
    //Evento tastiera tasto rilasciato
    glutKeyboardUpFunc(keyboardReleasedEvent);

    //gestione animazione
    //void glutTimerFunc(unsigned int msecs, void(*func)(int value), value);
    //Description: it registers the timer callback func to be triggered in at least
    //        msecs milliseconds.The value parameter to the timer callback will be the value of
    //        the value parameter to glutTimerFunc.
    glutTimerFunc(66, update, 0);
    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
    );

    init();
    glutMainLoop();
}

