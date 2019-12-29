#include "HUD_Logger.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


using namespace std;

extern float w_width, w_height;

HUD_Logger* HUD_Logger::logger;
bool HUD_Logger::ready;

void stroke_char_output(int x, int y, int z, std::string s, void * font)
{
	int i;
	glRasterPos3f(x, y, 0);
	for (i = 0; i < s.length(); i++)
		glutStrokeCharacter(font, s[i]);
}

HUD_Logger::HUD_Logger() {
	this->last_frame = chrono::high_resolution_clock::now();
	this->last_update = chrono::high_resolution_clock::now();
	this->frame_time = 0.0;
}

HUD_Logger* HUD_Logger::get()
{
	if (!ready) {
		logger =  new HUD_Logger();
		ready = true;
	}
	return logger;
}

void HUD_Logger::print_FPS()
{
	auto time = chrono::high_resolution_clock::now();
	auto nano_frame_time = chrono::duration_cast<std::chrono::microseconds>(time - last_frame).count();
	this->last_frame = time;

	if (chrono::duration_cast<std::chrono::milliseconds>(time - last_update).count() > 500.0)
	{
		frame_time = nano_frame_time;
		last_update = chrono::high_resolution_clock::now();
	}
	string fps = "Fps: " + std::to_string(1000.0 / frame_time * 1000.0);
	string f_time = "Frame time: " + std::to_string(frame_time / 1000.0) + "ms";

	
	int total_width_fps = 0,  total_width_f_time = 0, total_height = glutStrokeHeight(GLUT_STROKE_ROMAN);;

	for (unsigned int i = 0; i < fps.length(); i++)
	{
		total_width_fps += glutStrokeWidth(GLUT_STROKE_ROMAN, fps[i]);
	}
	for (unsigned int i = 0; i < f_time.length(); i++)
	{
		total_width_f_time += glutStrokeWidth(GLUT_STROKE_ROMAN, f_time[i]);
	}
	glLineWidth(2.0);
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(w_width - total_width_fps * 0.2, w_height - total_height * 0.2, 0.0);
	glScalef(0.2, 0.2, 1.0);
	stroke_char_output(0.0 , 0.0, 0.0, fps, GLUT_STROKE_ROMAN);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(w_width - total_width_f_time * 0.2, w_height - total_height * 2.0 * 0.2, 0.0);
	glScalef(0.2, 0.2, 1.0);
	stroke_char_output(0.0, 0.0, 0.0, f_time, GLUT_STROKE_ROMAN);
	glPopMatrix();
}

void HUD_Logger::printMiddleScreenMsg(std::string msg)
{
	float size = 0.5;
	glColor3f(1.0, 1.0, 1.0);
	int total_width = 0, total_height = glutStrokeHeight(GLUT_STROKE_ROMAN);;
	for (unsigned int i = 0; i < msg.length(); i++)
	{
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, msg[i]);
	}
	glPushMatrix();
	glTranslatef(w_width / 2.0f - total_width*size / 2.0f, w_height / 2.0f, 0.0);
	glScalef(size, size, 1.0);
	//glTranslatef(- total_width / 2.0f, -total_height / 2.0f, 0.0);
	stroke_char_output(0.0, 0.0, 0.0, msg, GLUT_STROKE_ROMAN);
	glPopMatrix();
}

