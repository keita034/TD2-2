#pragma once
#include"ErrorException.h"

/// <summary>
/// 定数バッファ
/// </summary>
class ConstantBuffer
{

private:

	// 定数バッファ生成に成功したか
	bool isValid = false;
	char PADING[7] = {};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;

	// 定数バッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView = {};

	void* bufferMappedPtr = nullptr;

	size_t bufferSize;

public:
	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	void Create(size_t size);

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid();

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;

	/// <summary>
	/// 定数バッファビューを返す
	/// </summary>
	/// <returns>定数バッファビュー</returns>
	D3D12_CONSTANT_BUFFER_VIEW_DESC GetViewDesc();

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource();

	/// <summary>
	/// マップ用ポインタを取得
	/// </summary>
	/// <returns></returns>
	void* GetPtr();

	~ConstantBuffer() = default;
	ConstantBuffer() = default;


private:

	ConstantBuffer(const ConstantBuffer&) = delete;
	void operator = (const ConstantBuffer&) = delete;

};