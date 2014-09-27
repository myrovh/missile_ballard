#include "Game.h"

Game::Game()
{
	renderer = NULL;
	texture_manage = NULL;
	mesh_manage = NULL;
	input_manage = NULL;
	sound_manage = NULL;
}

Game::~Game()
{
	for(unsigned int i = 0; i < particle_queue.size(); i++)
	{
		particle_queue[i]->release(texture_manage);
	}

	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->release(mesh_manage);
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

	if(sound_manage != NULL)
	{
		delete sound_manage;
		sound_manage = NULL;
	}

	if(input_manage != NULL)
	{
		delete input_manage;
		input_manage = NULL;
	}
}

bool Game::initialise(HWND window_handler, bool fullscreen, Input_Manager* input, Renderer* renderer)
{
	input_manage = input;
	this->renderer = renderer;

	sound_manage = new Sound_Manager();
	if(FAILED(sound_manage->initialise()))
	{
		return FALSE;
	}
	texture_manage = new Texture_Manager();
	mesh_manage = new Mesh_Manager(texture_manage);

	return TRUE;
}

bool Game::initialise_content()
{
	camera = new Camera(D3DXVECTOR3(0, 3, 3), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0),
							  D3DX_PI / 2, 640 / (float)480, 0.1f, 200.0f);

	//START Texture Loading
	texture_manage->load(renderer->get_device(), "texture/Button.png");
	texture_manage->load(renderer->get_device(), "texture/EnemyShipTexture-Blue.png");
	texture_manage->load(renderer->get_device(), "texture/EnemyShipTexture-Green.png");
	texture_manage->load(renderer->get_device(), "texture/EnemyShipTexture-Red.png");
	texture_manage->load(renderer->get_device(), "texture/LaserBlast.png");
	texture_manage->load(renderer->get_device(), "texture/PlayerShipTexture.png");
	texture_manage->load(renderer->get_device(), "texture/skybox.png");
	texture_manage->load(renderer->get_device(), "texture/point_particle.bmp");
	//END Texture Loading

	//START Mesh Loading
	if(!mesh_manage->load(renderer->get_device(), "mesh/EnemyShip-Blue.x"))
	{
		return FALSE;
	}
	if(!mesh_manage->load(renderer->get_device(), "mesh/EnemyShip-Green.x"))
	{
		return FALSE;
	}
	if(!mesh_manage->load(renderer->get_device(), "mesh/EnemyShip-Red.x"))
	{
		return FALSE;
	}
	if(!mesh_manage->load(renderer->get_device(), "mesh/LaserBlast.x"))
	{
		return FALSE;
	}
	if(!mesh_manage->load(renderer->get_device(), "mesh/PlayerShip.x"))
	{
		return FALSE;
	}
	if(!mesh_manage->load(renderer->get_device(), "mesh/Skybox.x"))
	{
		return FALSE;
	}
	//END Mesh Loading

	//START Sound Loading
	sound_manage->load("sound/engine.wav");
	//END Sound Loading

	//START Object Creation
	object_queue.push_back((new Ship_Enemy(mesh_manage->get_mesh("mesh/EnemyShip-Blue.x"), D3DXVECTOR3(3.0f, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0.2f)));
	object_queue.push_back((new Ship_Enemy(mesh_manage->get_mesh("mesh/EnemyShip-Red.x"), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0.2f)));
	object_queue.push_back((new Ship_Enemy(mesh_manage->get_mesh("mesh/EnemyShip-Green.x"), D3DXVECTOR3(-3.0f, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0.2f)));
	object_queue.push_back((new Ship_Enemy(mesh_manage->get_mesh("mesh/LaserBlast.x"), D3DXVECTOR3(0, 1.0f, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.2f, 0.2f)));
	object_queue.push_back((new Ship_Player(mesh_manage->get_mesh("mesh/PlayerShip.x"), D3DXVECTOR3(0.0f, 0.0f, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, input_manage, sound_manage->get_sound("sound/engine.wav"))));
	object_queue.push_back((new Ship_Enemy(mesh_manage->get_mesh("mesh/Skybox.x"), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0)));
	//END Object Creation

	//START Particle Spawner Creation
	particle_queue.push_back(new Particle_Spawner);
	particle_queue[0]->set_position(D3DXVECTOR3(0.0f, 10.0f, 0));
	particle_queue[0]->set_particle_texture(texture_manage->get_texture("texture/point_particle.bmp"));
	particle_queue[0]->set_max_particles(1000);
	particle_queue[0]->set_number_to_release(5);
	particle_queue[0]->set_release_interval(0.05f);
	particle_queue[0]->set_lifetime(5.0f);
	particle_queue[0]->set_size(0.3f);
	particle_queue[0]->set_particle_colour(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));
	particle_queue[0]->set_velocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	particle_queue[0]->set_gravity(D3DXVECTOR3(0.0f, -3.0f, 0.0f));
	//particle_queue[0]->set_wind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	particle_queue[0]->set_velocity_variance(10.0f);
	particle_queue[0]->initialise(renderer->get_device());
	//END Particle Spawner Creation

	//START Button Creation
	button_queue.push_back(new Button(this, texture_manage->get_texture("texture/Button.png"), 
		renderer->get_font(), "test", 128, 64, D3DXVECTOR3(320, 240, 0), &Game::action));

	for(size_t i = 0; i < button_queue.size(); i++)
	{
		if(!button_queue[i]->initialise_button(renderer->get_device()))
		{
			return FALSE;
		}
	}
	//END Button Creation

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
		font_output << "X " << input_manage->get_mouse_x_centered() << "\n"
					<< "Y " << input_manage->get_mouse_y_centered() << "\n";
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

	//Update particle spawners
	for(size_t i = 0; i < particle_queue.size(); i++)
	{
		particle_queue[i]->update(timestamp);
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
	renderer->render(object_queue, particle_queue, button_queue, text_queue, camera);
}

void Game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}