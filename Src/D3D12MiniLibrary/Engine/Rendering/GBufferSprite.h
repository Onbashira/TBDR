#pragma once
#include <map>
#include <memory>
#include <string>
#include "../Resource/VertexBuffer.h"
#include "../Resource/ConstantBuffer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Resource/ShaderResource.h"
#include "../Resource/DepthStencilBuffer.h"
#include "../Window/Window.h"
#include "../Util/Math.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Resource/UnorderdAccessView.h"

namespace K3D12 {

	class GraphicsCommandList;
	class Window;
	class StructuredBuffer;

	enum class GEOMETRY_TYPE {
		NORMAL,
		SPECULAR,
		ALBEDO,
		DEPTH,
		SPECULER_MAP,
		DIFFUSE_MAP,
		RESULT,
		RESULT2,

		GEOMETRY_TYPE_MAX
	};

	enum class SHADER_PARAMATER_INDEX_LIGHTING {
		CAMERA_INFO,
		NORMAL_MAP,
		SPECULAR_POWER_MAP,
		ABLEDO_MAP,
		DEPTH_MAP,
		LIGHT_BUFFER,
		RESULT,

		SPECULAR_MAP,
		DIFFUSE_MAP,

	};

	enum class SHADER_PARAMATER_INDEX_RESULT {
		ALBEDO_MAP,
		SPECULAR_MAP,
		DIFFUSE_MAP
	};

	class GBufferSprite
	{


	private:
		unsigned int									_threadDimensionX;
		unsigned int									_threadDimensionY;
		Window*											_window;
		unsigned int									_windowWidth;
		unsigned int									_windowHeight;
		Vertex3D										_vertex[4];
		VertexBuffer									_vertexBuffer;
		DescriptorHeap									_geometryShaderHeap;		//NORMAL + ALBEDO + SPEC + SunOculusion
		DescriptorHeap									_rtvHeap;					//上記と同じ
		DescriptorHeap									_uavHeap;					//SpecularMap , DiffuseMap
		DepthStencil*									_bindedDepthStencil;		//このクラスにバインドされているデプスステンシル
		CameraInfo										_cameraInfo;				//ジオメトリパスで使ったカメラの情報を持っておく
		ConstantBuffer									_cameraConstantBuffer;		//ライティング時にコンピュートシェーダへ送るデータ
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>		_geometryRenderTargets;
		std::map<GEOMETRY_TYPE, ShaderResource>			_geometryResource;
		std::shared_ptr<GraphicsCommandList>			_list;

	public:

	private:
		HRESULT	InitializeVertex();
		HRESULT InitializeResource(unsigned int spriteWidth, unsigned int spriteHeight);
		HRESULT InitializeDescriptorHeap();
		HRESULT InitializeGeometryView(DepthStencil* depth);
		HRESULT CreateLightingPassPso();
	public:

		HRESULT CreateGBuffer(unsigned int spriteWidth, unsigned int spriteHeight, Window* window, std::shared_ptr<GraphicsCommandList> list, DepthStencil* depth);

		D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUHandle(GEOMETRY_TYPE type);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(GEOMETRY_TYPE type);
		D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle(GEOMETRY_TYPE type);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle(GEOMETRY_TYPE type);
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle(GEOMETRY_TYPE type);
		D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle(GEOMETRY_TYPE type);

		Resource* GetGBufferResource(GEOMETRY_TYPE type);

		//ジオメトリパスでレンダーターゲットとして使うテクスチャ群をセット
		void SetGeometryRenderTarget(DepthStencil* depth = nullptr);

		//カメラの情報をセットする
		void SetCameraInfo(CameraInfo& cameraInfo);

		//ジオメトリパスを終えたテクスチャをセットする
		void SetGeometryResultTexture(DepthStencil* depth = nullptr);

		//ライティングで結果を書き込むUAVをシェーダーにセットする。
		void SetLightingRenderUAV();

		//ライティングで使うであろうライトをシェーダーにセットする
		void SetLightStructuredBuffer(UnorderedAccessValue* lightUav);

		//レンダリング結果のCPUハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE GetResultTextureCPUHandle();

		//ライティングパスを終えたテクスチャ群をシェーダーにセットする
		void SetLightingResult();

		//ライティングパスを非同期コンピュートで実行
		void StartLightingPass(UnorderedAccessValue* lightsBuffer, DepthStencil* depthResource = nullptr);

		//各パスにおけるリソースの状態を変位させる
		void TransitionGeometryPassStart();
		void TransitionGeometryPassEnd();
		void TransitionLightingPassStart(DepthStencil* depthResource = nullptr);
		void TransitionLightingPassEnd(DepthStencil* depthResource = nullptr);

		void TransitionResultTextureToSR();
		void TransitionResultTextureToUAV();
		void TransitionResultTextureToCopySource();
		void TransitionResultTextureToGeneric();
		void TransitionResultTextureToRT();


		void TransitionResultTexture2ToSR();
		void TransitionResultTexture2ToUAV();
		void TransitionResultTexture2ToCopySource();
		void TransitionResultTexture2ToRT();

		void TransitionResultTexture2ToGeneric();

		//操作するコマンドリストを変える
		void BindingCommandList(std::shared_ptr<GraphicsCommandList> newCommandList);
		//描画に使うデプスステンシルリソースを変える
		void BindingDepthStencill(DepthStencil* depthStencil);

		void Discard();
		void SetHeaps();
		VertexBuffer& GetVertexBuffer();

		GBufferSprite();
		~GBufferSprite();
	};

}