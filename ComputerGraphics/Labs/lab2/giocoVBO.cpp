// GiocoVBO.cpp
//
// Keyboard PRESS:
// SPACE	attack
// a		left
// d		right

#include <iostream>
#include <sstream>
# include <GL/glew.h>
# include <GL/freeglut.h>
#include <cmath>

#define PI 3.14159265358979323846
#define nvertices 24
using namespace std;

int width = 1280; // dimensioni iniziali della finestra
int height = 720;

int		frame = 0;
float	dxnemici = 0;
float	dynemici = 0;
float	NumeroColpiti = 0;
int		Vnemici = 220;
int		VerticiUfo = 220;
double	playerSpeed = 0; //velocita orizzontale (pixel per frame)
int		scuoti = 0;
double	accelerazione = 1;		// forza di accelerazione data dalla tastiera
float	playerPositionX = width / 2;		//coordinate della posizione iniziale dell'UFO
float	playerPositionY = height * 0.1;
float	posxN, posyN;			//coordinate della posizione dei nemici
float	posizione_di_equilibrio = playerPositionY;
float	angolo = 0;
double	range_fluttuazione = 15;	// fluttuazione
double	angle = 0;					// angolo di fluttuazione
double	angle_offset = 10;			// quanto è accentuata l'oscillazione angolare
double	float_yoffset = 0;			// distacco dalla posizione d'equilibrio
int		frame_animazione = 0;		// usato per animare la fluttuazione
int		n_vertici = 0;

typedef struct { double r, g, b, a; } ColorRGBA;

// Dichiarazioni Variabili VBO
GLuint vbo_nemici, vbo_colore_nemici, vbo_ufo, vbo_colore_ufo;
GLuint vbo_luna, vbo_colore_luna, vbo_alone_luna, vbo_colore_alone;

string	Ncolpiti;
float	pos_proiettile_MONDO_x, pos_proiettile_MONDO_y;
float	posx_Proiettile, posy_Proiettile;
int		nemici_num_colonne = 5;
int		nemici_per_colonna = 3;
bool	luna_colpita = false;
bool	**colpito;
float	xluna = 320;
float	yluna = 640;

typedef struct {
    float x, y, z;
} Point;

//strutture per la geometria del gioco
ColorRGBA *colore_luna, *colore_alone, *coloreC, *coloreCN;
Point *luna, *alone_luna;
Point* ufo, *nemici;

//Alcune utility
double  degtorad(double angle) {
    return angle * PI / 180;
}

double  radtodeg(double angle) {
    return angle / (PI / 180);
}

// valore assoluto
double static Abs(double num) {
    return num < 0 ? -num : num;
}

// ritorna un numero casuale in un certo range
double  random_range(double min, double max) {
    return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
}

string floatToString(float value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

void bitmap_output(int x, int y, int z, string s,
                   void *font)
{
    int i;
    glRasterPos3f(x, y, 0);

    for (i = 0; i < s.length(); i++)
        glutBitmapCharacter(font, s[i]);
}

//Colori corpo ufo
ColorRGBA color_metaldark = { 0.6, 0.6 , 0.6, 1.0 };
ColorRGBA color_metallight = { 0.8, 0.8 , 0.8, 1.0 };

ColorRGBA color_cabindark = { 0.2, 0.6, 0.9 , 1.0 };
ColorRGBA color_cabinlight = { 0.7 , 0.85, 1.0 , 1.0 };

void costruisci_cerchio(float cx, float cy, float raggiox, float raggioy, Point *Cerchio) {
    int i;
    float stepA = (2 * PI) / nvertices;
    int componenti = 0;

    for (i = 0; i < nvertices; i++)
    {
        Cerchio[componenti].x = cx + cos((double)i *stepA)*raggiox;
        Cerchio[componenti].y = cy + sin((double)i *stepA)*raggioy;
        Cerchio[componenti].z = 0;
        Cerchio[componenti + 1].x = cx + cos((double)(i + 1) *stepA)*raggiox;
        Cerchio[componenti + 1].y = cy + sin((double)(i + 1) *stepA)*raggioy;
        Cerchio[componenti + 1].z = 0;
        Cerchio[componenti + 2].x = 0;
        Cerchio[componenti + 2].y = 0;
        Cerchio[componenti + 2].z = 0;

        componenti += 3;
    }
}

// Gestione Interazione
void updateProiettile(int value)
{
    //Ascissa del proiettile durante lo sparo (non cambia mai)
    posx_Proiettile = 0;
    //Ordinata del proiettile durante lo sparo
    posy_Proiettile++;

    //L'animazione deve avvenire finchè  l'ordinata del proiettile raggiunge un certo valore fissato
    if (posy_Proiettile <= 50)
        glutTimerFunc(5, updateProiettile, 0);
    else
        posy_Proiettile = 0;

    glutPostRedisplay();
}

void updateNemici(int value)
{
    //Ascissa del proiettile durante lo sparo
    frame++;

    if (frame % 30 == 0)
    {
        dxnemici--;
        dynemici--;
    }
    glutTimerFunc(5, updateNemici, 0);

    glutPostRedisplay();
}

bool pressing_left = false;
bool pressing_right = false;
bool pressing_attack = false;

void keyboardPressedEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
            pressing_attack = true;
            updateProiettile(0);
            break;
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
        case ' ':
            pressing_attack = false;
            break;
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
    //Interpolazione lineare tra a e b secondo amount
    return (1 - amount)*a + amount * b;
}

void update(int a)
{
    bool moving = false;
    //Movimento in orizzontale
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
    if (float_yoffset >= 0) {
        frame_animazione += 6;
        if (frame_animazione >= 360) {
            frame_animazione -= 360;
        }
    }

    if (!moving) {
        // Se l'ufo non si sta muovendo con i tasti a e d
        //  decremento od incremento la velocita' iniziale fino a portarla
        //  a zero e l'ufo continua a rimbalzare sul posto

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
    //Aggioramento della posizione in x della navicella,
    // che regola il movimento orizzontale
    playerPositionX += playerSpeed;

    //Se l'ufo esce dallo schermo lateralmente (x minore di 0 o maggiore di width)
    //la navicella rimbalza ai bordi dello schermo
    if (playerPositionX < 0) {
        playerPositionX = 0;
        playerSpeed = -playerSpeed * 0.8;
    }
    if (playerPositionX > width) {
        playerPositionX = width;
        playerSpeed = -playerSpeed * 0.8;
    }
    // movimento UFO
    // calcolo y come somma dei seguenti contributi: pos. di equilibrio, oscillazione periodica
    playerPositionY = posizione_di_equilibrio + sin(degtorad(frame_animazione))*range_fluttuazione;
    // movimento nemici
    angolo = cos(degtorad(frame_animazione))*angle_offset - playerSpeed * 1.3;

    glutPostRedisplay();
    glutTimerFunc(15, update, 0);
}

void costruisci_ufo(Point* ufo, ColorRGBA *coloreC, bool nemico)
{
    Point *corpo1, *corpo2;
    corpo1 = new Point[72];
    corpo2 = new Point[72];

    costruisci_cerchio(0, 0, 1, 1, ufo);
    for (int i = 0; i < nvertices; i++)
    {
        coloreC[i * 3] = color_metaldark;
        coloreC[i * 3 + 1] = color_metaldark;
        coloreC[i * 3 + 2] = color_metallight;
    }

    costruisci_cerchio(0.05, 0.1, 0.7, 0.7, corpo1);
    int cont = 72;
    for (int i = 0; i < 72; i++)
    {
        ufo[cont + i] = corpo1[i];
    }

    for (int i = 0; i < nvertices; i++)
    {
        coloreC[cont + i * 3] = { 1,1,1,32. / 255 };
        coloreC[cont + i * 3 + 1] = { 1,1,1,32. / 255 };
        coloreC[cont + i * 3 + 2] = { 1,1,1,32. / 255 };
    }

    //Costruisci decorazione triangolare
    cont = 144;
    ufo[cont].x = 0;
    ufo[cont].y = 0;
    ufo[cont].z = 0;
    ufo[cont + 1].x = 1;
    ufo[cont + 1].y = 1;
    ufo[cont + 1].z = 0;
    ufo[cont + 2].x = -1;
    ufo[cont + 2].y = 1;
    ufo[cont + 2].z = 0;

    //Colore della decorazione
    if (nemico)
    {
        coloreC[cont] = { 0,0,1,0.4 };
        coloreC[cont + 1] = { 0,0.4,1,1 };
        coloreC[cont + 2] = { 0,0.2,1,1 };
    }
    else
    {
        coloreC[cont] = { 1,0,1,0.4 };
        coloreC[cont + 1] = { 1,0.4,1,1 };
        coloreC[cont + 2] = { 1,0.2,1,1 };
    }
    //Costruisci cabina
    cont = 147;
    costruisci_cerchio(0.0, 0.2, 0.5, 0.6, corpo2);
    for (int i = 0; i < 72; i++)
    {
        ufo[cont + i] = corpo2[i];
    }
    for (int i = 0; i < nvertices; i++)
    {
        coloreC[cont + i * 3] = color_cabindark;
        coloreC[cont + i * 3 + 1] = color_cabindark;
        coloreC[cont + i * 3 + 2] = color_cabinlight;
    }
    cont = 219;

    //Proiettile
    ufo[cont].x = 0.0;
    ufo[cont].y = 0.0;
    ufo[cont].z = 0.0;
    coloreC[cont] = { 1.0,1.0,1.0,1.0 };
}

void InitVBO() {
    //Questa funzione costruisce i vbo degli oggetti nella scena: luna, alone, ufo nemico
    // ed i vbo dei corrispondenti colori
    // Viene chiamata nel main dopo il glewInit() in quanto le funzioni per generare i VBO
    // fanno parte dell'estensione di opengl.

    //Viene generato il VBO e gli viene assegnato l'hanfr vbo_luna (variabile globale di tipo GLuint)
    glGenBuffers(1, &vbo_luna);
    //Viene reso attivo il VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_luna);
    //Alloca lo spazio sulla GPU e copia i dati a partire da luna
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(Point), luna, GL_STATIC_DRAW);
    //Rilascia il VBO appena creato
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_colore_luna);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_luna);
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(ColorRGBA), colore_luna, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_alone_luna);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_alone_luna);
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(Point), alone_luna, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_colore_alone);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_alone);
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(ColorRGBA), colore_alone, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_ufo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ufo);
    glBufferData(GL_ARRAY_BUFFER, VerticiUfo * sizeof(Point), ufo, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &vbo_colore_ufo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_ufo);
    glBufferData(GL_ARRAY_BUFFER, VerticiUfo * sizeof(ColorRGBA), coloreC, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_nemici);   // create two VBO handles, one position, one color handle
    glBindBuffer(GL_ARRAY_BUFFER, vbo_nemici);   // bind the first handle
    glBufferData(GL_ARRAY_BUFFER, Vnemici * sizeof(Point), nemici, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up
    glGenBuffers(1, &vbo_colore_nemici);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_nemici);
    glBufferData(GL_ARRAY_BUFFER, Vnemici * sizeof(ColorRGBA), coloreCN, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // clean up

// A questo punto abbiamo spostato la geometria e i dati del colore su GPU
}

//Costruisci scena: luna, ufo e nemico e dei colori corrispondenti
//Questa funzione viene chiamata nel main, e non nel main loop,
//cioè viene eseguita solo una volta e non ad ogni ciclo di disegno
void costruisci_scena(void)
{
    //Colore dell'ufo
    coloreC = new ColorRGBA[VerticiUfo];
    //Colore dei nemici
    coloreCN = new ColorRGBA[VerticiUfo];

    // Draw MOON:
    //La luna sara' costituita da 24 triangoli che hanno un vertice coincidente
    //con l'Origine, quindi 72 vertici (alcuni dei quali sono ripetuti)
    luna = new Point[72];
    colore_luna = new ColorRGBA[72];

    //Alone di luce della luna e del colore di ogni suo vertice:
    alone_luna = new Point[72];
    colore_alone = new ColorRGBA[72];

    ColorRGBA c_top;
    c_top = { 1.0, 1.0, 1.0, 1.0 };
    ColorRGBA c_bot;
    c_bot = { 1.0, 0.9, 1.0, 1.0 };

    //costruzione luna
    costruisci_cerchio(0, 0, 1, 1, luna);
    for (int i = 0; i < nvertices; i++)
    {
        colore_luna[i * 3] = c_top;
        colore_luna[i * 3 + 1] = c_top;
        colore_luna[i * 3 + 2] = c_bot;

    }
    c_top = { 255,255,255,0 };
    c_bot = { 255,220,255,255 };

    //Costruzione Alone e dei suoi colori
    costruisci_cerchio(0, 0, 1, 1, alone_luna);
    for (int i = 0; i < nvertices; i++)
    {
        colore_alone[i * 3] = c_top;
        colore_alone[i * 3 + 1] = c_top;
        colore_alone[i * 3 + 2] = c_bot;
    }

    // Draw UFO:
    // Utente: 73 triangoli più un vertice che rappresenta il proiettile (220 vertici)
    ufo = new Point[VerticiUfo];
    costruisci_ufo(ufo, coloreC, true);

    // Nemico: 73 triangoli più un vertice che rappresenta il proiettile (nascosto)
    nemici = new Point[VerticiUfo];
    costruisci_ufo(nemici, coloreCN, false);
}
void disegna_luna_e_alone()
{
    // Disegna Luna
    glPushMatrix();
    glTranslatef(xluna, yluna, 0);
    glScalef(30, 30, 1);
    //Costruisce un array di vertici:
    // primo argomento indica le coordinate di ogni vertice della montagna,
    // seconda componente il tipo, (FLOAT, INT; DOUBLE),
    // terzo indica il sizeof di ogni vertice (tipo Point),
    // quarto puntatore alla struttura a partire dalla quale si costruita il vertex array.*/
    glBindBuffer(GL_ARRAY_BUFFER, vbo_luna);
    glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_luna);
    glColorPointer(4, GL_DOUBLE, 0, (char *)NULL);
    glDrawArrays(GL_TRIANGLES, 0, 72);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    // Disegna Alone luna
    glPushMatrix();
    glTranslatef(320, 640, 0);
    glScalef(80, 80, 1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_alone_luna);
    glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_alone);
    glColorPointer(4, GL_DOUBLE, 0, (char *)NULL);
    glDrawArrays(GL_TRIANGLES, 0, 72);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
    /////////////////////////////////////////////////////
}
void drawScene(void)
{
    ColorRGBA c_top;
    ColorRGBA c_bot;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    if (scuoti > 0) {
        glPushMatrix();
        glTranslated(random_range(-scuoti, scuoti), random_range(-scuoti, scuoti), 0);
        disegna_luna_e_alone();
        glPopMatrix();
        scuoti = 0;
    }
    disegna_luna_e_alone();
    ///////////////////////////////////////////////////////
    // Disegna Nemici
    float passo_Nemici = ((float)width) / nemici_num_colonne;
    float passo_righe = 50;
    for (int i = 0; i < nemici_per_colonna; i++)
    {
        posyN = height - i * passo_righe - 20;
        for (int j = 0; j < nemici_num_colonne; j++)
        {
            posxN = j * (passo_Nemici)+passo_Nemici / 2;
            if (!colpito[i][j]) {
                glPushMatrix();
                glTranslatef(posxN, posyN, 0);
                glTranslatef(dxnemici, dynemici, 0);
                glRotatef(angolo, 0, 0, 1);
                glScalef(55, 22.5, 1);

                //Rendo attivo il VBO dei vertici dei nemici
                glBindBuffer(GL_ARRAY_BUFFER, vbo_nemici);
                glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
                //Rendo attivo il VBO dei colori dei vertici nemici
                glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_nemici);
                glColorPointer(4, GL_DOUBLE, 0, (char *)NULL);
                glDrawArrays(GL_TRIANGLES, 0, Vnemici - 1);

                //Disattivo i buffer
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glPopMatrix();
            }
        }
    }
    for (int i = 0; i < nemici_per_colonna; i++)
    {
        posyN = height - i * passo_righe - 20 + dynemici;
        for (int j = 0; j < nemici_num_colonne; j++)
        {
            posxN = j * (passo_Nemici)+passo_Nemici / 2 + dxnemici;
            if (((pos_proiettile_MONDO_x >= posxN - 20) && (pos_proiettile_MONDO_x <= posxN + 20)) &&
                ((pos_proiettile_MONDO_y >= posyN - 20) && (pos_proiettile_MONDO_y <= posyN + 20)))
            {
                if (!colpito[i][j])
                {
                    NumeroColpiti++;
                    colpito[i][j] = true;
                }
            }
        }
    }

    ///////////////////////////////////////////////////////
    //Disegno l'ufo
    glPushMatrix();
    glTranslatef(playerPositionX, playerPositionY, 0);
    glScalef(55, 22.5, 1);
    glRotatef(180, 0, 0, 1);
    glPushMatrix();
    glRotatef(angolo, 0, 0, 1);
    //Rendo attivo il vbo contenente le posizioni dei vertici dell'ufo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ufo);
    glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
    //Rendo attivo il vbo contenente i colori dei vertici dell'ufo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colore_ufo);
    glColorPointer(4, GL_DOUBLE, 0, (char *)NULL);
    glDrawArrays(GL_TRIANGLES, 0, VerticiUfo - 1);
    glPopMatrix();
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    //Disegno il proiettile
    glPointSize(10.0);
    glTranslatef(posx_Proiettile, -posy_Proiettile, 0.0);
    glDrawArrays(GL_POINTS, VerticiUfo - 1, 1);

    float matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    //coordinate del proiettile nel sistema di riferimento OCS=(0.0,0.0)

    //COORDINATE DEL PROIETTILE NEL SISTEMA DI RIFERIMENTO WCS=OCS+(dx,dy)
    pos_proiettile_MONDO_x = 0.0+matrix[12];
    pos_proiettile_MONDO_y = 0.0+matrix[13];

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
    ///////////////////////////////////////////////////////

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    if (((pos_proiettile_MONDO_x >= xluna - 30) && (pos_proiettile_MONDO_x <= xluna + 30))
        && ((pos_proiettile_MONDO_y >= yluna - 30) && (pos_proiettile_MONDO_y <= yluna + 30)))
        scuoti = 10;

    //Aggiorno sulla finestra il numero di nemici colpiti
    glColor3f(1.0, 0.0, 0.0);
    string testo = "Nemici Colpiti ";
    //Trasformo in stringa il numero di nemici colpiti
    Ncolpiti = floatToString(NumeroColpiti);
    //Appendo alla stringa di testo che viene visualizzata il numero aggiornato di nemici colpiti
    testo.append(Ncolpiti.substr(0, 4));
    bitmap_output(350, 20, 0, testo, GLUT_BITMAP_TIMES_ROMAN_24);

    glutSwapBuffers();
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

void Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    colpito = new bool*[nemici_per_colonna];
    for (int i = 0; i < nemici_per_colonna; i++)
        colpito[i] = new bool[nemici_num_colonne];

    for (int i = 0; i < nemici_per_colonna; i++)
        for (int j = 0; j < nemici_num_colonne; j++)
        {
            colpito[i][j] = false;
        }

    //Costruisco la geometria della scena iniziale
    costruisci_scena();

    //Costruisco i VBO (buffer dei dati memorizzati sulla GPU)
    InitVBO();
}

int  main(int argc, char **argv)
{

    GLboolean GlewInitResult;

    glutInit(&argc, argv);

   // glutInitContextVersion(4, 2);
    //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("UFOGame");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboardPressedEvent);
    glutKeyboardUpFunc(keyboardReleasedEvent);

    //gestione animazione
    glutTimerFunc(11, update, 0);
    glutTimerFunc(24, updateNemici, 0);

    GlewInitResult = glewInit();

    Init();

    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
    );
    glutMainLoop();
}