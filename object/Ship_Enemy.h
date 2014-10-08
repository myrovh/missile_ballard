#ifndef SHIP_ENEMY_H
#define SHIP_ENEMY_H
#include "../engine/Object.h"

class Ship_Enemy : public Object
{
private:
	float spin_speed;

public:
	Ship_Enemy(Mesh* model, D3DXVECTOR3 position, float scale, float speed);

	void update(float timestep);
};

#endif