#include <memory>
#include "ResourceCreater.h"
#include "ShaderResource.h"
#include "ConstantBuffer.h"
#include "UnorderdAccessView.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DepthStencilBuffer.h"


K3D12::ResourceCreater::ResourceCreater()
{
}

K3D12::ResourceCreater::~ResourceCreater()
{
}


K3D12::ResourceCreater & K3D12::ResourceCreater::GetInstance()
{
	static ResourceCreater instance;
	return instance;
}

void K3D12::ResourceCreater::Initialize(std::weak_ptr<K3D12::D3D12Device> device)
{
	this->_device = device;
}

std::shared_ptr<K3D12::ShaderResource> K3D12::ResourceCreater::CreateShaderResource()
{
	return std::make_shared<K3D12::ShaderResource>();
}

std::shared_ptr<K3D12::ConstantBuffer> K3D12::ResourceCreater::CreateConstantBuffer()
{
	return std::make_shared<K3D12::ConstantBuffer>();
}

std::shared_ptr<K3D12::UnorderedAccessValue> K3D12::ResourceCreater::CreateUAV()
{
	return std::make_shared<K3D12::UnorderedAccessValue>();
}

std::shared_ptr<K3D12::VertexBuffer> K3D12::ResourceCreater::CreateVertexBuffer()
{
	return std::make_shared<K3D12::VertexBuffer>();
}

std::shared_ptr<K3D12::IndexBuffer> K3D12::ResourceCreater::CreateIndexBuffer()
{
	return std::make_shared<K3D12::IndexBuffer>();
}

std::shared_ptr<K3D12::Resource> K3D12::ResourceCreater::CreateResource()
{
	return std::make_shared<K3D12::Resource>();
}

std::shared_ptr<K3D12::DepthStencil> K3D12::ResourceCreater::CreateDepthStencil()
{
	return std::make_shared<K3D12::DepthStencil>();
}



