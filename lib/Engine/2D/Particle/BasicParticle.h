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

	//初期化
	virtual void Initialize()override;

	///<summary>
	///更新
	///</summary>
	virtual void Update()override;

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="scale">{開始時スケール,終了時スケール}</param>
	/// <param name="rotation">{開始時回転角,終了時回転角}</param>
	/// <param name="sColor">開始カラー</param>
	/// <param name="eColor">終了カラー</param>
	virtual void Add(
		UINT life, AliceMathF::Vector3& position, AliceMathF::Vector3& velocity,
		AliceMathF::Vector3& accel, AliceMathF::Vector2& scale, AliceMathF::Vector2& rotation
		, AliceMathF::Vector4& sColor, AliceMathF::Vector4& eColor)override;


	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(const TextureData& texture, Camera* camera, Material* material = nullptr)override;

private:

	//コピーコンストラクタ・代入演算子削除
	BasicParticle& operator=(const BasicParticle&) = delete;
	BasicParticle(const BasicParticle&) = delete;
};

