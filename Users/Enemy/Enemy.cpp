#include "Enemy.h"
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <time.h>



Enemy::Enemy() {
}

Enemy::~Enemy() {}

void Enemy::Initialize(objModel* model,objModel* model2) {
	assert(model);
	model_[0] = model;
	model_[1] = model2;

	input_ = Input::GetInstance();

	{
		worldTransForm[0].Initialize();
		worldTransForm[0].translation = { 0,-10,0 };
		worldTransForm[0].rotation = { 0,90,0 };

		worldTransForm[1].Initialize();
		worldTransForm[1].translation = { 0,0,0 };
		worldTransForm[1].parent = &worldTransForm[0];

		worldTransForm[2].Initialize();
		worldTransForm[2].translation = { 0,0,0 };
		worldTransForm[2].parent = &worldTransForm[1];

		worldTransForm[3].Initialize();
		worldTransForm[3].translation = { 0,0,0 };
		worldTransForm[3].parent = &worldTransForm[2];
		worldTransForm[4].Initialize();
		worldTransForm[4].translation = { 0,0,0 };
		worldTransForm[4].parent = &worldTransForm[2];

		worldTransForm[5].Initialize();
		worldTransForm[5].translation = { 0,0,0 };
		worldTransForm[5].parent = &worldTransForm[1];

		worldTransForm[6].Initialize();
		worldTransForm[6].translation = { 0,0,0 };
		worldTransForm[6].parent = &worldTransForm[5];
		worldTransForm[7].Initialize();
		worldTransForm[7].translation = { 0,0,0 };
		worldTransForm[7].parent = &worldTransForm[5];

		worldTransForm[8].Initialize();
		worldTransForm[9].Initialize();
		worldTransForm[8].parent = &worldTransForm[2];
		worldTransForm[9].parent = &worldTransForm[5];

	}

	ball[0].Initialize();
	ball[1].Initialize();
	ball[0].scale = AliceMathF::Vector3(0.1f, 0.1f, 0.1f);
	ball[1].scale = AliceMathF::Vector3(0.1f, 0.1f, 0.1f);
	ball[0].parent = &worldTransForm[2];
	ball[1].parent = &worldTransForm[5];

	isDead = false;
	YTmp = { 0,1,0 };
	speed = 0.0f;
	time = 0;
	splineCount_ = 0;
	rotate = 0.5f;
}

void Enemy::CalcVec(AliceMathF::Vector3 obj) {
	//ê≥ñ âºÉxÉNÉgÉã
	enemyTmp = obj - worldTransForm[0].translation;
	enemyTmp = enemyTmp.Normal();
	//âEÉxÉNÉgÉã
	enemyRight = YTmp.Cross(enemyTmp);
	enemyRight = enemyRight.Normal();
	//ê≥ñ ÉxÉNÉgÉã
	enemyFront = enemyRight.Cross(YTmp);
	enemyFront = enemyFront.Normal();
}


void Enemy::Update(AliceMathF::Vector3 obj, Camera* viewProjection_) {
	//ÉxÉNÉgÉãåvéZ
	CalcVec(obj);

	if (isHit == true) {
		hp--;
		isHit = false;
	}
	if (waitTime >= 0) {
		waitTime--;
	}

	rotate += 0.5;
	if (scene == 0) {	// ìoèÍ
		scene = 1;
		hp = 100;

		movePattern = 0;

		worldTransForm[0].translation = { 0,-10,0 };
		worldTransForm[1].translation = { 0,0,0 };
		worldTransForm[2].translation = { 0,0,0 };
		worldTransForm[3].translation = { 0,0,0 };
		worldTransForm[4].translation = { 0,0,0 };
		worldTransForm[5].translation = { 0,0,0 };
		worldTransForm[6].translation = { 0,0,0 };
		worldTransForm[7].translation = { 0,0,0 };

		worldTransForm[0].rotation = { 0,0,0 };
		worldTransForm[1].rotation = { 0,0,0 };
		worldTransForm[2].rotation = { 0,0,0 };
		worldTransForm[3].rotation = { 0,0,0 };
		worldTransForm[4].rotation = { 0,0,0 };
		worldTransForm[5].rotation = { 0,0,0 };
		worldTransForm[6].rotation = { 0,0,0 };
		worldTransForm[7].rotation = { 0,0,0 };
	}
	if (scene == 1) {	// ìoèÍ
		entryMovePatt();
	}

	if (scene == 2) {	// 
		scene = 3;
	}

	if (scene == 3) {	// êÌì¨
		battleMovePatt();
		if (hp <= 0) {
			scene = 4;
		}
	}

	if (scene == 4) {	// åÇîj
		worldTransForm[1].translation.y -= 2;
		if (worldTransForm[1].translation.y <= -80) {
			scene = 5;
		}
	}

	for (int i = 0; i < _countof(worldTransForm); i++) {

		worldTransForm[i].TransUpdate(viewProjection_);
	}
	for (int i = 0; i < _countof(ball); i++) {

		ball[i].TransUpdate(viewProjection_);
	}

}

void Enemy::entryMovePatt() {

	switch (movePattern)
	{
	case 0:

		worldTransForm[0].translation = { 0,-10,0 };
		worldTransForm[1].translation = { 0,0,0 };
		worldTransForm[2].translation = { 0,0,0 };
		worldTransForm[3].translation = { 0,0,0 };
		worldTransForm[4].translation = { 0,0,0 };
		worldTransForm[5].translation = { 0,0,0 };
		worldTransForm[6].translation = { 0,0,0 };
		worldTransForm[7].translation = { 0,0,0 };

		worldTransForm[0].rotation = { 0,0,0 };
		worldTransForm[1].rotation = { 0,0,0 };
		worldTransForm[2].rotation = { 0,0,0 };
		worldTransForm[3].rotation = { 0,0,0 };
		worldTransForm[4].rotation = { 0,0,0 };
		worldTransForm[5].rotation = { 0,0,0 };
		worldTransForm[6].rotation = { 0,0,0 };
		worldTransForm[7].rotation = { 0,0,0 };

		movePattern = 1;
		break;
	case 1:

		if (worldTransForm[1].translation.y <= 10) {

			worldTransForm[1].translation.y += 0.1f;
		}


		if (splineCount_ >= 1) {
			worldTransForm[1].translation.y = 10;
			movePattern = 2;


			worldTransForm[1].translation = { 0,10,0 };
			worldTransForm[2].translation = { 0,0,0 };
			worldTransForm[3].translation = { 0,0,0 };
			worldTransForm[4].translation = { 0,0,0 };
			worldTransForm[5].translation = { 0,0,0 };
			worldTransForm[6].translation = { 0,0,0 };
			worldTransForm[7].translation = { 0,0,0 };

			worldTransForm[0].rotation = { 0,0,0 };
			worldTransForm[1].rotation = { 0,0,0 };
			worldTransForm[2].rotation = { 0,0,0 };
			worldTransForm[3].rotation = { 0,0,0 };
			worldTransForm[4].rotation = { 0,0,0 };
			worldTransForm[5].rotation = { 0,0,0 };
			worldTransForm[6].rotation = { 0,0,0 };
			worldTransForm[7].rotation = { 0,0,0 };
		}

		break;
	case 2:

		if (worldTransForm[2].translation.x <= 20) {

			worldTransForm[2].translation.x += 0.1f;
			worldTransForm[5].translation.x += -0.1f;
		}


		if (splineCount_ >= 2) {
			worldTransForm[2].translation.x = 20;
			worldTransForm[5].translation.x = -20;
			movePattern = 3;

			worldTransForm[3].translation = { 0,0,0 };
			worldTransForm[4].translation = { 0,0,0 };
			worldTransForm[6].translation = { 0,0,0 };
			worldTransForm[7].translation = { 0,0,0 };

			worldTransForm[0].rotation = { 0,0,0 };
			worldTransForm[1].rotation = { 0,0,0 };
			worldTransForm[2].rotation = { 0,0,0 };
			worldTransForm[3].rotation = { 0,0,0 };
			worldTransForm[4].rotation = { 0,0,0 };
			worldTransForm[5].rotation = { 0,0,0 };
			worldTransForm[6].rotation = { 0,0,0 };
			worldTransForm[7].rotation = { 0,0,0 };
		}


		break;
	case 3:
		if (worldTransForm[4].translation.x <= 10) {

			worldTransForm[3].translation.x += -0.1f;
			worldTransForm[4].translation.x += 0.1f;
			worldTransForm[6].translation.x += -0.1f;
			worldTransForm[7].translation.x += 0.1f;
		}


		if (splineCount_ >= 3) {
			worldTransForm[3].translation.x = -10;
			worldTransForm[4].translation.x = 10;
			worldTransForm[6].translation.x = -10;
			worldTransForm[7].translation.x = 10;
			movePattern = 4;

			worldTransForm[0].rotation = { 0,0,0 };
			worldTransForm[1].rotation = { 0,0,0 };
			worldTransForm[2].rotation = { 0,0,0 };
			worldTransForm[3].rotation = { 0,0,0 };
			worldTransForm[4].rotation = { 0,0,0 };
			worldTransForm[5].rotation = { 0,0,0 };
			worldTransForm[6].rotation = { 0,0,0 };
			worldTransForm[7].rotation = { 0,0,0 };
		}

		break;

	case 4:

		break;
	}


}


void Enemy::battleMovePatt() {
	if (movePattern != 3) {
		worldTransForm[0].rotation.y += 0.5f;
	}

	worldTransForm[8].rotation = AliceMathF::Vector3(rotate, rotate, rotate);
	worldTransForm[9].rotation = AliceMathF::Vector3(rotate, rotate, rotate);

	worldTransForm[3].rotation.z += 1.0f;
	worldTransForm[4].rotation.z += 1.0f;
	worldTransForm[6].rotation.z += -1.0f;
	worldTransForm[7].rotation.z += -1.0f;

	//worldTransForm[1].translation_.y = sin(3.1415 * 2 / 500 * Count)*10;
	//worldTransForm[4].translation_.y = sin(3.1415 * 2 / 250 * Count)*10;


	/*if (input_->PushKey(DIK_UP)) {
		worldTransForm[1].translation_.y += 0.5;
	}if (input_->PushKey(DIK_DOWN)) {
		worldTransForm[1].translation_.y -= 0.5;
	}*/




	switch (movePattern)
	{
	case 0:// ë“ã@
		Count++;
		if (Count >= 500) {
			if (hp < 15) {
				movePattern = rand() % 3 + 1;
				Count = 0;
			}
			else {
				movePattern = rand() % 3;
				Count = 0;
			}
		}
		attacktime = 0;

		break;

	case 1:// çUåÇ1 ìÀêi

		if (motionPattern == 0) {
			state = 1;

			worldTransForm[0].rotation.y += -1;

			if (attacktime >= 50) {
				motionPattern = 1;
				attacktime = 0;
			}
		}
		//çUåÇ
		if (motionPattern == 1) {
			state = 2;

			worldTransForm[0].rotation.y += 2;

			if (attacktime >= 20) {
				motionPattern = 2;
				attacktime = 0;
			}

		}

		//çdíº
		if (motionPattern == 2) {

			if (attacktime >= 50) {
				movePattern = 4;
			}
		}

		break;

	case 2:// çUåÇ2

		if (motionPattern == 0) {
			state = 1;

			worldTransForm[1].translation.y += 0.5;

			if (attacktime >= 50) {
				motionPattern = 1;
				attacktime = 0;
			}
		}
		//çUåÇ
		if (motionPattern == 1) {
			state = 2;

			worldTransForm[1].translation.y += -2;

			if (worldTransForm[1].translation.y < 10.0f) {
				worldTransForm[1].translation.y = 10.0f;
				motionPattern = 2;
				attacktime = 0;
			}

			//if (attacktime >= 20) {
			//	motionPattern = 2;
			//	attacktime = 0;
			//}

		}

		//çUåÇ
		if (motionPattern == 2) {
			state = 3;

			ball[0].scale.y = 0.3f;
			ball[0].scale.x += 0.5f;
			ball[0].scale.z += 0.5f;

			ball[1].scale.y = 0.3f;
			ball[1].scale.x += 0.5f;
			ball[1].scale.z += 0.5f;

			if (ball[0].scale.x > 30.0f) {

				motionPattern = 3;
				attacktime = 0;
				ball[0].scale.x = 0;
				ball[0].scale.z = 0;
			}
			if (ball[1].scale.x > 30.0f) {

				motionPattern = 3;
				attacktime = 0;
				ball[1].scale.x = 0;
				ball[1].scale.z = 0;
			}

			if (attacktime >= 30) {
				motionPattern = 3;
				attacktime = 0;
				ball[0].scale.x = 0;
				ball[0].scale.z = 0;
				ball[1].scale.x = 0;
				ball[1].scale.z = 0;
			}

		}

		//çdíº
		if (motionPattern == 3) {

			if (attacktime >= 25) {
				movePattern = 4;
			}
		}


		break;

	case 3:// ÅH

		if (motionPattern != 0) {
			worldTransForm[1].translation.y = (sin(3.1415 * 2 / 500 * Count) * 10) + 10;
			Count++;
		}

		if (500 < Count) {
			Count = 0;
		}
		// èÄîıà⁄ìÆ
		if (motionPattern == 0) {
			state = 1;


			if (worldTransForm[1].translation.y < 9.9f) {
				worldTransForm[1].translation.y += 0.1;
			}
			else if (worldTransForm[1].translation.y > 10.1f) {
				worldTransForm[1].translation.y -= 0.1;
			}
			else {
				motionPattern = 1;
				attacktime = 0;
			}
		}
		// è„â∫à⁄ìÆäJén
		if (motionPattern == 1) {
			state = 2;
			acceleration += 0.002f;
			worldTransForm[0].rotation.y += acceleration;

			if (acceleration >= 1.5f) {
				motionPattern = 2;
				attacktime = 0;
			}
		}
		//çUåÇ
		if (motionPattern == 2) {
			state = 3;

			worldTransForm[0].rotation.y += acceleration;

			if (attacktime >= 600) {
				motionPattern = 3;
				attacktime = 0;
			}
		}

		//çdíº
		if (motionPattern == 3) {

			acceleration -= 0.002f;

			worldTransForm[0].rotation.y += acceleration;
			if (acceleration < 0) {
				acceleration = 0;
			}

			if (acceleration <= 0.0f) {

				//worldTransForm[0].rotation_.y = 10;

				if (worldTransForm[1].translation.y < 9.9f) {
					worldTransForm[1].translation.y++;
				}
				else if (worldTransForm[1].translation.y > 10.1f) {
					worldTransForm[1].translation.y--;
				}
				else
				{
					movePattern = 4;
				}
			}
		}

		break;

	case 4:	// éûä‘èâä˙âª

		attacktime = 0;
		motionPattern = 0;
		movePattern = 0;
		Count = 0;

		break;
	}

	time++;
	attacktime++;

}

void Enemy::Draw(Camera* viewProjection_) {

	if (isDead == false) {
		{
			model_[0]->Draw(&worldTransForm[1]);
			model_[0]->Draw(&worldTransForm[3]);
			model_[0]->Draw(&worldTransForm[4]);
			model_[0]->Draw(&worldTransForm[6]);
			model_[0]->Draw(&worldTransForm[7]);

			model_[0]->Draw(&worldTransForm[8]);
			model_[0]->Draw(&worldTransForm[9]);
			model_[0]->Draw(&worldTransForm[8]);

		}

		model_[1]->Draw(&ball[0]);
		model_[1]->Draw(&ball[1]);

	}
}


void Enemy::Hit() {
	for (int i = 0; i < _countof(worldTransForm); i++) {
		if (worldTransForm[i].translation.x < 0.5 && worldTransForm[i].translation.x > -0.5) {
			if (worldTransForm[i].translation.z < 0.5 && worldTransForm[i].translation.z > -0.5) {
				if (isDead == false) {
					isDead = true;
				}
			}
		}
	}
}

void Enemy::OnColision() {
	isHit = true;
	waitTime = 10;
}