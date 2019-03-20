#include <memory>
#include "RenderingPathObject.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../Util/Utility.h"

K3D12::RenderingPathObject::RenderingPathObject()
{
}


K3D12::RenderingPathObject::~RenderingPathObject()
{

}

void K3D12::RenderingPathObject::OnStartInitializer(std::weak_ptr<GraphicsCommandList> list)
{
	this->_onStartInitializerFunc(list);
}

void K3D12::RenderingPathObject::OnBeforExcuteInitializer(std::weak_ptr<GraphicsCommandList> list)
{
	this->_onBeforExcuteInitializerFunc(list);

}

void K3D12::RenderingPathObject::OnAfterExcuteInitializer(std::weak_ptr<GraphicsCommandList> list)
{
	this->_onAfterExcuteInitializerFunc(list);

}

void K3D12::RenderingPathObject::SetOnStartInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>list)> initilalizer)
{
	_onStartInitializerFunc = initilalizer;
}

void K3D12::RenderingPathObject::SetOnBeforExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>list)> initilalizer)
{
	_onBeforExcuteInitializerFunc = initilalizer;
}

void K3D12::RenderingPathObject::SetOnAfterExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>list)> initilalizer)
{
	_onAfterExcuteInitializerFunc = initilalizer;
}

void K3D12::RenderingPathObject::SetEnable(bool isEnable)
{
	_isEnable = isEnable;
}
