#ifndef SHIP_PLAYER_H
#define SHIP_PLAYER_H
#include "../engine/Object.h"
#include "../engine/Input_Manager.h"
#include "../engine/Sound.h"
#include "../engine/Collision_Sphere.h"
#include "../Missile_factory.h"

class Ship_Player : public Object
{
private:
	Input_Manager* input_manage;
	Sound* engine_sound;
	std::vector<Object*>* missile_queue;
	Missile_Factory* missile_spawner;
	float rotation_speed;
	float translation_speed;

public:
	Ship_Player(Mesh* model, variable_map* constructor_settings, variable_map* variable_settings,
				Input_Manager* input_manage, Sound* engine_sound, 
				std::vector<Object*>* missile_queue, Missile_Factory* missile_spawner);

	void update(float timestep);
	void reload_variables();
};

#endif