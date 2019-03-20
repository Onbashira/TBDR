#pragma once

#include <memory>
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"





namespace K3D12 {

	class IndexBuffer;
	class VertexBuffer;
	class ConstantBuffer;
	class ShaderResource;

	//メッシュに必要な情報を持ったクラス
	class ModelMesh : public K3D12::GameObject,
		public K3D12::DrawableComponent
	{
	private:

	protected:
		std::unique_ptr<K3D12::IndexBuffer>	_indexBuffer;	//モデルの頂点インデックス
		std::unique_ptr<K3D12::VertexBuffer>	_vertexBuffer;	//モデルの頂点バッファ
		std::unique_ptr<K3D12::DescriptorHeap> _heap;			//デスクリプタヒープ　（変形情報 + マテリアル数 + テクスチャ数)

		unsigned int					_transformStartPoint;	//変形情報デスクリプタの開始位置		(maybe 0)
		unsigned int					_materialStartPoint;		//マテリアルデスクリプタの開始位置	(maybe 0+1 ~ x)
		unsigned int					_textureStartPoint;		//テクスチャデスクリプタの開始位置		(maybe x+1 ~ y)

		K3D12::ConstantBuffer					_materialBuffer;	//マテリアル情報のバッファ
		std::vector<std::weak_ptr<K3D12::ShaderResource>> _textureResource; //テクスチャリソースへの参照
	public:

	private:

	protected:

	public:

		void BindingVertexBuffer(K3D12::GraphicsCommandList* list);
		void BindingIndexBuffer(K3D12::GraphicsCommandList* list);
		void BindingDescriptorHeaps(K3D12::GraphicsCommandList* list);
		void SetTextureCount(unsigned int textureCount = 0);
		void SetMaterialCount(unsigned int materialCount = 1);
		ModelMesh();
		~ModelMesh();
	};
}

