#include "objModel.h"
#include"TextureManager.h"
#include"AliceFunctionUtility.h"
#include"DefaultMaterial.h"

objModel::objModel()
{
}

objModel::~objModel()
{
}

void objModel::Create(const char* filePath,bool smoothing)
{
	std::vector<std::string> files;
	files = AliceFunctionUtility::getFileNames(filePath);
	std::string fileName;

	for (std::string file : files)
	{
		if (file.find("obj") != std::string::npos)
		{
			fileName = file;
		}

	}
	//�t�@�C���X�g���[��
	std::ifstream file;
	//obj�t�@�C�����J��
	file.open(fileName);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//���_���W
	std::vector<AliceMathF::Vector3> positions;
	//�@���x�N�g��
	std::vector<AliceMathF::Vector3> normals;
	//�e�N�X�`��UV
	std::vector<AliceMathF::Vector2> texcoords;

	PosNormalUv tmp = { {},{},{} };

	//1�s���ǂݍ���
	std::string line;
	while (std::getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		std::getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			std::string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(filePath, filename);

		}
		//�擪������V�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			AliceMathF::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			AliceMathF::Vector2 texcoord;
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			AliceMathF::Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		//�擪�����Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (std::getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1����������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				uint16_t indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				//�X���b�V�����΂�
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				//�X���b�V�����΂�
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				//���_�f�[�^�̒ǉ�
				tmp.pos = positions[static_cast<size_t>(indexPosition) - 1];
				tmp.normal = normals[static_cast<size_t>(indexNormal) - 1];
				tmp.uv = texcoords[static_cast<size_t>(indexTexcoord) - 1];
				vertices.push_back(tmp);

				if (smoothing)
				{
					smoothData[static_cast<size_t>(indexPosition)].emplace_back(static_cast<uint32_t>(vertices.size()-1));
				}

				//�C���f�b�N�X�f�[�^�̒ǉ�
				indices.emplace_back(static_cast<uint32_t>(indices.size()));

			}
		}
	}

	//�t�@�C�������
	file.close();

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	}

	maxVert = static_cast<UINT>(vertices.size());
	maxIndex = static_cast<UINT>(indices.size());

	//���_�o�b�t�@�E�C���f�b�N�X����
	CreatVertexIndexBuffer();

	//���_�o�b�t�@�ւ̃f�[�^�]��
	vertexBuffer->Update(vertices.data());

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	indexBuffer->Update(indices.data());

	constBuffMaterial = std::make_unique<ConstantBuffer>();
	constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));
	ConstBuffDataMaterial tmpMaterial = modelMaterial.GetConstBuffMaterial();
	constBuffMaterial->Update(&tmpMaterial);
}

void objModel::Initialize(ModelShape shape)
{
	static_cast<void>(shape);
}

void objModel::Draw(Transform* transform, Material* material)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();


	if (!material)
	{
		modelMaterialData = DefaultMaterial::GetDefaultMaterial()->DEFAULT_TEXTURE_MATERIAL.get();
	}
	else
	{
		modelMaterialData = material;
	}

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&ibView);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, transform->GetconstBuff()->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial->GetAddress());
	light->SetConstBufferView(cmdList.Get(), 2);

	// SRV�q�[�v�̐ݒ�R�}���h
	cmdList->SetDescriptorHeaps(1, textureData.srvHeap.GetAddressOf());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(3, textureData.gpuHandle);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(maxIndex, 1, 0, 0, 0);

}


void objModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;

	std::string filePath = directoryPath + "/" + filename;

	file.open(filePath);
	if (file.fail())
	{
		assert(0);
	}

	std::string line;
	while (std::getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		std::getline(line_stream, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_stream >> modelMaterial.name;
		}
		if (key == "Ka")
		{
			line_stream >> modelMaterial.ambient.x;
			line_stream >> modelMaterial.ambient.y;
			line_stream >> modelMaterial.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> modelMaterial.diffuse.x;
			line_stream >> modelMaterial.diffuse.y;
			line_stream >> modelMaterial.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> modelMaterial.specular.x;
			line_stream >> modelMaterial.specular.y;
			line_stream >> modelMaterial.specular.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> modelMaterial.textureFiename;
			LoadTexture(directoryPath, modelMaterial.textureFiename);
		}
	}

	//�t�@�C�������
	file.close();
}

void objModel::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	std::string filepath = directoryPath + "/" + filename;

	textureData = TextureManager::GetInstance()->LoadTexture(filepath);
}

void objModel::Initialize(const char* filePath, bool smoothing)
{
	ModelInitialize();

	Create(filePath, smoothing);
}

void objModel::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (;itr != smoothData.end(); itr++)
	{
		//�e�ʗp�̋��ʓ_�R���N�V����
		std::vector<uint32_t>& v = itr->second;
		//�S���_�̖@���𕽋ς���
		AliceMathF::Vector3 normal = {};
		for (uint32_t index : v)
		{
			normal += AliceMathF::Vector3{vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
		}

		normal = normal / static_cast<float>(v.size());
		normal.Normal();
		//���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
		for (uint32_t index : v)
		{
			vertices[index].normal = { normal.x,normal.y,normal.z };
		}

	}
}