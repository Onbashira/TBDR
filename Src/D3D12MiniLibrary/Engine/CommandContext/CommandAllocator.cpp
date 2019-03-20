#include "CommandAllocator.h"
#include "../Util/D3D12Common.h"


K3D12::CommandAllocator::CommandAllocator()
{
}


K3D12::CommandAllocator::~CommandAllocator()
{
	Discard();
}


Microsoft::WRL::ComPtr<ID3D12CommandAllocator> K3D12::CommandAllocator::GetAllocator()
{
	return this->_allocator;
}

void K3D12::CommandAllocator::Discard()
{
	_allocator.Reset();
}

