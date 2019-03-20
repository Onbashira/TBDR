#include <memory>
#include "Light.h"
#include "../../CommandContext/GraphicsCommandList.h"
#include "../../Resource/Resource.h"
#include "../../Util/Utility.h"

void K3D12::Light::Update()
{
}

void K3D12::Light::SetClusterIndex(unsigned int clusterIndex)
{
	this->_clusterIndex = clusterIndex;
}

unsigned int K3D12::Light::GetClusterIndex()
{
	return this->_clusterIndex;
}

const K3D12::LightInfo & K3D12::Light::GetInfo()
{
	return _info;
}

void K3D12::Light::SetInfo(K3D12::LightInfo & info)
{
	_info = info;
}

void K3D12::Light::WriteToResourceOffset(K3D12::Resource * resource, unsigned int offset)
{
	resource->Update(&this->_info, sizeof(K3D12::LightInfo), offset);
}

void K3D12::Light::WriteToResourceIndex(K3D12::Resource * resource, unsigned int index)
{
	resource->Update(&this->_info, sizeof(K3D12::LightInfo), sizeof(K3D12::LightInfo) * index);
}

void K3D12::Light::WriteToResource256ByteAlignment(K3D12::Resource * resource, unsigned int offset)
{
	resource->Update(&this->_info, Util::Alignment256Bytes(sizeof(K3D12::LightInfo)), offset);
}

K3D12::Light::Light(K3D12::LightInfo lightInfo)
	: _info(lightInfo),GameObject()
{
	//位置セット
	this->_transform.SetPos(_info.pos);
	//ライトが向いている方向から回転行列を取得
}

K3D12::Light::Light() :
	_info(), GameObject()
{
}


K3D12::Light::~Light()
{

}

