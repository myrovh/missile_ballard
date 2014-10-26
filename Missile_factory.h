#ifndef MISSILE_FACTORY_H
#define MISSILE_FACTORY_H

#include "object/Missile.h"
#include "engine/Mesh.h"
#include <vector>

class Missile_Factory
{
private:
	std::vector<Mesh*> meshes;
	std::vector<D3DXVECTOR3> locations;
	std::vector<variable_map*> constructor_setting_queue;
	std::vector<variable_map*> variable_setting_queue;
	UINT current_count;
	UINT current_spawn_location; //change to iterator
	UINT current_spawn_mesh; //change to iterator
	UINT spawn_limit;
	Object* origin;
	Object* target;
	void increase_enemy_count() {
		current_count++;
	}

public:
	Missile_Factory();
	~Missile_Factory() {}
	void set_origin(Object* origin);
	void set_target(Object* target);
	void set_spawn_limit(UINT spawn_limit) {
		this->spawn_limit = spawn_limit;
	}
	void add_mesh(Mesh* mesh);
	void add_variable_pair(variable_map* constructor_settings, variable_map* variable_settings);
	void add_location(D3DXVECTOR3 location);
	void reduce_enemy_cout() {
		current_count--;
	}
	Missile* spawn();
};

#endif