#include "PMDModel.h"
#include "PMDLoader.h"
#include "../../TextureComponent/TextureLoader.h"
#include "../../TextureComponent/TextureManager.h"

K3D12::PMDLoader::PMDLoader()
{
}

K3D12::PMDLoader::~PMDLoader()
{
}


void K3D12::PMDLoader::LoadVertex(FILE * fp, PMDModel * model)
{
	unsigned int vertexCount = 0;
	fread(&vertexCount, sizeof(vertexCount), 1, fp);

	model->vertices.resize(vertexCount);

	for (auto& vertex : model->vertices) {
		fread(&vertex, sizeof(PMDVertex), 1, fp);
	}
}

void K3D12::PMDLoader::LoadIndexList(FILE * fp, PMDModel * model)
{
	unsigned int indexCount = 0;
	fread(&indexCount, sizeof(indexCount), 1, fp);

	model->verticesIndex.resize(indexCount);

	for (auto& index : model->verticesIndex) {
		fread(&index, sizeof(unsigned int), 1, fp);
	}
}

void K3D12::PMDLoader::LoadMaterial(FILE * fp, PMDModel * model)
{
	unsigned int materialCount = 0;
	fread(&materialCount, sizeof(unsigned int), 1, fp);

	model->materials.resize(materialCount);
	for (auto& material : model->materials) {
		fread(&material, sizeof(PMDMaterial), 1, fp);
	}
}

void K3D12::PMDLoader::LoadSkelton(FILE * fp, PMDModel * model)
{
	unsigned short		boneCount = 0;
	fread(&boneCount, sizeof(unsigned int), 1, fp);

	model->bonesInfo.resize(boneCount);
	for (auto& bone : model->bonesInfo) {
		fread(&bone, sizeof(PMDBone), 1, fp);
	}
	model->bones.resize(boneCount);
	for (unsigned int i = 0; i < boneCount; ++i) {

		model->bones[i].head = model->bonesInfo[i].boneHeadPosition;
		model->bones[i].tail = model->bonesInfo[model->bonesInfo[i].tailIndex].boneHeadPosition;
	}
}

void K3D12::PMDLoader::LoadIK(FILE * fp, PMDModel * model)
{
	unsigned short ikDataCount = 0;
	unsigned int	loadSize = sizeof(unsigned int) * 2 + sizeof(unsigned int) + sizeof(unsigned char) + sizeof(float);
	fread(&ikDataCount, sizeof(unsigned short), 1, fp);

	model->ikData.resize(ikDataCount);

	for (auto& ikData : model->ikData) {
		fread(&ikData, loadSize, 1, fp);
		ikData.childBone.resize(ikData.chainLength);

		for (auto& child : ikData.childBone) {
			fread(&child, sizeof(unsigned short), 1, fp);
		}
	}
}

void K3D12::PMDLoader::LoadMorph(FILE * fp, PMDModel * model)
{
}

std::shared_ptr<K3D12::PMDModel> K3D12::PMDLoader::Load(std::string path)
{

	//ファイル読み込み
	if (path.length() == 0) {
		return nullptr;
	}

	FILE* fp;
	std::shared_ptr<PMDModel> model;
	fopen_s(&fp, path.c_str(), "rb");
	if ((fp) == NULL) {

		return nullptr;
	}

	model = std::make_shared<PMDModel>();

	//ヘッダ読み込み
	PMDHeader header{};
	fread(&header, sizeof(PMDHeader), 1, fp);

	//一応念のため283バイト位置にシークしとくsizeof(PMDHeader)でもいける（１バイトアラインメントしてるから)
	fseek(fp, 283, SEEK_SET);


	model->modelPath = path;
	//頂点リスト読み込み
	{
		LoadVertex(fp, model.get());
	}
	//頂点インデックス配列読み込み
	{
		LoadIndexList(fp, model.get());
	}
	//マテリアル読み込み
	{
		LoadMaterial(fp, model.get());
	}
	//ボーン読み込み
	{
		LoadSkelton(fp, model.get());
	}
	//IK読み込み
	{
		LoadIK(fp, model.get());
	}
	fclose(fp);
	return model;
}




