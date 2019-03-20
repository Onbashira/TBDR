#include "PMDModel.h"


K3D12::PMDModel::PMDModel()
{

}

K3D12::PMDModel::~PMDModel()
{

}

//HRESULT PMDModel::Draw(GraphicsCommandList * commandList)
//{
//	//ボーンとモーションデータ、変形方法、IK、による頂点変形ステージと、計算ステージを作る。
//
//	//モデルアップデート
//	{
//		//定数バッファをシェーダのレジスタにセット
//		//シーン定数　座標
//		ID3D12DescriptorHeap* sceneHeaps[] = { _transformBufferHeap.GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(sceneHeaps), sceneHeaps);
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(0, _transformBufferHeap.GetGPUHandle(0));
//		//インデックスを使用し、トライアングルリストを描画
//		commandList->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		commandList->GetCommandList()->IASetVertexBuffers(0, 1, &_vertexBuffer.GetView());
//		commandList->GetCommandList()->IASetIndexBuffer(&_indexBuffer.GetView());
//	}
//	UINT offset = 0;
//
//	for (size_t i = 0; i < _materials.size(); ++i)
//	{
//		// テクスチャとマテリアルを設定.
//
//		//テクスチャの設定
//
//
//		ID3D12DescriptorHeap* texHeaps[] = { _textureHeap.lock()->GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(texHeaps), texHeaps);
//		////ルートパラメータに合わせてる
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(3, _textureHeap.lock()->GetGPUHandle(static_cast<UINT>(i)));// コマンドリストに設定
//
//		//マテリアルの設定
//		ID3D12DescriptorHeap* mat_heaps[] = { _materialBufferHeaps.GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(mat_heaps), mat_heaps);
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(2, _materialBufferHeaps.GetGPUHandle(static_cast<UINT>(i)));					//マテリアル
//
//		// 描画コマンドを生成.
//
//		commandList->GetCommandList()->DrawIndexedInstanced(_materials[i].faceVertex, 1, offset, 0, 0);
//
//		offset += _materials[i].faceVertex;
//	}
//	return S_OK;
//}
//
//HRESULT PMDModel::Update(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
//{
//	//モデルアップデート
//	//{
//	//	static constexpr float PI = 3.14159265358979323846264338f;
//	//	static int cnt{};
//	//	++cnt;
//
//
//	//	//カメラの設定(あとで引数指定できるようにする)
//	//	_transformData.View = view;
//	//	_transformData.Proj = projection;
//
//	//	//オブジェクトの回転の設定
//	//	_transformData.World = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(static_cast<float>(cnt % 1800)) / 5.0f);
//
//	//	_transformBuffer.Update(&_transformData, sizeof(_transformData), 0);
//
//	//	//ボーンアップデート
//	//	/*std::fill(_boneTree.nodeMatrix.begin(), _boneTree.nodeMatrix.end(), DirectX::XMMatrixIdentity());
//	//	BendFromMotion();
//
//	//	_boneMatrixBuffer.Update(&_boneTree.nodeMatrix[0], _boneTree.nodeMatrix.size() *sizeof(DirectX::XMMATRIX),0);*/
//	//}
//	return S_OK;
//}
//
//HRESULT PMDModel::AnimationUpdate()
//{
//	return E_NOTIMPL;
//}
//
//void PMDModel::Discard()
//{
//	_vertexBuffer.Discard();
//	_indexBuffer.Discard();
//	_materialBuffer.Discard();
//
//	_vertices.clear();
//	_verticesIndex.clear();
//	_materials.clear();
//
//}
//
//std::vector<PMDMaterial>& PMDModel::GetMaterials()
//{
//	return _materials;
//}

