#pragma once
#include <map>
#include <string>
#include "../Util/D3D12Common.h"



namespace K3D12 {

	class CommandQueue;
	class Fence;
	class PipelineStateObject;
	class DepthStencil;
	class CommandListLibrary;

	class GraphicsCommandList
	{
		friend class CommandListLibrary;
	private:
		//グラフィクスコマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>	_commandList;
		//コマンドリスト
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		_commandAllocator;
		//描画で使うデプスステンシル
		DepthStencil*										_bindedDepthStencil;
		//描画時に書き込むターゲットへのCPUメモリハンドルとキー
		std::map<unsigned int, D3D12_CPU_DESCRIPTOR_HANDLE>	_bindedRenderTargets;
		//コマンドリストの名前
		std::string											_commandListName;
		//コマンドアロケータの名前
		std::string											_commandAllocatorName;
	public:

	private:

	public:

		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		HRESULT SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		HRESULT UAVBarrie(ID3D12Resource * resource);

		HRESULT ExcutionCommandList(CommandQueue* queue, Fence* fence = nullptr);

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	GetCommandList();

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		GetAllocator();

		HRESULT						ResetCommandList(ID3D12PipelineState* pInitialState = nullptr);

		HRESULT						ResetAllocator();

		HRESULT						CloseCommandList();

		HRESULT						ConnectingPSO(PipelineStateObject* graphicsPSO);

		HRESULT						BindingDepthStencil(DepthStencil* depth);

		void						AddRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE handles[]);

		void						AddRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE handle, unsigned int index = 0);

		void						UnBindingRenderTargets();

		void						UnBindingRenderTarget(unsigned int renderTargetIndex = 0);

		void						OMSetBindingRenderTargets();

		void						OMSetRenderTargets(unsigned int numRenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, bool RTsSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle = nullptr);

		void						Discard();

		void						SetName(std::string objectName);

		void						SetCommandListName(std::string name);

		void						SetCommandAllocatorName(std::string name);


		GraphicsCommandList();
		~GraphicsCommandList();
	};

};