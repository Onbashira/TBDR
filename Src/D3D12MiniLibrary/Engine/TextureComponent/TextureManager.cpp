#include "TextureManager.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"




K3D12::TextureManager::~TextureManager()
{
	Discard();
}


K3D12::TextureManager::TextureManager()
{
	//ヌルテクスチャの作成
	std::shared_ptr<ShaderResource> resource = std::make_shared<ShaderResource>();
	{
		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
		props.CreationNodeMask = 0;
		props.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Width = 16;
		resDesc.Height = 16;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 0;
		resDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

		resource->Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	//参照の追加
	resource->SetName("Nulltexture ");
	_modelTextureResourceMap[this->_nullTextureName] = resource;
	//参照の破棄
	resource.reset();

}

std::weak_ptr<K3D12::ShaderResource> K3D12::TextureManager::GetSpriteShaderResource(std::string name)
{
	return _modelTextureResourceMap[name];
}

std::weak_ptr<K3D12::ShaderResource> K3D12::TextureManager::GetModelTextureShaderResource(std::string modelPath)
{
	return _modelTextureResourceMap[modelPath];
}

std::weak_ptr<K3D12::ShaderResource> K3D12::TextureManager::GetNullTextureShaderResource()
{
	return _modelTextureResourceMap[this->_nullTextureName];
}

void K3D12::TextureManager::Discard()
{
	_modelTextureResourceMap.clear();
}


