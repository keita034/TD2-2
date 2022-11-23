#include "Model.h"
#include"PrimitiveModel.h"
#include"objModel.h"

Light* Model::light = nullptr;

AliceMathF::Matrix4& Model::GetMatWorld()
{
	return matWorld;
}

const std::vector<PosNormalUv> Model::GetVertices()
{
	return vertices;
}

const std::vector<uint32_t> Model::GetIndices()
{
	return indices;
}

void Model::SetLight(Light* light_)
{
	light = light_;
}

PrimitiveModel* Model::CreatePrimitiveModel(ModelShape type)
{
	PrimitiveModel* model = new PrimitiveModel();
	model->Initialize(type);
	return model;
}

objModel* Model::CreateObjModel(const char* filePath, bool smoothing)
{
	objModel* model = new objModel();
	model->Initialize(filePath, smoothing);
	return model;
}

void Model::ModelDraw(Transform* transform, Material* material)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	cmdList->SetPipelineState(material->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(material->rootSignature->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&ibView);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, transform->GetconstBuff()->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial->GetAddress());
	light->SetConstBufferView(cmdList.Get(), 3);

	// SRV�q�[�v�̐ݒ�R�}���h
	cmdList->SetDescriptorHeaps(1, material->textureData.srvHeap.GetAddressOf());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(2, material->textureData.gpuHandle);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(maxIndex, 1, 0, 0, 0);
}

void Model::ModelInitialize()
{
	device = DirectX12Core::GetInstance()->GetDevice();
	cmdList = DirectX12Core::GetInstance()->GetCommandList();

}

void Model::CreatVertexIndexBuffer()
{

	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(maxVert, sizeof(PosNormalUv));

	indexBuffer = std::make_unique<IndexBuffer>();
	indexBuffer->Create(maxIndex);
}
