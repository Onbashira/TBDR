#pragma once
#include <memory>
#include <vector>
#include "Math.h"
#include "../../Animator/Animator.h"
#include "../../Animator/StateMachineController.h"
#include "./MMDModelResourceData.h"
#include "../ModelMesh.h"

namespace K3D12 {

	class GraphicsCommandList;
	class PipelineStateObject;
	class ModelConverter;

	struct MMDRendererMaterial
	{
		//拡散反射光 w = alpha
		Vector4 diffuse;
		//鏡面反射光	w = pow
		Vector4 specular;
		//環境光
		Vector3 ambient;
		//自己発光色
		Vector3 emissive;
		//スフィアマップのブレンディングタイプ
		int	sphereBlendType;
	};


	//ゲーム本編で使うモデル
	class MMDModel :
		public ModelMesh
	{
		friend class ModelConverter;
		friend class ModelPool;

		enum SHADER_ROOT_PARAMATER_INDEX {
			SHADER_ROOT_PARAMATER_INDEX_CAMERA = 0,
			SHADER_ROOT_PARAMATER_INDEX_TRANSFORM,
			SHADER_ROOT_PARAMATER_INDEX_MATERIAL,
			SHADER_ROOT_PARAMATER_INDEX_TEXTURE,
			SHADER_ROOT_PARAMATER_INDEX_TOON_MAP,
			SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE,
		};
	private:
		//モデルのオリジナルデータ
		std::weak_ptr<MMDModelResourceData>	_resourceData;
	protected:
		std::vector<Vertex3D>				_vertexes;

		Animator							_animator;

	public:
	private:

		Matrix CulcBDEF1(K3D12::WeightDeform& deform);
		Matrix CulcBDEF2(WeightDeform& deform);
		Matrix CulcBDEF4(WeightDeform& deform);
		std::pair<Vector3, float> CulcSDEF(WeightDeform& deform);
		Matrix CulcQDEF(WeightDeform& deform);
		std::pair<float, float> CulcSDEFWaight(Vector3 r0, Vector3 r1);

	protected:

		void	TransformUpdate();

		void	SkeltonReset();

		void	SetAnimationData(std::string motionPath);

		virtual void	SetCommandList(std::weak_ptr<GraphicsCommandList>& commandList);

		virtual void	SetPipelineState(PipelineStateObject* pso);

		virtual void    SetRootSignature(RootSignature * rootSignature);

		virtual void	RegistBundle();

	public:
		//モデル初期化
		void Initialize();
		//トランスフォームのアップデート
		virtual void Update();

		void	AnimationUpdate();

		//アップデート関数と分離したいアップデートを行う
		virtual void LateUpdate();
		//リストにDrawコマンドを積み込む(ジオメトリレンダリング）
		virtual void Draw();

		virtual void TestDraw();

		virtual Animator& Animator();

		void SetModelSkelton();

		MMDModel();
		virtual ~MMDModel();
	};
}