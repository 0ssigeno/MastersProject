//
// Created by simone on 20/03/19.
//
#include "stdio.h"
#include <vector>
# include <GL/glew.h>
# include <GL/freeglut.h>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
#define PI 3.14159265358979323846
#define nVertices 48

typedef struct { double r, g, b, a; } ColorRGBA;
typedef struct  {
    float x;
    float y;
    float alpha;
    float xFactor;
    float yFactor;
    float drag;
    ColorRGBA colorP;
} PARTICLE;
typedef struct  {
    float x;
    float y;
    bool  colpito;
    ColorRGBA colorP;
} Brick;


std::vector <PARTICLE> particles;

std::vector <Brick> bricks;



float width = 1280; // dimensioni iniziali della finestra
float height = 720;

bool pressingLeft;
bool pressingRight;

//const float playerW=width/15;
const float playerW=width/10;
const float playerH=height / 30;
float  playerPositionX = (width/2)-(playerW/2);
float  playerPositionY = height / 10;
double playerSpeed = 0;
double playerMaxSpeed = 20;

const float ballRadius=width/100;
float  ballPositionX = (width/2)-(ballRadius/2);
float  ballPositionY = height / 2;
double ballSpeed=0;
double ballMaxSpeed=3;
double ballAngle = 270 ;
double tollerance = 10;



int numMattoncini=10;
int rows=3;
float spaceFromTop=height/20;
float widthBrick= (width / (numMattoncini+1));
float heightBrick= (height / 20);
int bricksHitted=0;
bool wingame=false;
bool endgame=false;

ColorRGBA computeRainbow() {
    static float rgb[3] = { 1.0, 0.0, 0.0 };
    static int fase = 0, counter = 0;
    const float step = 0.1;
    ColorRGBA paint;

    switch (fase) {
        case 0: rgb[1] += step;
            break;
        case 1: rgb[0] -= step;
            break;
        case 2: rgb[2] += step;
            break;
        case 3: rgb[1] -= step;
            break;
        case 4: rgb[0] += step;
            break;
        case 5: rgb[2] -= step;
            break;
        default:
            break;
    }

    counter++;
    if (counter > 1.0 / step) {
        counter = 0;
        fase < 5 ? fase++ : fase = 0;
    }

    paint.r = rgb[0];
    paint.g = rgb[1];
    paint.b = rgb[2];
    return paint;
}
void makeParticles(float x,float y){
    ColorRGBA rgb = computeRainbow();
    for (int i = 0; i < 10; i++) {
        PARTICLE p;
        p.x = x;
        p.y = y;
        p.alpha = 1.0;
        p.drag = 1.25;
        p.xFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
        p.yFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
        p.colorP.r = rgb.r;
        p.colorP.g = rgb.g;
        p.colorP.b = rgb.b;
        particles.push_back(p);
    }
}



void keyboardPressedEvent(unsigned char key, int x, int y){
    switch (key){
        case 'a':
            pressingLeft = true;
            break;

        case 'd':
            pressingRight = true;
            break;

        case 27:
            exit(0);
        case 32: //spazio per partire/pausa
            if(ballSpeed==ballMaxSpeed){
                              ballSpeed=0;
            }else{
                ballSpeed=ballMaxSpeed;

            }
            break;


        default:
            break;
    }
}

void keyboardReleasedEvent(unsigned char key, int x, int y){
    switch (key){
        case 'a':
            pressingLeft = false;
            break;

        case 'd':
            pressingRight = false;
            break;
        default:
            break;
    }
}
void resize(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawBall(float x, float y,float raggio, ColorRGBA colore) {
    int i;
    float stepA = static_cast<float>((2 * PI) / nVertices);

    for (i = 0; i < nVertices; i++)
    {
        glBegin(GL_TRIANGLES);
        glColor4d(colore.r , colore.g , colore.b , colore.a );
        glVertex2f(static_cast<GLfloat>(x + cos((double)i * stepA) * raggio),
                   static_cast<GLfloat>(y + sin((double)i * stepA) * raggio));
        glVertex2f(static_cast<GLfloat>(x + cos((double)(i + 1) * stepA) * raggio),
                   static_cast<GLfloat>(y + sin((double)(i + 1) * stepA) * raggio));
        glVertex2f(x, y);

        glEnd();
    }
}
void drawRectangle(float x, float y, float width, float height, ColorRGBA color_top) {

    glBegin(GL_POLYGON);
    glColor4d(color_top.r , color_top.g , color_top.b, color_top.a );
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void createWall(){
    int r = 0,g=0,b=0,i=0;
    for (int y=1;y<=rows;y+=1) {
        for(float x=0;x<width;x+=widthBrick){
            if (i % 4== 0){
                r =1;
                g =1;
                b=0;
            }else if(i % 4== 1){
                r=1;
                g=0;
                b=0;
            }else if(i % 4== 2){
                r=0;
                g=1;
                b=0;
            }else if(i % 4== 3){
                r=0;
                g=0;
                b=1;
            }

            ColorRGBA brickColor= { float(r),float(g),float(b) ,1.0 };
            Brick brick={x,height-(heightBrick*y)-spaceFromTop,false,brickColor};
            bricks.push_back(brick);
            i++;
        }

    }



}
void drawRainbow(float radius,int startX,int startY) {
    float x2, y2, cx, cy;
    int cache = 0;
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(40.0);
    glBegin(GL_LINES);
    for (int angle = 274; angle < 457; angle += 1) {
        float angleRadiants = angle * 3.1 / 180;
        x2 = radius * sin((double) angleRadiants)+startX;
        y2 = radius * cos((double) angleRadiants)+startY;
        if (cache) {
            glVertex2f(cx, cy);
            glVertex2f(x2, y2);
        }
        cache = 1;
        cx = x2;
        cy = y2;
    }
    glEnd();

}
void drawWall(){
    for (Brick brick : bricks){
        if(!brick.colpito){
            drawRectangle(brick.x,brick.y,widthBrick,heightBrick,brick.colorP);

        }

    }


}
void bitmap_output(int x, int y, int z, string s, void *font){
    int i;
    glRasterPos3f(x, y, 0);

    for (i = 0; i < s.length(); i++)
        glutBitmapCharacter(font, s[i]);
}


void drawScore(){
        glColor3f(1.0, 0.0, 0.0);
        string text = "Bricks Hitted: ";
        text.append(std::to_string(bricksHitted));
        bitmap_output(350, 20, 0, text, GLUT_BITMAP_TIMES_ROMAN_24);
        glutSwapBuffers();


}

void init(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    createWall();



}
void drawParticles(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (unsigned long i = 0; i < particles.size(); i++) {
        particles.at(i).xFactor /= particles.at(i).drag;
        particles.at(i).yFactor /= particles.at(i).drag;

        particles.at(i).x += particles.at(i).xFactor;
        particles.at(i).y += particles.at(i).yFactor;

        particles.at(i).alpha -= 0.05;

        glColor4f(static_cast<GLfloat>(particles.at(i).colorP.r), static_cast<GLfloat>(particles.at(i).colorP.g),
                  static_cast<GLfloat>(particles.at(i).colorP.b), particles.at(i).alpha);
        glVertex2f(particles.at(i).x, particles.at(i).y);

        if (particles.at(i).alpha <= 0.0) {
            particles.erase(particles.begin()+i);
        }
    }
    glEnd();
    glDisable(GL_BLEND);

}
void drawAllrainbow(){
    glColor3ub(148,0,211);
    drawRainbow(300,width/2,180);
    glColor3ub(75,0,130);
    drawRainbow(290,width/2,180);
    glColor3ub(0,0,255);
    drawRainbow(280,width/2,180);
    glColor3ub(0,255,0);
    drawRainbow(270,width/2,180);
    glColor3ub(255,255,0);
    drawRainbow(260,width/2,180);
    glColor3ub(255,127,0);
    drawRainbow(250,width/2,180);
    glColor3ub(255,0,0);
    drawRainbow(240,width/2,180);

}
ColorRGBA playerColor= { 0.25,0.6 ,1.0, 1.0 };
ColorRGBA ballColor= { 1.0,1.0 ,1.0, 1.0 };

void drawScene(){

    //Definisce il colore con cui verrà  presentato lo schermo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3d(0,0,0);
    glVertex2f(0, 0);
    glVertex2f(0,width);

    glVertex2f(height,width);
    glVertex2f(height,0);
    glEnd();
    if(endgame|| wingame){

        drawAllrainbow();
        glColor3f(1.0, 0.0, 0.0);
        string text = "FINAL SCORE: ";
        text.append(std::to_string(bricksHitted));
        bitmap_output(width/2-90,300 , 0, text, GLUT_BITMAP_TIMES_ROMAN_24);
        glutSwapBuffers();
    }
    else{
        drawAllrainbow();

        drawWall();
        drawScore();

        drawRectangle(playerPositionX, playerPositionY, playerW ,playerH , playerColor);

        drawBall(ballPositionX,ballPositionY,ballRadius,ballColor);
        drawParticles();

    }

    //Forza il processo di rendering della scena
    glFlush();
}
void endGame(){
    endgame=true;
}


void checkAngle(){
    if(ballAngle>360){
        ballAngle-=360;

    }
    if(ballAngle<0){
        ballAngle+=360;
    }

    double radians=ballAngle* PI /180;
    double diffX=cos(radians)*ballSpeed;
    double diffY=sin(radians)*ballSpeed ;

    ballPositionX+=diffX;
    ballPositionY+=diffY;
}
void checkPlayerMovement(){
    bool moving = false;

    if (pressingLeft)    {
        playerSpeed = (-playerMaxSpeed);
        moving = true;
    }
    if (pressingRight)
    {
        playerSpeed = playerMaxSpeed;
        moving = true;
    }

    if (!moving) {
        if (playerSpeed != 0){
            playerSpeed = 0;

        }
    }
    playerPositionX += playerSpeed;

    if (playerPositionX < 0) {
        playerPositionX = 0;}
    if (playerPositionX > (width-playerW)) {
        playerPositionX = width-playerW;

    }

}
void checkReboundSides(){
    if(ballAngle>=0 && ballAngle<=90){
        if(ballPositionX>width-ballRadius){
            ballAngle+=60;
        }if(ballPositionY>height-ballRadius){
            ballAngle-=90;
        }

    }
    else if(ballAngle>=90 && ballAngle<=180){
        if(ballPositionX<0){
            ballAngle-=60;
        }if(ballPositionY>height-ballRadius){
            ballAngle+=90;

        }
    }
    else if(ballAngle>=180 && ballAngle<=270){
        if(ballPositionX<0){
            ballAngle+=60;
        }if(ballPositionY<0){
            endGame();
        }
    }
    else if(ballAngle>=270 && ballAngle<=360){
        if(ballPositionX>width-ballRadius){
            ballAngle-=60;
        }if(ballPositionY<0){
            endGame();
        }
    }
    checkAngle();
}

float lerp(float a1,float b1, float t){
    return((1-t)*a1 -t*b1);
}

void playerBallAngle(){
    float lungTouched=ballPositionX-playerPositionX;
    float rapp=lungTouched/playerW;
    ballAngle=abs(lerp(160,10,rapp));

}

void checkPlayerBall(){
    if((ballPositionX>=playerPositionX -tollerance )&&(ballPositionX<=playerPositionX +playerW + tollerance) && (ballPositionY >=playerPositionY - tollerance) && (ballPositionY<=playerPositionY +playerH+ tollerance)){
        for(float i=playerPositionX; i < playerPositionX+playerW; i+=15){
            makeParticles(i,playerPositionY);
        }
        playerBallAngle();

    }
    checkAngle();
}

void checkBallBricks(){
    for (auto &brick : bricks) {
        if((ballPositionX>= brick.x)&&(ballPositionX<= brick.x +widthBrick) && (ballPositionY >= brick.y ) && (ballPositionY<=brick.y+heightBrick) && !brick.colpito) {
            brick.colorP={0.0,0.0,0.0,1.0};
            brick.colpito=true;
            ballAngle+=90;
            bricksHitted+=1;
        }

    }

    checkAngle();
}
bool checkWin(){
    for (auto &brick : bricks) {
        if(!brick.colpito){
            return false;
        }

    }
    return true;
}
void update(int unused){
    checkPlayerMovement();
    checkReboundSides();
    checkPlayerBall();
    checkBallBricks();
    wingame=checkWin();
    glutPostRedisplay();
    glutTimerFunc(22, update, 0);
}

int  main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(static_cast<int>(width), static_cast<int>(height));
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Arkanoid");

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);

    //Evento tastiera tasti premuti
    glutKeyboardFunc(keyboardPressedEvent);
    //Evento tastiera tasto rilasciato
    glutKeyboardUpFunc(keyboardReleasedEvent);
    glutTimerFunc(22, update, 0);
    init();
    glutMainLoop();


}
