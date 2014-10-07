#include "Ship_Enemy.h"

Ship_Enemy::Ship_Enemy(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation, float scale, float speed)
		 : Object(model, position, rotation, scale)
{
	spin_speed = speed;
}

void Ship_Enemy::update(float timesetp)
{
	D3DXQUATERNION rotation_test;
	D3DXQuaternionRotationYawPitchRoll(&rotation_test, timesetp * spin_speed, 0.0f, 0.0f);
	D3DXQuaternionNormalize(&rotation_test, &rotation_test);
	rotation *= rotation_test;
}