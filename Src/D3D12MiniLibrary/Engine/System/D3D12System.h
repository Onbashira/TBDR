#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "../Util/D3D12Common.h"
#include "../Window/Window.h"
#include "../Device/D3D12Device.h"
#include "../Factory/Factory.h"
#include "../AsyncComponent/Fence.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../CommandContext/CommandQueue.h"
#include "../Resource/RenderTarget.h"
#include "../Input/InputManager.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Model/ModelPool.h"
#include "../Input/GamePadManager.h"
#include "../Rendering/GBufferSprite.h"
#include "../GameConponent/Primitive/PrimitiveCreater.h"


namespace K3D12 {

	class D3D12System
	{

	private:
		inline static D3D12System*				_instance = nullptr;
		D3D12Device						_device;
		Factory							_factory;
		Fence							_fence;
		Window							_window;
		UINT							_windowWidth;
		UINT							_windowHeight;
		std::wstring					_appClassName;
		//GraphicsContextLibrary			_graphicsLibrary;
		CommandQueue					_commandQueue;
		RenderTarget					_renderTarget;
		InputManager					_inputManager;
		Camera							_mainCamera;
		ModelPool						_modelPool;
		PrimitiveCreater				_primitiveCreater;
		GBufferSprite					_geometryBufferSprite;
	public:

	private:

		HRESULT InitializeWindow();

		HRESULT InitializeD3D12(unsigned int bufferCount = 2, bool useWarpDevice = false);

		HRESULT InitializeDevice(bool useWarpDevice = false);

		HRESULT InitializeDXGIFactory();

		HRESULT InitializeRenderTargets(unsigned int bufferCount = 2);

		HRESULT InitializeCommandQueue();

		HRESULT InitializeCommandList();

		HRESULT InitializeFence();

		HRESULT InitializePSO();

		HRESULT InitializeSpritePSO();

		HRESULT InitializePrimitivePSO();

		HRESULT InitializeFxAAPSO();

		HRESULT InitializeModelTransparency();

		HRESULT InitializeModelOpacity();

		HRESULT	InitializeModelPool();

		HRESULT	InitializePrimitiveCreater();

		HRESULT InitializeMainCamera();

		HRESULT CommandListConnecting();

		HRESULT BindingGBufferRenderTarget();

		HRESULT InitiazlieGBufferSprite();

		void TermWindow();

		void TermD3D12();

		D3D12System();

	public:
		static void Create();

		static D3D12System& GetInstance();

		static void Destroy();

		static HRESULT Initialize(UINT windowWidth, UINT windowHeight, UINT backBufferCount = 2, bool useWarpDevice = false);

		static HRESULT Initialize(UINT backBufferCount = 2, bool useWarpDevice = false);

		static void InitializeCamera(CameraType type, const Vector3& pos, const Vector3 & target, const Vector3& up, float nearClip, float farClip, float fov);

		static void SetWindowSize(UINT widths, UINT height);

		static void SetWindowName(std::wstring name);

		static int MessageLoop();

		static void ScreenFlip();

		static void ClearScreen();

		static HRESULT LoadTexture(std::string path);

		static std::weak_ptr<MMDModel> LoadModel(std::string modelPath);

		static std::shared_ptr<GamePad> GetController(int padID = 0);

		static void	 InitializeController(int controllerCount = 4);

		static  Window& GetWindow();

		static void InputUpdate();

		static HRESULT CreatePSO(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3DBlob* rootSignature = nullptr);

		static HRESULT CreatePSO(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ID3DBlob* rootSignature = nullptr);

		static HRESULT CreateCommandList(std::string name, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE& type);

		static std::shared_ptr<PipelineStateObject> GetPSO(std::string name);

		static std::shared_ptr<GraphicsCommandList>	GetCommandList(std::string name);

		static std::shared_ptr<GraphicsCommandList>	GetMasterCommandList();

		static std::shared_ptr<CommandQueue>		GetMasterCommandQueue();

		static Camera& GetCamera();

		ID3D12Device * GetDevice();

		IDXGIFactory4* GetFactory();

		Fence*		   GetFence();

		static InputManager&  GetInput();

		CommandQueue*	GetCommandQueue();

		static Plane* CreatePlane();

		~D3D12System();
	};

#ifndef GET_DEVICE
#define GET_DEVICE K3D12::D3D12System::GetInstance().GetDevice()
#endif // !GET_DEVICE

#ifndef CHECK_RESULT 
#define CHECK_RESULT(result) if(FAILED(result)){return result;}
#endif // !CHECK_RESULT

#ifndef GET_SYSTEM_FENCE 
#define GET_SYSTEM_FENCE K3D12::D3D12System::GetInstance().GetFence()
#endif // !GET_DEVICE


}