#pragma once

#include "./Resource.h"
#include "../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {

	class StructuredBuffer :
		public Resource
	{
	private:

		DescriptorHeap _heap;

		D3D12_BUFFER_UAV _uavBuffer;

		unsigned int   _elementNum;
		unsigned int   _elementSize;

		void CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC& srv);

		void CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, ID3D12Resource* counterResource = nullptr);

	public:
		HRESULT Create(unsigned int elementSize, unsigned int elementNum = 1, ID3D12Resource* counterResource = nullptr);



		D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle();

		D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle();
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle();

		unsigned int GetElementSize();

		unsigned int GetElementNum();

		StructuredBuffer();
		~StructuredBuffer();
	};
}
