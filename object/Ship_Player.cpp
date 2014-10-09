#include "Ship_Player.h"

Ship_Player::Ship_Player(Mesh* model, D3DXVECTOR3 position, float scale, Input_Manager* input_manage, Sound* engine_sound)
		 : Object(model, position, scale)
{
	this->input_manage = input_manage;
	this->engine_sound = engine_sound;
	axis_rotation = cos(0/2);
}

void Ship_Player::update(float timestep)
{
	/*
	//Left for Right response
	if(input_manage->get_key_down(VK_LEFT))
	{
		rotation.y -= ROTATION_SPEED * timestep;
	}
	if(input_manage->get_key_down(VK_RIGHT))
	{
		rotation.y += ROTATION_SPEED * timestep;
	}
	*/

	//Create a new rotation based on mouse location
	D3DXVECTOR3 mouse_vector;
	mouse_vector.x = input_manage->get_mouse_x_centered();
	mouse_vector.y = input_manage->get_mouse_y_centered();
	mouse_vector.z = 1.0f;
	mouse_vector = -mouse_vector;

	if(mouse_vector.x > DEAD_ZONE || mouse_vector.x < -DEAD_ZONE)
	{
		if(mouse_vector.y > DEAD_ZONE || mouse_vector.y < -DEAD_ZONE)
		{
			D3DXQUATERNION mouse_point;
			D3DXQUATERNION new_rotation;
			D3DXQuaternionRotationAxis(&mouse_point, &mouse_vector, axis_rotation);
			D3DXQuaternionNormalize(&mouse_point, &mouse_point);
			D3DXQuaternionNormalize(&rotation, &rotation);
			mouse_point = mouse_point * rotation;
			D3DXQuaternionSlerp(&new_rotation, &rotation, &mouse_point, 0.05f);

			rotation = new_rotation;
		}
	}

	//Calculate which way is forward
	D3DXVECTOR3 forward = {0, 0, 1.0f};
	D3DXMATRIX transform_forward;
	D3DXMatrixRotationQuaternion(&transform_forward, &rotation);
	D3DXVec3TransformCoord(&forward, &forward, &transform_forward);
	forward = -forward;


	//Stop sound if buttons are not being pressed
	if(engine_sound)
	{
		engine_sound->stop();
	}

	D3DXVECTOR3 temp_vector = vector_position;
	//Add or subtract our new "forward" direction to our position
	if(input_manage->get_key_down('W'))
	{
		temp_vector -= forward * TRANSLATE_SPEED * timestep;
		if(engine_sound)
		{
			engine_sound->play();
		}
	}
	if(input_manage->get_key_down('S'))
	{
		temp_vector += forward * TRANSLATE_SPEED * timestep;
		if(engine_sound)
		{
			engine_sound->play();
		}
	}
	//Rotate around the forward axis
	if(input_manage->get_key_down('E'))
	{
		//none
		if(engine_sound)
		{
			engine_sound->play();
		}
	}
	if(input_manage->get_key_down('Q'))
	{
		//none
		if(engine_sound)
		{
			engine_sound->play();
		}
	}

	vector_position = temp_vector;
}
