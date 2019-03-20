#include <memory>
#include <chrono>
#include <sstream>

#include "MMDModel.h"
#include "../../DescriptorHeap/DescriptorHeap.h"
#include "../../CommandContext/GraphicsCommandList.h"
#include "../../Util/Utility.h"
#include "../../System/D3D12System.h"
#include "../../GameConponent/GameObject/GameObject.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"
#include "../../Util/Logger.h"

K3D12::MMDModel::MMDModel()
{
}


K3D12::MMDModel::~MMDModel()
{
	_materialBuffer.Discard();
}


Matrix K3D12::MMDModel::CulcBDEF1(WeightDeform & deform)
{
	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef1.boneIndex]);
}

Matrix K3D12::MMDModel::CulcBDEF2(WeightDeform & deform)
{

	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef2.boneIndex01] * deform.bdef2.boneWeight01 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef2.boneIndex02] * deform.bdef2.boneWeight02);
}

Matrix K3D12::MMDModel::CulcBDEF4(WeightDeform & deform)
{
	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex01] * deform.bdef4.boneWeight01 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex02] * deform.bdef4.boneWeight02 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex03] * deform.bdef4.boneWeight03 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex04] * deform.bdef4.boneWeight04);
}

std::pair<Vector3, float> K3D12::MMDModel::CulcSDEF(WeightDeform & deform)
{
	float w1 = 0.0;
	float w2 = 0.0f;
	auto& skeltonRef = _resourceData.lock()->_boneTree;
	auto a = CulcSDEFWaight(deform.sdef.r0 + deform.sdef.c, deform.sdef.r1 + deform.sdef.c);

	//ウェイト値の代入
	w1 = a.first;
	w2 = a.second;
	//R0値R1値
	Vector3 r0 = Vector3(deform.sdef.r0 + deform.sdef.c);
	Vector3 r1 = Vector3(deform.sdef.r1 + deform.sdef.c);

	auto mat01 = skeltonRef->bonesMatrix[deform.sdef.boneIndex01] * deform.sdef.boneWeight01;
	auto mat02 = skeltonRef->bonesMatrix[deform.sdef.boneIndex02] * deform.sdef.boneWeight02;

	Matrix rotMat = mat01 * w1 + mat02 * w2;

	//C値の算出
	auto prC = Vector3::Transform(deform.sdef.c, rotMat);

	//加算処理
	{
		Matrix mat = mat01 + mat02;
		Vector3 v0 = Vector3::Transform(r0, mat02 + mat * deform.sdef.boneWeight01) - Vector3::Transform(r0, mat);
		Vector3 v1 = Vector3::Transform(r1, mat01 + mat * deform.sdef.boneWeight01) - Vector3::Transform(r1, mat);

		prC += v0 * w1 + v1 * w2;
	}

	std::pair<Vector3, float> ret;
	ret.first = prC;
	ret.second = w2;

	return ret;
}

Matrix K3D12::MMDModel::CulcQDEF(WeightDeform & deform)
{

	return Matrix();
}

std::pair<float, float> K3D12::MMDModel::CulcSDEFWaight(Vector3 r0, Vector3 r1)
{
	float l0 = r0.Length();
	float l1 = r1.Length();
	auto ret = std::pair<float, float>(0.0f, 0.0f);
	if (abs(l0 - l1) < 0.00001f) {
		ret.first = 0.5f;
	}
	else {
		ret.first = ::Saturate(l0 / (l0 + l1));
	}
	ret.second = 1.0f - ret.first;
	return ret;
}

void K3D12::MMDModel::TransformUpdate()
{
	//移動量回転量スケール値のコミット
}

void K3D12::MMDModel::AnimationUpdate()
{

	//ボーン情報から頂点ブレンディング
	for (unsigned int i = 0; i < _vertexes.size(); ++i) {
		Matrix rotMat;
		auto vertex = this->_vertexes[i];
		auto& deform = _resourceData.lock()->_vertexes[i].deformation;
		switch (_resourceData.lock()->_vertexes[i].deformType)
		{
		case MMDWeightDeformType::BDEF1:
			deform.bdef1;
			rotMat = CulcBDEF1(deform);
			vertex.pos = Vector3::Transform(vertex.pos, rotMat);
			vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
			_vertexes[i] = vertex;
			break;
		case MMDWeightDeformType::BDEF2:
			deform.bdef2;
			rotMat = CulcBDEF2(deform);
			vertex.pos = Vector3::Transform(vertex.pos, rotMat);
			vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
			_vertexes[i] = vertex;

			break;
		case MMDWeightDeformType::BDEF4:
			deform.bdef4;

			rotMat = rotMat = CulcBDEF4(deform);
			vertex.pos = Vector3::Transform(vertex.pos, rotMat);
			vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
			_vertexes[i] = vertex;

			break;
		case MMDWeightDeformType::SDEF:
			deform.sdef;
			{
				auto& skeltonRef = _resourceData.lock()->_boneTree;
				auto mcPair = CulcSDEF(deform);
				Vector4 q0 = Quaternion::CreateFromRotationMatrix(skeltonRef->bonesMatrix[deform.sdef.boneIndex01]);
				Vector4 q1 = Quaternion::CreateFromRotationMatrix(skeltonRef->bonesMatrix[deform.sdef.boneIndex02]);
				Quaternion q = Quaternion::Slerp(q0, q1, mcPair.second);

				vertex.pos = mcPair.first + Vector3::Rotate(vertex.pos - deform.sdef.c, q);
				vertex.normal = Vector3::Rotate(vertex.normal, q).Normalize();

			}
			_vertexes[i] = vertex;
			break;
		case MMDWeightDeformType::QDEF:
			deform.qdef;

			break;
		default:
			break;
		}
	}
	this->_vertexBuffer->Map(0, nullptr);
	this->_vertexBuffer->Update(&this->_vertexes[0], _vertexes.size() * sizeof(Vertex3D), 0);
	this->_vertexBuffer->Unmap(0, nullptr);
}

void K3D12::MMDModel::SkeltonReset()
{
	_animator.ResetSkelton();
}

void K3D12::MMDModel::SetAnimationData(std::string motionPath)
{

}

void K3D12::MMDModel::SetCommandList(std::weak_ptr<GraphicsCommandList>& commandList)
{
	this->_commandList = commandList;
}

void K3D12::MMDModel::SetPipelineState(PipelineStateObject * pso)
{
	this->_pipelineState.reset(pso);
}

void K3D12::MMDModel::SetRootSignature(RootSignature * rootSignature)
{
	this->_rootSignature.reset(rootSignature);

}

void K3D12::MMDModel::RegistBundle()
{
	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindingIndexBuffer(&_bundleList);
	BindingVertexBuffer(&_bundleList);
	BindingDescriptorHeaps(&_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TRANSFORM, _heap->GetGPUHandle(this->_transformStartPoint));
	unsigned int surfaceOffset = 0;

	for (UINT index = 0; index < _resourceData.lock()->_materials.size(); index++) {

		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_MATERIAL, _heap->GetGPUHandle(_materialStartPoint + index));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TEXTURE, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].textureTableIndex));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TOON_MAP, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].toonIndex));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].sphereIndex));

		_bundleList.GetCommandList()->DrawIndexedInstanced(_resourceData.lock()->_materials[index].surfaceCount, 1, surfaceOffset, 0, 0);
		surfaceOffset += _resourceData.lock()->_materials[index].surfaceCount;
	};

	_bundleList.CloseCommandList();
}

void K3D12::MMDModel::Initialize()
{
	InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
}

void K3D12::MMDModel::Update()
{
	GameObject::UpdateTransformBuffer();

}

void K3D12::MMDModel::LateUpdate()
{
	AnimationUpdate();
	SkeltonReset();
	//this->_vertexBuffer->Map(0, nullptr);
	//this->_vertexBuffer->Update(&_resourceData.lock()->_originVert[0], _resourceData.lock()->_originVert.size() * sizeof(Vertex3D), 0);
	//this->_vertexBuffer->Unmap(0, nullptr);
	for (int i = 0;i < 	this->_vertexes.size();++i) {
		this->_vertexes[i] = this->_resourceData.lock()->_originVert[i];
	}
}

void K3D12::MMDModel::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->BindingShaderObject();
	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(SHADER_ROOT_PARAMATER_INDEX_CAMERA, D3D12System::GetInstance().GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	BindingDescriptorHeaps(_commandList.lock().get());
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());

}

void K3D12::MMDModel::TestDraw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObject();
	BindingIndexBuffer(_commandList.lock().get());
	BindingVertexBuffer(_commandList.lock().get());
	BindingDescriptorHeaps(_commandList.lock().get());
	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(SHADER_ROOT_PARAMATER_INDEX_CAMERA, D3D12System::GetInstance().GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());

	_commandList.lock()->GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TRANSFORM, _heap->GetGPUHandle(this->_transformStartPoint));
	unsigned int surfaceOffset = 0;
	for (UINT index = 0; index < _resourceData.lock()->_materials.size(); index++) {

		_commandList.lock()->GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_MATERIAL, _heap->GetGPUHandle(_materialStartPoint + index));
		_commandList.lock()->GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TEXTURE, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].textureTableIndex));
		_commandList.lock()->GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TOON_MAP, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].toonIndex));
		_commandList.lock()->GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE, _heap->GetGPUHandle(_textureStartPoint + _resourceData.lock()->_materials[index].sphereIndex));

		_commandList.lock()->GetCommandList()->DrawIndexedInstanced(_resourceData.lock()->_materials[index].surfaceCount, 1, surfaceOffset, 0, 0);
		surfaceOffset += _resourceData.lock()->_materials[index].surfaceCount;
	};
}

K3D12::Animator & K3D12::MMDModel::Animator()
{
	return _animator;
}

void K3D12::MMDModel::SetModelSkelton()
{
	_animator.SetModelSkelton(this->_resourceData.lock()->_boneTree);
}
