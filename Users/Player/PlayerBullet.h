#pragma once
#pragma once
#include "Transform.h"
#include "objModel.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "Camera.h"

///<summary>
///���L����
///</summary>
class PlayerBullet {
public:
	///<summary>
	///������
	///</summary>

	void Initialize(objModel* model, const AliceMathF::Vector3& position,int move,int rot);;



	///<summary>
	///�X�V
	///</summary>

	void Update(int length);

	///<summary>
	///�`��
	///</summary>

	void Draw(Camera* viewProjection_);

	bool IsDead()const { return isDead; }

	void CirclePosition( int angle, int length);
	void Afin(Transform& worldTransform_);
	public:

	//���[���h�ϊ��f�[�^
	Transform worldTransform;
	//���f��
	objModel* model = nullptr;
	//�e�N�X�`���n���h��
	TextureData textureHandle;

	//���x
	AliceMathF::Vector3 velocity;

	//����<fim>
	static const int32_t kLifeTime = 60 * 1;

	//�f�X�^�C�}�[
	int32_t deathTimer = kLifeTime;

	//�f�X�t���O
	bool isDead = false;

	int rot = 0;
	float PI = 3.141592;
	int moveDirection;
	const float RotSpeed = 3 * (PI / 180);
	float rotationRot = -rot * (PI / 180);
};