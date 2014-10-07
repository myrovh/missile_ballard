#include "Ship_Player.h"

Ship_Player::Ship_Player(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation, float scale, Input_Manager* input_manage, Sound* engine_sound)
		 : Object(model, position, rotation, scale)
{
	this->input_manage = input_manage;
	this->engine_sound = engine_sound;
	axis_rotation = 0.01f;
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
	mouse_vector.z = 40.0f;

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
			D3DXQuaternionSlerp(&new_rotation, &rotation, &mouse_point, 1.0f);
			rotation = new_rotation;
		}
	}

	D3DXVECTOR3 temp_vector = vector_position;
	D3DXVECTOR3 forward;
	D3DXMATRIX rotation_matrix;

	D3DXMatrixRotationQuaternion(&rotation_matrix, &rotation);
	D3DXVec3TransformCoord(&forward, &forward, &rotation_matrix);

	//D3DXQuaternionToAxisAngle(&rotation, &forward, nullptr);
	D3DXVec3Normalize(&forward, &forward);
	//Use the y rotation value to modify the concept of "forward"
	//D3DXMATRIX rotation_y;
	//D3DXMatrixRotationY(&rotation_y, rotation.y);
	//D3DXVec3TransformCoord(&forward, &forward, &rotation_y);
	//D3DXMATRIX rotation_matrix;
	//D3DXMatrixRotationQuaternion(&rotation_matrix, &new_rotation);
	//D3DXVec3TransformCoord(&forward, &forward, &rotation_matrix);


	if(engine_sound)
	{
		engine_sound->stop();
	}

	//Add or subtract our new "forward" direction to our position
	if(input_manage->get_key_down(VK_UP))
	{
		temp_vector += forward * TRANSLATE_SPEED * timestep;
		if(engine_sound)
		{
			engine_sound->play();
		}
	}
	if(input_manage->get_key_down(VK_DOWN))
	{
		temp_vector -= forward * TRANSLATE_SPEED * timestep;
		if(engine_sound)
		{
			engine_sound->play();
		}
	}

	vector_position = temp_vector;
}
