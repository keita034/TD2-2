#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include "Camera.h"
#include"Particle.h"

class BasicParticle : public Particle
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

public:

	BasicParticle() = default;
	~BasicParticle() = default;

	//������
	virtual void Initialize()override;

	///<summary>
	///�X�V
	///</summary>
	virtual void Update()override;

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="scale">{�J�n���X�P�[��,�I�����X�P�[��}</param>
	/// <param name="rotation">{�J�n����]�p,�I������]�p}</param>
	/// <param name="sColor">�J�n�J���[</param>
	/// <param name="eColor">�I���J���[</param>
	virtual void Add(
		UINT life, AliceMathF::Vector3& position, AliceMathF::Vector3& velocity,
		AliceMathF::Vector3& accel, AliceMathF::Vector2& scale, AliceMathF::Vector2& rotation
		, AliceMathF::Vector4& sColor, AliceMathF::Vector4& eColor)override;


	///<summary>
	///�r���{�[�h�`��
	///</summary>
	virtual void Draw(const TextureData& texture, Camera* camera, Material* material = nullptr)override;

private:

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	BasicParticle& operator=(const BasicParticle&) = delete;
	BasicParticle(const BasicParticle&) = delete;
};

