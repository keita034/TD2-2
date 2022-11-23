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
	///初期化
	///</summary>

	void Initialize(objModel* model1, objModel* model2, objModel* model3, TextureData textureHandle);



	///<summary>
	///更新
	///</summary>

	void Update();
	void Move();
	void Afin(Transform& worldTransform_);

	 AliceMathF::Vector3 ConvertToVector3(Transform& worldTransform_, AliceMathF::Vector3 vec);


	///<summary>
	///描画
	///</summary>

	void Draw(Camera* viewProjection_);

	///<summary>
	///攻撃
	///</summary>

	void Attack();

	void CirclePosition(AliceMathF::Vector3 worldTransform, int angle, int length);

	void WeaponCirclePosition(AliceMathF::Vector3 pos, int angle, int length);

	void AttackWeaponCircle(AliceMathF::Vector3 pos, int angle, float angle2, int length);

	AliceMathF::Vector3 GetWorldTranslation() { return worldTransform.translation; }

	float leap(const int start, const int end, const float t);

	int GetMoveDirection() { return moveDirection; }

public:
	//ワールド変換データ
	Transform worldTransform;

	//ワールド変換データ
	Transform weaponWorldTransform;

	Transform fakeWeaponWorldTransform;
	//モデル
	objModel* model = nullptr;

	//モデル
	objModel* weaponModel = nullptr;
	objModel* bulletModel = nullptr;
	//テクスチャハンドル
	TextureData textureHandle;

	Input* input = nullptr;

	//ビュープロジェクション
	Camera* viewProjection;

	float PI = 3.141592;

	int rot = 0;
	int wRot = 15;
	int attackWeapon = 0;
	int attackFlag = 0;

	int moveDirection = 0;//0が右1が左

	float gravity;

	int jumpFlag;
	int junpFrame;

	int junpAttackFlag = 0;

	float playerJumpSpeed;

	//時間計測に必要なデータ
	float startCount = 0;
	float nowCount = 0;
	float elapsedCount = 0;

	float maxTime = 5.0f;

	int playerCharge = 0;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	uint32_t ULTHandle = 0;


};