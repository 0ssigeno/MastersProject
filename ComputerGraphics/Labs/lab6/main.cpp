/******************************************************************************************
LAB 06
Gestione interattiva di una scena 3D mediante controllo da mouse e da tastiera.
I modelli geometrici in scena sono mesh poligonali caricati da file in formato *.obj,  
con associata una parametrizzazione (sfere e cubo), oggetti poligonali creati in modo procedurale
(toroide).  
W/w		incrementa/decrementa NumWrap toro
N/n		incrementa/decrementa NumPerWrap toro

Si possono ereditare dalla esercitazione 3 i seguenti tool:
CTRL+WHEEL = pan orizzontale della telecamera
SHIFT+WHEEL = pan verticale della telecamera
WHELL = ZOOM IN/OUT se si � in navigazione, altrimenti agisce sulla trasformazione dell'oggetto
g r s	per le modalit� di lavoro: traslate/rotate/scale
x y z	per l'asse di lavoro
wcs/ocs selezionabili dal menu pop-up

OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software
based on the OpenGL Shading Language (GLSL) specifications.
*******************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS // for fscanf

#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "HUD_Logger.h"
#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SHIFT_WHEEL_UP 11
#define SHIFT_WHEEL_DOWN 12
#define CTRL_WHEEL_UP 19
#define CTRL_WHEEL_DOWN 20

#define NUM_SHADERS 8

using namespace std;

static int WindowWidth = 1120;
static int WindowHeight = 630; 
GLfloat aspect_ratio = 16.0f / 9.0f;

typedef struct {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	GLuint vertexArrayObjID;
	GLuint vertexBufferObjID;
	GLuint normalBufferObjID;
	GLuint uvBufferObjID;
} Mesh;

typedef enum { 
	RED_PLASTIC,
	EMERALD,
	BRASS,
	SLATE,
	NO_MATERIAL
} MaterialType;

typedef struct {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular; 
	GLfloat shininess; 
} Material;

typedef enum { // used also as index, don't modify order
	GOURAUD,
	PHONG,
	BLINN,
	TOON,
	TEXTURE_ONLY,
	TEXTURE_PHONG,
	PASS_THROUGH,
	WAVE
} ShadingType;

typedef struct {
	Mesh mesh;
	MaterialType material;
	ShadingType shading;
	GLuint textureID;
	glm::mat4 M;
	string name;
} Object;

typedef struct {
	GLuint light_position_pointer;
	GLuint light_color_pointer;
	GLuint light_power_pointer;
	GLuint material_diffuse;
	GLuint material_ambient;
	GLuint material_specular;
	GLuint material_shininess;
} LightShaderUniform;

typedef struct {
	GLuint P_Matrix_pointer;
	GLuint V_Matrix_pointer;
	GLuint M_Matrix_pointer;
} BaseShaderUniform;

const string MeshDir = "/home/ossigeno/University/lab6/Mesh/";
const string TextureDir = "/home/ossigeno/University/lab6/Textures/";
const string ShaderDir = "/home/ossigeno/University/lab6/Shaders/";
static Object Axis, Grid;
static vector<Object> objects;
static vector<Material> materials;
static int selected_obj = 0;

struct {
	// Variables controlling the torus mesh resolution
	int NumWraps = 10;
	int NumPerWrap = 8;
	// Variables controlling the size of the torus
	float MajorRadius = 3.0;
	float MinorRadius = 1.0;
	int torus_index;
} TorusSetup;

// Materiali disponibili
glm::vec3 red_plastic_ambient = { 0.1, 0.0, 0.0}, red_plastic_diffuse = { 0.6, 0.1, 0.1}, red_plastic_specular = { 0.7, 0.6, 0.6}; GLfloat red_plastic_shininess = 32.0f;
glm::vec3 brass_ambient = { 0.1, 0.06, 0.015}, brass_diffuse = { 0.78, 0.57, 0.11}, brass_specular = { 0.99, 0.91, 0.81}; GLfloat brass_shininess = 27.8f;
glm::vec3 emerald_ambient = { 0.0215, 0.04745, 0.0215}, emerald_diffuse = { 0.07568, 0.61424, 0.07568}, emerald_specular = { 0.633, 0.727811, 0.633}; GLfloat emerald_shininess = 78.8f;
glm::vec3 slate_ambient = { 0.02, 0.02, 0.02}, slate_diffuse = { 0.1, 0.1, 0.1}, slate_specular{ 0.4, 0.4, 0.4}; GLfloat slate_shininess = 1.78125f;

typedef struct {
	glm::vec3 position; 
	glm::vec3 color;
	GLfloat power;
} point_light;

static point_light light;

/*camera structures*/
constexpr float CAMERA_ZOOM_SPEED = 0.1f;
constexpr float CAMERA_TRASLATION_SPEED = 0.01f;

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
	SCALING,
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

//Shaders Uniforms 
static vector<LightShaderUniform> light_uniforms; // for shaders with light
static vector<BaseShaderUniform> base_uniforms; // for ALL shaders
static vector<GLuint> shaders_IDs; //Pointers to the shader programs


// Main initialization funtion
void init();
// Display Funtion
void display(); 
// Reshape Function
void resize(int w, int h); 
// Mouse Function
void mouseClick(int button, int state, int x, int y);
// Keyboard:  g traslate r rotate s scale x,y,z axis esc 
void keyboardDown(unsigned char key, int x, int y);
// Special key arrow: select active object (arrows left,right)
void special(int key, int x, int y); 
// Main Menu
void main_menu_func(int option);
// Sub menu for materials
void material_menu_function(int option); // EMPTY SEE EX3 
// Build openGL menus
void buildOpenGLMenu();
// Trackball: Convert a 2D screen point into a 3D point on the semi-sphere trackball
glm::vec3 getTrackBallPoint(float x, float y);
// Trackball: rotate the position vector 
void mouseActiveMotion(int x, int y);
void moveCameraForward();// EMPTY SEE EX3
void moveCameraBack();// EMPTY SEE EX3 
void moveCameraLeft();// EMPTY SEE EX3 
void moveCameraRight();// EMPTY SEE EX3 
void moveCameraUp();// EMPTY SEE EX3 
void moveCameraDown();// EMPTY SEE EX3 
// EMPTY SEE EX3: Crea ed applica la matrice di trasformazione alla matrice dell'oggeto discriminando tra WCS e OCS.
//	La funzione � gia invocata con un input corretto, � sufficiente concludere la sua implementazione.
void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor); 
/* Mesh Functions*/
void compute_Torus(Mesh* mesh);
// Generate the buffers VAO and VBO for a mesh 
void generate_and_load_buffers(bool generate, Mesh* mesh);
// Read an .obj polygon mesh
void loadObjFile(string file_path, Mesh* mesh);
//Uses stb_image.h to read an image, then loads it.
GLuint loadTexture(string path);
// Draw Axis and grid 
void drawAxisAndGrid();
// 2D fixed pipeline Font rendering on screen
void printToScreen();
//my functions and variables
static float waveTime;
bool amImoving=false;
void generateRainbowMap(Object &obj0);
void generateShadesOfGrey(Object &obj0);
void generate_texture(Object &obj0);
void init_wave();
void moveWaves();

int main(int argc, char** argv) {

	GLboolean GlewInitResult;
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Model Viewer with Shaders ");

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
void init_light_object() {
	Mesh sphereS = {};
	loadObjFile(MeshDir + "sphere_n_t_smooth.obj", &sphereS);
	generate_and_load_buffers(true, &sphereS);
	Object obj = {};
	obj.mesh = sphereS;
	obj.material = MaterialType::NO_MATERIAL;
	obj.shading = ShadingType::PASS_THROUGH; 
	obj.name = "light";
	obj.M = glm::scale(glm::translate(glm::mat4(1), light.position), glm::vec3(0.2, 0.2, 0.2));
	objects.push_back(obj);
}

void generateRainbowMap(Object &obj0){
    GLubyte image[64][64][3];
    for (int i = 0; i < 64; i++) {
        for(int z=0;z<8;z++){
            for(int j=z*8; j<(z+1)*8;j++){
                int c1,c2,c3;
                switch(z){
                    case 0:
                        //rosso
                        c1=255;
                        c2=0;
                        c3=0;
                        break;
                    case 1:
                        c1=255;
                        c2=165;
                        c3=0;
                        break;
                    case 2:
                        c1=255;
                        c2=255;
                        c3=0;
                        break;
                    case 3:
                        c1=0;
                        c2=255;
                        c3=0;
                        break;
                    case 4:
                        c1=0;
                        c2=0;
                        c3=255;
                        break;
                    case 5:
                        c1=75;
                        c2=0;
                        c3=130;
                        break;
                    case 6:
                        c1=143;
                        c2=0;
                        c3=255;
                        break;
                    case 7:
                        c1=143;
                        c2=0;
                        c3=255;
                        break;


                }
                image[i][j][0] = (GLubyte)c1;
                image[i][j][1] = (GLubyte)c2;
                image[i][j][2] = (GLubyte)c3;
            }

            }
        }
        glGenTextures(1, &obj0.textureID);
        glBindTexture(GL_TEXTURE_2D, obj0.textureID);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,  //the target
                     0, // the mip map level we want to generate
                     GL_RGB, // the format of the texture
                     64, //texture_size, width
                     64, //texture_size, heigth
                     0,  // border, leave 0
                     GL_RGB, // we assume is a RGB color image with 24 bit depth per pixel
                     GL_UNSIGNED_BYTE, // the data type
                     image);

    }


void generateShadesOfGrey(Object &obj0){
    GLubyte image[64][64][3];

    for (int i = 0; i < 64; i++) {
        for (int z = 0; z < 8; z++) {
            for (int j = z * 8; j < (z + 1) * 8; j++) {
                int c = (z + 1) * 32;
                image[i][j][0] = (GLubyte) c;
                image[i][j][1] = (GLubyte) c;
                image[i][j][2] = (GLubyte) c;
            }
        }
    }
    glGenTextures(1, &obj0.textureID);
    glBindTexture(GL_TEXTURE_2D, obj0.textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,  //the target
                 0, // the mip map level we want to generate
                 GL_RGB, // the format of the texture
                 64, //texture_size, width
                 64, //texture_size, heigth
                 0,  // border, leave 0
                 GL_RGB, // we assume is a RGB color image with 24 bit depth per pixel
                 GL_UNSIGNED_BYTE, // the data type
                 image);

    }


void generate_texture(Object &obj0){
    GLubyte image[64][64][3];

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
            image[i][j][0] = (GLubyte)c;
            image[i][j][1] = (GLubyte)c;
            image[i][j][2] = (GLubyte)c;
        }
    }

    /////////////////////////////////////////
    glGenTextures(1, &obj0.textureID);
    glBindTexture(GL_TEXTURE_2D, obj0.textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,  //the target
                 0, // the mip map level we want to generate
                 GL_RGB, // the format of the texture
                 64, //texture_size, width
                 64, //texture_size, heigth
                 0,  // border, leave 0
                 GL_RGB, // we assume is a RGB color image with 24 bit depth per pixel
                 GL_UNSIGNED_BYTE, // the data type
                 image);

}

void init_textured_plane() {
	
	Mesh surface = {};
	surface.vertices = { {-2,0,-2}, {-2,0,2}, {2,0,2}, {2,0,-2}, {-2,0,-2}, {2,0,2} };
	surface.normals = { {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} };
	//Tex coords are out of bound to show the GL_REPEAT effect
	//surface.texCoords = { {-4,-4}, {-4,+4}, {4,4}, {4,-4}, {-4,-4}, {4,4} }; // out of bound UVs
	surface.texCoords = { {0,0}, {0,1}, {1,1}, {1,0}, {0,0}, {1,1} }; // standard UVs
	generate_and_load_buffers(true, &surface);
	Object obj0 = {};
	obj0.mesh = surface;
	obj0.material = MaterialType::NO_MATERIAL;
	obj0.shading = ShadingType::TEXTURE_ONLY;

	/////////////////////////////////////////////////////////////////////////
	//  Compute checkboard procedural_texture image of dimension width x width x 3 (RGB)
	/////////////////////////////////////////////////////////////////////////
    generate_texture(obj0);
	obj0.name = "Plane";
	obj0.M = glm::translate(glm::mat4(1), glm::vec3(-5., 0., -5.));
	objects.push_back(obj0);
}

void init_cube() {

	Mesh cube = {};
	loadObjFile(MeshDir + "cube_n_t_flat.obj", &cube);
	generate_and_load_buffers(true, &cube);
	Object obj1 = {};
	obj1.mesh = cube;
	obj1.material = MaterialType::EMERALD;
	obj1.shading = ShadingType::TEXTURE_ONLY;
	obj1.textureID = loadTexture(TextureDir + "cube_tex.jpg");
	obj1.name = "Textured Cube";
	obj1.M = glm::translate(glm::mat4(1), glm::vec3(-5., 0., 5.));
	objects.push_back(obj1);
}

void init_torus() {

	Mesh torus = {};
	compute_Torus(&torus);
	generate_and_load_buffers(true, &torus);
	// Object Setup with NO texture, will use the light shader and a material for color
	Object obj2 = {};
	obj2.mesh = torus;
	obj2.material = MaterialType::BRASS;
	//todo phong
	obj2.shading = ShadingType::PHONG; // BLINN
	//todo texture
    //obj2.shading = ShadingType::TEXTURE_ONLY; // BLINN
    //obj2.textureID = loadTexture(TextureDir + "bombolone_2.jpg");

    //todo texturephong
    //obj2.shading = ShadingType::TEXTURE_PHONG; // BLINN
    //obj2.textureID = loadTexture(TextureDir + "bombolone_2.jpg");
    //todo toon
    //obj2.shading = ShadingType::TOON; // BLINN

    //todo procedural
    //generateRainbowMap(obj2);
    //generateShadesOfGrey(obj2);
    obj2.name = "Torus";

    obj2.M = glm::translate(glm::mat4(1), glm::vec3(5., 0., 5.));
	objects.push_back(obj2);
	TorusSetup.torus_index = objects.size() - 1;
}



void init_sphere_FLAT() {

	Mesh sphereF = {};
	loadObjFile(MeshDir + "sphere_n_t_flat.obj", &sphereF);
	generate_and_load_buffers(true, &sphereF);
	// Object Setup with NO texture, will use the light shader and a material for color and light behavior
	Object obj3 = {};
	obj3.mesh = sphereF;
	obj3.material = MaterialType::EMERALD;
	obj3.shading = ShadingType::GOURAUD;

    obj3.name = "Sphere FLAT";
	obj3.M = glm::translate(glm::mat4(1), glm::vec3(3., 0., -6.));
	objects.push_back(obj3);
}

void init_sphere_SMOOTH() {
	Mesh sphereS = {};
	loadObjFile(MeshDir + "sphere_n_t_smooth.obj", &sphereS);
	generate_and_load_buffers(true, &sphereS);
	// Object Setup with NO texture, will use the light shader and a material for color and light behavior
	Object obj4 = {};
	obj4.mesh = sphereS;
	obj4.material = MaterialType::RED_PLASTIC;
	obj4.shading = ShadingType::BLINN;
	obj4.name = "Sphere SMOOTH";
	obj4.M = glm::translate(glm::mat4(1), glm::vec3(6., 0., -3.));
	objects.push_back(obj4);
}
void init_wave() {
    Mesh _wave = {};
    loadObjFile(MeshDir + "GridPlane.obj", &_wave);
    generate_and_load_buffers(true, &_wave);
    Object obj1 = {};
    obj1.mesh = _wave;
    obj1.material = MaterialType::BRASS;
    obj1.shading = ShadingType::WAVE;
    obj1.name = "wave after wave";
    obj1.textureID = loadTexture(TextureDir + "WoodGrain.bmp");
    obj1.M = glm::translate(glm::mat4(1), glm::vec3(10., 2., -5.));
    objects.push_back(obj1);
}

void init_axis() {
	Mesh _grid = {};
	loadObjFile(MeshDir + "axis.obj", &_grid);
	generate_and_load_buffers(true, &_grid);
	Object obj1 = {};
	obj1.mesh = _grid;
	obj1.material = MaterialType::NO_MATERIAL;
	obj1.shading = ShadingType::TEXTURE_ONLY;
	obj1.textureID = loadTexture(TextureDir + "AXIS_TEX.png");
	obj1.name = "axis";
	obj1.M = glm::scale(glm::mat4(1),glm::vec3(2.f,2.f,2.f));
	Axis = obj1;
}

void init_grid() {
	Mesh _grid = {};
	loadObjFile(MeshDir + "reference_grid.obj", &_grid);
	generate_and_load_buffers(true, &_grid);
	Object obj1 = {};
	obj1.mesh = _grid;
	obj1.material = MaterialType::NO_MATERIAL;
	obj1.shading = ShadingType::PASS_THROUGH;
	obj1.name = "grid";
	obj1.M = glm::scale(glm::mat4(1), glm::vec3(2.f, 2.f, 2.f));
	Grid = obj1;
}

void init() {
// Default render settings
	OperationMode = NAVIGATION;
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glCullFace(GL_BACK);	// remove faces facing the background
	glEnable(GL_LINE_SMOOTH);

	//Light initialization
	light.position = {5.0,5.0,-5.0};
	light.color = {1.0,1.0,1.0};
	light.power = 3.f;

	// Materials setup
	materials.resize(5);
	materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
	materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
	materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
	materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
	materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

	materials[MaterialType::EMERALD].name = "Emerald";
	materials[MaterialType::EMERALD].ambient = emerald_ambient;
	materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
	materials[MaterialType::EMERALD].specular = emerald_specular;
	materials[MaterialType::EMERALD].shininess = emerald_shininess;

	materials[MaterialType::BRASS].name = "Brass";
	materials[MaterialType::BRASS].ambient = brass_ambient;
	materials[MaterialType::BRASS].diffuse = brass_diffuse;
	materials[MaterialType::BRASS].specular = brass_specular;
	materials[MaterialType::BRASS].shininess = brass_shininess;

	materials[MaterialType::SLATE].name = "Slate";
	materials[MaterialType::SLATE].ambient = slate_ambient;
	materials[MaterialType::SLATE].diffuse = slate_diffuse;
	materials[MaterialType::SLATE].specular = slate_specular;
	materials[MaterialType::SLATE].shininess = slate_shininess;

	materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
	materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1,1,1);
	materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].shininess = 1.f;

	// SHADERS configuration section
	shaders_IDs.resize(NUM_SHADERS);
	light_uniforms.resize(5); // allocate space for uniforms of PHONG, BLINN and GOURAND + TOON
	base_uniforms.resize(NUM_SHADERS); // allocate space for uniforms of PHONG,BLINN,GOURAND,TOON,WAVE,TEXTURE_ONLY

	//Gourand Shader loading
	shaders_IDs[GOURAUD] = initShader(ShaderDir + "v_gouraud.glsl", ShaderDir + "f_gouraud.glsl");
	BaseShaderUniform base_unif = {};
	//Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "M");
	base_uniforms[ShadingType::GOURAUD] = base_unif;
	LightShaderUniform light_unif = {};
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[GOURAUD], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[GOURAUD], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[GOURAUD], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[GOURAUD], "material.shininess");
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.power");
	light_uniforms[ShadingType::GOURAUD] = light_unif;
	//Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[GOURAUD]);
	//Shader uniforms initialization
	glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[GOURAUD].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[GOURAUD].light_power_pointer, light.power);
//-----------------------------------------------------------------------------------------------//
	//Phong Shader loading
    // TODO
    shaders_IDs[PHONG] = initShader(ShaderDir + "v_phong.glsl", ShaderDir + "f_phong.glsl");
    base_unif = {};
    //Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
    base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "P");
    base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "V");
    base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "M");
    base_uniforms[ShadingType::PHONG] = base_unif;
    light_unif = {};
    light_unif.material_ambient = glGetUniformLocation(shaders_IDs[PHONG], "material.ambient");
    light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[PHONG], "material.diffuse");
    light_unif.material_specular = glGetUniformLocation(shaders_IDs[PHONG], "material.specular");
    light_unif.material_shininess = glGetUniformLocation(shaders_IDs[PHONG], "material.shininess");
    light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.position");
    light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.color");
    light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.power");
    light_uniforms[ShadingType::PHONG] = light_unif;
    //Rendiamo attivo lo shader
    glUseProgram(shaders_IDs[PHONG]);
    //Shader uniforms initialization
    glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
    glUniform3f(light_uniforms[PHONG].light_color_pointer, light.color.r, light.color.g, light.color.b);
    glUniform1f(light_uniforms[PHONG].light_power_pointer, light.power);
//-----------------------------------------------------------------------------------------------//
	//Texture_Phong Shader loading
	// TODO
    shaders_IDs[TEXTURE_PHONG] = initShader(ShaderDir + "v_texture_phong.glsl", ShaderDir + "f_texture_phong.glsl");
    base_unif = {};
    //Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
    base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "P");
    base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "V");
    base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "M");
    base_uniforms[ShadingType::TEXTURE_PHONG] = base_unif;
    light_unif = {};
    light_unif.material_ambient = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "material.ambient");
    light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "material.diffuse");
    light_unif.material_specular = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "material.specular");
    light_unif.material_shininess = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "material.shininess");
    light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "light.position");
    light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "light.color");
    light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_PHONG], "light.power");
    light_uniforms[ShadingType::TEXTURE_PHONG] = light_unif;
    //Rendiamo attivo lo shader
    glUseProgram(shaders_IDs[TEXTURE_PHONG]);
    //Shader uniforms initialization
    glUniform3f(light_uniforms[TEXTURE_PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
    glUniform3f(light_uniforms[TEXTURE_PHONG].light_color_pointer, light.color.r, light.color.g, light.color.b);
    glUniform1f(light_uniforms[TEXTURE_PHONG].light_power_pointer, light.power);
//-----------------------------------------------------------------------------------------------//
	//Wave Shader loading
	//TODO
    shaders_IDs[WAVE] = initShader(ShaderDir + "v_wave.glsl",ShaderDir + "f_wave.glsl");
    base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "P");
    base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "V");
    base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "M");
    base_uniforms[ShadingType::WAVE] = base_unif;
    waveTime=glGetUniformLocation(shaders_IDs[WAVE],"T");
	glUseProgram(shaders_IDs[WAVE]);
    glUniform4fv(glGetUniformLocation(shaders_IDs[WAVE], "Color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
//-----------------------------------------------------------------------------------------------//
	//Blinn Shader loading
	shaders_IDs[BLINN] = initShader(ShaderDir + "v_blinn.glsl", ShaderDir + "f_blinn.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "M");
	base_uniforms[ShadingType::BLINN] = base_unif;
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[BLINN], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[BLINN], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[BLINN], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[BLINN], "material.shininess");
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.power");
	light_uniforms[ShadingType::BLINN] = light_unif;
	//Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[BLINN]);
	//Shader uniforms initialization
	glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[BLINN].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[BLINN].light_power_pointer, light.power);

    //Texture Shader loading
    shaders_IDs[TEXTURE_ONLY] = initShader(ShaderDir + "v_texture.glsl", ShaderDir + "f_texture.glsl");
    //Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
    base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_ONLY], "P");
    base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_ONLY], "V");
    base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[TEXTURE_ONLY], "M");
    base_uniforms[ShadingType::TEXTURE_ONLY] = base_unif;

    //Pass-Through Shader loading
	shaders_IDs[PASS_THROUGH] = initShader(ShaderDir + "v_passthrough.glsl", ShaderDir + "f_passthrough.glsl");
	//Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "M");
	base_uniforms[ShadingType::PASS_THROUGH] = base_unif;
	glUseProgram(shaders_IDs[PASS_THROUGH]);
	glUniform4fv(glGetUniformLocation(shaders_IDs[PASS_THROUGH], "Color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));


	//Toon Shader loading
    shaders_IDs[TOON] = initShader(ShaderDir + "v_toon.glsl", ShaderDir + "f_toon.glsl");
    base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "P");
    base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "V");
    base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "M");
    base_uniforms[ShadingType::TOON] = base_unif;
    light_unif.material_ambient = glGetUniformLocation(shaders_IDs[TOON], "material.ambient");
    light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[TOON], "material.diffuse");
    light_unif.material_specular = glGetUniformLocation(shaders_IDs[TOON], "material.specular");
    light_unif.material_shininess = glGetUniformLocation(shaders_IDs[TOON], "material.shininess");
    light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[TOON], "light.position");
    light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[TOON], "light.color");
    light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[TOON], "light.power");
    light_uniforms[ShadingType::TOON] = light_unif;
    //Rendiamo attivo lo shader
    glUseProgram(shaders_IDs[TOON]);
    //Shader uniforms initialization
    glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
    glUniform3f(light_uniforms[TOON].light_color_pointer, light.color.r, light.color.g, light.color.b);
    glUniform1f(light_uniforms[TOON].light_power_pointer, light.power);


    // Camera Setup
	ViewSetup = {};
	ViewSetup.position = glm::vec4(10.0, 10.0, 10.0, 0.0);
	ViewSetup.target = glm::vec4(0.0, 0.0, 0.0, 0.0);
	ViewSetup.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);
	PerspectiveSetup = {};
	PerspectiveSetup.aspect = (GLfloat)WindowWidth / (GLfloat)WindowHeight;
	PerspectiveSetup.fovY = 45.0f;
	PerspectiveSetup.far_plane = 2000.0f;
	PerspectiveSetup.near_plane = 1.0f;

	//////////////////////////////////////////////////////////////////////
	//				OBJECTS IN SCENE
	//////////////////////////////////////////////////////////////////////
	
	// FLAT SPHERE (face normals) no texture with material, uses a shader with lighting
	init_sphere_FLAT();

	// SMOOTH SPHERE (vertex normals) no texture with material, uses a shader with lighting
	init_sphere_SMOOTH();

	//PLANE with a procedural texture but no material, uses a texture-only shader
	init_textured_plane();

	//CUBE with a texture image but no material, uses a texture-only shader
	init_cube();

	//Reference point of the position of the light
	init_light_object();

	// White Axis
	init_axis();

	// White Grid for reference
	init_grid();

	// PARAMETRIC TORUS no texture with material, uses a shader with lighting
	init_torus();
	//My wave
	init_wave();

}
void display() {
	glClearColor(0.4, 0.4, 0.4, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	// Draw the central Axis point of reference and the grid
	drawAxisAndGrid();
    for (int i = 0; i < objects.size(); i++) {
		//Shader selection
		switch (objects[i].shading) {
		case ShadingType::GOURAUD:	
			glUseProgram(shaders_IDs[GOURAUD]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[GOURAUD].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
			//Material loading
			glUniform3fv(light_uniforms[GOURAUD].material_ambient, 1, glm::value_ptr(materials[objects[i].material].ambient));
			glUniform3fv(light_uniforms[GOURAUD].material_diffuse, 1, glm::value_ptr(materials[objects[i].material].diffuse));
			glUniform3fv(light_uniforms[GOURAUD].material_specular, 1, glm::value_ptr(materials[objects[i].material].specular));
			glUniform1f(light_uniforms[GOURAUD].material_shininess, materials[objects[i].material].shininess);
            //TODO aggiunta per la luce
            glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
            glUniform3f(light_uniforms[GOURAUD].light_color_pointer, light.color.r, light.color.g, light.color.b);
            glUniform1f(light_uniforms[GOURAUD].light_power_pointer, light.power);

                break;
            case ShadingType::PHONG:
                glUseProgram(shaders_IDs[PHONG]);
                // Caricamento matrice trasformazione del modello
                glUniformMatrix4fv(base_uniforms[PHONG].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
                //Material loading
                glUniform3fv(light_uniforms[PHONG].material_ambient, 1, glm::value_ptr(materials[objects[i].material].ambient));
                glUniform3fv(light_uniforms[PHONG].material_diffuse, 1, glm::value_ptr(materials[objects[i].material].diffuse));
                glUniform3fv(light_uniforms[PHONG].material_specular, 1, glm::value_ptr(materials[objects[i].material].specular));
                glUniform1f(light_uniforms[PHONG].material_shininess, materials[objects[i].material].shininess);

                glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
                glUniform3f(light_uniforms[PHONG].light_color_pointer, light.color.r, light.color.g, light.color.b);
                glUniform1f(light_uniforms[PHONG].light_power_pointer, light.power);

                break;
            case ShadingType::TEXTURE_PHONG:
                glUseProgram(shaders_IDs[TEXTURE_PHONG]);
                // Caricamento matrice trasformazione del modello
                glUniformMatrix4fv(base_uniforms[TEXTURE_PHONG].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
                //Material loading
                glUniform3fv(light_uniforms[TEXTURE_PHONG].material_ambient, 1, glm::value_ptr(materials[objects[i].material].ambient));
                glUniform3fv(light_uniforms[TEXTURE_PHONG].material_diffuse, 1, glm::value_ptr(materials[objects[i].material].diffuse));
                glUniform3fv(light_uniforms[TEXTURE_PHONG].material_specular, 1, glm::value_ptr(materials[objects[i].material].specular));
                glUniform1f(light_uniforms[TEXTURE_PHONG].material_shininess, materials[objects[i].material].shininess);

                glUniform3f(light_uniforms[TEXTURE_PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
                glUniform3f(light_uniforms[TEXTURE_PHONG].light_color_pointer, light.color.r, light.color.g, light.color.b);
                glUniform1f(light_uniforms[TEXTURE_PHONG].light_power_pointer, light.power);
                glActiveTexture(GL_TEXTURE0); // this addresses the first sampler2D uniform in the shader
                glBindTexture(GL_TEXTURE_2D, objects[i].textureID);

                break;
		case ShadingType::BLINN:
			glUseProgram(shaders_IDs[BLINN]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[BLINN].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
			//Material loadingin
			glUniform3fv(light_uniforms[BLINN].material_ambient, 1, glm::value_ptr(materials[objects[i].material].ambient));
			glUniform3fv(light_uniforms[BLINN].material_diffuse, 1, glm::value_ptr(materials[objects[i].material].diffuse));
			glUniform3fv(light_uniforms[BLINN].material_specular, 1, glm::value_ptr(materials[objects[i].material].specular));
			glUniform1f(light_uniforms[BLINN].material_shininess, materials[objects[i].material].shininess);

            glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
            glUniform3f(light_uniforms[BLINN].light_color_pointer, light.color.r, light.color.g, light.color.b);
            glUniform1f(light_uniforms[BLINN].light_power_pointer, light.power);

                break;
		case ShadingType::TEXTURE_ONLY:
            //todo es3
            glUseProgram(shaders_IDs[TEXTURE_ONLY]);
            // Caricamento matrice trasformazione del modello
            glUniformMatrix4fv(base_uniforms[TEXTURE_ONLY].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
            glActiveTexture(GL_TEXTURE0); // this addresses the first sampler2D uniform in the shader
            glBindTexture(GL_TEXTURE_2D, objects[i].textureID);



                break;
		case ShadingType::PASS_THROUGH:
			glUseProgram(shaders_IDs[PASS_THROUGH]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[PASS_THROUGH].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
			break;
		case ShadingType::TOON:
            glUseProgram(shaders_IDs[TOON]);
            // Caricamento matrice trasformazione del modello
            glUniformMatrix4fv(base_uniforms[TOON].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
            //Material loading
            glUniform3fv(light_uniforms[TOON].material_ambient, 1, glm::value_ptr(materials[objects[i].material].ambient));
            glUniform3fv(light_uniforms[TOON].material_diffuse, 1, glm::value_ptr(materials[objects[i].material].diffuse));
            glUniform3fv(light_uniforms[TOON].material_specular, 1, glm::value_ptr(materials[objects[i].material].specular));
            glUniform1f(light_uniforms[TOON].material_shininess, materials[objects[i].material].shininess);

            glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
            glUniform3f(light_uniforms[TOON].light_color_pointer, light.color.r, light.color.g, light.color.b);
            glUniform1f(light_uniforms[TOON].light_power_pointer, light.power);

                break;
		case ShadingType::WAVE:
            //Todo wave
            glUseProgram(shaders_IDs[WAVE]);
            glUniformMatrix4fv(base_uniforms[WAVE].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects[i].M));
            glUniform1f(waveTime,glutGet(GLUT_ELAPSED_TIME));
            glActiveTexture(GL_TEXTURE0); // this addresses the first sampler2D uniform in the shader
            glBindTexture(GL_TEXTURE_2D, objects[i].textureID);

                break;
		default:
			break;
		}
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(objects[i].mesh.vertexArrayObjID);
		glDrawArrays(GL_TRIANGLES, 0, objects[i].mesh.vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

	}

	// OLD fixed pipeline for Text and symbols
	glUseProgram(0);
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

    // Fixed Pipeline matrices for retro compatibility
    glUseProgram(0); // Embedded openGL shader
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(PerspectiveSetup.fovY, PerspectiveSetup.aspect, PerspectiveSetup.near_plane, PerspectiveSetup.far_plane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ViewSetup.position.x, ViewSetup.position.y, ViewSetup.position.z,
              ViewSetup.target.x, ViewSetup.target.y, ViewSetup.target.z,
              ViewSetup.upVector.x, ViewSetup.upVector.y, ViewSetup.upVector.z);

    // Programmable Pipeline matrices for object rendering
    glm::mat4 P = glm::perspective(PerspectiveSetup.fovY, PerspectiveSetup.aspect, PerspectiveSetup.near_plane, PerspectiveSetup.far_plane);
    glm::mat4 V = glm::lookAt(glm::vec3(ViewSetup.position), glm::vec3(ViewSetup.target), glm::vec3(ViewSetup.upVector));

    for (int i = 0; i < shaders_IDs.size();i++) {
        glUseProgram(shaders_IDs[i]);
        glUniformMatrix4fv(base_uniforms[i].P_Matrix_pointer, 1, GL_FALSE, value_ptr(P));
        glUniformMatrix4fv(base_uniforms[i].V_Matrix_pointer, 1, GL_FALSE, value_ptr(V));
    }
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
	float amount = 0.08f;
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
		modifyModelMatrix(glm::vec3(0), axis, amount * 20.0f, 1.0f);
		break;
	case SCALING:
		modifyModelMatrix(glm::vec3(0), axis, 0.0f, 1.0f + amount);
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
		// Selezione della modalit� di trasformazione
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
        case 'W': TorusSetup.NumWraps++;
            compute_Torus(&(objects[TorusSetup.torus_index].mesh));
            generate_and_load_buffers(false, &(objects[TorusSetup.torus_index].mesh));
            break;
        case 'w':
            if (TorusSetup.NumWraps > 4) {
                TorusSetup.NumWraps--;
                compute_Torus(&(objects[TorusSetup.torus_index].mesh));
                generate_and_load_buffers(false, &(objects[TorusSetup.torus_index].mesh));

            }
            break;
        case 'N':
            TorusSetup.NumPerWrap++;
            compute_Torus(&(objects[TorusSetup.torus_index].mesh));
            generate_and_load_buffers(false, &(objects[TorusSetup.torus_index].mesh));
            break;
        case 'n':
            if (TorusSetup.NumPerWrap > 4) {
                TorusSetup.NumPerWrap--;
                compute_Torus(&(objects[TorusSetup.torus_index].mesh));
                generate_and_load_buffers(false,&(objects[TorusSetup.torus_index].mesh));
            }
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
		selected_obj = selected_obj > 0 ? selected_obj - 1 : objects.size() - 1;
		break;
	case GLUT_KEY_RIGHT:
		selected_obj = (selected_obj + 1) < objects.size() ? selected_obj + 1 : 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void moveWaves(){

    glutPostRedisplay();

}
void main_menu_func(int option)
{
	switch (option)
	{
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
            glutIdleFunc(moveWaves);
            amImoving=true;
        }
        break;
    default:
		break;
	}
}


void material_menu_function(int option) 
{
	//EMPTY SEE EX3
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
	glutAddMenuEntry("Culling: ON", MenuOption::CULLING_ON);
	glutAddMenuEntry("Culling: OFF", MenuOption::CULLING_OFF);
	glutAddSubMenu("Material", materialSubMenu);
	glutAddMenuEntry("World coordinate system", MenuOption::CHANGE_TO_WCS);
	glutAddMenuEntry("Object coordinate system", MenuOption::CHANGE_TO_OCS);
    glutAddMenuEntry("Move waves", MenuOption::MOVEMENT);

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
void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
    if(objects[selected_obj].name=="light" && OperationMode==TRASLATING){
       light.position+=translation_vector;
    }

    glPushMatrix();
    glLoadIdentity();
    switch (TransformMode) {
        case WCS:
            glTranslatef(translation_vector[0],translation_vector[1],translation_vector[2]);
            glRotatef(angle,rotation_vector[0],rotation_vector[1],rotation_vector[2]);
            glScalef(scale_factor,scale_factor,scale_factor);
            glMultMatrixf(glm::value_ptr(objects[selected_obj].M));
            break;
        case OCS:
            glMultMatrixf(glm::value_ptr(objects[selected_obj].M));
            glTranslatef(translation_vector[0],translation_vector[1],translation_vector[2]);
            glRotatef(angle,rotation_vector[0],rotation_vector[1],rotation_vector[2]);
            glScalef(scale_factor,scale_factor,scale_factor);
            break;
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(objects[selected_obj].M));
    glPopMatrix();
}

void generate_and_load_buffers(bool generate, Mesh* mesh)
{
	if (generate) {
		// Genero 1 Vertex Array Object
		glGenVertexArrays(1, &mesh->vertexArrayObjID);
		// Genero 1 Vertex Buffer Object per i vertici
		glGenBuffers(1, &mesh->vertexBufferObjID);
		// Genero 1 Buffer Object per le normali
		glGenBuffers(1, &mesh->normalBufferObjID);
		// Genero 1 Buffer Object per le coordinate texture
		glGenBuffers(1, &mesh->uvBufferObjID);
	}

	glBindVertexArray(mesh->vertexArrayObjID);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), &mesh->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(glm::vec3), mesh->normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, mesh->texCoords.size() * sizeof(glm::vec2), mesh->texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,					// attribute index in the shader
		2,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void loadObjFile(string file_path, Mesh* mesh)
{
	FILE * file = fopen(file_path.c_str(), "r");
	if (file == NULL) {
		std::cerr << "\nFailed to open obj file! --> " << file_path << std::endl;
		std::getchar();
		exit(EXIT_FAILURE);
	}
	// tmp data structures
	vector<GLuint> vertexIndices, normalIndices, uvIndices;
	vector<glm::vec3> tmp_vertices,  tmp_normals;
	vector<glm::vec2> tmp_uvs;

	char lineHeader[128];
	while (fscanf(file, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, " %f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			tmp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			GLuint v_a, v_b, v_c; // index in position array
			GLuint n_a, n_b, n_c; // index in normal array
			GLuint t_a, t_b, t_c; // index in UV array

			fscanf(file, "%s", lineHeader);
			if (strstr(lineHeader, "//")) { // case: v//n v//n v//n
				sscanf(lineHeader, "%d//%d",&v_a, &n_a);
				fscanf(file,"%d//%d %d//%d\n", &v_b, &n_b, &v_c, &n_c);
				n_a--, n_b--, n_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
			}
			else if (strstr(lineHeader, "/")) {// case: v/t/n v/t/n v/t/n
				sscanf(lineHeader, "%d/%d/%d", &v_a, &t_a, &n_a);
				fscanf(file, "%d/%d/%d %d/%d/%d\n", &v_b, &t_b, &n_b, &v_c, &t_c, &n_c);
				n_a--, n_b--, n_c--;
				t_a--, t_b--, t_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
				uvIndices.push_back(t_a); uvIndices.push_back(t_b); uvIndices.push_back(t_c);
			}
			else {// case: v v v
				sscanf(lineHeader, "%d", &v_a);
				fscanf(file,"%d %d\n", &v_b, &v_c);
			}
			v_a--; v_b--; v_c--;
			vertexIndices.push_back(v_a); vertexIndices.push_back(v_b); vertexIndices.push_back(v_c);
		}
	}
	fclose(file);

	// If normals and uvs are not loaded, we calculate them for a default smooth shading effect
	if (tmp_normals.size() == 0) {
		tmp_normals.resize(vertexIndices.size() / 3, glm::vec3(0.0, 0.0, 0.0));
		// normal of each face saved 1 time PER FACE!
		for (int i = 0; i < vertexIndices.size(); i += 3)
		{
			GLushort ia = vertexIndices[i];
			GLushort ib = vertexIndices[i + 1];
			GLushort ic = vertexIndices[i + 2];
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(tmp_vertices[ib]) - glm::vec3(tmp_vertices[ia]),
				glm::vec3(tmp_vertices[ic]) - glm::vec3(tmp_vertices[ia])));
			tmp_normals[ i / 3 ] = normal;
			//Put an index to the normal for all 3 vertex of the face
			normalIndices.push_back(i/3);
			normalIndices.push_back(i/3);
			normalIndices.push_back(i/3);
		}
	}
	//if texture coordinates were not included we fake them
	if (tmp_uvs.size() == 0) {
		tmp_uvs.push_back(glm::vec2(0)); //dummy uv
		for (int i = 0; i < vertexIndices.size(); i += 3)
		{
			// The UV is dummy
			uvIndices.push_back(0);
			uvIndices.push_back(0);
			uvIndices.push_back(0);
		}
	}
	// The data for loaded for the glDrawArrays call, this is a simple but non optimal way of storing mesh data.
	// However, you could optimize the mesh data using a index array for both vertex positions, 
	// normals and textures and later use glDrawElements
	int i = 0;
	// Following the index arrays, the final arrays that will contain the data for glDrawArray are built.
	for (int i = 0; i < vertexIndices.size();  i++) {

		mesh->vertices.push_back(tmp_vertices[vertexIndices[i]]);
		mesh->normals.push_back(tmp_normals[normalIndices[i]]);
		mesh->texCoords.push_back(tmp_uvs[uvIndices[i]]);
	}
}

GLuint loadTexture(string path)
{
	int width, height, texChannels;
	GLuint textureID;
	stbi_uc* pixels = stbi_load(path.data(), &width, &height, &texChannels, STBI_rgb_alpha);
	if (!pixels) {
		std::cerr << "\nFailed to load texture image! --> " << path << std::endl;
		std::getchar();
		exit(EXIT_FAILURE);
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Texture displacement logic
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture sampling logic
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// data loading in memory
	glTexImage2D(GL_TEXTURE_2D,  //the target
		0, // the mip map level we want to generate
		GL_RGBA, 
		width, 
		height, 
		0, // border, leave 0
		GL_RGBA, // we assume is a RGB color image with 24 bit depth per pixel
		GL_UNSIGNED_BYTE, // the data type
		pixels);
	glGenerateMipmap(GL_TEXTURE_2D);// automatic mip maps generation

	stbi_image_free(pixels);
	return textureID;
}

/*
 * Computes the Vertex attributes data for segment number j of wrap number i.
 */
void computeTorusVertex(int i, int j, Mesh* mesh) {

	float theta = 2.f * glm::pi<float>() *(float)i / (float)TorusSetup.NumWraps;
	float phi = 2.f * glm::pi<float>()*(float)j / (float)TorusSetup.NumPerWrap;
	float sinphi = sin(phi);
	float cosphi = cos(phi);
	float sintheta = sin(theta);
	float costheta = cos(theta);

	float tmp = TorusSetup.MajorRadius + TorusSetup.MinorRadius * cosphi;
	float x = sintheta * tmp;
	float y = TorusSetup.MinorRadius * sinphi;
	float z = costheta * tmp;

	mesh->vertices.emplace_back(x, y, z);
	mesh->normals.emplace_back(sintheta*cosphi, sinphi, costheta*cosphi);
	//TODO consegna3
    mesh->texCoords.emplace_back( (1.0f/TorusSetup.NumWraps)*j, (1.0/TorusSetup.NumPerWrap)*i );

}

void compute_Torus(Mesh* mesh)
{
    mesh->vertices.clear();
    mesh->normals.clear();
    mesh->texCoords.clear();
    // draw the torus as NumWraps strips one next to the other
    for (int i = 0; i < TorusSetup.NumWraps; i++) {
        for (int j = 0; j <= TorusSetup.NumPerWrap; j++) {
            // first face   3
            //				| \
			//				1--2
            computeTorusVertex(i, j, mesh);
            computeTorusVertex(i + 1, j,mesh);
            computeTorusVertex(i , j + 1, mesh);
            // second face  1--3
            //				 \ |
            //				   2
            computeTorusVertex(i, j + 1, mesh);
            computeTorusVertex(i + 1, j, mesh);
            computeTorusVertex(i + 1, j + 1, mesh);
        }
    }
}

void drawAxisAndGrid()
{
	glUseProgram(shaders_IDs[Grid.shading]);
	glUniformMatrix4fv(base_uniforms[Grid.shading].M_Matrix_pointer, 1, GL_FALSE, value_ptr(Grid.M));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(Grid.mesh.vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, Grid.mesh.vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glUseProgram(shaders_IDs[Axis.shading]);
	// Caricamento matrice trasformazione del modello
	glUniformMatrix4fv(base_uniforms[Axis.shading].M_Matrix_pointer, 1, GL_FALSE, value_ptr(Axis.M));
	glActiveTexture(GL_TEXTURE0); // this addresses the first sampler2D uniform in the shader
	glBindTexture(GL_TEXTURE_2D, Axis.textureID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(Axis.mesh.vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, Axis.mesh.vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void printToScreen()
{
	string axis = "Axis: ";
	string mode = "Navigate/Modify: ";
	string obj = "Object: " + objects[selected_obj].name;
	string ref = "WCS/OCS: ";
	string mat = "Material: " + materials[objects[selected_obj].material].name;
	switch (WorkingAxis) {
	case X: axis += "X"; break;
	case Y: axis += "Y"; break;
	case Z: axis += "Z"; break;
	}
	switch (OperationMode) {
	case TRASLATING: mode += "Translate"; break;
	case ROTATING: mode += "Rotate"; break;
	case SCALING: mode += "Scale"; break;
	case NAVIGATION: mode += "Navigate"; break;
	}
	switch (TransformMode) {
	case OCS: ref += "OCS"; break;
	case WCS: ref += "WCS"; break;
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, WindowHeight * aspect_ratio, 0, WindowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	vector<string> lines;
	lines.push_back(mat);
	lines.push_back(obj);
	lines.push_back(axis);
	lines.push_back(mode);
	lines.push_back(ref);
	glDisable(GL_DEPTH_TEST);
	HUD_Logger::get()->printInfo(lines);
	glEnable(GL_DEPTH_TEST);

	resize(WindowWidth, WindowHeight);
}
