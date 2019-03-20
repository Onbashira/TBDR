#include "ShaderManager.h"


K3D12::ShaderManager::ShaderManager()
{
}

K3D12::ShaderManager & K3D12::ShaderManager::GetInstance()
{
	static ShaderManager instance;
	return instance;

}

bool K3D12::ShaderManager::IsLoaded(std::string name)
{
	return _pool.IsLooded(name);
}

Microsoft::WRL::ComPtr<ID3DBlob> K3D12::ShaderManager::Get(std::string name)
{
	return *_pool.Get(name);
}

void K3D12::ShaderManager::Set(std::string name, Microsoft::WRL::ComPtr<ID3DBlob> shader)
{
	_pool.Set(name, shader);
}

void K3D12::ShaderManager::Erase(std::string name)
{
	_pool.Erase(name);
}

void K3D12::ShaderManager::Discard()
{
	_pool.DiscardMap();
}

K3D12::ShaderManager::~ShaderManager()
{
	Discard();
}
