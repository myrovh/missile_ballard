#include "Game.h"

Game::Game()
{
	renderer = NULL;
	texture_manage = NULL;
	mesh_manage = NULL;
	input_manage = NULL;
	sound_manage = NULL;
	state_machine = NULL;
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

	if(state_machine)
	{
		delete state_machine;
		state_machine = NULL;
	}
}

//! Create Manager classes
/*!
	Order of Instantiation is:
	State_Machine
	Input_Manager
	File_Manager should be loaded here
	Renderer
	Sound_Manager
	Texture_Manager
	Mesh_Manager
	Game Assets Loaded
	Set State to default state
*/
bool Game::initialise(HWND window_handler, bool fullscreen, Input_Manager* input, Renderer* renderer)
{
	//Create state machine
	state_machine = new State_Machine<Game_State, Game>(this, EMPTY);
	state_machine->register_state(MENU, &Game::menu_enter, &Game::menu_update,
								  &Game::menu_render, &Game::menu_exit);
	state_machine->register_state(GAME, &Game::game_enter, &Game::game_update,
								  &Game::game_render, &Game::game_exit);
	state_machine->register_state(GAME_OVER, &Game::gameover_enter, &Game::gameover_update,
								  &Game::gameover_render, &Game::gameover_exit);

	//Insert input_manager and renderer
	input_manage = input;
	this->renderer = renderer;

	//Create Sound manager
	sound_manage = new Sound_Manager();
	if(FAILED(sound_manage->initialise()))
	{
		return FALSE;
	}
	texture_manage = new Texture_Manager();
	mesh_manage = new Mesh_Manager(texture_manage);

	if(!initialise_content())
	{
		return FALSE;
	}

	state_machine->change_state(MENU);

	return TRUE;
}

bool Game::initialise_content()
{
	frame_count = 0;
	//START Load script files
	file_manage.load_constructor_files();
	file_manage.load_variable_files();
	//END Load script files

	//START Texture Loading
	variable_map* texture_list = file_manage.get_file("texture_list.sdsc");
	if(!texture_list->empty())
	{
		variable_map::iterator texture_iterator;
		for(texture_iterator = texture_list->begin(); texture_iterator != texture_list->end(); texture_iterator++)
		{
			std::string file_name;
			file_name = boost::any_cast<std::string>(texture_iterator->second);
			if(FAILED(texture_manage->load(renderer->get_device(), file_name)));
			{
				//return FALSE;
			}
		}
	}
	//END Texture Loading

	//START Mesh Loading
	variable_map* mesh_list = file_manage.get_file("mesh_list.sdsc");
	if(!mesh_list->empty())
	{
		variable_map::iterator mesh_iterator;
		for(mesh_iterator = mesh_list->begin(); mesh_iterator != mesh_list->end(); mesh_iterator++)
		{
			std::string file_name;
			file_name = boost::any_cast<std::string>(mesh_iterator->second);
			if(!mesh_manage->load(renderer->get_device(), file_name));
			{
				//return FALSE;
			}
		}
	}
	//END Mesh Loading

	//START Sound Loading
	variable_map* sound_list = file_manage.get_file("sound_list.sdsc");
	if(!sound_list->empty())
	{
		variable_map::iterator sound_iterator;
		for(sound_iterator = sound_list->begin(); sound_iterator != sound_list->end(); sound_iterator++)
		{
			std::string file_name;
			file_name = boost::any_cast<std::string>(sound_iterator->second);
			if(!sound_manage->load(file_name))
			{
				//return FALSE;
			}
		}
	}
	//END Sound Loading

	//START Factory Creation
	test_factory = new Missile_Factory();
	//END Factory Creation

	//START Object Creation
	variable_map* object_list = file_manage.get_file("object_list.sdsc");
	if(!object_list->empty())
	{
		variable_map::iterator object_iterator;
		for(object_iterator = object_list->begin(); object_iterator != object_list->end(); object_iterator++)
		{
			std::string construct_data = boost::any_cast<std::string>(object_iterator->first) + ".sdsc";
			std::string variable_data = boost::any_cast<std::string>(object_iterator->first) + ".sdsv";
			variable_map* constructor_settings = file_manage.get_file(construct_data);
			variable_map* variable_settings = file_manage.get_file(variable_data);
			Mesh* mesh_data = mesh_manage->get_mesh(boost::any_cast<std::string>(constructor_settings->at("mesh")));
			//two get files here one sdsc and another sdsv based on the object_list name
			//load file here
			object_queue.push_back(new Static_Object(mesh_data, constructor_settings, variable_settings));
		}
	}

	variable_map* player_constructor = file_manage.get_file("player.sdsc");
	std::string player_mesh = boost::any_cast<std::string>(player_constructor->at("mesh"));
	std::string player_engine_sound = boost::any_cast<std::string>(player_constructor->at("engine_sound"));
	object_queue.push_back((new Ship_Player(mesh_manage->get_mesh(player_mesh),
							file_manage.get_file("player.sdsc"), file_manage.get_file("player.sdsv"),
							input_manage, sound_manage->get_sound(player_engine_sound), &object_queue, test_factory)));
	//END Object Creation

	test_factory->set_origin(object_queue[1]);
	test_factory->set_target(object_queue[0]);
	test_factory->add_variable_pair(file_manage.get_file("missile_normal.sdsc"), file_manage.get_file("missile_normal.sdsv"));
	test_factory->add_mesh(mesh_manage->get_mesh("mesh/EnemyShip-Red.x"));
	test_factory->add_location(D3DXVECTOR3(1.0f, 0, 1.5f));
	test_factory->add_location(D3DXVECTOR3(-1.0f, 0, 1.5f));

	//START Camera Creation
	camera = new Camera_Third(D3DXVECTOR3(0, 0, 10), object_queue[1], D3DXVECTOR3(0, 1, 0),
							  D3DX_PI / 2, 640 / (float)480, 0.1f, 200.0f);
	//END Camera Creation

	//START Particle Spawner Creation
	particle_queue.push_back(new Particle_Spawner);
	particle_queue[0]->set_position(D3DXVECTOR3(0.0f, 10.0f, 0));
	particle_queue[0]->set_particle_texture(texture_manage->get_texture("texture/pointparticle.bmp"));
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
		renderer->get_font(), "Start Game", 128, 64, D3DXVECTOR3(renderer->get_width() / 2, renderer->get_height() / 2, 0), &Game::cycle_state));

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
		D3DCOLOR_ARGB(255, 255, 255, 255)));
	// END Text box for mouse coordinates

	//TODO create function to erase all .sdsc files from the file manager they are only required for init
	//TODO object will need a clear_constructor function to set its .sdsc pointer to null and null checks on all calls to and sdc files

	return TRUE;
}

void Game::update(float timestamp)
{
	input_manage->begin_update();

	state_machine->update(timestamp);

	input_manage->end_update();
}

void Game::render()
{
	renderer->render(object_queue, particle_queue, button_queue, text_queue, camera);
}

void Game::menu_enter()
{
	button_queue[0]->set_visible();
}

void Game::menu_update(float timestep)
{
	//Action on Esc pressed
	if(input_manage->get_key_up(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	//Update buttons
	for(size_t i = 0; i < button_queue.size(); i++)
	{
		button_queue[i]->update(input_manage->get_mouse_x(), input_manage->get_mouse_y(),
								input_manage->get_mouse_up(LEFT_MOUSE));
	}
}

void Game::menu_render()
{

}

void Game::menu_exit()
{
	button_queue[0]->set_invisible();
}

void Game::game_enter()
{
	//Make mouse coord display visible
	text_queue[0]->set_visible();

	//Make objects visible
	for(size_t i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->set_visible();
	}

	//Make particle spawners visible
	for(size_t i = 0; i < particle_queue.size(); i++)
	{
		particle_queue[i]->set_visible();
	}
}

void Game::game_update(float timestep)
{
	//Action on Esc pressed
	if(input_manage->get_key_up(VK_ESCAPE))
	{
		state_machine->change_state(MENU);
	}

	//Update text[0]
	std::stringstream font_output;
	if(text_queue[0]->is_visible())
	{
		font_output << "X " << input_manage->get_mouse_x_centered() << "\n"
					<< "Y " << input_manage->get_mouse_y_centered() << "\n";
		text_queue[0]->update(font_output.str());
		font_output.str("");
	}

	//Update objects
	for(size_t i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestep);
	}

	//Update particle spawners
	for(size_t i = 0; i < particle_queue.size(); i++)
	{
		particle_queue[i]->update(timestep);
	}

	//Check for collision
	/*
	if(Collision::check_collision(object_queue[0]->get_hit_box(), object_queue[3]->get_hit_box()))
	{
		std::stringstream trace_output;
		trace_output << "object hit at frame " << frame_count << "\n";
		trace(trace_output.str().c_str());
	}
	*/

	frame_count++;
}

void Game::game_render()
{

}

void Game::game_exit()
{
	//Make objects invisible
	for(size_t i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->set_invisible();
	}

	//Make particle spawners invisible
	for(size_t i = 0; i < particle_queue.size(); i++)
	{
		particle_queue[i]->set_invisible();
	}
}

void Game::gameover_enter()
{

}

void Game::gameover_update(float timestep)
{

}

void Game::gameover_render()
{

}

void Game::gameover_exit()
{

}

void Game::cycle_state(int value)
{
	trace("State Change");
	state_machine->change_state(GAME);
}

void Game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}