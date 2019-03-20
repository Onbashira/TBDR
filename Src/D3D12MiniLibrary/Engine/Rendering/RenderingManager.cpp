#include <tuple>
#include <map>
#include <vector>
#include "../Resource/RenderTarget.h"
#include "../CommandContext/CommandQueue.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "RenderingPathObject.h"
#include "RenderingPathObject.h"
#include "RenderingManager.h"



K3D12::RenderingManager::RenderingManager()
{
}


K3D12::RenderingManager::~RenderingManager()
{
}

HRESULT K3D12::RenderingManager::IntializeRenderingManager(CommandQueue & commandQueue, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{
	return E_NOTIMPL;
}

void K3D12::RenderingManager::ExcutionRendering()
{
	//すべてのオブジェクトを回す
	std::vector<std::string> excutionListNames;
	for(auto& paths:this->_renderingPathMap){
		if (_masterPathEnableMap[paths.first.second]) {
			for (auto& path : paths.second) {
				if (path.second._isEnable) {
					path.second.OnBeforExcuteInitializer(D3D12System::GetCommandList(path.first));
					excutionListNames.push_back(path.first);
				}
			}

			//実行

		}
	}
}

void K3D12::RenderingManager::InitializeOnStart()
{
	//すべてのオブジェクトを回す
	for (auto& paths : this->_renderingPathMap) {
		for (auto& path : paths.second) {
			if (path.second._isEnable) {
				path.second.OnStartInitializer(D3D12System::GetCommandList(paths.first.second));
			}
		}
	}
}

void K3D12::RenderingManager::InitializeOnBeforExcute()
{
	//すべてのオブジェクトを回す
	for (auto& paths : this->_renderingPathMap) {
		for (auto& path : paths.second) {
			if (path.second._isEnable) {
				path.second.OnBeforExcuteInitializer(D3D12System::GetCommandList(paths.first.second));
			}
		}
	}
}

void K3D12::RenderingManager::InitializeOnAfterExcute()
{
	//すべてのオブジェクトを回す
	for (auto& paths : this->_renderingPathMap) {
		for (auto& path : paths.second) {
			if (path.second._isEnable) {
				path.second.OnAfterExcuteInitializer(D3D12System::GetCommandList(paths.first.second));
			}
		}
	}
}

std::weak_ptr<K3D12::GraphicsCommandList> K3D12::RenderingManager::GetRenderingCommandList(std::string pathName, std::string childPathName)
{
	return std::weak_ptr<GraphicsCommandList>();
}

std::weak_ptr<K3D12::GraphicsCommandList> K3D12::RenderingManager::GetRenderingCommandList(unsigned int pathIndex, unsigned int childPathIndex)
{
	return std::weak_ptr<GraphicsCommandList>();
}

std::weak_ptr<K3D12::GraphicsCommandList> K3D12::RenderingManager::GetRenderingCommandList(unsigned int pathIndex, std::string childPathName)
{
	return std::weak_ptr<GraphicsCommandList>();
}

std::weak_ptr<K3D12::GraphicsCommandList> K3D12::RenderingManager::GetRenderingCommandList(std::string pathName, unsigned int childPathIndex)
{
	return std::weak_ptr<K3D12::GraphicsCommandList>();
}

unsigned int K3D12::RenderingManager::GetRenderingPathIndex(std::string pathName, std::string childPathName)
{
	return 0;
}

unsigned int K3D12::RenderingManager::GetRenderingPathIndex(std::string pathName, unsigned int childPathIndex)
{
	return 0;
}

std::string K3D12::RenderingManager::GetRenderingPathName(unsigned int pathIndex, std::string childPathName)
{
	return std::string();
}

std::string K3D12::RenderingManager::GetRenderingPathName(unsigned int pathIndex, unsigned int childPathIndex)
{
	return std::string();
}

void K3D12::RenderingManager::SetActiveMasterPath(std::string masterPathName, bool isEnable)
{
}

void K3D12::RenderingManager::SetActiveChildPath(std::string masterPathName, std::string childPathName, bool isEnable)
{
}

std::pair<unsigned int, std::weak_ptr<K3D12::GraphicsCommandList>> K3D12::RenderingManager::RegistRenderingPath(std::string name)
{
	return std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>();
}

std::pair<unsigned int, std::weak_ptr<K3D12::GraphicsCommandList>> K3D12::RenderingManager::InsertRenderingPath(std::string name, unsigned int insertPathIndex)
{
	return std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>();
}

