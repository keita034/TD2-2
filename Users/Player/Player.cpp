#include "Player.h"
#include"AudioManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(objModel* model1, objModel* model2,objModel* model3 , TextureData textureHandle)
{
	assert(model1);
	assert(model2);
	//引数として受け取ったデータをメンバ変数に記録する
	model = model1;
	weaponModel = model2;
	bulletModel = model3;
	this->textureHandle = textureHandle;

	jumpFlag = 0;
	gravity = 9.0f;
	junpFrame = 0;

	//シングルトンインスタンスを取得する
	input = Input::GetInstance();

	//ワールド変換の初期化
	worldTransform.Initialize();
	fakeWeaponWorldTransform.Initialize();
	weaponWorldTransform.translation = { 0,3,0 };
	weaponWorldTransform.parent = &fakeWeaponWorldTransform;
	weaponWorldTransform.Initialize();


	rot = 0;
	wRot = 15;
	attackWeapon = 0;
	attackFlag = 0;

	moveDirection = 0;//0が右1が左	
	junpAttackFlag = 0;

	playerJumpSpeed = 0;
	junpFrame = 0;
	worldTransform.translation.y = 0;
	jumpFlag = 0;

	//時間計測に必要なデータ
	startCount = 0;
	nowCount = 0;
	elapsedCount = 0;

	maxTime = 5.0f;

	playerCharge = 0;

	ULTHandle = AudioManager::GetInstance()->LoadAudio("Resources/ULT.mp3");
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->IsDead();});

	

	//キャラクター移動処理
	Move();
	//キャラクター攻撃更新
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(20);
	}

	if (attackFlag == 1) {
		if (moveDirection == 0) {
			fakeWeaponWorldTransform.rotation.x += 15 * (PI / 180);
		}
		if (moveDirection == 1) {
			fakeWeaponWorldTransform.rotation.x -= 15 * (PI / 180);
		}
		if (fakeWeaponWorldTransform.rotation.x > 180 * (PI / 180) || fakeWeaponWorldTransform.rotation.x < -180 * (PI / 180)) {
			attackFlag = 0;
			fakeWeaponWorldTransform.rotation.x = 0;
		}
	}
	if (junpAttackFlag == 1) {
		if (moveDirection == 0) {
			fakeWeaponWorldTransform.rotation.x += 30 * (PI / 180);
		}
		if (moveDirection == 1) {
			fakeWeaponWorldTransform.rotation.x -= 30 * (PI / 180);
		}
		if (fakeWeaponWorldTransform.rotation.x > 360 * (PI / 180) || fakeWeaponWorldTransform.rotation.x < -360 * (PI / 180)) {
			junpAttackFlag = 0;
			fakeWeaponWorldTransform.rotation.x = 0;
		}
	}
	if (junpAttackFlag == 0 && attackFlag == 0) {
		fakeWeaponWorldTransform.rotation.x = 0;
	}

}

void Player::Move()
{
	//キャラクターの移動ベクトル
	AliceMathF::Vector3 move = { 0,0,0 };

	const float speed = 1.00f;
	const float RotSpeed = 1 * (PI / 180);

	if (input->KeepPush(DIK_A)) {
		rot -= speed;
		wRot -= speed;
		worldTransform.rotation.y += RotSpeed;

		moveDirection = 1;
	}
	else if (input->KeepPush(DIK_D)) {
		rot += speed;
		wRot += speed;
		worldTransform.rotation.y -= RotSpeed;

		moveDirection = 0;
	}


	//W押されたらジャンプ
	if (input->TriggerPush(DIK_W)) {
		jumpFlag = 1;
	}
	if (jumpFlag == 1) {
		//ジャンプフレーム
		junpFrame++;
		playerJumpSpeed = 1.3f - gravity * (static_cast<float>(junpFrame) / 100.0f);
		worldTransform.translation.y += playerJumpSpeed;
	}

	//0から下にいかないように
	if (worldTransform.translation.y < 0) {
		playerJumpSpeed = 0;
		junpFrame = 0;
		worldTransform.translation.y = 0;
		jumpFlag = 0;
		/*junpAttackFlag = 0;*/
	}

	if (input->KeepPush(DIK_U)) {
		worldTransform.rotation.y -= RotSpeed;
	}
	else if (input->KeepPush(DIK_I)) {
		worldTransform.rotation.y += RotSpeed;
	}

	CirclePosition(worldTransform.translation, rot, 20);
	fakeWeaponWorldTransform.translation = { worldTransform.translation.x,worldTransform.translation.y,worldTransform.translation.z };
	weaponWorldTransform.scale = { 1,1,1 };
	fakeWeaponWorldTransform.rotation.y = worldTransform.rotation.y;
	weaponWorldTransform.rotation.x = 90 * (PI / 180);

	worldTransform.rotation.x = 90 * (PI / 180);

	//キーボード入力による移動処理

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	if (moveDirection == 1) {
		weaponWorldTransform.rotation.z = 180 * (PI / 180);
		worldTransform.rotation.z = 180 * (PI / 180);
	}
	else
	{
		weaponWorldTransform.rotation.z = 0 * (PI / 180);
		worldTransform.rotation.z = 0 * (PI / 180);
	}

}



void Player::Draw(Camera* viewProjection_)
{

	worldTransform.TransUpdate2(viewProjection_);

	fakeWeaponWorldTransform.TransUpdate2(viewProjection_);

	weaponWorldTransform.TransUpdate2(viewProjection_);

	model->Draw(&worldTransform);

	if (attackFlag == 1 || junpAttackFlag == 1) {
		weaponModel->Draw(&weaponWorldTransform);
	}
	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}
void Player::Attack()
{
	if (input->KeepPush(DIK_F))
	{
		
		if (playerCharge >= 100) {
			AudioManager::GetInstance()->PlayWave(ULTHandle);
			//弾の速度
			const float kBulletSpeed = 1.0f;
			AliceMathF::Vector3 velocity(0, 0, kBulletSpeed);

			//弾を生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel, worldTransform.translation, moveDirection, rot);

			//弾を発射する
			bullets_.push_back(std::move(newBullet));

			playerCharge = 0;
		}
	}

	if (input->TriggerPush(DIK_SPACE)) {
		if (attackFlag == 0 && junpAttackFlag == 0) {
			if (jumpFlag == 0) {
				attackFlag = 1;
			}
			if (jumpFlag == 1) {
				junpAttackFlag = 1;
			}
			playerCharge += 20;
		}
	}
}
void Player::CirclePosition(AliceMathF::Vector3 pos, int angle, int length)
{
	float  radius = angle * (PI / 180);

	pos = { cosf(radius) * length,pos.y, sinf(radius) * length };

	worldTransform.translation = pos;

}
void Player::WeaponCirclePosition(AliceMathF::Vector3 pos, int angle, int length)
{
	float  radius = angle * (PI / 180);

	pos = { cosf(radius) * length ,pos.y, (sinf(radius) * length) };

	weaponWorldTransform.translation = pos;
}
void Player::AttackWeaponCircle(AliceMathF::Vector3 pos, int angle, float angle2, int length)
{
	float  radius = angle * (PI / 180);

	float  radius2 = angle2 * (PI / 180);

	pos = { cosf(radius) * length,cosf(radius2) * 3 , sinf(radius) * length };

	weaponWorldTransform.translation = pos;

}
float Player::leap(const int start, const int end, const float t)
{
	return start * (1.0f - t) + end * t;
}
void Player::Afin(Transform& worldTransform_)
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
	//Z用回転軸の設定
	matRotZ = {
	cos(worldTransform_.rotation.z),sin(worldTransform_.rotation.z),0,0,
	-sin(worldTransform_.rotation.z),cos(worldTransform_.rotation.z),0,0,
	0,0,1,0,
	0,0,0,1
	};

	//X用回転軸の設定
	matRotX = {
	1,0,0,0,
	0,cos(worldTransform_.rotation.x),sin(worldTransform_.rotation.x),0,
	0,-sin(worldTransform_.rotation.x),cos(worldTransform_.rotation.x),0,
	0,0,0,1
	};

	//Y用回転軸の設定
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
	//各軸の回転行列を合成
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


AliceMathF::Vector3 Player::ConvertToVector3(Transform& mat, AliceMathF::Vector3 vec)
{
	AliceMathF::Vector3 retVec = {};

	retVec.x = vec.x * mat.matWorld.m[0][0] + vec.y * mat.matWorld.m[1][0] + vec.z * mat.matWorld.m[2][0];

	retVec.y = vec.x * mat.matWorld.m[0][1] + vec.y * mat.matWorld.m[1][1] + vec.z * mat.matWorld.m[2][1];

	retVec.z = vec.x * mat.matWorld.m[0][2] + vec.y * mat.matWorld.m[1][2] + vec.z * mat.matWorld.m[2][2];

	return retVec;
}