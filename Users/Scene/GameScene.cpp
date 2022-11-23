#include "GameScene.h"

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

	//ライト初期化
	light.reset(Light::Create());
	light->SetLightColor(lightColor);
	//モデルにライトをセット
	Model::SetLight(light.get());

	camera = std::make_unique<Camera>();
	camera->Initialize(UpdateProjMatrixFunc_Perspective);

	
	texData = TextureManager::GetInstance()->LoadTexture("Resources/Default/boss2.png");
	sprite.reset(Sprite::Create3DSprite(texData));
	texTrans.Initialize();
	texTrans2.Initialize();
//	texTrans2.translation = { 0,0,0 };
//	texTrans2.rotation = { 0,1,0 };

	texData2D= TextureManager::GetInstance()->LoadTexture("Resources/Default/mario.jpg");
	texTrans2D.Initialize();
	texTrans2D.translation = { 640,360,0 };

	button = new Button;
	cursor = new Cursor();

	texCursor = TextureManager::GetInstance()->LoadTexture("Resources/Default/smallOityan.jpg");
	texCursorTrans.Initialize();
	texCursorTrans.translation = { 640,360,0 };

	objModel.reset(Model::CreateObjModel("Resources/Default/skydome"));

	model = std::make_unique<fbxModel>();
	model->Initialize();
	//model->Create("Resources/FBX", false, false, true,true);
	//modelTrans.translation = { 0.0f,-75.0f,180.0f };
	modelTrans.translation = { 0.0f,0.0f,0.0f };

	modelTrans.Initialize();
	animatio = std::make_unique<fbxAnimation>();

	//FbxLoader::GetInstance()->LoadAnimation(animatio.get(),"Resources/FBX","aruku");
}

void GameScene::Update()
{
	fr++;


	//model->AnimationUpdate(animatio.get(),75);
	if (input->KeepPush(DIK_LEFT))
	{
		AliceMathF::Vector3 move = {0.0f,0.0f,0.0f };

		move = { 0.0f,0.1f,0.0f };
		modelTrans.rotation += move;
	}

	if (input->KeepPush(DIK_RIGHT))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { 0.0f,-0.1f,0.0f };
		modelTrans.rotation += move;
	}
	if (input->KeepPush(DIK_UP))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { 1.0f,0.0f,0.0f };
		modelTrans.rotation += move;
	}
	if (input->KeepPush(DIK_DOWN))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { -1.0f,0.0f,0.0f };
		modelTrans.rotation += move;
	}

	if (input->KeepPush(DIK_W))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { 0.0f,-1.0f,0.0f };
		modelTrans.translation += move;
	}
	if (input->KeepPush(DIK_S))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { 0.0f,1.0f,0.0f };
		modelTrans.translation += move;
	}

	if (input->KeepPush(DIK_A))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = { 0.0f,0.0f,-1.0f };
		modelTrans.translation += move;
	}
	if (input->KeepPush(DIK_D))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0f,0.0f };

		move = {0.0f,0.0f,1.0f };
		modelTrans.translation += move;
	}
	//texTrans2.TransUpdate(camera.get());
	modelTrans.TransUpdate(camera.get());
	//switch (sceneNum)
	//{
	//case 0:
	//	cursor->Update();
	//	break;

	//case 1:
	//	//ライトの光線方向設定
	//	AliceMathF::Vector3 rightDir = { 10.0f,3.0f,5.0f };
	//	light->SetLightDir(rightDir);
	//	light->Update();

		texTrans2.TransUpdate(camera.get());
	//	texTrans.TransUpdate(camera.get());

		frame++;
	//	cursor->Update();
	//	texCursorTrans.translation = { cursor->GetCursor().x, cursor->GetCursor().y, 0.0f };
	//	break;
	//}

	
}

void GameScene::Draw()
{
	model->Draw(&modelTrans);

	//switch (sceneNum)
	//{
	//case 0:
	//	
	//	button->DrawButton(640.0f, 360.0f, 100.0f, 50.0f, { 255,255,255,255 }, cursor->GetCursor().x, cursor->GetCursor().y, true, sceneNum, 1);
	//	cursor->DrawCursor(10.0f, 10.0f, { 255,255,255,255 }, false);
	//	break;

	//case 1:
		//sprite->AnimationDraw(texTrans2, 64, 64, frame, 5);
	objModel->Draw(&modelTrans);
	//	button->DrawGraphButton(texData2D, texTrans2D, cursor->GetCursor().x, cursor->GetCursor().y, sceneNum, 0);
	//	cursor->DrawGraphCursor(texCursor, texCursorTrans);
	//	break;
	//}

	//printf("sceneNum=%d\n", sceneNum);
}

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}