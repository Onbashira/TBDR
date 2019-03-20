#include "Sphere.h"
#include "../../Util/Math.h"
#include "../../Model/ModelMesh.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../RendererMaterial.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"
#include "PrimitiveComponent.h"
#include "PrimitiveObject.h"

Vector3 ConvertRTPToXYZ(float r, float theta, float phi);

K3D12::Sphere::Sphere()
{
}


K3D12::Sphere::~Sphere()
{
}


void K3D12::Sphere::Initializer()
{
	this->_transformStartPoint = 0;
	this->_materialStartPoint = _transformStartPoint + 1;
	this->_textureStartPoint = _materialStartPoint + 1;

	//PolygonCreation
	{
		unsigned int cornerNum = 20;
		float rad = 10.0f;

		float angleY = 0.0f;
		float angleX = 0.0f;
		float theta1 = 0.0f, theta2;
		float phi1 = 0.0f, phi2;
		float angleDelta = 0.0f;

		angleDelta = 2.0f * F_PI / cornerNum;
		theta2 = theta1 + angleDelta;
		phi2 = phi1 + angleDelta;
		Vertex3D plane[4];

		for (unsigned int i = 0; i < cornerNum / 2; i++) {
			plane[0].pos = ConvertRTPToXYZ(rad, theta1, phi1);
			plane[0].normal = (plane[0].pos).Normalize();
			plane[1].pos = ConvertRTPToXYZ(rad, theta1, phi2);
			plane[1].normal = (plane[1].pos).Normalize();

			plane[2].pos = ConvertRTPToXYZ(rad, theta2, phi1);
			plane[2].normal = (plane[2].pos).Normalize();

			plane[3].pos = ConvertRTPToXYZ(rad, theta2, phi2);
			plane[3].normal = (plane[3].pos).Normalize();

			theta1 += angleDelta;
			theta2 += angleDelta;
		}
		phi1 += angleDelta;
		phi2 += angleDelta;
	}

	// Resource Creation
	{
		this->_indexBuffer->Create(this->_info._list.size() * sizeof(unsigned int), sizeof(unsigned int), &this->_info._list[0]);
		this->_vertexBuffer->Create(this->_info._vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &this->_info._vertexes[0]);
		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		_materialBuffer.Create(Util::Alignment256Bytes(sizeof(RendererMaterial)));
		_materialBuffer.SetName("PrimitveMaterialBuffer");
	}

	//Material Update
	this->_info._materialInfo.ambient = Vector3::zero;
	this->_info._materialInfo.diffuse = Vector4(Vector3::one, 1.0f);
	this->_info._materialInfo.specular = Vector4(Vector3::zero, 0.0f);
	this->_info._materialInfo.emissive = Vector3(Vector3::one) / 10.0f;
	_materialBuffer.Update(&this->_info._materialInfo, sizeof(RendererMaterial), 0);


	//TransformUpdate
	SetScale(Vector3::one);
	Translate(Vector3::zero);
	SetRotation(Quaternion::CreateFromEulerAngles(Vector3::zero));
	GameObject::UpdateTransformBuffer();


	//HeapCreation

	// 0 = Transform,1 = materialInfo,2 = ShaderTexture(default is null)
	_heap->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);

	//ViewCreation
	{

		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _transformBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

			_transformBuffer.CreateView(view, _heap->GetCPUHandle(_transformStartPoint));
		}

		{

			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _materialBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(RendererMaterial)));

			_materialBuffer.CreateView(view, _heap->GetCPUHandle(_materialStartPoint));
		}

		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = _info._shaderResource.lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			_info._shaderResource.lock()->CreateView(srvDesc, _heap->GetCPUHandle(_textureStartPoint));
		}
	}
	//SetBundleDrawCall
	{
		RegistBundle();
	}
}

void K3D12::Sphere::RegistBundle()
{
	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindingIndexBuffer(&_bundleList);
	BindingVertexBuffer(&_bundleList);
	BindingDescriptorHeaps(&_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(1, _heap->GetGPUHandle(_transformStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(2, _heap->GetGPUHandle(_materialStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(3, _heap->GetGPUHandle(_textureStartPoint));

	_bundleList.GetCommandList()->DrawIndexedInstanced(static_cast<unsigned int>(this->_info._list.size()), 1, 0, 0, 0);

	_bundleList.GetCommandList()->Close();
}

void K3D12::Sphere::SetPipelineState(PipelineStateObject * pipelineState)
{
	_pipelineState.reset(pipelineState);

}

void K3D12::Sphere::SetRootSignature(RootSignature * rootSignature)
{
	_rootSignature.reset(rootSignature);

}

void K3D12::Sphere::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(0, D3D12System::GetInstance().GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	ID3D12DescriptorHeap* heap[] = { _heap->GetPtr() };
	this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());

}

void K3D12::Sphere::Update()
{
	//GameObject::UpdateTransformBuffer();
}

void K3D12::Sphere::LateUpdate()
{
}

Vector3 ConvertRTPToXYZ(float r, float theta, float phi)
{
	Vector3	ret;

	ret.x = r * sin(theta)*cos(phi);
	ret.y = r * cos(theta);
	ret.z = r * sin(theta)*sin(phi);

	return ret;
}
