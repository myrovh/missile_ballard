#include "Ship_Player.h"

Ship_Player::Ship_Player(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation, float scale, Input_Manager* input_manage)
		 : Object(model, position, rotation, scale)
{
	this->input_manage = input_manage;
}

void Ship_Player::update(float timesetp)
{
	//Left for Right response
	if(input_manage->get_key_down(VK_LEFT))
	{
		rotation.y -= ROTATION_SPEED * timesetp;
	}
	if(input_manage->get_key_down(VK_RIGHT))
	{
		rotation.y += ROTATION_SPEED * timesetp;
	}

	//Use the y rotation value to modify the concept of "forward"
	D3DXVECTOR3 temp_vector = vector_position;
	D3DXVECTOR3 forward(0, 0, 1);
	D3DXMATRIX rotation_y;
	D3DXMatrixRotationY(&rotation_y, rotation.y);
	D3DXVec3TransformCoord(&forward, &forward, &rotation_y);
	float temp = timesetp;

	//Add or subtract our new "forward" direction to our position
	if(input_manage->get_key_down(VK_UP))
	{
		temp_vector += forward * TRANSLATE_SPEED * temp;
	}
	if(input_manage->get_key_down(VK_DOWN))
	{
		temp_vector -= forward * TRANSLATE_SPEED * temp;
	}

	vector_position = temp_vector;
}
