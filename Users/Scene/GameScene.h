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

#include"Player.h"
#include"Enemy.h"


class GameScene
{
private:

	Mesh* mesh2D = nullptr;
	Mesh3D* mesh3D = nullptr;
	Input* input = nullptr;

	std::unique_ptr<Light> light;
	AliceMathF::Vector4 lightColor = { 1, 1, 1, 1 };

	std::unique_ptr<Camera> camera;

	AudioManager* audio = nullptr;
	std::array<std::unique_ptr<Sprite2D>, 10>sprite;
	std::array<TextureData, 10> textureHandle;
	std::array<Transform, 10>spriteTrans;

	std::array<std::unique_ptr<Sprite2D>, 5> ButtonSprite;
	std::array<TextureData, 5> ButtonTH;
	std::array<Transform, 5>ButtonSpriteTrans;

	int scene = 0;

	//�e�N�X�`���n���h��

	TextureData numTex;
	std::unique_ptr<Sprite2D>numSprite[3];
	std::array<Transform, 3> numSpriteTrans;

	std::unique_ptr<Sprite2D>bossGaugeBox;
	TextureData bossGaugeBoxTex;
	Transform bossGaugeBoxTrans;

	std::unique_ptr<Sprite2D>bossGauge;
	TextureData bossGaugeTex;
	Transform bossGaugeTrans;

	///���L����
	Player* player = nullptr;
	Transform floor;

	std::array<std::unique_ptr<objModel>, 6> model;

	std::unique_ptr<objModel> bulletModel;
	std::unique_ptr<objModel> ballModel;
	Enemy* enemy = nullptr;
	Transform skyDome;

	float focalLengs;


	// ���Ԍv���ɕK�v�ȃf�[�^
	size_t startCount = 0;
	size_t nowCount = 0;
	size_t elapsedCount = 0;

	// ��ԂŎg���f�[�^
	// start -> end �� 5[��] �Ŋ���������
	AliceMathF::Vector3 start{ -50.0f, 30.0f, -50.0f };
	AliceMathF::Vector3 p1{ -25.0f, 10.0f, -25.0f };
	AliceMathF::Vector3 p2{ +12.5f, 5.0f, -25.0f };
	AliceMathF::Vector3 end{ +25.0f, 10.0f, 25 };
	AliceMathF::Vector3 end2{ +40.0f, 15.0f, 40 };

	std::vector<AliceMathF::Vector3> points{ start,start,p1,p2,end,end2 };

	// P1 ����X�^�[�g����
	size_t startIndex = 1;

	float maxTime = 5.0f;
	float timeRate, timeLerp;

	int gameTimer = 0;
	int waitTime;

	// �Q�[�����[�v�Ŏg���ϐ��̐錾

	AliceMathF::Vector3 position;

	AliceMathF::Vector3 pos, viewPos;

	float PI = 3.141592;

	int rot = 0;

	//�T�E���h
	uint32_t clickHandle = 0;
	uint32_t gameHandle = 0;
	uint32_t titleHandle = 0;
	uint32_t damageHandle = 0;
	uint32_t attackHandle = 0;
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

	void ViewPlayer();

	void CirclePosition(AliceMathF::Vector3 pos, int angle, int length);

private:

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};