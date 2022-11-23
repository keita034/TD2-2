#pragma once
#pragma once
#include "Transform.h"
#include "objModel.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "Camera.h"

///<summary>
///自キャラ
///</summary>
class PlayerBullet {
public:
	///<summary>
	///初期化
	///</summary>

	void Initialize(objModel* model, const AliceMathF::Vector3& position,int move,int rot);;



	///<summary>
	///更新
	///</summary>

	void Update(int length);

	///<summary>
	///描画
	///</summary>

	void Draw(Camera* viewProjection_);

	bool IsDead()const { return isDead; }

	void CirclePosition( int angle, int length);
	void Afin(Transform& worldTransform_);
	public:

	//ワールド変換データ
	Transform worldTransform;
	//モデル
	objModel* model = nullptr;
	//テクスチャハンドル
	TextureData textureHandle;

	//速度
	AliceMathF::Vector3 velocity;

	//寿命<fim>
	static const int32_t kLifeTime = 60 * 1;

	//デスタイマー
	int32_t deathTimer = kLifeTime;

	//デスフラグ
	bool isDead = false;

	int rot = 0;
	float PI = 3.141592;
	int moveDirection;
	const float RotSpeed = 3 * (PI / 180);
	float rotationRot = -rot * (PI / 180);
};