#include "Missile.h"

Missile::Missile(Mesh* model, D3DXVECTOR3 position, float scale, float speed, Object*	target)
		 : Object(model, position, scale)
{
	this->move_speed = speed;
	this->target = target;
	this->hit_box = new Collision_Sphere(D3DXVECTOR3(0, 0, 0), 3.0f);
}

Missile::Missile(Mesh* model, D3DXVECTOR3 position, float scale)
		 : Object(model, position, scale)
{
	this->target = NULL;
	move_speed = 0;
}

void Missile::update(float timestep)
{
	if(target)
	{
		D3DXVECTOR3 target_location = target->get_object_location();
		/*
		D3DXQUATERNION target_rotation = target->get_object_rotation();

		D3DXQUATERNION new_rotation;

		D3DXQuaternionRotationAxis(&new_rotation, &target_location, axis_rotation);

		rotation = new_rotation;

		D3DXMATRIX scale;
		D3DXMATRIX rotation_matrix;
		D3DXMATRIX translation;


		//Calculate direction towards target
		D3DXVECTOR3 forward = {1.0f, 1.0f, 1.0f};
		D3DXMATRIX transform_forward;
		D3DXMatrixRotationQuaternion(&transform_forward, &target_rotation);
		D3DXVec3TransformCoord(&forward, &forward, &transform_forward);
		forward = -forward;
		*/
		D3DXVECTOR3 direction = target_location - vector_position;
		D3DXVECTOR3 movement;
		D3DXVec3Normalize(&movement, &movement);
		vector_position -= movement * timestep;

		//vector_position += target_increment * timestep;
	}

	//Update location of hit_box
	if(hit_box != NULL)
	{
		hit_box->update_position(this->get_object_location());
	}
}