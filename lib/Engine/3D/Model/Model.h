#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"
#include"DirectX12Core.h"
#include"Light.h"
#include"Transform.h"
#include"Material.h"
#include"ConstantBuffer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"

enum class ModelShape
{
	Cube,//������
	Sphere,//��
	Capsule,//�J�v�Z��
	Cylinder,//�~��
	Cone,//�~��
};

enum ShaderType
{
	Default,
	Flat,
	Gouraud,
	Lambert,
	Phong,

};

class PrimitiveModel;
class objModel;

class Model
{
protected:

	TextureData textureData;

	HRESULT result = S_OK;
	char PADDING[4]={};
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer> vertexBuffer;
	//�C���f�b�N�X�o�b�t�@
	std::unique_ptr<IndexBuffer> indexBuffer;
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	char PADDING1[4]={};
	//�C���f�b�N�X�̐�
	UINT maxIndex = 0u;;
	char PADDING2[4]={};
	//���_�̐�
	UINT maxVert = 0u;
	//���[���h�s��
	AliceMathF::Matrix4 matWorld;
	//���_�f�[�^
	std::vector<PosNormalUv>vertices;
	//���_�C���f�b�N�X
	std::vector<uint32_t> indices;
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<uint32_t, std::vector<uint32_t>>smoothData;
	//���C�g
	static Light* light;
	//�}�e���A��
	ModelMaterial modelMaterial{};
	//�萔�o�b�t�@�}�e���A��
	std::unique_ptr<ConstantBuffer> constBuffMaterial;

	Material* modelMaterialData;


public:

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	virtual AliceMathF::Matrix4& GetMatWorld();

	/// <summary>
	/// ���_���W���擾
	/// </summary>
	/// <returns>���_���W�z��</returns>
	virtual const std::vector<PosNormalUv>GetVertices();

	/// <summary>
	/// �C���f�b�N�X���擾
	/// </summary>
	/// <returns>�C���f�b�N�X���W�z��</returns>
	virtual const std::vector<uint32_t>GetIndices();

	/// <summary>
	/// ���C�g�̃Z�b�g
	/// </summary>
	/// <param name="light">���C�g</param>
	static void SetLight(Light* light_);

	Model() = default;
	virtual ~Model()= default;

	virtual void Draw(Transform* transform,Material* material) = 0;

	static PrimitiveModel* CreatePrimitiveModel(ModelShape type);

	static objModel* CreateObjModel(const char* filePath, bool smoothing = false);


protected:

	virtual void Initialize(ModelShape shape) = 0;

	virtual void Initialize(const char* filePath, bool smoothing) = 0;

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	virtual void Create(bool smoothing) = 0;

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="filePath">�I�u�W�F�N�g�܂ł̃t�@�C���p�X</param>
	virtual void Create(const char* filePath, bool smoothing) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	virtual void ModelDraw(Transform* transform, Material* material);

	//������
	virtual void ModelInitialize();


	//���_�o�b�t�@�E�C���f�b�N�X����
	virtual void CreatVertexIndexBuffer();

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Model& operator=(const Model&) = delete;
	Model(const Model&) = delete;
};