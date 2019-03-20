#pragma once
#include <memory>
#include "../Device/D3D12Device.h"
#include "../Util/NonCopyable.h"
namespace K3D12{
class ShaderResource;
class ConstantBuffer;
class UnorderedAccessValue;
class VertexBuffer;
class IndexBuffer;
class Resource;
class DepthStencil;


class ResourceCreater : private NonCopyable
{
private:
	std::weak_ptr<K3D12::D3D12Device> _device;
public:
private:
	ResourceCreater();
public:
	static ResourceCreater&					GetInstance();
	void									Initialize(std::weak_ptr<K3D12::D3D12Device> device);
	std::shared_ptr<ShaderResource>			CreateShaderResource();
	std::shared_ptr<ConstantBuffer>			CreateConstantBuffer();
	std::shared_ptr<UnorderedAccessValue>	CreateUAV();
	std::shared_ptr<VertexBuffer>			CreateVertexBuffer();
	std::shared_ptr<IndexBuffer>			CreateIndexBuffer();
	std::shared_ptr<Resource>				CreateResource();
	std::shared_ptr<DepthStencil>			CreateDepthStencil();

	~ResourceCreater();
};

}