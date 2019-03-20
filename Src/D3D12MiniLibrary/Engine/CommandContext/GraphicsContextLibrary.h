#pragma once
#include <d3d12.h>
#include <vector>
#include "../Util/NonCopyable.h"
#include "CommandListLibrary.h"
#include "../ShaderComponent/ShaderObjectLibrary.h"



namespace K3D12 {
	class CommandQueue;
	class DepthStencil;
	class Fence;


	class GraphicsContextLibrary : private NonCopyable
	{
	private:
		CommandListLibrary			_commandListLibrary;
		ShaderObjectLibrary			_shaderObjectLibrary;

	public:
	private:
		GraphicsContextLibrary();
	public:

		void    CloseAllCommandLists();

		void	CloseCommandList(std::string commandListName);

		void	ResetAllCommandList();

		void    ResetCommandList(std::string commandListName);

		HRESULT ExcutionCommandList(CommandQueue* queue, std::string commandListName = " CommandList", Fence* fence = nullptr);

		HRESULT ExcutionCommandListAsync(CommandQueue* queue, std::vector<std::string> commandListNames, Fence* fence = nullptr);

		HRESULT ExcutionAllCommandLists(CommandQueue* queue, Fence* fence = nullptr);

		HRESULT CreateCommandList(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob* rootSignature = nullptr);

		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC gps, ID3DBlob * rootSignature = nullptr);

		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		void SetCommandList(std::string commandListName, std::shared_ptr<GraphicsCommandList> commandList);

		void SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso);

		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		std::shared_ptr<GraphicsCommandList> GetGraphicsCommandList(std::string commandListName = "CommandList");

		std::shared_ptr<PipelineStateObject> GetPSO(std::string psoName);

		std::shared_ptr<RootSignature> GetRootSignature(std::string rsName);

		void EraseCommandList(std::string commandListName);

		void ErasePSO(std::string psoName);

		void EraseRootSignature(std::string psoName);

		HRESULT Connecting(std::string commandList, std::string psoName);

		HRESULT SetRenderTargetsToAllCommandList(unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget, DepthStencil* depth = nullptr);

		HRESULT SetRenderTargetsToSelectedCommandList(std::string* commandListName, unsigned int numCommandList, unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget = true, DepthStencil* depth = nullptr);

		HRESULT SetRenderTargets(std::string commandListName, unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget = true, DepthStencil* depth = nullptr);

		void Discard();

		static GraphicsContextLibrary& GetInstance();

		~GraphicsContextLibrary();
	};

}