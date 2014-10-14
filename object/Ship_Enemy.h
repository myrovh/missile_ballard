#ifndef SHIP_ENEMY_H
#define SHIP_ENEMY_H
#include "../engine/Object.h"
#include "../engine/Collision_Sphere.h"

class Ship_Enemy : public Object
{
private:
	float move_speed;
	Object* target;

public:
	Ship_Enemy(Mesh* model, D3DXVECTOR3 position, float scale, float speed, Object* target);
	Ship_Enemy(Mesh* model, D3DXVECTOR3 position, float scale);

	void update(float timestep);
};

#endif