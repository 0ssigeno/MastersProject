//
// Created by simone on 20/03/19.
//


// ParticleSystem.cpp
//

#include <cmath>
#include <time.h>
# include <GL/glew.h>
# include <GL/freeglut.h>
#include <vector>
#include <cstdio>
#include "Particle.h"

using namespace std;
typedef struct  {
    float r;
    float g;
    float b;
} color ;

typedef struct  {
    float x;
    float y;
    float alpha;
    float xFactor;
    float yFactor;
    float drag;
    color colorP;
} PARTICLE;



vector <PARTICLE> particles;
float height = 768.0;

//void setup();
//void drawScene();
//void update(int value);
//void log(int value);
//void resize(int w, int h);
//void myMouse(int button, int state, GLint	xmouse, GLint ymouse);
//void mouseMotion(int x, int	y);
//color computeRainbow();


void setup() {
    float r = 0.0, g = 0.0, b = 0.0;
    glClearColor(r, g, b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1366.0, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setup();
}
color computeRainbow() {
    static float rgb[3] = { 1.0, 0.0, 0.0 };
    static int fase = 0, counter = 0;
    const float step = 0.1;
    color paint;

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
    //fprintf(stdout, "Rosso e verde e blu: %f,%f,%f, counter= %i\n", rgb[0], rgb[1], rgb[2], counter);

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

void myMouse(int button, int state, GLint	xmouse, GLint ymouse)
{
    if (state == GLUT_UP) {
        //ShowCursor(true);
    }
}

void mouseMotion(int x, int	y) {
    color rgb = computeRainbow();
    for (int i = 0; i < 10; i++) {
        PARTICLE p;
        p.x = x;
        p.y = height - y;
        p.alpha = 1.0;
        p.drag = 1.05;
        p.xFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
        p.yFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
        p.colorP.r = rgb.r;
        p.colorP.g = rgb.g;
        p.colorP.b = rgb.b;
        particles.push_back(p);
    }
}

void drawScene() {
    setup();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < particles.size(); i++) {
        particles.at(i).xFactor /= particles.at(i).drag;
        particles.at(i).yFactor /= particles.at(i).drag;

        particles.at(i).x += particles.at(i).xFactor;
        particles.at(i).y += particles.at(i).yFactor;

        particles.at(i).alpha -= 0.05;

        glColor4f(particles.at(i).colorP.r, particles.at(i).colorP.g, particles.at(i).colorP.b, particles.at(i).alpha);
        glVertex2f(particles.at(i).x, particles.at(i).y);

        if (particles.at(i).alpha <= 0.0) {
            particles.erase(particles.begin()+i);
        }
    }
    glEnd();
    glDisable(GL_BLEND);
    glFlush();
}

void update(int value ) {
    glutPostRedisplay();
    glutTimerFunc(20, update, 0);
}

void log(int value) {
    fprintf(stdout, "Particelle in memoria: %i\n", particles.size());
    glutTimerFunc(500, log, 0);
}


int main(int argc, char **argv)
{
    GLboolean	GlewInitResult;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(1366, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sistema Particellare");

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutMouseFunc(myMouse);
    glutTimerFunc(20, update, 0);
    glutTimerFunc(500, log, 0);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    GlewInitResult = glewInit();
    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    setup();
    srand((unsigned)time(NULL));

    glutMainLoop();
    return 0;
}

