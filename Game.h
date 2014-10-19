#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h> //vsprintf
#include <vector> //object storage
#include <sstream> //font insertion

#include "engine/Input_Manager.h"
#include "engine/Texture_Manager.h"
#include "engine/Mesh_Manager.h"
#include "engine/Sound_Manager.h"
#include "engine/Renderer.h"
#include "engine/Camera.h"
#include "engine/Camera_Third.h"
#include "engine/Text.h"
#include "engine/Button.h"
#include "engine/Object.h"
#include "engine/Particle_Spawner.h"
#include "engine/State_Machine.h"

#include "object/Missile.h"
#include "object/Ship_Player.h"

enum Game_State
{
	EMPTY,
	MENU,
	GAME,
	GAME_OVER
};

class Game
{
private:
	Renderer* renderer;
	Input_Manager* input_manage;
	Texture_Manager* texture_manage;
	Mesh_Manager* mesh_manage;
	Sound_Manager* sound_manage;
	Camera* camera;
	State_Machine<Game_State, Game>* state_machine;
	std::vector<Object*> object_queue;
	std::vector<Text*> text_queue;
	std::vector<Button*> button_queue;
	std::vector<Particle_Spawner*> particle_queue;
	unsigned long long frame_count;

public:
	Game();
	~Game();

	bool initialise(HWND window_handler, bool fullscreen, Input_Manager* input, Renderer* render);
	bool initialise_content();
	void update(float timestep);
	void render();

	//Menu State Functions
	void menu_enter();
	void menu_update(float timestep);
	void menu_render();
	void menu_exit();

	//Game State Functions
	void game_enter();
	void game_update(float timestep);
	void game_render();
	void game_exit();

	//Game Over State Functions
	void gameover_enter();
	void gameover_update(float timestep);
	void gameover_render();
	void gameover_exit();

	void trace(const char * fmt, ...);
	void cycle_state(int value);
};

#endif
