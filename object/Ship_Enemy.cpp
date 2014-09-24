#include "Ship_Enemy.h"

Ship_Enemy::Ship_Enemy(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation, float scale, float speed)
		 : Object(model, position, rotation, scale)
{
	spin_speed = speed;
}

void Ship_Enemy::update(float timesetp)
{
	rotation.y += spin_speed * timesetp;
}