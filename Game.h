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
#include "engine/Renderer.h"
#include "engine/Camera.h"
#include "engine/Text.h"
#include "engine/Button.h"
#include "engine/Object.h"
#include "object/Ship_Enemy.h"

class Game
{
private:
	Renderer* renderer;
	Input_Manager* input_manage;
	Texture_Manager* texture_manage;
	Mesh_Manager* mesh_manage;
	Camera* camera;
	std::vector<Object*> object_queue;
	std::vector<Text*> text_queue;
	std::vector<Button*> button_queue;

public:
	Game();
	~Game();

	bool initialise(HWND window_handler, bool fullscreen, Input_Manager* input, Renderer* render);
	bool initialise_content();
	void update(float timestep);
	void render();
	void trace(const char * fmt, ...);
	void action(int value);
};

#endif
