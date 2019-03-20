#pragma once
#include <vector>
#include "../../Util/Math.h"

namespace K3D12 {

	class ModelConverter;

	enum class PMXGlobalTypes {
		TEXT_ENCORDING_TYPE,
		ADDITIONAL_UV_COUNT,
		VERTEX_INDEX_SIZE,
		TEXTURE_INDEX_SIZE,
		MATERIAL_INDEX_SIZE,
		BONE_INDEX_SIZE,
		MORPH_INDEX_SIZE,
		RIGIDBODY_IDNEX_SIZE,
		PMX_GLOBAL_TYPES_MAX
	};

	struct PMXHeader {
		unsigned char signature[4];
		float version;
		unsigned char globalCount;
		std::vector<unsigned char> globals;

		std::wstring modelName;
		std::wstring modelNameEnglish;
		std::wstring comment;
		std::wstring commentEnglish;

	};

	struct BDEF1 {
		int boneIndex;
		BDEF1() : boneIndex(0) {};
		~BDEF1() {};
	};

	struct BDEF2 {
		int boneIndex01;
		int boneIndex02;
		float boneWeight01;
		float boneWeight02;
		BDEF2() : boneIndex01(0), boneIndex02(0), boneWeight01(0.0f), boneWeight02(0.0f) {

		}
		~BDEF2() {

		};
	};

	struct BDEF4 {
		int boneIndex01;
		int boneIndex02;
		int boneIndex03;
		int boneIndex04;
		float boneWeight01;
		float boneWeight02;
		float boneWeight03;
		float boneWeight04;
		BDEF4() : boneIndex01(0), boneIndex02(0), boneIndex03(0), boneIndex04(0),
			boneWeight01(0.0f), boneWeight02(0.0f), boneWeight03(0.0f), boneWeight04(0.0f) {

		}
		~BDEF4() {

		};
	};

	struct SDEF {
		int boneIndex01;
		int boneIndex02;
		float boneWeight01;
		float boneWeight02;
		Vector3 c;
		Vector3 r0;
		Vector3 r1;
		SDEF() : boneIndex01(0), boneIndex02(0),
			boneWeight01(0), boneWeight02(0), c(), r0(), r1() {

		}
		~SDEF() {

		};
	};

	struct QDEF {
		int boneIndex01;
		int boneIndex02;
		int boneIndex03;
		int boneIndex04;
		float boneWeight01;
		float boneWeight02;
		float boneWeight03;
		float boneWeight04;

		QDEF() : boneIndex01(0), boneIndex02(0), boneIndex03(0), boneIndex04(0),
			boneWeight01(0.0f), boneWeight02(0.0f), boneWeight03(0.0f), boneWeight04(0.0f) {

		}
		~QDEF() {

		};
	};

	union WeightDeform
	{
		BDEF1	bdef1;
		BDEF2	bdef2;
		BDEF4	bdef4;
		SDEF	sdef;
		QDEF	qdef;

		WeightDeform() : sdef() {};
		WeightDeform(const WeightDeform& other) {
			this->sdef = other.sdef;
		};
		WeightDeform& operator=(const WeightDeform& value) {
			sdef = value.sdef;
			return *this;
		};
		~WeightDeform() {};
	};

	struct PMXVertex {
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		std::vector<Vector4> additionalUV; //Can be 0 ;
		unsigned char weightDeformType;
		WeightDeform weightDeform;
		float edgeScale;
		PMXVertex() {};
		~PMXVertex() {};
		PMXVertex(const PMXVertex &) {};
	};


	struct PMXSurface {
		//[surface 0 -> A, surface 1 -> B, surface 2 -> C] = triangle [vertex A, vertex B, vertex C]
		std::vector<unsigned int> surfaceIndex;
	};
	struct PMXTexturePath {
		std::vector<std::wstring> path;
	};

	union ToonValue {
		unsigned char	unionToon;
		int		individualToon;
		ToonValue() : individualToon(0) {};
		~ToonValue() {};
		ToonValue(const ToonValue& other) {
			individualToon = other.individualToon;
		};
	};

	struct PMXMaterial {
		std::wstring				nameLocal;
		std::wstring				nameUniversal;
		Vector4						diffuseColor;
		Vector3						specularColor;
		float						specularStrength;
		Vector3						ambientColor;
		unsigned char						drawingFlags;
		Vector4						edgeColor;
		float						edgwScale;
		int							textureIndex;
		int							environmentTextureIndex;
		unsigned char						environmentBlendFlags;
		unsigned char						toonReference;

		ToonValue					toonValue;
		std::vector<std::wstring>	metaData;
		int							surfaceCount;
	};

	struct InheritBone {
		int parentIndex;
		float parentInfluence;

	};
	struct BoneFixedAxis {
		Vector3 axisDirection;

	};
	struct BoneLocalCoordinate {
		Vector3 right;
		Vector3 up;

	};

	struct BoneExternalParent {
		int index;
	};

	struct IKAngleLimit {

		Vector3 minimamAngle;
		Vector3 maximumAngle;
		IKAngleLimit() : minimamAngle(), maximumAngle() {};
		~IKAngleLimit() {};
	};

	struct IKLinks {
		int boneIndex;
		unsigned char hasLimit;
		IKAngleLimit limits;
		IKLinks() : boneIndex(0), hasLimit(0), limits() {};
		~IKLinks() {};
	};

	struct BoneIK {
		int targetIndex;
		int loopCount;
		float limitRadian;
		int linkCount;
		std::vector<IKLinks> ikLinks;
		BoneIK() : targetIndex(0), loopCount(0), limitRadian(0.0f), linkCount(0) {};
	};

	struct PMXBone {
		std::wstring				boneNameLocal;
		std::wstring				boneNameUniversal;
		Vector3						bonePosition;
		int							parentBoneIndex;
		int							deformationHierarchyLayer;

		//○ボーンフラグ
		//0x0001  : 接続先(PMD子ボーン指定)表示方法 -> 0 : 座標オフセットで指定 1 : ボーンで指定
		//0x0002 : 回転可能
		//0x0004 : 移動可能
		//0x0008 : 表示
		//0x0010 : 操作可
		//0x0020 : IK
		//0x0080 : ローカル付与 | 付与対象 0 : ユーザー変形値／IKリンク／多重付与 1 : 親のローカル変形量
		//0x0100 : 回転付与
		//0x0200 : 移動付与
		//0x0400 : 軸固定
		//0x0800 : ローカル軸
		//0x1000 : 物理後変形
		//0x2000 : 外部親変形
		unsigned short				boneFlags;

		Vector3						tailPosition;
		int							connectBoneIndex;

		InheritBone					inheritBone;
		BoneFixedAxis				fixedAxis;
		BoneLocalCoordinate			boneAxis;
		BoneExternalParent			externalParent;
		BoneIK						ikData;
	};

	enum class MorphType
	{
		Group,
		Vertex,
		Bone,
		UV,
		UVadd1,
		UVadd2,
		UVadd3,
		UVadd4,
		Material,
		Flip,
		Impulse
	};

	struct GroupMorph
	{
		unsigned int index;
		float influence;
	};

	struct VertexMorph
	{
		unsigned int vertexIndex;
		Vector3 translation;
	};

	struct BoneMorph
	{
		unsigned int boneIndex;
		Vector3 translation;
		Vector4 rotation;
	};

	struct UVMorph
	{
		unsigned int vertexIndex;
		Vector4 uvOffset;
	};

	struct MaterialMorph
	{
		int materialIndex;//-1は全材質対象外
		unsigned char offsetcalType;//0:乗算 1:加算
		Vector4 diffuse;
		Vector3 specular;
		float specularity;
		Vector3 ambient;
		Vector4 edgeColor;
		float edgeSize;
		Vector4 textureTint;
		Vector4 environmentTint;
		Vector4 toonTint;
	};

	struct Flip {
		int morphIndex;
		unsigned char localFlag;
		Vector3 velocity;
		Vector3 torque;
	};

	struct Impulus {
		int	rigidBodyIndex;
		unsigned char	localFlag;
		Vector3 velocity;
		Vector3 torque;
	};

	union MorphData
	{
		VertexMorph vertexMorph;
		UVMorph uvMorph;
		BoneMorph boneMorph;
		MaterialMorph materialMorph;
		GroupMorph groupMorph;
		MorphData() {};
		~MorphData() {}
		MorphData(const MorphData &) {};;
	};

	struct PMXMorph {
		std::wstring nameLocal;
		std::wstring nameUniversal;
		unsigned char controllPanel;
		unsigned char morphType;
		unsigned int morphCount;
		std::vector<MorphData> morphs;

	};

	struct DisplayData
	{
		unsigned char type;
		unsigned int index;
	};

	struct PMXDiplay
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;
		unsigned char flags; //0;normal 1:specialFrame
		unsigned int count;
		std::vector<DisplayData> datas;
	};

	struct PMXRigidBody
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;

		unsigned int boneIndex;//可変

		unsigned char groupID;
		unsigned short nonCollisionGroup;
		unsigned char shape;

		Vector3 size;
		Vector3 pos;
		Vector3 rotation;//ラジアン

		float mass;//質量
		float moveAttenuation;//移動減衰
		float rotationDamping;//回転減衰
		float boundPower;//反発力(係数?
		float frictionPower;//摩擦力(係数?

		unsigned char phisicsMode;//0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
	};

	struct SixDof
	{
		unsigned int rigidAIndex;
		unsigned int rigidBIndex;

		Vector3 pos;
		Vector3 rotation;

		Vector3 posMinimum;
		Vector3 posMaximum;
		Vector3 rotationMinimum;
		Vector3 rotationMaximum;

		Vector3 positionSpring;
		Vector3 rotationSpring;
	};
	enum class JointType {
		Spring6Dof,
		SixDof,
		P2P,
		ConeTwist,
		Slider,
		Hinge
	};
	struct PMXJoint
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;

		unsigned char type;//pmx2.0では0のみ
		SixDof spring;
	};

	//モデルの初期情報が入った情報体で、これをコンバートしてモデルを作る
	class PMXModel
	{
	private:
	public:
		std::string					modelPath;
		PMXHeader					header;
		std::vector<PMXVertex>		vertcies;
		PMXSurface					surfaces;
		PMXTexturePath				paths;
		std::vector<PMXMaterial>	materials;
		std::vector<PMXBone>		bones;
		std::vector<PMXMorph>		morphDatas;
		std::vector<PMXDiplay>		displayInfo;
		std::vector<PMXRigidBody>	rigidBodies;
		std::vector<PMXJoint>		joints;
	private:
	public:
		PMXModel();
		virtual ~PMXModel();
	};

}