#include "PlayerBullet.h"

void PlayerBullet::Initialize(objModel* model, const AliceMathF::Vector3& position, int move, int rot)
{
	assert(model);

	this->model = model;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform.translation = position;
	moveDirection = move;
	this->rot = rot;
	rotationRot = -rot * (PI / 180);

}

void PlayerBullet::Update(int length)
{

	if (moveDirection == 1) {
		rot -= 3;
		rotationRot += RotSpeed;
	}
	else if (moveDirection == 0) {
		rot += 3;
		rotationRot -= RotSpeed;
	}
	worldTransform.rotation.y = rotationRot;

	CirclePosition(rot, 20);
	//���Ԍo�߂Ńf�X
	if (--deathTimer <= 0) {
		isDead = true;
	}

}

void PlayerBullet::Draw(Camera* viewProjection_)
{
	worldTransform.TransUpdate2(viewProjection_);
	model->Draw(&worldTransform);
}

void PlayerBullet::CirclePosition(int angle, int length)
{
	float  radius = angle * (PI / 180);

	worldTransform.translation = { cosf(radius) * length,worldTransform.translation.y, sinf(radius) * length };
}

void PlayerBullet::Afin(Transform& worldTransform_)
{
	AliceMathF::Matrix4 matScale;
	AliceMathF::Matrix4 matRot;
	AliceMathF::Matrix4 matRotX;
	AliceMathF::Matrix4 matRotY;
	AliceMathF::Matrix4 matRotZ;
	AliceMathF::Matrix4 matTrans;
	matScale = {
	worldTransform_.scale.x,0,0,0,
	0,worldTransform_.scale.y,0,0,
	0,0,worldTransform_.scale.z,0,
	0,0,0,1
	};
	matTrans = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	worldTransform_.translation.x,worldTransform_.translation.y,worldTransform_.translation.z,1
	};
	//Z�p��]���̐ݒ�
	matRotZ = {
	cos(worldTransform_.rotation.z),sin(worldTransform_.rotation.z),0,0,
	-sin(worldTransform_.rotation.z),cos(worldTransform_.rotation.z),0,0,
	0,0,1,0,
	0,0,0,1
	};

	//X�p��]���̐ݒ�
	matRotX = {
	1,0,0,0,
	0,cos(worldTransform_.rotation.x),sin(worldTransform_.rotation.x),0,
	0,-sin(worldTransform_.rotation.x),cos(worldTransform_.rotation.x),0,
	0,0,0,1
	};

	//Y�p��]���̐ݒ�
	matRotY = {
	cos(worldTransform_.rotation.y),0,-sin(worldTransform_.rotation.y),0,
	0,1,0,0,
	sin(worldTransform_.rotation.y),0,cos(worldTransform_.rotation.y),0,
	0,0,0,1
	};

	matRot = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
	};
	//�e���̉�]�s�������
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	worldTransform_.matWorld = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	worldTransform_.matWorld *= matScale;
	worldTransform_.matWorld *= matRot;
	worldTransform_.matWorld *= matTrans;
}
