#include "GameScene.h"

AliceMathF::Vector3 splinePosition(const std::vector<AliceMathF::Vector3>& points, size_t startIndex, float t);

int GetHundredDigit(int nowTime)
{
	if (nowTime > 1000)
	{
		int a = nowTime % 1000;
		return a / 100;
	}
	return nowTime / 100;
}
int GetTenDigit(int nowTime)
{
	if (nowTime > 100)
	{
		int a = nowTime % 100;
		return a / 10;
	}
	return nowTime / 10;
}

int GetOneDigit(int nowTime)
{
	if (nowTime > 10)
	{
		int a = nowTime % 10;
		return a;
	}
	return nowTime;
}

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	input = Input::GetInstance();
	mesh2D = Mesh::GetInstance();
	audio = AudioManager::GetInstance();

	//ライト初期化
	light.reset(Light::Create());
	light->SetLightColor(lightColor);
	//モデルにライトをセット
	Model::SetLight(light.get());

	model[0].reset(Model::CreateObjModel("Resources/cube"));
	model[1].reset(Model::CreateObjModel("Resources/circle"));
	model[2].reset(Model::CreateObjModel("Resources/enemy"));
	model[3].reset(Model::CreateObjModel("Resources/skydome"));
	model[4].reset(Model::CreateObjModel("Resources/Playerblend"));
	model[5].reset(Model::CreateObjModel("Resources/cane"));
	bulletModel.reset(Model::CreateObjModel("Resources/ULT"));
	ballModel.reset(Model::CreateObjModel("Resources/ball"));
	floor.Initialize();
	floor.translation = AliceMathF::Vector3(0, -0.05, 0);
	floor.scale = AliceMathF::Vector3(20, 50, 20);

	skyDome.Initialize();
	skyDome.scale = AliceMathF::Vector3(1000, 1000, 1000);

	enemy = new Enemy();
	enemy->Initialize(model[2].get(), ballModel.get());

	//ビュープロジェクションの初期化
	camera = std::make_unique<Camera>();
	camera->Initialize(UpdateProjMatrixFunc_Perspective);
	camera->target = AliceMathF::Vector3(0, 0, 0);
	camera->eye = start;
	camera->SetFar(5000.0f);
	focalLengs = 40;

	//ファイル名を指定してテクスチャを読み込む
	textureHandle[0] = TextureManager::Load("Resources/player.png");
	textureHandle[1] = TextureManager::Load("Resources/title.png");
	textureHandle[2] = TextureManager::Load("Resources/Result.png");

	textureHandle[3] = TextureManager::Load("Resources/Move.png");
	textureHandle[4] = TextureManager::Load("Resources/Skill.png");
	textureHandle[5] = TextureManager::Load("Resources/PlayerGauge.png");

	bossGaugeBoxTex = TextureManager::Load("Resources/BossGaugeBox.png");
	bossGaugeTex = TextureManager::Load("Resources/BossGauge.png");
	numTex = TextureManager::Load("Resources/TimeFont.png");

	for (size_t i = 0; i < 3; i++)
	{
		numSprite[i] = std::make_unique<Sprite2D>();
		numSprite[i]->Initialize(numTex);
	}

	for (size_t i = 0; i < 3; i++)
	{
		numSpriteTrans[i].Initialize();
		numSpriteTrans[i].translation = { 50.0f + i*80.0f, 150.0f ,0.0f };
	}

	sprite[0] = std::make_unique<Sprite2D>();
	sprite[0]->Initialize(textureHandle[1]);
	sprite[0]->SetAnchorPoint({ 0,0 });
	spriteTrans[0].Initialize();

	sprite[1] = std::make_unique<Sprite2D>();
	sprite[1]->Initialize(textureHandle[2]);
	sprite[1]->SetAnchorPoint({ 0,0 });
	spriteTrans[1].Initialize();

	sprite[2] = std::make_unique<Sprite2D>();
	sprite[2]->Initialize(textureHandle[3]);
	sprite[2]->SetAnchorPoint({ 0,0 });
	spriteTrans[2].Initialize();

	sprite[3] = std::make_unique<Sprite2D>();
	sprite[3]->Initialize(textureHandle[4]);
	sprite[3]->SetAnchorPoint({ 0,0 });
	spriteTrans[3].Initialize();

	sprite[4] = std::make_unique<Sprite2D>();
	sprite[4]->Initialize(textureHandle[5]);
	sprite[4]->SetAnchorPoint({ 0,0 });
	spriteTrans[4].Initialize();

	ButtonTH[0] = TextureManager::Load("Resources/TitleOff.png");
	ButtonTH[1] = TextureManager::Load("Resources/TitleOn.png");
	ButtonTH[2] = TextureManager::Load("Resources/ReTryOn.png");
	ButtonTH[3] = TextureManager::Load("Resources/ReTryOff.png");
	ButtonTH[4] = TextureManager::Load("Resources/StertPless.png");

	ButtonSprite[0] = std::make_unique<Sprite2D>();
	ButtonSprite[0]->Initialize(ButtonTH[0]);
	ButtonSprite[0]->SetAnchorPoint({ 0,0 });
	ButtonSpriteTrans[0].Initialize();

	ButtonSprite[1] = std::make_unique<Sprite2D>();
	ButtonSprite[1]->Initialize(ButtonTH[1]);
	ButtonSprite[1]->SetAnchorPoint({ 0,0 });
	ButtonSpriteTrans[1].Initialize();

	ButtonSprite[2] = std::make_unique<Sprite2D>();
	ButtonSprite[2]->Initialize(ButtonTH[2]);
	ButtonSprite[2]->SetAnchorPoint({ 0,0 });
	ButtonSpriteTrans[2].Initialize();

	ButtonSprite[3] = std::make_unique<Sprite2D>();
	ButtonSprite[3]->Initialize(ButtonTH[3]);
	ButtonSprite[3]->SetAnchorPoint({ 0,0 });
	ButtonSpriteTrans[3].Initialize();

	ButtonSprite[4] = std::make_unique<Sprite2D>();
	ButtonSprite[4]->Initialize(ButtonTH[4]);
	ButtonSprite[4]->SetAnchorPoint({ 0,0 });
	ButtonSpriteTrans[4].Initialize();

	//自キャラの生成
	player = new Player();
	//自キャラの初期化
	player->Initialize(model[4].get(), model[5].get(), bulletModel.get(), textureHandle[0]);
	player->Update();

	//viewProjection_.eye.y += 10;

	enemy->scene = 1;
	waitTime = 10;

	//初期化
	bossGaugeBox = std::make_unique<Sprite2D>();
	bossGaugeBox->Initialize(bossGaugeBoxTex);
	bossGaugeBoxTrans.Initialize();
	bossGaugeBoxTrans.translation = { 640,50,0 };

	bossGauge = std::make_unique<Sprite2D>();
	bossGauge->Initialize(bossGaugeTex);
	bossGaugeTrans.Initialize();
	bossGaugeTrans.translation = { 640,52,0 };

	gameHandle = audio->LoadAudio("Resources/Clutch_For_Gain.mp3");
	clickHandle = audio->LoadAudio("Resources/click.mp3");
	titleHandle = audio->LoadAudio("Resources/title.mp3");
	damageHandle = audio->LoadAudio("Resources/damage.mp3");
	attackHandle = audio->LoadAudio("Resources/attack.mp3");

	audio->PlayWave(titleHandle,true);

}

void GameScene::Update()
{
	switch (scene)
	{
	case 0:

		if (input->TriggerPush(DIK_SPACE)) {
			
			scene = 1;
			audio->PlayWave(clickHandle);
			audio->StopWave(titleHandle);
			audio->PlayWave(gameHandle,true);


		}
		break;

	case 2:
		if (input->TriggerPush(DIK_SPACE)) {
			audio->PlayWave(clickHandle);
			audio->PlayWave(titleHandle, true);

			for (size_t i = 0; i < 3; i++)
			{
				numSpriteTrans[i].translation = { 50.0f + i * 80.0f, 150.0f ,0.0f };
			}

			focalLengs = 40;

			enemy->scene = 0;
			enemy->movePattern = 0;
			enemy->motionPattern = 0;
			enemy->splineCount_ = 0;
			enemy->hp = 100;

			// P1 からスタートする
			startIndex = 1;
			timeRate = 1.0f;
			nowCount = 0;
			startCount = 0;

			elapsedCount = 0;

			// 補間で使うデータ
			// start -> end を 5[ｓ] で完了させる
			start = { -50.0f, 30.0f, -50.0f };
			p1 = { -25.0f, 10.0f, -25.0f };
			p2 = { +12.5f, 5.0f, -25.0f };
			end2 = { +40.0f, 15.0f, 40 };

			points = { start,start,p1,p2,end,end2 };

			position = AliceMathF::Vector3(0, 0, 0);

			camera->Initialize(UpdateProjMatrixFunc_Perspective);
			camera->eye = AliceMathF::Vector3(-50.0f, 30.0f, -50.0f);
			camera->SetFar(5000.0f);
			player->Initialize(model[4].get(), model[5].get(), bulletModel.get(), textureHandle[0]);
			player->worldTransform.translation = AliceMathF::Vector3(20.0f, 0, 0);
			player->worldTransform.rotation = AliceMathF::Vector3(0, 0, 0);
			player->fakeWeaponWorldTransform.rotation = AliceMathF::Vector3(0, 0, 0);
			player->weaponWorldTransform.rotation = AliceMathF::Vector3(0, 0, 0);


			player->Update();
			viewPos = AliceMathF::Vector3(0, 0, 0);
			rot = 0;
			gameTimer = 0;
			waitTime = 10;

			scene = 0;
		}

		break;
	case 3:
		if (input->TriggerPush(DIK_SPACE)) {
			scene = 0;

		}

		break;

	case 1:

		if ((nowCount / 60) >= 120) {
			scene = 2;
		}

		//更新
		bossGauge->SetSize({ enemy->hp * 9.6f,68 });
		bossGaugeTrans.translation = { 640 - ((100 - enemy->hp) * 4.8f), 52,0 };

		camera->SetFovAngleY(AliceMathF::FieldOfViewY(focalLengs, 35.0f));
		enemy->Update(AliceMathF::Vector3(0, 0, 0), camera.get());

		pos = AliceMathF::Vector3(0, 0, 0);


		nowCount++; // 1フレーム当たりに増えるcount

		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount * 3) / 100.0f;

		timeRate = elapsedTime / maxTime;

		enemy->splineCount_ = startIndex;

		if (timeRate >= 1.0f) {
			if (startIndex < points.size() - 3) {
				startIndex += 1;
				timeRate -= 1.0f;
				startCount = nowCount;
			}
			else {
				timeRate = 1.0f;
			}
		}

		if (enemy->scene == 1) {
			player->worldTransform.translation = AliceMathF::Vector3(20, 0, 0);
			position = splinePosition(points, startIndex, timeRate);

			camera->eye = position;
			if (position == end) {
				enemy->scene = 2;
			}
		}

		if (enemy->scene == 2) { // 適用count リセット
			startCount = nowCount;
		}

		if (enemy->scene == 3) {
			gameTimer++;

			if (waitTime >= 0) {
				waitTime--;
			}

			ViewPlayer();
			camera->eye = AliceMathF::Lerp(position, viewPos, timeRate);
			player->Update();
			for (int i = 2; i < _countof(enemy->worldTransForm); i++) {
				// 杖 
				if (Collision::BoxCollision(AliceMathF::GetWorldPosition(player->weaponWorldTransform),
					AliceMathF::GetWorldPosition(enemy->worldTransForm[i]),
					AliceMathF::Vector3(1, 1, 1),
					AliceMathF::Vector3(1, 1, 1))) {
					if (enemy->isDead == false) {
						audio->PlayWave(attackHandle);
						enemy->OnColision();
					}
				}
				if (Collision::BoxCollision(AliceMathF::GetWorldPosition(player->worldTransform),
					AliceMathF::GetWorldPosition(enemy->worldTransForm[i]),
					AliceMathF::Vector3(1, 1, 1),
					AliceMathF::Vector3(1, 1, 1))) {
					if (waitTime <= 0) {
						audio->PlayWave(damageHandle);
						gameTimer += 120;
						waitTime = 20;
					}

				}
				AliceMathF::Vector3 posA, posB;

				posA = AliceMathF::GetWorldPosition(player->worldTransform);
				//敵更新
				for (int i = 0; i < _countof(enemy->ball); i++) {
					posB = AliceMathF::GetWorldPosition(enemy->ball[i]);

					float a = std::pow(posB.x - posA.x, 2.0f) + std::pow(posB.y - posA.y, 2.0f) +
						std::pow(posB.z - posA.z, 2.0f);
					float lenR = std::pow((enemy->ball->scale.x + 1), 2.0);
					if (a <= lenR) {
						if (waitTime <= 0) {
							audio->PlayWave(damageHandle);
							gameTimer += 120;
							waitTime = 20;
						}
					}
				}

				/// <summary>
				/// 弾と敵の当たり判定
				/// </summary>
				AliceMathF::Vector3 posA_, posB_;
				for (std::unique_ptr<PlayerBullet>& bullet : player->bullets_) {
					posA_ = AliceMathF::GetWorldPosition(bullet->worldTransform);
					//敵更新
					for (int i = 0; i < _countof(enemy->worldTransForm); i++) {
						posB_ = AliceMathF::GetWorldPosition(enemy->worldTransForm[i]);

						float a = std::pow(posB_.x - posA_.x, 2.0f) + std::pow(posB_.y - posA_.y, 2.0f) +
							std::pow(posB_.z - posA_.z, 2.0f);
						float lenR = std::pow((1 + 1), 2.0);

						// 球と球の交差判定
						if (enemy->isDead == false) {
							if (a <= lenR) {
								// 自キャラの衝突時コールバックを呼び出す
								bullet->isDead = true;
								// 敵弾の衝突時コールバックを呼び出す
								enemy->OnColision();
								audio->PlayWave(attackHandle);
							}
						}
					}
				}
			}
		}
		if (enemy->scene == 5) {
			scene = 2;
			audio->StopWave(gameHandle);
		}


		camera->target = pos;
		break;

	}
	{
		//行列計算
		floor.TransUpdate(camera.get());
	}
	{
		//行列計算
		skyDome.TransUpdate(camera.get());
	}


}

void GameScene::Draw()
{
	if (scene >= 1) {
		enemy->Draw(camera.get());
		//自キャラの描画
		player->Draw(camera.get());

		model[1]->Draw(&floor);
	}
	model[3]->Draw(&skyDome);


	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (scene == 0) {
		spriteTrans[0].translation = { 400,50,0 };
		sprite[0]->Draw(spriteTrans[0]);

		ButtonSpriteTrans[4].translation = { 500,500,0 };
		ButtonSprite[4]->Draw(ButtonSpriteTrans[4]);
	}
	if (scene == 1) {
		//タイマー
		float Time[3] = {
			static_cast<float>(GetHundredDigit(gameTimer / 60))
			,static_cast<float>(GetTenDigit(gameTimer / 60))
			,static_cast<float>(GetOneDigit(gameTimer / 60)) };

		for (int i = 0; i < 3; i++)
		{
			numSprite[i]->AnimationDraw(numSpriteTrans[static_cast<size_t> (i)], 40, 40, Time[i], 1);
		}

		spriteTrans[2].translation = { 0,500,0 };

		sprite[2]->SetSize({ 300, 200 });
		sprite[2]->Draw(spriteTrans[2]);

		if (player->playerCharge >= 100) {
			spriteTrans[3].translation = { 470, 500,0 };
			sprite[3]->SetSize({ 400, 122 });
			sprite[3]->Draw(spriteTrans[3]);
		}

		bossGauge->Draw(bossGaugeTrans);
		bossGaugeBox->Draw(bossGaugeBoxTrans);

	}
	if (scene == 2) {
				//タイマー
		float Time[3] = {
			static_cast<float>(GetHundredDigit(gameTimer / 60))
			,static_cast<float>(GetTenDigit(gameTimer / 60))
			,static_cast<float>(GetOneDigit(gameTimer / 60)) };

		sprite[1]->Draw(spriteTrans[1]);

		ButtonSpriteTrans[1].translation = { 500,500,0 };

		ButtonSprite[1]->Draw(ButtonSpriteTrans[1]);

		for (int i = 0; i < 3; i++)
		{
			numSpriteTrans[i].translation = { 550.0f + i * 80.0f, 300.0f ,0.0f };
			numSprite[i]->AnimationDraw(numSpriteTrans[static_cast<size_t> (i)], 40, 40, Time[i], 1);
		}
	}
	if (scene == 3) {
		sprite[2]->Draw(spriteTrans[2]);
	}
}

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::ViewPlayer()
{
	const float speed = 1.00f;
	const float RotSpeed = 1 * (PI / 180);

	if (input->KeepPush(DIK_A)) {
		rot -= speed;
	}
	else if (input->KeepPush(DIK_D)) {
		rot += speed;
	}
	CirclePosition(camera->eye, rot, 60);
}

void GameScene::CirclePosition(AliceMathF::Vector3 pos_, int angle, int length)
{
	float  radius = angle * (PI / 180);

	pos_ = { cosf(radius) * length, pos_.y, sinf(radius) * length };

	viewPos = pos_;

}


AliceMathF::Vector3 splinePosition(const std::vector<AliceMathF::Vector3>& points, size_t startIndex, float t) {

	// 補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];	// Pnの値を返す
	if (startIndex < 1)return points[1];	// P1の値を返す

	// ｐ0～ｐ３ の制御点を取得する ※ ｐ1～ｐ2
	AliceMathF::Vector3 p0 = points[startIndex - 1];
	AliceMathF::Vector3 p1 = points[startIndex];
	AliceMathF::Vector3 p2 = points[startIndex + 1];
	AliceMathF::Vector3 p3 = points[startIndex + 2];

	// Catmull-Rom の式による補間
	AliceMathF::Vector3 position;
	position = {
	2 * p1 + (-p0 + p2) * t +
	(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
	(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t) };

	return position * 0.5f;
}