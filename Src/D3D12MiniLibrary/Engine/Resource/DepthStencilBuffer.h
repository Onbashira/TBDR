#pragma once
#include <memory>
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "ShaderResource.h"

namespace K3D12 {

	class GraphicsCommandList;

	class DepthStencil : public Resource
	{
	private:
		float											_clearColor[4];
		DescriptorHeap									_dsvHeap;
		DescriptorHeap                                  _readOnlySRVHeap;
	public:
		HRESULT											Create(UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT clearValueFormat);
		HRESULT											CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC srv, D3D12_CPU_DESCRIPTOR_HANDLE handle);
		void											Discard();
		DescriptorHeap*									GetDSVHeapPtr() { return &_dsvHeap; };
		DescriptorHeap*									GetReadOnlySRVHeap() { return &_readOnlySRVHeap; };
		void											ClearDepthStencil(std::shared_ptr<K3D12::GraphicsCommandList> list);
		DepthStencil();
		~DepthStencil();
	};

}