#include"3DMesh.h"
#include"WindowsApp.h"
#include"DefaultMaterial.h"

Mesh3D* Mesh3D::mesh = nullptr;

#pragma region パブリック

//線を描画する
void Mesh3D::DrawLine(AliceMathF::Vector3 start, AliceMathF::Vector3 end, AliceMathF::Vector4 color)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = lineBuff->vertexBuffer->GetView();

	assert(lineCount < lineMaxCount);

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{start.x, start.y, start.z}, {color.x, color.y, color.z, color.w}},
	  {{end.x, end.y, end.z}, {color.x, color.y, color.z, color.w}},
	};

	UINT indxcount = lineCount * lineVertexCount;

	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//頂点バッファへのデータ転送
	copy(vertices.begin(), vertices.end(), &lineBuff->vertMap[indxcount]);
	lineBuff->vertexBuffer->Update(vertices.data());

	// パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(DefaultMaterial::GetDefaultMaterial()->DEFAULT_LINE_MATERIAL[blendMode]->pipelineState->GetPipelineState());

	// ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(DefaultMaterial::GetDefaultMaterial()->DEFAULT_LINE_MATERIAL[blendMode]->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// 描画コマンド
	directX12Core->GetCommandList()->DrawInstanced(lineVertexCount, 1, indxcount, 0);

	lineCount++;
}

////三角形を描画する
//void Mesh3D::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, AliceMathF::Vector4 color, int fillFlag)
//{
//	if (fillFlag)
//	{
//		DrawTriangleFill(x1, y1, x2, y2, x3, y3, color);
//	}
//	else
//	{
//		DrawLine(x1, y1, x2, y2, color);
//		DrawLine(x2, y2, x3, y3, color);
//		DrawLine(x3, y3, x1, y1, color);
//
//	}
//}
//
////四角形を描画する
//void Mesh3D::DrawBox(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color, int fillFlag)
//{
//	if (fillFlag)
//	{
//		DrawBoxFill(x, y, radiusX, radiusY, angle, color);
//
//	}
//	else
//	{
//
//		DrawLine(x - radiusX, y - radiusY, x + radiusX, y - radiusY, color);//左上から右上
//		DrawLine(x - radiusX, y - radiusY, x - radiusX, y + radiusY, color);//左上から左下
//		DrawLine(x + radiusX, y - radiusY, x + radiusX, y + radiusY, color);//右上から右下
//		DrawLine(x - radiusX, y + radiusY, x + radiusX, y + radiusY, color);//左下から右下
//	}
//}
//
////楕円を描画する
//void Mesh3D::DrawEllipse(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color, int fillMode)
//{
//	constexpr UINT NumDiv = 64; // 分割数
//	float RadianPerDivision = AliceMathF::AX_2PIF / float(NumDiv); // 分割1単位の角度
//
//	//分割点分割数より1つ多くしておいて1周回るようにする
//	std::array<float, NumDiv + 1> pointsX{};
//	std::array<float, NumDiv + 1> pointsY{};
//	float angleCos = cos(angle);
//	float angleSin = sin(angle);
//	for (size_t i = 0; i < NumDiv; ++i)
//	{
//		float baseX = cos(RadianPerDivision * static_cast<float>(i)) * radiusX;
//		float baseY = sin(RadianPerDivision * static_cast<float>(i)) * radiusY;
//		pointsX[i] = float(baseX * angleCos - baseY * angleSin + x);
//		pointsY[i] = float(baseY * angleCos + baseX * angleSin + y);
//	}
//
//	//最後と最初は一緒
//	(*pointsX.rbegin()) = (*pointsX.begin());
//	(*pointsY.rbegin()) = (*pointsY.begin());
//
//	if (fillMode)
//	{
//		for (size_t index = 0; index < NumDiv; ++index)
//		{
//			DrawTriangleFill(x, y, pointsX[index], pointsY[index], pointsX[index + 1], pointsY[index + 1], color);
//		}
//	}
//	else
//	{
//		for (size_t index = 0; index < NumDiv; ++index)
//		{
//			DrawLine(pointsX[index], pointsY[index], pointsX[index + 1], pointsY[index + 1], color);
//		}
//	}
//}

//カウント初期化
void Mesh3D::DrawReset()
{
	triangleCount = 0;
	lineCount = 0;
	boxCount = 0;
}

//ブレンドモードを設定する
void Mesh3D::SetBlendMode(BlendMode mode)
{
	blendMode = (UINT)mode;
}

//色コードを取得する
AliceMathF::Vector4 Mesh3D::GetColor(int red, int blue, int green, int alpha)
{
	AliceMathF::Vector4 color{};

	color.x = static_cast<float>(red) / 255.0f;
	color.y = static_cast<float>(blue) / 255.0f;
	color.z = static_cast<float>(green) / 255.0f;
	color.w = static_cast<float>(alpha) / 255.0f;

	return color;
}

AliceMathF::Vector4 Mesh3D::GetColor(AliceMathF::Vector4 color)
{
	AliceMathF::Vector4 result{};

	result.x = static_cast<float>(color.x) / 255.0f;
	result.y = static_cast<float>(color.y) / 255.0f;
	result.z = static_cast<float>(color.z) / 255.0f;
	result.w = static_cast<float>(color.w) / 255.0f;

	return result;
}


//コンストラクタ
Mesh3D::Mesh3D()
{
	directX12Core = DirectX12Core::GetInstance();

	//三角形バッファ
	UINT vertexCount = triangleVertexCount * triangleMaxCount;
	UINT indexCount = triangleIndexCount * triangleMaxCount;
	triangleBuff = CreateBuff(vertexCount, indexCount);

	//線バッファ
	vertexCount = lineVertexCount * lineMaxCount;
	indexCount = lineIndexCount * lineMaxCount;
	lineBuff = CreateBuff(vertexCount, indexCount);

	//四角形バッファ
	vertexCount = boxVertexCount * boxMaxCount;
	indexCount = boxIndexCount * boxMaxCount;
	boxBuff = CreateBuff(vertexCount, indexCount);

	CreatConstBuff();

}

#pragma endregion

#pragma region プライベート

Mesh3D* Mesh3D::GetInstance()
{
	if (mesh == nullptr)
	{
		mesh = new Mesh3D();
	}

	return mesh;
}

void Mesh3D::Destroy()
{
	delete mesh;
}

void Mesh3D::SetCamera(Camera* camera_)
{
	camera = camera_;
}

//三角形を描画する(中身塗りつぶし)
void Mesh3D::DrawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, AliceMathF::Vector4 color)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = triangleBuff->vertexBuffer->GetView();

	assert(triangleCount < triangleMaxCount);

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{x1, y1, 0.0f}, {color.x, color.y, color.z, color.w}},
	  {{x2, y2, 0.0f}, {color.x, color.y, color.z, color.w}},
	  {{x3, y3, 0.0f}, {color.x, color.y, color.z, color.w}},
	};
	UINT vertexCount = triangleCount * triangleVertexCount;
	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//頂点バッファへのデータ転送
	copy(vertices.begin(), vertices.end(), &triangleBuff->vertMap[vertexCount]);
	triangleBuff->vertexBuffer->Update(triangleBuff->vertMap);

	// パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(DefaultMaterial::GetDefaultMaterial()->DEFAULT_MESH_MATERIAL[blendMode]->pipelineState->GetPipelineState());

	// ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(DefaultMaterial::GetDefaultMaterial()->DEFAULT_MESH_MATERIAL[blendMode]->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// 描画コマンド
	directX12Core->GetCommandList()->DrawInstanced(triangleVertexCount, 1, vertexCount, 0);

	// 使用カウント上昇
	triangleCount++;
}

//四角形を描画する(中身塗りつぶし)
void Mesh3D::DrawBoxFill(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = boxBuff->vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = boxBuff->indexBuffer->GetView();

	assert(boxCount < boxMaxCount);

	float left = -radiusX;
	float top = -radiusY;
	float right = radiusX;
	float bottom = radiusY;

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{left, bottom, 0.0f},{color.x, color.y, color.z, color.w}},//左下
	  {{left, top, 0.0f},{color.x, color.y, color.z, color.w}},//左上
	  {{right, top, 0.0f},{color.x, color.y, color.z, color.w}},//右上
	  {{right, bottom, 0.0f},{color.x, color.y, color.z, color.w}},//右下
	};

	//インデックス
	std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//回転
	for (auto& vertex : vertices)
	{
		//回転
		vertex.pos = {
		  vertex.pos.x * cosf(angle) + vertex.pos.y * -sinf(angle),
		  vertex.pos.x * sinf(angle) + vertex.pos.y * cosf(angle), vertex.pos.z };

		//平行移動
		vertex.pos.x += x;
		vertex.pos.y += y;
	}

	UINT vertexCount = boxCount * boxVertexCount;
	UINT indxCount = boxCount * boxIndexCount;

	//頂点バッファへのデータ転送
	std::copy(vertices.begin(), vertices.end(), &boxBuff->vertMap[vertexCount]);
	boxBuff->vertexBuffer->Update(boxBuff->vertMap);

	//インデックスバッファへのデータ転送
	std::copy(indices.begin(), indices.end(), &boxBuff->indexMap[indxCount]);
	boxBuff->indexBuffer->Update(boxBuff->indexMap);

	//パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(DefaultMaterial::GetDefaultMaterial()->DEFAULT_MESH_MATERIAL[blendMode]->pipelineState->GetPipelineState());

	//ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(DefaultMaterial::GetDefaultMaterial()->DEFAULT_MESH_MATERIAL[blendMode]->rootSignature->GetRootSignature());

	//プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	//頂点バッファの設定
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファの設定
	directX12Core->GetCommandList()->IASetIndexBuffer(&ibView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	//描画コマンド
	directX12Core->GetCommandList()->DrawIndexedInstanced(boxIndexCount, 1, static_cast<UINT>(indxCount), static_cast<INT>(vertexCount), 0);

	//使用カウント上昇
	boxCount++;
}



//定数バッファ生成(2D座標変換行列)
void Mesh3D::CreatConstBuff()
{
	//定数バッファ生成
	constBuffTransform = std::make_unique<ConstantBuffer>();
	constBuffTransform->Create(sizeof(ConstBufferDataTransform));

	AliceMathF::MakeOrthogonalL(
		0.0f, static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().width),
		static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().height), 0.0f,
		0.0f, 1.0f, projectionMat);

	constBuffTransform->Update(&projectionMat);

}



///<summary>
///バッファ作成
///</summary>
///<param name="vertexCount">頂点数</param>
///<param name="indexCount">インデックス数</param>
///<returns>バッファ</returns>
std::unique_ptr <Buff> Mesh3D::CreateBuff(UINT vertexCount, UINT indexCount)
{
	std::unique_ptr <Buff> buff = std::make_unique<Buff>();

	if (vertexCount > 0)
	{

		buff->vertexBuffer = std::make_unique<VertexBuffer>();
		buff->vertexBuffer->Create(vertexCount, sizeof(PosColor));

		buff->vertMap = static_cast<PosColor*>(malloc(vertexCount * sizeof(PosColor)));

	}

	if (indexCount > 0)
	{
		buff->indexBuffer = std::make_unique<IndexBuffer>();
		buff->indexBuffer->Create(indexCount);

		buff->indexMap = static_cast<uint32_t*>(malloc(indexCount * sizeof(uint32_t)));
	}

	return buff;
}

#pragma endregion
