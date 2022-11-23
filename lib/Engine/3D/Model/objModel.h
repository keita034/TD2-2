#pragma once
#include"Model.h"
#include"DefaultMaterial.h"

class objModel : public Model
{
private:

public:

	objModel();
	~objModel();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	virtual void Draw(Transform* transform, Material* material = nullptr)override;

	//������
	virtual void Initialize(const char* filePath, bool smoothing)override;

private:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	virtual void Create(const char* filePath, bool smoothing)override;

	virtual void Initialize(ModelShape shape);

	void LoadMaterial(const std::string& directoryPath,const std::string& filename);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	virtual void Create(bool smoothing)override
	{
		static_cast<void>(smoothing);
	};

	void CalculateSmoothedVertexNormals();

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	objModel& operator=(const objModel&) = delete;
	objModel(const objModel&) = delete;
};