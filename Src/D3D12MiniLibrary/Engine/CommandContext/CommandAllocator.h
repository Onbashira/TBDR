#pragma once
#include "../Util/D3D12Common.h"

namespace K3D12 {
	class CommandAllocator
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;
	public:
	private:
	public:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAllocator();
		void Discard();
		CommandAllocator();
		~CommandAllocator();
	};
}

