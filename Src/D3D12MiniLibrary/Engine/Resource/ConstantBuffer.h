#pragma once
#include "../Util/D3D12Common.h"
#include "Resource.h"

namespace K3D12 {

	class DescriptorHeap;

	class ConstantBuffer : public Resource
	{
	private:
	public:

	private:
	public:
		virtual HRESULT Create(ULONG64 size);
		HRESULT			CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		ConstantBuffer();
		~ConstantBuffer();
	};
}