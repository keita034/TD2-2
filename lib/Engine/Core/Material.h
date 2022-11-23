#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"DirectX12Core.h"
#include"AliceUtility.h"
#include"Shader.h"
#include"PipelineState.h"
#include"RootSignature.h"

class Material
{
public:
	//�e�N�X�`���f�[�^
	TextureData textureData;

	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	//�u�����h�X�e�[�g
	D3D12_BLEND_DESC blenddesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	//�p�C�v���C���X�e�[�g
	std::unique_ptr<PipelineState> pipelineState;

	//���[�g�V�O�l�`��
	std::unique_ptr<RootSignature> rootSignature;

	//���_�V�F�[�_
	std::unique_ptr<Shader> vertexShader;
	//�s�N�Z���V�F�[�_
	std::unique_ptr<Shader> pixelShader;
	//�W�I���g���V�F�[�_
	std::unique_ptr<Shader> geometryShader;
	//�n���V�F�[�_
	std::unique_ptr<Shader> hullShader;
	//�h���C���V�F�[�_
	std::unique_ptr<Shader> domainShader;

	//�[�x�t���O
	bool depthFlag = true;
	char PADING[3]{};

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	Material() = default;
	~Material() = default;

private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;
};
