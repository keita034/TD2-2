#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"
#include"ConstantBuffer.h"
#include"AliceUtility.h"

class Transform
{
private:
	// 定数バッファ
	std::shared_ptr<ConstantBuffer> constBuff;
	//定数バッファのマッピング用ポインタ
	worldViewpojCamera constBuffMap;

	//初期化用のビュー行列計算
	static AliceMathF::Matrix4 defaultViewMat;

	//初期化用のプロジェクション行列計算
	static AliceMathF::Matrix4 defaultProjectionMat;


public:
	// ローカル座標
	AliceMathF::Vector3 translation = { 0.0f, 0.0f, 0.0f };
	// X,Y,Z軸回りのローカル回転角
	AliceMathF::Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	// ローカルスケール
	AliceMathF::Vector3 scale = { 1.0f, 1.0f, 1.0f };

	AliceMathF::Matrix4 matWorld;

	// 親となるワールド変換へのポインタ
	const Transform* parent = nullptr;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransUpdate(Camera* camera);
	void TransUpdate2(Camera* camera);
	void Update();

	void MakeWorldMatrix();

	ID3D12Resource* GetconstBuff();

	worldViewpojCamera* GetWorldViewpojCamera();
};

namespace AliceMathF
{
	Matrix4 MakeWorldMatrix4(Transform& transform);

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	Vector3 GetWorldPosition(Transform& transform);
}