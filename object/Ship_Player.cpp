#include "Ship_Player.h"

Ship_Player::Ship_Player(Mesh* model, D3DXVECTOR3 position, float scale, Input_Manager* input_manage, Sound* engine_sound, std::vector<Object*>* missile_queue, Missile_Factory* missile_spawner)
		 : Object(model, position, scale)
{
	this->input_manage = input_manage;
	this->engine_sound = engine_sound;
	this->missile_spawner = missile_spawner;
	axis_rotation = cos(0 / 2);
	this->hit_box = new Collision_Sphere(D3DXVECTOR3(0, 0, 0), 3.0f);
	this->missile_queue = missile_queue;
}

void Ship_Player::update(float timestep)
{
	//Create a vector based on mouse location
	int mouse_x_location = input_manage->get_mouse_x_centered();
	int mouse_y_location = input_manage->get_mouse_y_centered();

	//Calculate which way is forward
	D3DXVECTOR3 forward = {0, 0, 1.0f};
	D3DXMATRIX transform_forward;
	D3DXMatrixRotationQuaternion(&transform_forward, &rotation);
	D3DXVec3TransformCoord(&forward, &forward, &transform_forward);
	forward = -forward;

	//Rotate ship on x and y axis based on mouse location
	float x_rotation_angle = mouse_x_location * timestep;
	float y_rotation_angle = mouse_y_location * timestep;
	D3DXMATRIX x_rotation;
	D3DXMATRIX y_rotation;
	D3DXQUATERNION new_rotation;

	D3DXMatrixRotationY(&x_rotation, x_rotation_angle);
	D3DXMatrixRotationX(&y_rotation, y_rotation_angle);
	D3DXMATRIX matrix_rotation = x_rotation * y_rotation;
	//D3DXQuaternionRotationAxis(&new_rotation, &new_forward, 0);
	D3DXQuaternionRotationMatrix(&new_rotation, &matrix_rotation);
	rotation = new_rotation;

	//Stop sound if buttons are not being pressed
	if(engine_sound)
	{
		engine_sound->stop();
	}

	//Add or subtract new "forward" direction to our position
	D3DXVECTOR3 temp_vector = vector_position;
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
	vector_position = temp_vector;
	if(input_manage->get_key_up(VK_SPACE))
	{
		missile_queue->push_back(missile_spawner->spawn());
		missile_queue->back()->set_visible();
	}

	//Update location of hit_box
	hit_box->update_position(this->get_object_location());
}
