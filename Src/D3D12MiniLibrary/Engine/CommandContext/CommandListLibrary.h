#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <map>
#include <vector>



namespace K3D12 {

	class GraphicsCommandList;
	class PipelineStateObject;
	class GraphicsContextLibrary;
	class DepthStencil;

	class CommandListLibrary
	{
		friend class GraphicsContextLibrary;
	private:
		std::map<std::string, std::shared_ptr<GraphicsCommandList>> _library;
	public:

	private:

	public:

		HRESULT Create(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		void Set(std::string commandListName, std::shared_ptr<GraphicsCommandList> commandList);

		std::shared_ptr<GraphicsCommandList> Get(std::string commandListName);

		void Erase(std::string commandListName);

		void ConnectingPSO(std::string commandListName, PipelineStateObject& pso);

		void BindingDepthStencil(std::string commandListName, DepthStencil* depthStencil);

		void AddRenderTargets(std::string commandListName, D3D12_CPU_DESCRIPTOR_HANDLE handles[]);

		void AddRenderTarget(std::string commandListName, D3D12_CPU_DESCRIPTOR_HANDLE handle, unsigned int index);

		void OMSetBindingRenderTarget(std::string commandListName);

		void OMSetRenderTargets(std::string commandListName, unsigned int numRenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[], bool RTsSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

		void Discard();

		CommandListLibrary();
		~CommandListLibrary();
	};

}