//
// Created by simone on 18/03/19.
//

#include "lab1.h"
/*
 * Lab-01-1819_students.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Usage:
 *   Left click to place a control point.
 *		Maximum number of control points allowed is currently set at 64.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *	 Press escape to exit.
 */

#include <stdlib.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define MaxNumPts 64
float PointArray[MaxNumPts][3];
int NumPts = 0;

// Window size in pixels
int WindowHeight;
int WindowWidth;



const int dimArray=3;
int mouseOver=-1;
bool isMoving;
int movingPoint;

/* Prototypes */
void addNewPoint(float x, float y);
void removeFirstPoint();
void removeLastPoint();
void drawBezierGL();
void motion(int x, int y);
void passive(int x, int y);
void castelAlgo();
void drawBezierCastel();


void myKeyboardFunc (unsigned char key, int x, int y)
{
    switch (key) {
        case 'f':
            removeFirstPoint();
            glutPostRedisplay();
            break;
        case 'l':
            removeLastPoint();
            glutPostRedisplay();
            break;

        case 27:			// Escape key
            exit(0);

    }
}





//implementazione standard bezier
void drawBezierGL() {

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NumPts, &PointArray[0][0]);

    // Draw the curve
    glBegin(GL_LINE_STRIP);  // Use GL_LINE_STRIP instead to fill the gaps
    {
        for (int i = 0; i < 100; i++)
        {
            glEvalCoord1f(((float)i) / 100.0);
        }
    }
    glEnd();
}
//calcolo lerp
void lerp( float* p1, float* p2, float t, float* ret) {
    ret[0] = ( (1-t) * p1[0] ) + ( t * p2[0]);
    ret[1] = ( (1-t) * p1[1] ) + ( t * p2[1]);
    ret[2] = ( (1-t) * p1[2] ) + ( t * p2[2]);
}
//sono pigro
void assignArray(float* p1, float* p2){
    p1[0]=p2[0];
    p1[1]=p2[1];
    p1[2]=p2[2];
}

//implementazione casteljau
void castelAlgo(float val, float resu[]) {
    //temp dei punti
    float tempArray[MaxNumPts][3];
    for (int i = 0; i < NumPts; i++) {
        assignArray(tempArray[i],PointArray[i]);
    }
    //algo
    for (int i = 1; i < NumPts; i++) {
        for (int j = 0; j < NumPts - i; j++) {
            lerp(tempArray[j],tempArray[j+1],val,tempArray[j]);

        }
    }
    assignArray(resu,tempArray[0]);

}
//calcolo distanza due punti
float pointPointDistance(float *p1, float *p2) {
    return sqrtf(powf(p1[0] - p2[0], 2) + powf(p1[1] - p2[1], 2));
}

//calcolo distanza punto linea
float pointLineDistance(float *p0, float *p1, float *p2) {
    return fabsf(
            ( (p2[0] - p1[0]) * (p1[1] - p0[1]) ) // (x2 - x1) * (y1 - y0)
            - ( (p1[0] - p0[0] ) * (p2[1] - p1[1]) ) // - (x1 - x0) * (p2 - p1)
    ) / pointPointDistance(p1, p2);
}

//implementazione algoritmo addattivo
void adaptiveSubdivision(float points[MaxNumPts][dimArray], float tolerance) {
    int i, j;
    float temp[NumPts][dimArray];
    float curve1[NumPts][dimArray];
    float curve2[NumPts][dimArray];
    bool canApproxLine = true;

    for (i = 1; i < NumPts - 1; i++) {
        if(pointLineDistance(points[i], points[0], points[NumPts - 1]) > tolerance) {
            canApproxLine = false;
        }
    }
    if(canApproxLine) {
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[NumPts - 1][0], points[NumPts - 1][1], points[NumPts - 1][2]);

    } else {

        for (i = 0; i < NumPts; i++) {
            assignArray(temp[i],points[i]);
        }
        assignArray(curve1[0], temp[0]);
        assignArray(curve2[NumPts - 1], temp[NumPts - 1]);

        for (i = 1; i < NumPts; i++) {
            for (j = 0; j < NumPts - i; j++) {
                lerp(temp[j], temp[j+1], 0.5f, temp[j]);
            }
            assignArray(curve1[i], temp[0]);
            assignArray(curve2[NumPts - i - 1], temp[NumPts - i - 1]);

        }
        adaptiveSubdivision(curve1, tolerance);
        adaptiveSubdivision(curve2, tolerance);
    }

}


//invocazione bezier adattivo
void drawBezierAdaptive(){
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NumPts, &PointArray[0][0]);
    glBegin(GL_LINE_STRIP);  // Use GL_LINE_STRIP instead to fill the gaps
    adaptiveSubdivision(PointArray,0.0005);
    glEnd();
}


//implementazione bezier
void drawBezierCastel() {

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NumPts, &PointArray[0][0]);

    // Draw the curve
    glBegin(GL_LINE_STRIP);  // Use GL_LINE_STRIP instead to fill the gaps
    float result[3];
    int val = 10000;
    for (int i = 0; i < val; i++)
    {
        castelAlgo(((float)i) / (float)val,result);
        glVertex3f(result[0], result[1], 0.0);

    }

    glEnd();
}


void removeFirstPoint() {
    int i;
    printf("RemoveF\n");
    if ( NumPts>0 ) {
        // Remove the first point, slide the rest down
        NumPts--;
        for ( i=0; i<NumPts; i++ ) {
            PointArray[i][0] = PointArray[i+1][0];
            PointArray[i][1] = PointArray[i+1][1];
            PointArray[i][2] = 0;
        }
    }
}

//controllo se il mouse e' sopra ad un punto
void passive(int x, int y) {
    float xp = (float)x / ((float)(WindowWidth - 1));
    float yp = (float)(WindowHeight - 1 - y) / ((float)(WindowHeight - 1));
    float appross = 0.03;
    for (int i=0; i< NumPts; i++){
        float dist = sqrt(pow(PointArray[i][0] - xp, 2) + pow(PointArray[i][1] - yp, 2));
        if (dist < appross) {
            mouseOver = i;
            glutPostRedisplay();
            return;
        }
        else {
            mouseOver = -1;
        }
    }
    glutPostRedisplay();
}

//spostamento di un punto
void motion(int x, int y){

    if (isMoving) {
        float xPos = ((float)x) / ((float)(WindowWidth - 1));
        float yPos = (float)(WindowHeight - 1 - y) / ((float)(WindowHeight - 1));
        PointArray[movingPoint][0] = xPos;
        PointArray[movingPoint][1] = yPos;
        glutPostRedisplay();
    }
}


// Left button presses place a control point.
void myMouseFunc( int button, int state, int x, int y ) {
    if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ){

        if (mouseOver != -1) {
            isMoving = true;
            movingPoint = mouseOver;
        }
        else {
            //SIMO prende le posizioni x e y dove � stato clickato il mouse
            float xPos = ((float)x) / ((float)(WindowWidth - 1));
            float yPos = ((float)y) / ((float)(WindowHeight - 1));

            yPos = 1.0f - yPos;			// Flip value since y position is from top row.

            addNewPoint(xPos, yPos);
            glutPostRedisplay();
        }


    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isMoving = false;
    }

}

// Add a new point to the end of the list.
// Remove the first point in the list if too many points.
void removeLastPoint() {
    printf("RemoveL\n");
    if ( NumPts>0 ) {
        NumPts--;
    }
}

// Add a new point to the end of the list.
// Remove the first point in the list if too many points.
void addNewPoint( float x, float y ) {
    if ( NumPts>=MaxNumPts ) {
        removeFirstPoint();
    }
    PointArray[NumPts][0] = x;
    PointArray[NumPts][1] = y;
    PointArray[NumPts][2] = 0;
    NumPts++;

}

void display(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2);//dimensione linea
    // Draw the line segments

    if ( NumPts>1 ) {
        glColor3f(0.0f, 0.5f, 0.8f);			// Reddish/purple lines


        glBegin( GL_LINE_STRIP );

        for ( i=0; i<NumPts; i++ ) {
            glVertex2f( PointArray[i][0], PointArray[i][1] );
        }
        glEnd();
    }




    // Draw the interpolated points second.
    glColor3f( 0.0f, 0.0f, 0.0f);			// Draw points in black
    //modificare QUA per gli altri algoritmi
    //drawBezierCastel();
   drawBezierAdaptive();
   glBegin( GL_POINTS );

    for ( i=0; i<NumPts; i++ ) {
        glVertex2f( PointArray[i][0], PointArray[i][1] );
    }
    glEnd();
    if (mouseOver != -1) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3f(PointArray[mouseOver][0], PointArray[mouseOver][1], PointArray[mouseOver][2]);
        glEnd();

    }
    glFlush();
}

void initRendering() {
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    // Make big points and wide lines.  (This may be commented out if desired.)
    glPointSize(10);//SIMO dimensione punti


    // The following commands should induce OpenGL to create round points and
    //	antialias points and lines.  (This is implementation dependent unfortunately, and
    //  may slow down rendering considerably.)
    //  You may comment these out if you wish.
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MAP1_VERTEX_3);


}

void resizeWindow(int w, int h)
{
    WindowHeight = (h>1) ? h : 2;
    WindowWidth = (w>1) ? w : 2;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    initRendering();

    glutDisplayFunc(display);
    glutReshapeFunc(resizeWindow);
    glutKeyboardFunc(myKeyboardFunc);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(motion); //drag
    glutPassiveMotionFunc(passive); //passive motion

    glutMainLoop();

    return 0;					// This line is never reached
}
