#pragma once
#include <chrono>
#include <string>

/*
Questa classe statica contiene un singleton utilizzato come ipotetico logger a schermo.
I calcoli delle tempistiche sono resi precisi dalla libreria chrono.
*/
class HUD_Logger
{
private:
	HUD_Logger();
public:
	/*
	restituisce il logger, se no esiste lo crea...
	*/
	static HUD_Logger* get();
	void print_FPS();
	void printMiddleScreenMsg(std::string msg);
private:
	static HUD_Logger* logger;
	static bool ready;
	std::chrono::time_point<std::chrono::steady_clock> last_frame;
	std::chrono::time_point<std::chrono::steady_clock> last_update;
	double frame_time;
	int fps;
};

