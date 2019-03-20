#pragma once
#include <vector>
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../Resource/ConstantBuffer.h"
#include "../../DescriptorHeap/DescriptorHeap.h"
#include "../../Util/Math.h"

//シェーダーに渡す頂点情報
namespace K3D12 {
	struct ModelTranportVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
	};

	//シェーダー渡すための古典的シェーディングマテリアル
	struct ModelTrasnpotingMaterial {
		Vector3 diffuse;
		float	diffusePower;
		Vector3 specular;
		float	specularPower;

	};

	//ボーンツリーのノードを定義
	struct ModelBoneNode {
		//自身のインデックス
		unsigned short index;
		//親のインデックス
		unsigned short parentIndex;
		//ボーンの位置
		Vector3 pos;
		//
		Vector3 parentPos;
		///子供
		std::vector<ModelBoneNode> children;
	};

	struct ModelBoneTree {
		ModelBoneNode* rootNode;

	};

	//計算用ボーンツリーを含むボーンの情報群
	struct ModelBoneInformation {
		///ボーン総数
		unsigned int boneNum;


	};

	//シェーダーに渡すためのテクスチャデータ
	struct ModelTextureData {

	};

	//

	//一般化したモデルのリソース
	class ModelResource
	{
	private:
		//変更しない
		IndexBuffer	_indexBuffer;
		//変更される可能性がある（初期ボーンの情報が必要）
		VertexBuffer _vertexBuffer;
		//変更されないはず。するかも
		ConstantBuffer _materialBuffer;
		//ボーン情報はシェーダに渡さない？ので、初期ポーズのボーン配列を持っておく（ツリー構造）
		std::vector<int>	_indexList;
		std::vector<Vertex3D> _vertexes;
		// 

	public:
	private:
	public:
		ModelResource();
		~ModelResource();
	};

}