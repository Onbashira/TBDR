#include <memory>
#include "ModelMesh.h"
#include "../Resource/VertexBuffer.h"
#include "../Resource/IndexBuffer.h"
#include "../CommandContext/GraphicsCommandList.h"

#include "../DescriptorHeap/DescriptorHeap.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"


K3D12::ModelMesh::ModelMesh() :
	_vertexBuffer(std::make_unique<VertexBuffer>()),
	_indexBuffer(std::make_unique< IndexBuffer>()),
	_heap(std::make_unique< DescriptorHeap>()),
	_textureStartPoint(0),
	_materialStartPoint(0)
{
	
}


K3D12::ModelMesh::~ModelMesh()
{
	_vertexBuffer->Discard();
	_vertexBuffer.reset();
	_vertexBuffer.release();
	_indexBuffer->Discard();
	_indexBuffer.reset();
	_indexBuffer.release();
	_heap->Discard();
	_heap.reset();
	_textureStartPoint = 0;
	_materialStartPoint = 0;
}


void K3D12::ModelMesh::BindingVertexBuffer(GraphicsCommandList * list)
{
	list->GetCommandList()->IASetVertexBuffers(0,1,&_vertexBuffer->GetView());
}

void K3D12::ModelMesh::BindingIndexBuffer(GraphicsCommandList * list)
{
	list->GetCommandList()->IASetIndexBuffer(&_indexBuffer->GetView());
}

void K3D12::ModelMesh::BindingDescriptorHeaps(GraphicsCommandList * list)
{
	ID3D12DescriptorHeap* heaps[] = { this->_heap->GetPtr() };

	list->GetCommandList()->SetDescriptorHeaps(1, heaps);
}

void K3D12::ModelMesh::SetTextureCount(unsigned int textureCount)
{
	_textureStartPoint = textureCount;
}

void K3D12::ModelMesh::SetMaterialCount(unsigned int materialCount)
{
	_materialStartPoint = materialCount;
}
