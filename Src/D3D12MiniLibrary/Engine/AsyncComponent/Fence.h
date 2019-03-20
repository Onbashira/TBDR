#pragma once
#include "../Util/D3D12Common.h"
#include <list>

//Forward declaration

//
namespace K3D12 {
	
	class CommandQueue;

	class Fence
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
		UINT64								_fenceValue;
		void*								_fenceEvent;
	public:

	private:

	public:
		HRESULT								Create(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags);
		void								Discard();
		Microsoft::WRL::ComPtr<ID3D12Fence>	GetFence();
		UINT64								GetFenceValue();
		void*								GetFenceEvent();
		HRESULT								Wait(CommandQueue* commandQueue);

		Fence();
		~Fence();
	};
}

