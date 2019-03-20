#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>
#include "../../Util/Math.h"


namespace K3D12 {

	class GraphicsCommandList;

#pragma pack(push,1)
	//ヘッダーファイル
	struct PMDHeader {
		char signature[3];
		float version;
		char name[20];
		char coment[0x100];
	};
#pragma pack(pop)

#pragma pack(push,1)
	//頂点情報
	struct PMDVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
		unsigned short boneIndex[2];
		unsigned char boneWeight;
		unsigned char edgeFlag;
	};
#pragma pack(pop)

#pragma pack(push,1)
	//マテリアル情報
	struct PMDMaterial {
		Vector3 diffuse;
		float alpha;
		float power;
		Vector3 specular;
		Vector3 emissive;
		unsigned char toonIndex;
		unsigned char visualFlag;
		unsigned int faceVertex;
		char textureName[20];
	};
#pragma pack(pop)

#pragma pack(push,1)
	//ボーン情報

	struct PMDBone {
		char boneName[20];
		unsigned short parentIndex;
		unsigned short tailIndex;
		char boneType;
		unsigned short IKBoneIndex;
		Vector3 boneHeadPosition;
	};
#pragma pack(pop)

	struct Bone {
		Vector3 head;
		Vector3 tail;
	};

	struct BoneNode {
		std::string				name;
		unsigned short					parentIndex;				//親へのインデクサ
		unsigned short					ownIndex;
		std::vector<BoneNode>	children;					//子へのアクセスポインタ
	};

	struct BoneTree {

		UINT							boneNum;
		std::map<std::string, int>		boneMap;
		std::map<std::string, BoneNode*>nodeAccessor;
		BoneNode						root;
		std::vector<Matrix>	nodeMatrix;
	};

	//IK情報
#pragma pack(push,1)
	struct PMDIKData {
		unsigned short boneIndex;
		unsigned short targetboneIndex;
		unsigned char chainLength;
		unsigned short recursiveCount;
		float controlWeight;
		std::vector<unsigned short> childBone;
	};
#pragma pack(pop)


	//もーふ情報
#pragma pack(push,1)
	struct PMDMorph {
	};
#pragma pack(pop)


	class PMDModel
	{
		//-------------------------------------------
		friend class PMDLoader;
		//-------------------------------------------

	private:
	public:
		std::string									modelPath;
		PMDHeader									header;
		std::vector<PMDVertex>						vertices;
		std::vector<unsigned short>							verticesIndex;
		std::vector<PMDMaterial>					materials;
		std::vector<PMDBone>						bonesInfo;
		std::vector<PMDIKData>						ikData;
		std::vector<Bone>							bones;
	private:
	public:
		PMDModel();
		virtual~PMDModel();
	};
}