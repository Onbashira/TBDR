#pragma once
#include "../Util/D3D12Common.h"
#include "Resource.h"

namespace K3D12 {

	class VertexBuffer : public K3D12::Resource
	{
		D3D12_VERTEX_BUFFER_VIEW _view;
		ULONG64 _size;
		UINT _stride;
	public:
		virtual HRESULT Create(ULONG64 size, UINT stride, const void* pVertices);
		void Discard();
		D3D12_VERTEX_BUFFER_VIEW GetView() const;

		VertexBuffer();
		~VertexBuffer();
	};
}