#include <memory>
#include "PrimitiveCreater.h"
#include "../../CommandContext/GraphicsCommandList.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"


K3D12::PrimitiveCreater::PrimitiveCreater()
{
}


K3D12::PrimitiveCreater::~PrimitiveCreater()
{
	_graphicsPipelineState.reset();
	_rootSignature.reset();
	this->_primitiveDrawCommandList.reset();
}


K3D12::Cube * K3D12::PrimitiveCreater::CreateCube()
{
	auto ptr = new K3D12::Cube();
	ptr->SetRootSignature(_rootSignature.lock().get());
	ptr->SetPipelineState(_graphicsPipelineState.lock().get());
	ptr->SetMasterCommandList(_primitiveDrawCommandList.lock());
	ptr->Initializer();
	return ptr;
}

K3D12::Sphere * K3D12::PrimitiveCreater::CreateSphere()
{
	return new K3D12::Sphere();
}

K3D12::Plane * K3D12::PrimitiveCreater::CreatePlane()
{
	auto ptr = new K3D12::Plane();
	ptr->SetRootSignature(_rootSignature.lock().get());
	ptr->SetPipelineState(_graphicsPipelineState.lock().get());
	ptr->SetMasterCommandList(_primitiveDrawCommandList.lock());

	ptr->Initializer();
	return ptr;
}

void K3D12::PrimitiveCreater::SetPrimitiveRootSignature(std::weak_ptr<K3D12::RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
}

void K3D12::PrimitiveCreater::SetPrimitivePipelineState(std::weak_ptr<K3D12::PipelineStateObject> graphicsPipelineState)
{
	_graphicsPipelineState = graphicsPipelineState;
}

void K3D12::PrimitiveCreater::SetPrimitiveDrawCommandList(std::weak_ptr<K3D12::GraphicsCommandList> primitiveDrawCommandList)
{
	this->_primitiveDrawCommandList = primitiveDrawCommandList;

}


