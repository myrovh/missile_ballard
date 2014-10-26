#ifndef SHIP_ENEMY_H
#define SHIP_ENEMY_H
#include "../engine/Object.h"
#include "../engine/Collision_Sphere.h"

class Missile : public Object
{
private:
	float translation_speed;
	Object* target;

public:
	Missile(Mesh* model, variable_map* constructor_settings, variable_map* variable_settings, D3DXVECTOR3 spawn_location, Object* target);

	void update(float timestep);
	void reload_variables();
};

#endif