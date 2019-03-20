#include <vector>
#include <memory>
#include "PrimitiveComponent.h"
#include "../../TextureComponent/TextureManager.h"
#include "../RendererMaterial.h"
#include "../../Resource/ShaderResource.h"
#include "../../Util/D3D12Common.h"

K3D12::PrimitiveComponent::PrimitiveComponent()
{
	_shaderResource = (TextureManager::GetInstance().GetNullTextureShaderResource().lock());
}


K3D12::PrimitiveComponent::~PrimitiveComponent()
{
	_shaderResource.reset();
}


void K3D12::PrimitiveComponent::AttachTexture(std::string texturePath , D3D12_CPU_DESCRIPTOR_HANDLE handle )
{
	TextureLoader::GetInstance().LoadSpriteTexture(texturePath, handle);
	_shaderResource = TextureManager::GetInstance().GetSpriteShaderResource(texturePath).lock();
}
