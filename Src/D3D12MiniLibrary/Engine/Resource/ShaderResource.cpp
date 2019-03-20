#include "ShaderResource.h"
#include "../System/D3D12System.h"



K3D12::ShaderResource::ShaderResource()
{
}


K3D12::ShaderResource::~ShaderResource()
{
}

void K3D12::ShaderResource::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC & srv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateShaderResourceView(this->GetResource(), &srv, cpuDescriptorHandle);
}

void K3D12::ShaderResource::CreateView(D3D12_RENDER_TARGET_VIEW_DESC & rtv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateRenderTargetView(this->GetResource(), &rtv, cpuDescriptorHandle);
}

void K3D12::ShaderResource::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, ID3D12Resource* counterResource)
{
	GET_DEVICE->CreateUnorderedAccessView(this->GetResource(), counterResource, &uav, cpuDescriptorHandle);
}

UINT64 K3D12::ShaderResource::GetWidth()
{
	return GetResource()->GetDesc().Width;
}

UINT64 K3D12::ShaderResource::GetHeight()
{
	return GetResource()->GetDesc().Height;
}

void K3D12::ShaderResource::Discard()
{

}



