/******************************************************************************************
LAB 03
Gestione interattiva di una scena 3D mediante controllo da mouse e da tastiera.
I modelli geometrici in scena sono mesh poligonali in formato *.obj

CTRL+WHEEL = pan orizzontale della telecamera
SHIFT+WHEEL = pan verticale della telecamera
WHELL = ZOOM IN/OUT se si è in navigazione, altrimenti agisce sulla trasformazione dell'oggetto
g r s	per le modalità di lavoro: traslate/rotate/scale
x y z	per l'asse di lavoro
wcs/ocs selezionabili dal menu pop-up

OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software 
based on the OpenGL Shading Language (GLSL) specifications.
*******************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS // for fscanf
#include <unistd.h>

#include <stdio.h>
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "HUD_Logger.h"

//#define SHIFT_WHEEL_UP 11
//#define SHIFT_WHEEL_DOWN 12
//#define CTRL_WHEEL_UP 19
//#define CTRL_WHEEL_DOWN 20

#define SHIFT_WHEEL_UP 3
#define SHIFT_WHEEL_DOWN 4
#define CTRL_WHEEL_UP 3
#define CTRL_WHEEL_DOWN 4

using namespace std;

static int WindowWidth = 1366;
static int WindowHeight = 768;
GLfloat aspect_ratio = 16.0f / 9.0f;

typedef struct {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLushort> indices;
    GLuint vertexArrayObjID;
    GLuint vertexBufferObjID;
    GLuint normalBufferObjID;
    GLuint indexBufferObjID;
} Mesh;

typedef enum {
    RED_PLASTIC,
    EMERALD,
    BRASS,
    SLATE
} MaterialType;

typedef struct {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shiness;
} Material;

typedef struct {
    Mesh mesh;
    MaterialType material;
    GLfloat model_matrix[16];
    string name;
} Object;



static int selectedObject=0;
const string MeshDir = "/home/ossigeno/University/lab3/Mesh/"; //PATH ASSOLUTO CON / FINALE
static vector<Object> objects;
static vector<Material> materials;

// Materiali disponibili
glm::vec3 red_plastic_ambient = { 0.0, 0.0, 0.0 }, red_plastic_diffuse = { 0.5, 0.0, 0.0 }, red_plastic_specular = { 0.7, 0.6, 0.6 }; GLfloat red_plastic_shininess = 32.0;
glm::vec3 brass_ambient = { 0.33, 0.22, 0.03 }, brass_diffuse = { 0.78, 0.57, 0.11 }, brass_specular = { 0.99, 0.91, 0.81 }; GLfloat brass_shininess = 27.8;
glm::vec3 emerald_ambient = { 0.0215, 0.1745, 0.0215 }, emerald_diffuse = { 0.07568, 0.61424, 0.07568 }, emerald_specular = { 0.633, 0.727811, 0.633 }; GLfloat emerald_shininess = 78.8;
glm::vec3 slate_ambient = { 0.02, 0.02, 0.02 }, slate_diffuse = { 0.02, 0.01, 0.01 }, slate_specular{ 0.4, 0.4, 0.4 }; GLfloat slate_shiness = .78125f;

static glm::vec4 lightPos = { 5.0f, 5.0f, 5.0f, 1.0f };



/*camera structures*/
constexpr float CAMERA_ZOOM_SPEED = 0.1f;
constexpr float CAMERA_TRASLATION_SPEED = 0.01f;

void cameraMotion();

struct {
    glm::vec4 position;
    glm::vec4 target;
    glm::vec4 upVector;
} ViewSetup;

struct {
    float fovY, aspect, near_plane, far_plane;
} PerspectiveSetup;

typedef enum {
    WIRE_FRAME,
    FACE_FILL,
    FLAT_SHADING,
    SMOOTH_SHADING,
    CULLING_ON,
    CULLING_OFF,
    CHANGE_TO_WCS,
    CHANGE_TO_OCS,
    MOVEMENT
} MenuOption;

enum {
    NAVIGATION,
    CAMERA_MOVING,
    TRASLATING,
    ROTATING,
    SCALING
} OperationMode;

enum {
    X,
    Y,
    Z
} WorkingAxis;

enum {
    OCS, // Object Coordinate System
    WCS // World Coordinate System
} TransformMode;

static bool moving_trackball = 0;
static int last_mouse_pos_Y;
static int last_mouse_pos_X;

void drawOne(int i);
// inizializzazione generale
void init();
void display();
void resize(int w, int h);
void mouseClick(int button, int state, int x, int y);
// Keyboard:  g traslate r rotate s scale x,y,z axis esc 
void keyboardDown(unsigned char key, int x, int y);
// Special key arrow: select active object (arrows left,right)
void special(int key, int x, int y);
// gestione delle voci principali del menu
void main_menu_func(int option);
// gestione delle voci principali del sub menu per i matriali
void material_menu_function(int option); //TODO
// costruisce i menu openGL
void buildOpenGLMenu();
// Trackball: Converte un punto 2D sullo schermo in un punto 3D sulla trackball
glm::vec3 getTrackBallPoint(float x, float y);
// Trackball: Effettua la rotazione del vettore posizione sulla trackball
void mouseActiveMotion(int x, int y);
void moveCameraForward();
void moveCameraBack();
void moveCameraLeft();//TODO
void moveCameraRight();//TODO
void moveCameraUp();//TODO
void moveCameraDown();//TODO
/*
	//TODO
	Crea ed applica la matrice di trasformazione alla matrice dell'oggeto discriminando tra WCS e OCS.
	La funzione è gia invocata con un input corretto, è sufficiente concludere la sua implementazione.
*/
void modifyModelMatrix(glm::vec4 translation_vector, glm::vec4 rotation_vector, GLfloat angle, GLfloat scale_factor);
/* Mesh Functions*/
// Genera i buffer per la mesh in input e ne salva i puntatori di openGL
void generate_and_load_buffers(Mesh* mesh);
// legge un file obj ed inizializza i vector della mesh in input
void loadObjFile(string file_path, Mesh* mesh);
// disegna l'origine del assi
void drawAxis(float scale, int drawLetters);
// disegna la griglia del piano xz (white)
void drawGrid(float scale, int dimension);
/*Logging to screen*/
void printToScreen();
//My functions and variables

bool amImoving=false;
const int bezierNumber=5;
float camTimer=0.0f;
const glm::vec3 bezierPoints[bezierNumber] = {
        { 0.0f, 7.0f, 0.000000f },
        { -17.0f, -3.0f, 0.000000f },
        { 16.0f, -26.0f, 0.000000f },
        { 18.0f, 15.0f, 0.000000f },
        { 0.0f, 7.0f, 0.000000f }
};

void lerp( float* p1, float* p2, float t, float* ret);
void drawOne(int i);
void cameraMotion();


int main(int argc, char** argv) {
    GLboolean GlewInitResult;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Model Viewer ");

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboardDown);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseActiveMotion);
    glutSpecialFunc(special);

    glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) {
        fprintf(
                stderr,
                "ERROR: %s\n",
                glewGetErrorString(GlewInitResult)
        );
        exit(EXIT_FAILURE);
    }
    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
    );
    init();
    buildOpenGLMenu();


    glutMainLoop();

    return 1;
}


void createMesh(string name){
    Mesh plane = {};
    loadObjFile(MeshDir + name+".obj", &plane); //SIMO caricamento
    generate_and_load_buffers(&plane);

    // Object Setup
    Object obj1 = {};
    obj1.mesh = plane;
    obj1.material = MaterialType::RED_PLASTIC;
    obj1.name = name;
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, obj1.model_matrix);
    //object = obj1;
    objects.push_back(obj1);
}

void init() {
// Default render settings
    OperationMode = NAVIGATION;

    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glCullFace(GL_BACK);	// remove faces facing the background

    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_FLAT);
    //FOG Setup for nice backgound transition
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    GLfloat fog_color[4] = { 0.5,0.5,0.5,1.0 };
    glFogfv(GL_FOG_COLOR, fog_color);
    glFogf(GL_FOG_START, 50.0f);
    glFogf(GL_FOG_END, 500.0f);
    // Light Setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    // Materials setup
    materials.resize(4);
    materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
    materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
    materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
    materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
    materials[MaterialType::RED_PLASTIC].shiness = red_plastic_shininess;

    materials[MaterialType::EMERALD].name = "Emerald";
    materials[MaterialType::EMERALD].ambient = emerald_ambient;
    materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
    materials[MaterialType::EMERALD].specular = emerald_specular;
    materials[MaterialType::EMERALD].shiness = emerald_shininess;

    materials[MaterialType::BRASS].name = "Brass";
    materials[MaterialType::BRASS].ambient = brass_ambient;
    materials[MaterialType::BRASS].diffuse = brass_diffuse;
    materials[MaterialType::BRASS].specular = brass_specular;
    materials[MaterialType::BRASS].shiness = brass_shininess;

    materials[MaterialType::SLATE].name = "Slate";
    materials[MaterialType::SLATE].ambient = slate_ambient;
    materials[MaterialType::SLATE].diffuse = slate_diffuse;
    materials[MaterialType::SLATE].specular = slate_specular;
    materials[MaterialType::SLATE].shiness = slate_shiness;

    // Camera Setup
    ViewSetup = {};
    ViewSetup.position = glm::vec4(10.0, 10.0, 10.0, 1.0);
    ViewSetup.target = glm::vec4(0.0, 0.0, 0.0, 1.0);
    ViewSetup.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);
    PerspectiveSetup = {};
    PerspectiveSetup.aspect = (GLfloat)WindowWidth / (GLfloat)WindowHeight;
    PerspectiveSetup.fovY = 45.0f;
    PerspectiveSetup.far_plane = 2000.0f;
    PerspectiveSetup.near_plane = 1.0f;

    // Mesh Loading

    createMesh("sphere");
    createMesh("airplane");
    createMesh("hand");
    createMesh("triceratops");
    createMesh("bunny");
    createMesh("horse");
    drawOne(0);


}
void drawOne(int i){
    glPushMatrix();
  glMultMatrixf(objects[i].model_matrix);
  glDisable(GL_LIGHTING);
  drawAxis(1.0, 0);
  glEnable(GL_LIGHTING);

  //Material Setup
  Material mat = materials[objects[i].material];
  glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(mat.ambient));
  glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(mat.diffuse));
  glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(mat.specular));
  glMaterialf(GL_FRONT, GL_SHININESS, mat.shiness);

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindVertexArray(objects[i].mesh.vertexArrayObjID);
  glDrawElements(GL_TRIANGLES, objects[i].mesh.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
  glPopMatrix();


}

void display() {
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //Static scene elements

    glDisable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lightPos));
    //glPushMatrix();
    //glTranslatef(lightPos.x, lightPos.y, lightPos.z);
    //glColor4d(1, 1, 1, 1);
    //glutSolidSphere(0.01, 10, 10); // Ligh ball
    //glPopMatrix();

    drawAxis(3.0, 1); // The central Axis point of reference
    drawGrid(10.0, 100); // The horizontal grid
    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    drawOne(selectedObject);
    //drawMany();


    printToScreen();

    glutSwapBuffers();

}

void resize(int w, int h)
{
    if (h == 0)	// Window is minimized
        return;
    int width = h * aspect_ratio;           // width is adjusted for aspect ratio
    int left = (w - width) / 2;
    // Set Viewport to window dimensions
    glViewport(left, 0, width, h);
    WindowWidth = w;
    WindowHeight = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(PerspectiveSetup.fovY, PerspectiveSetup.aspect, PerspectiveSetup.near_plane, PerspectiveSetup.far_plane);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ViewSetup.position.x, ViewSetup.position.y, ViewSetup.position.z,
              ViewSetup.target.x, ViewSetup.target.y, ViewSetup.target.z,
              ViewSetup.upVector.x, ViewSetup.upVector.y, ViewSetup.upVector.z);
}

void mouseClick(int button, int state, int x, int y)
{
    glutPostRedisplay();
    int modifiers = glutGetModifiers();
    if (modifiers == GLUT_ACTIVE_SHIFT) {
        switch (button)
        {
            case SHIFT_WHEEL_UP: moveCameraUp(); break;
            case SHIFT_WHEEL_DOWN: moveCameraDown(); break;

        }
        return;
    }
    if (modifiers == GLUT_ACTIVE_CTRL) {
        switch (button)
        {
            case CTRL_WHEEL_UP: moveCameraRight(); break;
            case CTRL_WHEEL_DOWN: moveCameraLeft(); break;
        }
        return;
    }

    glm::vec4 axis;
    float amount = 0.1f;
    // Imposto il valore della trasformazione
    switch (button) {
        case 3:// scroll wheel up
            amount *= 1;
            break;
        case 4:// scroll wheel down
            amount *= -1;
            break;
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) { moving_trackball = true; }
            if (state == GLUT_UP) { moving_trackball = false; }
            OperationMode = NAVIGATION;
            last_mouse_pos_X = x;
            last_mouse_pos_Y = y;
            break;
        default:
            break;
    }
    // Selezione dell'asse per le trasformazioni
    switch (WorkingAxis) {
        case X:	axis = glm::vec4(1.0, 0.0, 0.0, 0.0);
            break;
        case Y: axis = glm::vec4(0.0, 1.0, 0.0, 0.0);
            break;
        case Z: axis = glm::vec4(0.0, 0.0, 1.0, 0.0);
            break;
        default:
            break;
    }

    switch (OperationMode) {
        case TRASLATING:
            modifyModelMatrix(axis * amount, axis, 0.0f, 1.0f);
            break;
        case ROTATING:
            modifyModelMatrix(glm::vec4(0), axis, amount * 20.0f, 1.0f);
            break;
        case SCALING:
            //TODO scaling tutti gli assi o uno solo?
            modifyModelMatrix(glm::vec4(0), axis, 0.0f, 1.0+ amount);
            break;
        case NAVIGATION:
            // Wheel reports as button 3(scroll up) and button 4(scroll down)
            if (button == 3) {
                moveCameraBack();
            }
            else if (button == 4) {
                moveCameraForward();
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void mouseActiveMotion(int x, int y)
{
    // Spostamento su trackball del vettore posizione Camera
    if (!moving_trackball) {
        return;
    }
    glm::vec3 destination = getTrackBallPoint(x, y);
    glm::vec3 origin = getTrackBallPoint(last_mouse_pos_X, last_mouse_pos_Y);
    float dx, dy, dz;
    dx = destination.x - origin.x;
    dy = destination.y - origin.y;
    dz = destination.z - origin.z;
    if (dx || dy || dz) {
        // rotation angle = acos( (v dot w) / (len(v) * len(w)) ) o approssimato da ||dest-orig||;
        float pi = glm::pi<float>();
        float angle = sqrt(dx * dx + dy * dy + dz * dz) * (180.0 / pi);
        // rotation axis = (dest vec orig) / (len(dest vec orig))
        glm::vec3 rotation_vec = glm::cross(origin, destination);
        // calcolo del vettore direzione w = C - A
        glm::vec4 direction = ViewSetup.position - ViewSetup.target;
        // rotazione del vettore direzione w
        // determinazione della nuova posizione della camera
        ViewSetup.position = ViewSetup.target + glm::rotate(glm::mat4(1.0f), glm::radians(-angle), rotation_vec) * direction;
    }
    last_mouse_pos_X = x; last_mouse_pos_Y = y;
    glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y)
{
    switch (key) {
        // Selezione della modalità di trasformazione
        case 'g':
            OperationMode = TRASLATING;
            break;
        case 'r':
            OperationMode = ROTATING;
            break;
        case 's':
            OperationMode = SCALING;
            break;
        case 27:
            glutLeaveMainLoop();
            break;
            // Selezione dell'asse
        case 'x':
            WorkingAxis = X;
            break;
        case 'y':
            WorkingAxis = Y;
            break;
        case 'z':
            WorkingAxis = Z;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            if (selectedObject==0){
                selectedObject=objects.size();
            }
            selectedObject--;
            drawOne(selectedObject);
            break;
        case GLUT_KEY_RIGHT:
            if (selectedObject==objects.size()-1){
                selectedObject=0;
            }else {
                selectedObject++;
            }
            drawOne(selectedObject);
        break;
        default:
            break;
    }
    glutPostRedisplay();
}

void main_menu_func(int option)
{
    switch (option)
    {
        case MenuOption::FLAT_SHADING: glShadeModel(GL_FLAT);
            break;
        case MenuOption::SMOOTH_SHADING: glShadeModel(GL_SMOOTH);
            break;
        case MenuOption::WIRE_FRAME: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case MenuOption::FACE_FILL: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case MenuOption::CULLING_ON: glEnable(GL_CULL_FACE);
            break;
        case MenuOption::CULLING_OFF: glDisable(GL_CULL_FACE);
            break;
        case MenuOption::CHANGE_TO_OCS: TransformMode = OCS;
            break;
        case MenuOption::CHANGE_TO_WCS: TransformMode = WCS;
            break;
        case MenuOption::MOVEMENT:
            if(amImoving){
                glutIdleFunc(NULL);
               amImoving=false;
            }
            else{
                glutIdleFunc(cameraMotion);
                amImoving=true;
            }
            break;
        default:
            break;
    }
}
void material_menu_function(int option)
{
    switch (option){
        case MaterialType::RED_PLASTIC  : objects[selectedObject].material=MaterialType::RED_PLASTIC;
            break;
        case MaterialType::EMERALD: objects[selectedObject].material=MaterialType::EMERALD;
            break;
        case MaterialType::BRASS: objects[selectedObject].material=MaterialType::BRASS;
            break;
        case MaterialType::SLATE: objects[selectedObject].material=MaterialType::SLATE;
            break;

        default:
            break;
    }
}

void buildOpenGLMenu()
{
    int materialSubMenu = glutCreateMenu(material_menu_function);

    glutAddMenuEntry(materials[MaterialType::RED_PLASTIC].name.c_str(), MaterialType::RED_PLASTIC);
    glutAddMenuEntry(materials[MaterialType::EMERALD].name.c_str(), MaterialType::EMERALD);
    glutAddMenuEntry(materials[MaterialType::BRASS].name.c_str(), MaterialType::BRASS);
    glutAddMenuEntry(materials[MaterialType::SLATE].name.c_str(), MaterialType::SLATE);


    glutCreateMenu(main_menu_func); // richiama main_menu_func() alla selezione di una voce menu
    glutAddMenuEntry("Opzioni", -1); //-1 significa che non si vuole gestire questa riga
    glutAddMenuEntry("", -1);
    glutAddMenuEntry("Wireframe", MenuOption::WIRE_FRAME);
    glutAddMenuEntry("Face fill", MenuOption::FACE_FILL);
    glutAddMenuEntry("Smooth Shading", MenuOption::SMOOTH_SHADING);
    glutAddMenuEntry("Flat Shading", MenuOption::FLAT_SHADING);
    glutAddMenuEntry("Culling: ON", MenuOption::CULLING_ON);
    glutAddMenuEntry("Culling: OFF", MenuOption::CULLING_OFF);
    glutAddSubMenu("Material", materialSubMenu);
    glutAddMenuEntry("World coordinate system", MenuOption::CHANGE_TO_WCS);
    glutAddMenuEntry("Object coordinate system", MenuOption::CHANGE_TO_OCS);
    glutAddMenuEntry("Move around object", MenuOption::MOVEMENT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

glm::vec3 getTrackBallPoint(float x, float y)
{
    float zTemp;
    glm::vec3 point;
    //map to [-1;1]
    point.x = (2.0f * x - WindowWidth) / WindowWidth;
    point.y = (WindowHeight - 2.0f * y) / WindowHeight;

    zTemp = 1.0f - pow(point.x, 2.0) - pow(point.y, 2.0);
    point.z = (zTemp > 0.0f) ? sqrt(zTemp) : 0.0;
    return point;
}



void lerp( float* p1, float* p2, float t, float* ret) {
    ret[0] = ( (1-t) * p1[0] ) + ( t * p2[0]);
    ret[1] = ( (1-t) * p1[1] ) + ( t * p2[1]);
    ret[2] = ( (1-t) * p1[2] ) + ( t * p2[2]);
}
void cameraMotion(){


    float tmp[bezierNumber][3];

    for (int i = 0; i < bezierNumber; i++) {
        tmp[i][0]=bezierPoints[i][0];
        tmp[i][1]=bezierPoints[i][1];
        tmp[i][2]=bezierPoints[i][2];

    }

    for (int i = 1; i < bezierNumber; i++) {
        for (int j = 0; j < bezierNumber - i; j++) {
            lerp(tmp[j], tmp[j + 1], camTimer, tmp[j]);
        }
    }
    ViewSetup.position[0] = tmp[0][0];
    ViewSetup.position[2]= tmp[0][1];

    if ( camTimer + CAMERA_TRASLATION_SPEED <= 1.0f ) {
        camTimer += CAMERA_TRASLATION_SPEED;
    } else {
        camTimer = 0.0f;
    }

    glutPostRedisplay();
}



void moveCameraForward()
{
    float v[3];
    float tv[3];
    float origin[3] = {0, 0, 0};
    float t;
    t = CAMERA_ZOOM_SPEED * (float) 1;

    for(int i=0;i<3;i++){
        v[i]=origin[i]-ViewSetup.position[i];
    }
    for(int i=0;i<3;i++){
        tv[i]=v[i]*t;
    }

    for(int i=0;i<3;i++){
        ViewSetup.position[i]=tv[i]+ViewSetup.position[i];
    }


}

void moveCameraBack()
{   float v[3];
    float tv[3];
    float t;
    t = CAMERA_ZOOM_SPEED * (float) -1;
    for(int i=0;i<3;i++){
        v[i]=ViewSetup.target[i]-ViewSetup.position[i];
    }
    for(int i=0;i<3;i++){
        tv[i]=v[i]*t;
    }
    for(int i=0;i<3;i++){
        ViewSetup.position[i]+=tv[i];
    }

}

void moveCameraLeft()
{
    glm::vec4 vec={1.0,0.0,-1.0,0.0};
    ViewSetup.position+=vec;
    ViewSetup.target+=vec;
}

void moveCameraRight()
{
    glm::vec4 vec={1.0,0.0,-1.0,0.0};
    ViewSetup.position-=vec;
    ViewSetup.target-=vec;
}

void moveCameraUp()
{
    glm::vec4 vec={0.0,1.0,0.0,0.0};
    ViewSetup.position+=vec;
    ViewSetup.target+=vec;
}

void moveCameraDown()
{
    glm::vec4 vec={0.0,1.0,0.0,0.0};
    ViewSetup.position-=vec;
    ViewSetup.target-=vec;
}

void modifyModelMatrix(glm::vec4 translation_vector, glm::vec4 rotation_vector, GLfloat angle, GLfloat scale_factor)
{

    glPushMatrix();
    glLoadIdentity();
    switch (TransformMode) {
        case WCS:
            glTranslatef(translation_vector[0],translation_vector[1],translation_vector[2]);
            glRotatef(angle,rotation_vector[0],rotation_vector[1],rotation_vector[2]);
            glScalef(scale_factor,scale_factor,scale_factor);
            glMultMatrixf(objects[selectedObject].model_matrix);
            break;
        case OCS:
            glMultMatrixf(objects[selectedObject].model_matrix);
            glTranslatef(translation_vector[0],translation_vector[1],translation_vector[2]);
            glRotatef(angle,rotation_vector[0],rotation_vector[1],rotation_vector[2]);
            glScalef(scale_factor,scale_factor,scale_factor);
            break;
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, objects[selectedObject].model_matrix);
    glPopMatrix();


}

void generate_and_load_buffers(Mesh* mesh)
{
    // Genero 1 Vertex Array Object
    glGenVertexArrays(1, &mesh->vertexArrayObjID);
    glBindVertexArray(mesh->vertexArrayObjID);

    // Genero 1 Vertex Buffer Object per i vertici
    glGenBuffers(1, &mesh->vertexBufferObjID);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), &mesh->vertices[0], GL_STATIC_DRAW);
    glVertexPointer(
            3,                  // size
            GL_FLOAT,           // type
            0,                  // stride
            (void*)0            // array buffer offset
    );
    //Genero 1 Vertex Buffer Object per le normali
    glGenBuffers(1, &mesh->normalBufferObjID);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(glm::vec3), mesh->normals.data(), GL_STATIC_DRAW);
    glNormalPointer(
            GL_FLOAT,           // type
            0,                  // stride
            (void*)0            // array buffer offset
    );


    // Genero 1 Element Buffer Object per gli indici, Nota: GL_ELEMENT_ARRAY_BUFFER
    glGenBuffers(1, &mesh->indexBufferObjID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLshort), mesh->indices.data(), GL_STATIC_DRAW);
}

void loadObjFile(string file_path, Mesh* mesh)
{
    FILE * file = fopen(file_path.c_str(), "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        return;
    }
    char lineHeader[128];
    while (fscanf(file, "%s", lineHeader) != EOF) {
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            mesh->vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            GLuint a, b, c;
            fscanf(file, " %d %d %d\n", &a, &b, &c);
            a--; b--; c--;
            mesh->indices.push_back(a); mesh->indices.push_back(b); mesh->indices.push_back(c);
        }
    }
    mesh->normals.resize(mesh->vertices.size(), glm::vec3(0.0,0.0,0.0));
    for(int i=0; i<mesh->indices.size();i+=3){
        GLuint  a=mesh->indices[i];
        GLuint  b=mesh->indices[i+1];
        GLuint  c=mesh->indices[i+2];

        glm::vec3 aa= mesh->vertices[a];
        glm::vec3 bb= mesh->vertices[b];
        glm::vec3 cc= mesh->vertices[c];

        glm::vec3 result=glm::normalize(glm::cross(bb-aa,cc-aa));

        mesh->normals[a]+=result;
        mesh->normals[b]+=result;
        mesh->normals[c]+=result;
    }
    for (auto &normal : mesh->normals) {
        normal = glm::normalize(normal);
    }

}

void drawAxis(float scale, int drawLetters)
{
    glPushMatrix();
    glScalef(scale, scale, scale);
    glBegin(GL_LINES);

    glColor4d(1.0, 0.0, 0.0, 1.0);
    if (drawLetters)
    {
        glVertex3f(.8f, 0.05f, 0.0);  glVertex3f(1.0, 0.25f, 0.0); /* Letter X */
        glVertex3f(0.8f, .25f, 0.0);  glVertex3f(1.0, 0.05f, 0.0);
    }
    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0); /* X axis      */


    glColor4d(0.0, 1.0, 0.0, 1.0);
    if (drawLetters)
    {
        glVertex3f(0.10f, 0.8f, 0.0);   glVertex3f(0.10f, 0.90f, 0.0); /* Letter Y */
        glVertex3f(0.10f, 0.90f, 0.0);  glVertex3f(0.05, 1.0, 0.0);
        glVertex3f(0.10f, 0.90f, 0.0);  glVertex3f(0.15, 1.0, 0.0);
    }
    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 1.0, 0.0); /* Y axis      */


    glColor4d(0.0, 0.0, 1.0, 1.0);
    if (drawLetters)
    {
        glVertex3f(0.05f, 0, 0.8f);  glVertex3f(0.20f, 0, 0.8f); /* Letter Z*/
        glVertex3f(0.20f, 0, 0.8f);  glVertex3f(0.05, 0, 1.0);
        glVertex3f(0.05f, 0, 1.0);   glVertex3f(0.20, 0, 1.0);
    }
    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, 1.0); /* Z axis    */

    glEnd();
    glPopMatrix();
}

void drawGrid(float scale, int dimension)
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(scale, scale, scale);
    glBegin(GL_LINES);
    for (int x = -dimension; x < dimension; x++) {
        glVertex3f(x, 0.0f, -dimension);
        glVertex3f(x, 0.0f, dimension);
    }
    for (int z = -dimension; z < dimension; z++) {
        glVertex3f(-dimension, 0.0f, z);
        glVertex3f(dimension, 0.0f, z);
    }
    glEnd();
    glPopMatrix();
}


void printToScreen()
{
    string axis = "Asse: ";
    string mode = "Naviga/Modifica: ";
    string obj = "Oggetto: " + objects[selectedObject].name;
    string ref = "Sistema WCS/OCS: ";
    string mat = "Materiale: " + materials[objects[selectedObject].material].name;
    switch (WorkingAxis) {
        case X: axis += "X"; break;
        case Y: axis += "Y"; break;
        case Z: axis += "Z"; break;
    }
    switch (OperationMode) {
        case TRASLATING: mode += "Traslazione"; break;
        case ROTATING: mode += "Rotazione"; break;
        case SCALING: mode += "Scalatura"; break;
        case NAVIGATION: mode += "Naviga"; break;
    }
    switch (TransformMode) {
        case OCS: ref += "OCS"; break;
        case WCS: ref += "WCS"; break;
    }
    vector<string> lines;
    lines.push_back(mat);
    lines.push_back(obj);
    lines.push_back(axis);
    lines.push_back(mode);
    lines.push_back(ref);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, WindowHeight * aspect_ratio, 0, WindowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    HUD_Logger::get()->printInfo(lines);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    resize(WindowWidth, WindowHeight);
}
