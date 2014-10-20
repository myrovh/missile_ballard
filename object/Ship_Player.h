#ifndef SHIP_PLAYER_H
#define SHIP_PLAYER_H
#include "../engine/Object.h"
#include "../engine/Input_Manager.h"
#include "../engine/Sound.h"
#include "../engine/Collision_Sphere.h"
#include "../Missile_factory.h"

#define ROTATION_SPEED  3.0f
#define TRANSLATE_SPEED  20.0f
#define DEAD_ZONE 50

class Ship_Player : public Object
{
private:
	Input_Manager* input_manage;
	Sound* engine_sound;
	std::vector<Object*>* missile_queue;
	Missile_Factory* missile_spawner;

public:
	Ship_Player(Mesh* model, D3DXVECTOR3 position, float scale, 
				Input_Manager* input_manage, Sound* engine_sound, 
				std::vector<Object*>* missile_queue, Missile_Factory* missile_spawner);

	void update(float timestep);
};

#endif