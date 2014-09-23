#ifndef SHIP_ENEMY_H
#define SHIP_ENEMY_H
#include "../engine/Object.h"

class Ship_Enemy :
	public Object
{
private:
	float spin_speed;
	D3DXVECTOR3 throw_direction;
	D3DXVECTOR3 starting_location;
	D3DXVECTOR3 gravity;
	float gravity_force;
	float throw_force;
	float throw_entropy;

public:
	Ship_Enemy(Mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		float z_rotation, float scale, float speed);

	void update(float timestep);
};

#endif