#pragma once
#include "Transform.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "DebugText.h"

#include "Input.h"
#include "objModel.h"
#include"Camera.h"


class Enemy
{
public:

	Enemy();
	~Enemy();
	void Initialize(objModel* model, objModel* model2);
	void CalcVec(AliceMathF::Vector3 view);
	void entryMovePatt();
	void battleMovePatt();
	void Update(AliceMathF::Vector3 obj, Camera* viewProjection_);
	void Draw(Camera* viewProjection_);
	void Hit();
	void OnColision();

public:
	objModel* model_[2];
	std::array< std::unique_ptr<objModel>,10> models;
	Input* input_ = nullptr;

	Transform worldTransForm[10];
	Transform ball[2];
	int isDead = false;
	AliceMathF::Vector3 enemyTmp;
	AliceMathF::Vector3 YTmp;
	//右ベクトル
	AliceMathF::Vector3 enemyRight;
	//左ベクトル
	AliceMathF::Vector3 enemyLeft;
	//正面ベクトル
	AliceMathF::Vector3 enemyFront;
	//加速
	float speed;

	float rotate;

	int scene=0;

	int movePattern = 0;
	int motionPattern = 0;
	int state = 0;

	int hp=100;
	bool isHit;

	int attacktime = 0;
	int time = 0;
	int waitTime=0;

	int Count = 0;

	float acceleration = 0.0f;

	int r = 2;


	size_t splineCount_ = 0;

};

