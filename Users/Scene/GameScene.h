#pragma once

#include"ErrorException.h"
#include"Camera.h"
#include"Sprite2D.h"
#include"Sprite3D.h"
#include"Collision.h"
#include"Input.h"
#include"ParticleCreat.h"
#include"Light.h"
#include"Mesh.h"
#include"3DMesh.h"
#include"Transform.h"
#include"DefaultMaterial.h"
#include"AudioManager.h"
#include"Button.h"
#include"Cursor.h"
#include"fbxModel.h"
#include"fbxAnimation.h"
#include"FbxLoader.h"
#include"objModel.h"
#include"PrimitiveModel.h"

class GameScene
{
private:

	Mesh* mesh2D = nullptr;
	Mesh3D* mesh3D = nullptr;
	Input* input = nullptr;

	std::unique_ptr<Light> light;
	AliceMathF::Vector4 lightColor = { 1, 1, 1, 1 };

	std::unique_ptr<Camera> camera;

	std::unique_ptr<Sprite3D> sprite;
	std::unique_ptr<Sprite2D> sprite2D;

	float frame = 0.0f;

	TextureData texData;
	TextureData texData2D;

	Transform texTrans;
	Transform texTrans2;
	Transform texTrans2D;

	Button* button = nullptr;
	Cursor* cursor = nullptr;

	TextureData texCursor;
	Transform texCursorTrans;

	int sceneNum = 0;

	std::unique_ptr<fbxModel>model;

	std::unique_ptr<fbxAnimation>animatio;

	Transform modelTrans;

	uint64_t fr = 0;

	std::unique_ptr<objModel> objModel;

public:

	GameScene();
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�Q�b�^�[
	static GameScene* GetInstance();

private:

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};