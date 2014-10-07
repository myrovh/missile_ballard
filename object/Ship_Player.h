#ifndef SHIP_PLAYER_H
#define SHIP_PLAYER_H
#include "../engine/Object.h"
#include "../engine/Input_Manager.h"
#include "../engine/Sound.h"

#define ROTATION_SPEED  3.0f
#define TRANSLATE_SPEED  20.0f
#define DEAD_ZONE 50

class Ship_Player : public Object
{
private:
	Input_Manager* input_manage;
	Sound* engine_sound;

public:
	Ship_Player(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation,
		float scale, Input_Manager* input_manage, Sound* engine_sound);

	void update(float timestep);
};

#endif