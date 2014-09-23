#include "Ship_Enemy.h"

Ship_Enemy::Ship_Enemy(Mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		 float z_rotation, float scale, float speed)
		 : Object(model, position, x_rotation, y_rotation, z_rotation, scale)
{
	spin_speed = speed;
	starting_location = position;
	gravity.x = 0;
	gravity.y = -1;
	gravity.z = 0;
	gravity_force = 0;
	throw_force = 0;
}

void Ship_Enemy::update(float timesetp)
{
	y_rotation += spin_speed * timesetp;
}