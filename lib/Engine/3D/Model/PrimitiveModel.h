#pragma once
#include"ErrorException.h"
#include"Model.h"

class PrimitiveModel :public Model
{
private:
	//�L���[�u�̃C���f�b�N�X���ƒ��_��
	const UINT cubeMaxIndex = 36;
	const UINT cubeMaxVert = 24;

	//�~���̃C���f�b�N�X���ƒ��_��
	const UINT coneMaxIndex = 191;
	const UINT coneMaxVert = 66;

	//�V�����_�[�̃C���f�b�N�X���ƒ��_��
	const UINT cylinderMaxIndex = 384;
	const UINT cylinderMaxVert = 132;

	//���̃C���f�b�N�X���ƒ��_��
	const UINT sphereMaxIndex = 3072;
	const UINT sphereMaxVert = 560;

public:

	PrimitiveModel() = default;
	~PrimitiveModel() = default;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	void Draw(Transform* transform, Material* material = nullptr)override;

	//������
	void Initialize(ModelShape shape)override;

private:

	void CubeCreate();

	void ConeCreate();

	void CylinderCreate();

	void SphereCreate();


	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	virtual void Create(bool smoothing = false)override;

	virtual void Create(const char* filePath, bool smoothing = false);

	virtual void Initialize(const char* filePath, bool smoothing);

	PrimitiveModel& operator=(const PrimitiveModel&) = delete;
	PrimitiveModel(const PrimitiveModel&) = delete;
};
