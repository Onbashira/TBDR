#pragma once
#include "../Util/D3D12Common.h"
#include "Resource.h"

namespace K3D12 {
	class IndexBuffer : public Resource
	{
		D3D12_INDEX_BUFFER_VIEW _view;
		ULONG64 _size;

	public:
		virtual HRESULT Create(ULONG64 size, DXGI_FORMAT format, const void * pIndices);
		virtual HRESULT Create(ULONG64 size, size_t elementSize, const void * pIndices);

		void Discard();
		D3D12_INDEX_BUFFER_VIEW GetView()const;

		IndexBuffer();
		~IndexBuffer();
	};
}