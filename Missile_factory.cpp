#include "Missile_factory.h"

Missile_Factory::Missile_Factory()
{
	this->target = NULL;
	this->origin = NULL;
	this->spawn_limit = 100;
	this->current_spawn_location = 0;
}

void Missile_Factory::set_origin(Object* origin)
{
	this->origin = origin;
}

void Missile_Factory::set_target(Object* target)
{
	this->target = target;
}

void Missile_Factory::add_mesh(Mesh* mesh)
{
	meshes.push_back(mesh);
}

void Missile_Factory::add_location(D3DXVECTOR3 location)
{
	locations.push_back(location);
}

Missile* Missile_Factory::spawn()
{
	Missile* temp = NULL;
	if(target != NULL && origin != NULL)
	{
		//select next location
		current_spawn_location++;
		if(current_spawn_location >= locations.size())
		{
			current_spawn_location = 0;
		}
		//add location to origin location to get spawn point
		D3DXVECTOR3 spawn_point = origin->get_object_location() + locations[current_spawn_location];
		//select random mesh
		current_spawn_mesh++;
		if(current_spawn_mesh >= meshes.size())
		{
			current_spawn_mesh = 0;
		}
		temp = new Missile(meshes[current_spawn_mesh], spawn_point, 1.0f, 0.2f, target);
	}
	return temp;
}