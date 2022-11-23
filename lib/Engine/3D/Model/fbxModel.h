#pragma once
#include"ModelMesh.h"
#include"fbxAnimation.h"
#include"StructuredBuffer.h"

//struct PosNormUvTangeCol
//{
//	AliceMathF::Vector3 position; // �ʒu���W
//	AliceMathF::Vector3 normal; // �@��
//	AliceMathF::Vector2 uv; // uv���W
//	AliceMathF::Vector3 tangent; // �ڋ��
//	AliceMathF::Vector4 color; // ���_�F
//};



class fbxModel :public Model
{

private:

	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<ModelMesh> meshes;

	//�t�����h�N���X
	friend class FbxLoader;

	std::unique_ptr<ConstantBuffer> postureMatBuff;

	static std::shared_ptr<ComputeRelation> computeRelation;

	const aiScene* mScene = nullptr;

	std::vector<Node> nodes;

	float oldFrame = 0.0f;

	AliceMathF::Matrix4 globalInverseTransform;
	char PADING[4]{};

public:

	fbxModel() = default;
	~fbxModel() = default;

	virtual void Initialize();

	virtual void Draw(Transform* transform, Material* material = nullptr);

	void AnimationUpdate(const fbxAnimation* animation, float frame);

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="filePath">�I�u�W�F�N�g�܂ł̃t�@�C���p�X</param>
	void Create(const char* filePath, bool smoothing, bool inverseU = false, bool inverseV = false, bool animeFlag = false);

private:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	virtual void Create(const char* filePath, bool smoothing);
	virtual void Create(bool smoothing);
	virtual void Initialize(ModelShape shape);
	virtual void Initialize(const char* filePath, bool smoothing);
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	fbxModel& operator=(const fbxModel&) = delete;
	fbxModel(const fbxModel&) = delete;
};
