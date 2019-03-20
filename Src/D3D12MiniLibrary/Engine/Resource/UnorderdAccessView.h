#pragma once
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "Resource.h"

namespace K3D12 {

	//UAVはDXGI_FORMATはUNKNWON固定
	class UnorderedAccessValue : public K3D12::Resource
	{

	public:
		//基本的にSTRUCTURED
		enum class UAV_MODE {
			STRUCTURED,
			RAW
		};
	private:
		//Heapのオフセット値
		enum HEAP_OFFSET
		{
			UAV_DESCRIPTOR_OFFSET = 0,
			SRV_DESCRIPTOR_OFFSET = 1,
			HEAP_OFFSET_MAX,
		};

		Microsoft::WRL::ComPtr<ID3D12Resource>	_counterResource;

		DescriptorHeap							_heap;

		D3D12_UNORDERED_ACCESS_VIEW_DESC		_unorderedAccessViewDesc;

		UAV_MODE								_uavMode;
	public:

	private:

		HRESULT									CreateHeap(unsigned numElements, unsigned int nodeMask = 0);
		HRESULT									CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		HRESULT									CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		virtual HRESULT							Create(D3D12_HEAP_PROPERTIES heapProps, D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clearValue = nullptr) { return E_FAIL; };
	public:
		HRESULT									Create(unsigned int elementSize, unsigned int numElements, void* pBufferData, UAV_MODE mode = UAV_MODE::STRUCTURED);
		void									WriteToBuffer(unsigned int numElements, unsigned int elementSize, void* pBufferData);
		D3D12_CPU_DESCRIPTOR_HANDLE				GetSRVCPUHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE				GetUAVCPUHandle();
		D3D12_GPU_DESCRIPTOR_HANDLE				GetSRVGPUHandle();
		D3D12_GPU_DESCRIPTOR_HANDLE				GetUAVGPUHandle();
		void									Discard();
		Microsoft::WRL::ComPtr<ID3D12Resource>	GetCounterResource()const;
		DescriptorHeap*							GetHeap();
		UnorderedAccessValue();
		virtual~UnorderedAccessValue();
	};
}