
#include <memory>
#include <vector>
#include "PMXLoader.h"
#include "PMXModel.h"
#include "../../Util/Utility.h"


enum class PMXWeightDeformType {
	BDEF1,
	BDEF2,
	BDEF4,
	SDEF,
	QDEF
};

enum  class PMXMaterialFlags {
	NO_CULL,
	GROUND_SHADOW,
	DRAW_SHADOW,
	RECEIVE_SHADOW,
	HAS_EDGE,
	VERTEX_COLOR,
	POINT_DRAWING,
	LINE_DRAWING
};

enum class PMXBoneFlags {
	INDEXED_TAIL_POSITION,
	ROTATABLE,
	TRANSLATABLE,
	IS_VISIBLE,
	ENABLE,
	IK,
	INHERIT_ROTATION,
	INHERIT_TRANSLATION,
	FIXED_AXIS,
	LOCAL_COORDINATE,
	PHISICS_AFTER_DEFORM,
	EXTERNAL_PARENT_DEFORM

};

bool CheckBits(unsigned short bit, unsigned short checkbit)
{
	return ((bit & checkbit) == checkbit);
}

std::shared_ptr<K3D12::PMXModel> K3D12::PMXLoader::Load(std::string modelPath)
{
	if (modelPath.empty()) {
		return nullptr;
	}
	FILE* fp = nullptr;
	fopen_s(&fp, modelPath.c_str(), "rb");
	if (fp == NULL) {
		fclose(fp);
		return nullptr;
	}

	std::shared_ptr<K3D12::PMXModel> model = std::make_shared<K3D12::PMXModel>();
	model->modelPath = modelPath;
	//ヘッダ読み込み

	LoadInfo(fp, model.get());
	//頂点情報読み込み

	LoadVertex(fp, model.get());
	//面頂点ロード
	LoadSurface(fp, model.get());
	//テクスチャパスロード
	LoadTexture(fp, model.get());
	//マテリアルロード
	LoadMaterials(fp, model.get());
	//ボーン読み込み
	LoadBone(fp, model.get());
	//もーふ読み込み
	LoadMorph(fp, model.get());
	//表示枠
	LoadDisplay(fp, model.get());
	//剛体
	LoadRigidBody(fp, model.get());
	//JOINT
	LoadJoint(fp, model.get());
	fclose(fp);

	return model;
}

K3D12::PMXLoader::PMXLoader()
{
}


K3D12::PMXLoader::~PMXLoader()
{
}

void K3D12::PMXLoader::LoadInfo(FILE * fp, PMXModel * model)
{
	fread(&model->header.signature[0], sizeof(int), 1, fp);
	fread(&model->header.version, sizeof(float), 1, fp);
	fread(&model->header.globalCount, sizeof(unsigned char), 1, fp);

	model->header.globals.resize(model->header.globalCount);

	fread(&model->header.globals[0], sizeof(unsigned char), model->header.globals.size(), fp);

	unsigned int byteLength = 0;

	fread(&byteLength, sizeof(int), 1, fp);
	model->header.modelName.resize(4 + byteLength);
	fread(&model->header.modelName[0], sizeof(unsigned char), byteLength, fp);

	fread(&byteLength, sizeof(int), 1, fp);
	model->header.modelNameEnglish.resize(4 + byteLength);
	fread(&model->header.modelNameEnglish[0], sizeof(unsigned char), byteLength, fp);

	fread(&byteLength, sizeof(int), 1, fp);
	model->header.comment.resize(4 + byteLength);
	fread(&model->header.comment[0], sizeof(unsigned char), byteLength, fp);

	fread(&byteLength, sizeof(int), 1, fp);
	model->header.commentEnglish.resize(4 + byteLength);
	fread(&model->header.commentEnglish[0], sizeof(unsigned char), byteLength, fp);

}

void K3D12::PMXLoader::LoadVertex(FILE * fp, PMXModel * model)
{
	unsigned int vertexCount = 0;
	fread(&vertexCount, sizeof(unsigned int), 1, fp);

	//resize
	model->vertcies.resize(vertexCount);

	//loop
	for (auto& vertex : model->vertcies) {
		//fread(&vertex.position, sizeof(Vector3), 1, fp);
		Util::FileStream::FileRead<Vector3>(&vertex.position, fp);
		Util::FileStream::FileRead<Vector3>(&vertex.normal, fp);
		Util::FileStream::FileRead<Vector2>(&vertex.uv, fp);
		if (model->header.globals[static_cast<int>(PMXGlobalTypes::ADDITIONAL_UV_COUNT)] > 0) {
			vertex.additionalUV.resize(model->header.globals[static_cast<int>(PMXGlobalTypes::ADDITIONAL_UV_COUNT)]);
			for (auto& additionalUV : vertex.additionalUV) {
				Util::FileStream::FileRead<Vector4>(&additionalUV, fp);
			}
		}
		Util::FileStream::FileRead<unsigned char>(&vertex.weightDeformType, fp);

		//く　そ　な　が　分　岐
		//デフォームタイプで分岐
		//後で各処理を関数化した関数へのポインタを関数ポインタ配列に持たせて短くする
		switch (static_cast<PMXWeightDeformType>(vertex.weightDeformType))
		{
		case PMXWeightDeformType::BDEF1:
			Util::FileStream::FileRead<>(&(vertex.weightDeform.bdef1.boneIndex), fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			break;
		case PMXWeightDeformType::BDEF2:
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef2.boneIndex01, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef2.boneIndex02, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&vertex.weightDeform.bdef2.boneWeight01, fp);
			vertex.weightDeform.bdef2.boneWeight02 = 1.0f - vertex.weightDeform.bdef2.boneWeight01;
			break;
		case PMXWeightDeformType::BDEF4:
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef4.boneIndex01, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef4.boneIndex02, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef4.boneIndex03, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.bdef4.boneIndex04, fp, static_cast<unsigned int>(model->header.globals[static_cast<unsigned int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&vertex.weightDeform.bdef4.boneWeight01, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.bdef4.boneWeight02, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.bdef4.boneWeight03, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.bdef4.boneWeight04, fp);
			break;
		case PMXWeightDeformType::SDEF:
			Util::FileStream::FileRead<>(&vertex.weightDeform.sdef.boneIndex01, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.sdef.boneIndex02, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&vertex.weightDeform.sdef.boneWeight01, fp);
			vertex.weightDeform.sdef.boneWeight02 = 1.0f - vertex.weightDeform.sdef.boneWeight01;
			Util::FileStream::FileRead<Vector3>(&vertex.weightDeform.sdef.c, fp);
			Util::FileStream::FileRead<Vector3>(&vertex.weightDeform.sdef.r0, fp);
			Util::FileStream::FileRead<Vector3>(&vertex.weightDeform.sdef.r1, fp);
			break;
		case PMXWeightDeformType::QDEF:
			Util::FileStream::FileRead<>(&vertex.weightDeform.qdef.boneIndex01, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.qdef.boneIndex02, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.qdef.boneIndex03, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<>(&vertex.weightDeform.qdef.boneIndex04, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&vertex.weightDeform.qdef.boneWeight01, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.qdef.boneWeight02, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.qdef.boneWeight03, fp);
			Util::FileStream::FileRead<float>(&vertex.weightDeform.qdef.boneWeight04, fp);
			break;
		default:
			break;
		}

		Util::FileStream::FileRead<float>(&vertex.edgeScale, fp);
	}
}

void K3D12::PMXLoader::LoadSurface(FILE * fp, PMXModel * model)
{

	int surfaceCount = 0;
	Util::FileStream::FileRead<unsigned int>(&surfaceCount, fp);
	model->surfaces.surfaceIndex.resize(surfaceCount);
	for (auto& surface : model->surfaces.surfaceIndex) {
		Util::FileStream::FileRead<>(&surface, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::VERTEX_INDEX_SIZE)]));
	}
}


void K3D12::PMXLoader::LoadTexture(FILE * fp, PMXModel * model)
{
	int textureCount = 0;
	Util::FileStream::FileRead<int>(&textureCount, fp);
	model->paths.path.resize(textureCount);
	for (auto& path : model->paths.path) {
		unsigned int textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		if (textLength == 0) {
			continue;
		}
		path.resize(4 + textLength);
		Util::FileStream::FileRead<>(&path[0], fp, sizeof(unsigned char), textLength);
	}
}

void K3D12::PMXLoader::LoadMaterials(FILE * fp, PMXModel * model)
{
	unsigned int materialCount = 0;
	Util::FileStream::FileRead<unsigned int>(&materialCount, fp);
	model->materials.resize(materialCount);
	for (auto& material : model->materials) {
		unsigned int textLength = 0;

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		material.nameLocal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&material.nameLocal[0], fp, sizeof(unsigned char), textLength);

		textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		material.nameUniversal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&material.nameUniversal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<Vector4>(&material.diffuseColor, fp);
		Util::FileStream::FileRead<Vector3>(&material.specularColor, fp);
		Util::FileStream::FileRead<float>(&material.specularStrength, fp);
		Util::FileStream::FileRead<Vector3>(&material.ambientColor, fp);

		Util::FileStream::FileRead<unsigned char>(&material.drawingFlags, fp);

		Util::FileStream::FileRead<Vector4>(&material.edgeColor, fp);
		Util::FileStream::FileRead<float>(&material.edgwScale, fp);

		switch (static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]))
		{
		case 1:
			Util::FileStream::FileRead<>(&material.textureIndex, fp, static_cast<char>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//１Byte読み込み時の不正頂点を取得したならば
			if (material.textureIndex == UCHAR_MAX) {
				//代わりに不正情報を代入
				material.textureIndex = -1;
			}
			break;
		case 2:
			Util::FileStream::FileRead<>(&material.textureIndex, fp, static_cast<short>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//１Byte読み込み時の不正頂点を取得したならば
			if (material.textureIndex == USHRT_MAX) {
				//代わりに不正情報を代入
				material.textureIndex = -1;
			}
			break;
		case 4:
			Util::FileStream::FileRead<>(&material.textureIndex, fp, static_cast<int>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//１Byte読み込み時の不正頂点を取得したならば
			if (material.textureIndex == INT_MAX) {
				//代わりに不正情報を代入
				material.textureIndex = -1;
			}
			break;
		default:
			break;
		};

		switch (static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]))
		{
		case 1:
			Util::FileStream::FileRead<>(&material.environmentTextureIndex, fp, static_cast<char>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//１Byte読み込み時の不正頂点を取得したならば
			if (material.environmentTextureIndex == UCHAR_MAX) {
				//代わりに不正情報を代入
				material.environmentTextureIndex = -1;
			}
			break;
		case 2:
			Util::FileStream::FileRead<>(&material.environmentTextureIndex, fp, static_cast<short>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//2Byte読み込み時の不正頂点を取得したならば
			if (material.environmentTextureIndex == USHRT_MAX) {
				//代わりに不正情報を代入
				material.environmentTextureIndex = -1;
			}
			break;
		case 4:
			Util::FileStream::FileRead<>(&material.environmentTextureIndex, fp, static_cast<int>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
			//4Byte読み込み時の不正頂点を取得したならば
			if (material.environmentTextureIndex == INT_MAX) {
				//代わりに不正情報を代入
				material.environmentTextureIndex = -1;
			}
			break;
		default:
			break;
		};

		Util::FileStream::FileRead<unsigned char>(&material.environmentBlendFlags, fp);
		Util::FileStream::FileRead<unsigned char>(&material.toonReference, fp);
		switch (int(material.toonReference))
		{
		case 0:
			switch (static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]))
			{
			case 1:
				Util::FileStream::FileRead<>(&material.toonValue.individualToon, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
				//１Byte読み込み時の不正頂点を取得したならば
				if (material.toonValue.individualToon == UCHAR_MAX) {
					//代わりに不正情報を代入
					material.toonValue.individualToon = -1;
				}
				break;
			case 2:
				Util::FileStream::FileRead<>(&material.toonValue.individualToon, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
				//2Byte読み込み時の不正頂点を取得したならば
				if (material.toonValue.individualToon == USHRT_MAX) {
					//代わりに不正情報を代入
					material.toonValue.individualToon = -1;
				}
				break;
			case 4:
				Util::FileStream::FileRead<>(&material.toonValue.individualToon, fp, static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::TEXTURE_INDEX_SIZE)]));
				//4Byte読み込み時の不正頂点を取得したならば
				if (material.toonValue.individualToon == INT_MAX) {
					//代わりに不正情報を代入
					material.toonValue.individualToon = -1;
				}
				break;
			default:
				break;
			};
			break;
		case 1:
			Util::FileStream::FileRead<unsigned char>(&material.toonValue.unionToon, fp);
			break;
		default:
			break;
		}

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		material.metaData.resize(4 + textLength);
		Util::FileStream::FileRead<>(&material.metaData[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned int>(&material.surfaceCount, fp);
		material.surfaceCount;
	}
}

void K3D12::PMXLoader::LoadBone(FILE * fp, PMXModel * model)
{
	unsigned int boneCount = 0;

	Util::FileStream::FileRead<int>(&boneCount, fp);
	model->bones.resize(boneCount);
	int testLoadCount = 0;
	for (auto& bone : model->bones) {
		unsigned int textLength = 0;
		testLoadCount++;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		bone.boneNameLocal.resize(textLength);
		Util::FileStream::FileRead<>(&bone.boneNameLocal[0], fp, sizeof(unsigned char), textLength);

		textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		bone.boneNameUniversal.resize(textLength);
		Util::FileStream::FileRead<>(&bone.boneNameUniversal[0], fp, sizeof(unsigned char), textLength);


		Util::FileStream::FileRead<Vector3>(&bone.bonePosition, fp);
		Util::FileStream::FileRead<>(&bone.parentBoneIndex, fp,
			static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
		Util::FileStream::FileRead<int>(&bone.deformationHierarchyLayer, fp);
		Util::FileStream::FileRead<USHORT>(&bone.boneFlags, fp);

		if (CheckBits(bone.boneFlags, 0x0001))
		{
			Util::FileStream::FileRead<>(&bone.connectBoneIndex, fp,
				static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
		}
		else
		{
			Util::FileStream::FileRead<Vector3>(&bone.tailPosition, fp);
		}

		if (CheckBits(bone.boneFlags, 0x0100))
		{
			Util::FileStream::FileRead<>(&bone.inheritBone.parentIndex, fp,
				static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&bone.inheritBone.parentInfluence, fp);
		}

		if (CheckBits(bone.boneFlags, 0x0200))
		{
			Util::FileStream::FileRead<>(&bone.inheritBone.parentIndex, fp,
				static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			Util::FileStream::FileRead<float>(&bone.inheritBone.parentInfluence, fp);
		}

		if (CheckBits(bone.boneFlags, 0x0400))
		{
			Util::FileStream::FileRead<Vector3>(&bone.fixedAxis.axisDirection, fp);

		}

		if (CheckBits(bone.boneFlags, 0x0800))
		{
			Util::FileStream::FileRead<Vector3>(&bone.boneAxis.right, fp);
			Util::FileStream::FileRead<Vector3>(&bone.boneAxis.up, fp);
		}

		if (CheckBits(bone.boneFlags, 0x2000))
		{
			Util::FileStream::FileRead<int>(&bone.externalParent.index, fp);
		}
		//これがIKであるならば
		if (CheckBits(bone.boneFlags, 0x0020))
		{
			Util::FileStream::FileRead<>(&bone.ikData.targetIndex, fp,
				static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
			//fseek(fp, 1, SEEK_CUR);
			Util::FileStream::FileRead<int>(&bone.ikData.loopCount, fp);
			Util::FileStream::FileRead<float>(&bone.ikData.limitRadian, fp);
			Util::FileStream::FileRead<int>(&bone.ikData.linkCount, fp);

			bone.ikData.ikLinks.resize(bone.ikData.linkCount);
			for (auto& ik : bone.ikData.ikLinks)
			{
				Util::FileStream::FileRead<>(&ik.boneIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
				Util::FileStream::FileRead<unsigned char>(&ik.hasLimit, fp);
				if (ik.hasLimit == 1)
				{
					Util::FileStream::FileRead<Vector3>(&ik.limits.minimamAngle, fp);
					Util::FileStream::FileRead<Vector3>(&ik.limits.maximumAngle, fp);
				}
			}
		}

	}
}

void K3D12::PMXLoader::LoadMorph(FILE * fp, PMXModel * model)
{
	unsigned int morphCount;
	Util::FileStream::FileRead<unsigned int>(&morphCount, fp);
	model->morphDatas.resize(morphCount);
	for (auto& data : model->morphDatas)
	{
		unsigned int textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		data.nameLocal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&data.nameLocal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		data.nameUniversal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&data.nameUniversal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<UCHAR>(&data.controllPanel, fp);
		Util::FileStream::FileRead<UCHAR>(&data.morphType, fp);
		Util::FileStream::FileRead<unsigned int>(&data.morphCount, fp);
		data.morphs.resize(data.morphCount);
		for (auto& morph : data.morphs)
		{
			switch (static_cast<MorphType>(data.morphType))
			{
			case MorphType::Vertex:
				Util::FileStream::FileRead<>(&morph.vertexMorph.vertexIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::VERTEX_INDEX_SIZE)]));
				Util::FileStream::FileRead<Vector3>(&morph.vertexMorph.translation, fp);
				break;
			case MorphType::UV:
				Util::FileStream::FileRead<>(&morph.uvMorph.vertexIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::VERTEX_INDEX_SIZE)]));
				Util::FileStream::FileRead<Vector2>(&morph.uvMorph.uvOffset, fp);
				break;
			case MorphType::UVadd1:
				Util::FileStream::FileRead<>(&morph.uvMorph.vertexIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::VERTEX_INDEX_SIZE)]));
				Util::FileStream::FileRead<Vector3>(&morph.uvMorph.uvOffset, fp);
				break;
			case MorphType::UVadd2:
				Util::FileStream::FileRead<>(&morph.uvMorph.vertexIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::VERTEX_INDEX_SIZE)]));
				Util::FileStream::FileRead<Vector4>(&morph.uvMorph.uvOffset, fp);
				break;
			case MorphType::Bone:
				Util::FileStream::FileRead<>(&morph.boneMorph.boneIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
				Util::FileStream::FileRead<Vector3>(&morph.boneMorph.translation, fp);
				Util::FileStream::FileRead<Vector4>(&morph.boneMorph.rotation, fp);

				break;
			case MorphType::Material:
				Util::FileStream::FileRead<>(&morph.materialMorph.materialIndex, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::MATERIAL_INDEX_SIZE)]));
				Util::FileStream::FileRead<UCHAR>(&morph.materialMorph.offsetcalType, fp);
				Util::FileStream::FileRead<Vector4>(&morph.materialMorph.diffuse, fp);
				Util::FileStream::FileRead<Vector3>(&morph.materialMorph.specular, fp);
				Util::FileStream::FileRead<float>(&morph.materialMorph.specularity, fp);
				Util::FileStream::FileRead<Vector3>(&morph.materialMorph.ambient, fp);
				Util::FileStream::FileRead<Vector4>(&morph.materialMorph.edgeColor, fp);
				Util::FileStream::FileRead<float>(&morph.materialMorph.edgeSize, fp);
				Util::FileStream::FileRead<Vector4>(&morph.materialMorph.textureTint, fp);
				Util::FileStream::FileRead<Vector4>(&morph.materialMorph.environmentTint, fp);
				Util::FileStream::FileRead<Vector4>(&morph.materialMorph.toonTint, fp);
				break;
			case MorphType::Group:
				Util::FileStream::FileRead<>(&morph.groupMorph.index, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::MORPH_INDEX_SIZE)]));
				Util::FileStream::FileRead<float>(&morph.groupMorph.influence, fp);
				break;
			default:
				break;
			}
		}
	}
}

void K3D12::PMXLoader::LoadDisplay(FILE * fp, PMXModel * model)
{
	unsigned int displayCount = 0;
	Util::FileStream::FileRead<unsigned int>(&displayCount, fp);
	model->displayInfo.resize(displayCount);
	for (auto& data : model->displayInfo)
	{
		unsigned int textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		data.nameLocal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&data.nameLocal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		data.nameUniversal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&data.nameUniversal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned char>(&data.flags, fp);
		Util::FileStream::FileRead<unsigned int>(&data.count, fp);
		data.datas.resize(data.count);
		for (auto& display : data.datas)
		{
			Util::FileStream::FileRead<unsigned char>(&display.type, fp);
			switch (display.type)
			{
			case 0://ボーン
				Util::FileStream::FileRead<>(&display.index, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));
				break;
			case 1://モーフ
				Util::FileStream::FileRead<>(&display.index, fp,
					static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::MORPH_INDEX_SIZE)]));

				break;
			default:
				break;
			}
		}
	}
}

void K3D12::PMXLoader::LoadRigidBody(FILE * fp, PMXModel * model)
{
	unsigned int rigidCount = 0;
	Util::FileStream::FileRead<unsigned int>(&rigidCount, fp);
	model->rigidBodies.resize(rigidCount);

	for (auto& rigid : model->rigidBodies)
	{
		unsigned int textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		rigid.nameLocal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&rigid.nameLocal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		rigid.nameUniversal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&rigid.nameUniversal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<>(&rigid.boneIndex, fp,
			static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::BONE_INDEX_SIZE)]));

		Util::FileStream::FileRead<unsigned char>(&rigid.groupID, fp);
		Util::FileStream::FileRead<short>(&rigid.nonCollisionGroup, fp);

		Util::FileStream::FileRead<unsigned char>(&rigid.shape, fp);
		Util::FileStream::FileRead<Vector3>(&rigid.size, fp);

		Util::FileStream::FileRead<Vector3>(&rigid.pos, fp);
		Util::FileStream::FileRead<Vector3>(&rigid.rotation, fp);

		Util::FileStream::FileRead<float>(&rigid.mass, fp);
		Util::FileStream::FileRead<float>(&rigid.moveAttenuation, fp);
		Util::FileStream::FileRead<float>(&rigid.rotationDamping, fp);
		Util::FileStream::FileRead<float>(&rigid.boundPower, fp);
		Util::FileStream::FileRead<float>(&rigid.frictionPower, fp);

		Util::FileStream::FileRead<unsigned char>(&rigid.phisicsMode, fp);
	}
}

void K3D12::PMXLoader::LoadJoint(FILE * fp, PMXModel * model)
{
	unsigned int jointCount = 0;
	Util::FileStream::FileRead<unsigned int>(&jointCount, fp);
	model->joints.resize(jointCount);
	for (auto& joint : model->joints)
	{
		unsigned int textLength = 0;
		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		joint.nameLocal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&joint.nameLocal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned int>(&textLength, fp);
		joint.nameUniversal.resize(4 + textLength);
		Util::FileStream::FileRead<>(&joint.nameUniversal[0], fp, sizeof(unsigned char), textLength);

		Util::FileStream::FileRead<unsigned char>(&joint.type, fp);
		Util::FileStream::FileRead<>(&joint.spring.rigidAIndex, fp,
			static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::RIGIDBODY_IDNEX_SIZE)]));
		Util::FileStream::FileRead<>(&joint.spring.rigidBIndex, fp,
			static_cast<size_t>(model->header.globals[static_cast<int>(PMXGlobalTypes::RIGIDBODY_IDNEX_SIZE)]));
		Util::FileStream::FileRead<Vector3>(&joint.spring.pos, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.rotation, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.posMinimum, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.posMaximum, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.rotationMinimum, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.rotationMaximum, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.positionSpring, fp);
		Util::FileStream::FileRead<Vector3>(&joint.spring.rotationSpring, fp);

	}
}
