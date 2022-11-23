#pragma once
#include "Transform.h"
#include "objModel.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include <memory>
#include <list>
#include "PlayerBullet.h"

class Player {
public:

	Player();
	~Player();

	///<summary>
	///������
	///</summary>

	void Initialize(objModel* model1, objModel* model2, objModel* model3, TextureData textureHandle);



	///<summary>
	///�X�V
	///</summary>

	void Update();
	void Move();
	void Afin(Transform& worldTransform_);

	 AliceMathF::Vector3 ConvertToVector3(Transform& worldTransform_, AliceMathF::Vector3 vec);


	///<summary>
	///�`��
	///</summary>

	void Draw(Camera* viewProjection_);

	///<summary>
	///�U��
	///</summary>

	void Attack();

	void CirclePosition(AliceMathF::Vector3 worldTransform, int angle, int length);

	void WeaponCirclePosition(AliceMathF::Vector3 pos, int angle, int length);

	void AttackWeaponCircle(AliceMathF::Vector3 pos, int angle, float angle2, int length);

	AliceMathF::Vector3 GetWorldTranslation() { return worldTransform.translation; }

	float leap(const int start, const int end, const float t);

	int GetMoveDirection() { return moveDirection; }

public:
	//���[���h�ϊ��f�[�^
	Transform worldTransform;

	//���[���h�ϊ��f�[�^
	Transform weaponWorldTransform;

	Transform fakeWeaponWorldTransform;
	//���f��
	objModel* model = nullptr;

	//���f��
	objModel* weaponModel = nullptr;
	objModel* bulletModel = nullptr;
	//�e�N�X�`���n���h��
	TextureData textureHandle;

	Input* input = nullptr;

	//�r���[�v���W�F�N�V����
	Camera* viewProjection;

	float PI = 3.141592;

	int rot = 0;
	int wRot = 15;
	int attackWeapon = 0;
	int attackFlag = 0;

	int moveDirection = 0;//0���E1����

	float gravity;

	int jumpFlag;
	int junpFrame;

	int junpAttackFlag = 0;

	float playerJumpSpeed;

	//���Ԍv���ɕK�v�ȃf�[�^
	float startCount = 0;
	float nowCount = 0;
	float elapsedCount = 0;

	float maxTime = 5.0f;

	int playerCharge = 0;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	uint32_t ULTHandle = 0;


};