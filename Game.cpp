#include "Game.h"

Game::Game()
{
	renderer = NULL;
	texture_manage = NULL;
	mesh_manage = NULL;
	input_manage = NULL;
}

Game::~Game()
{
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->release(mesh_manage);
	}

	if(renderer != NULL)
	{
		delete renderer;
		renderer = NULL;
	}

	if(mesh_manage != NULL)
	{
		delete mesh_manage;
		mesh_manage = NULL;
	}

	if(texture_manage != NULL)
	{
		delete texture_manage;
		texture_manage = NULL;
	}

	if(input_manage != NULL)
	{
		delete input_manage;
		input_manage = NULL;
	}
}

bool Game::initialise(HWND window_handler, bool fullscreen, Input_Manager* input)
{
	input_manage = input;

	renderer = new Renderer();
	if(!renderer->initialise(window_handler, fullscreen))
	{
		return FALSE;
	}

	texture_manage = new Texture_Manager();
	mesh_manage = new Mesh_Manager(texture_manage);

	return TRUE;
}

bool Game::initialise_content()
{
	camera = new Camera();

	//START Button creation and initialization
	texture_manage->load(renderer->get_device(), "texture/Button.png");

	button_queue.push_back(new Button(this, texture_manage->get_texture("texture/Button.png"), 
		renderer->get_font(), "test", 128, 64, D3DXVECTOR3(320, 240, 0), &Game::action));

	for(size_t i = 0; i < button_queue.size(); i++)
	{
		if(!button_queue[i]->initialise_button(renderer->get_device()))
		{
			return FALSE;
		}
	}
	//END Button creation and initialization

	// START Text box for mouse coordinates
	RECT title_position;
	title_position.bottom = 100;
	title_position.top = 0;
	title_position.left = 0;
	title_position.right = 80;
	text_queue.push_back(new Text(title_position, DT_LEFT | DT_NOCLIP | DT_VCENTER,
		D3DCOLOR_ARGB(255, 255, 255, 255), TRUE));
	// END Text box for mouse coordinates

	return TRUE;
}

void Game::action(int value)
{
	trace("Hi!");
}

void Game::update(float timestamp)
{
	input_manage->begin_update();

	//Update text[0]
	std::stringstream font_output;
	if(text_queue[0]->is_visible())
	{
		font_output << "X " << input_manage->get_mouse_x() << "\n"
					<< "Y " << input_manage->get_mouse_y() << "\n";
		text_queue[0]->update(font_output.str());
		font_output.str("");
	}

	//Action on Esc pressed
	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	//Update objects
	for(size_t i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestamp);
	}

	//Update buttons
	for(size_t i = 0; i < button_queue.size(); i++)
	{
		button_queue[i]->update(input_manage->get_mouse_x(), input_manage->get_mouse_y(),
								input_manage->get_mouse_up(LEFT_MOUSE));
	}

	input_manage->end_update();
}

void Game::render()
{
	renderer->render(object_queue, button_queue, text_queue, camera);
}

void Game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}