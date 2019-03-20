#include <memory>
#include "GameObject.h"
#include "../Transform/Transform.h"
#include "../../CommandContext/GraphicsCommandList.h"
#include "../../Util/Utility.h"
#include "../../Util/Math.h"

void K3D12::GameObject::UpdateTransformBuffer()
{


	auto mat = Matrix::CreateFromQuaternion(this->_transform.GetRotation());
	mat.axisW.x = GetPos().x;
	mat.axisW.y = GetPos().y;
	mat.axisW.z = GetPos().z;
	mat.axisW.w = 1.0f;

	mat = GetSRTMatrix();

	_transformBuffer.Update(&mat, _initializeSize, 0);
}

void K3D12::GameObject::InitalizeTransformBuffer(size_t size)
{
	_initializeSize = static_cast<unsigned int>(size);
	_transformBuffer.Create(size);
}

void K3D12::GameObject::SetTransform(int rootParamaterIndex, std::shared_ptr<GraphicsCommandList> list)
{
	list->GetCommandList()->SetGraphicsRootConstantBufferView(rootParamaterIndex, _transformBuffer.GetResource()->GetGPUVirtualAddress());
}

void K3D12::GameObject::SetPos(const Vector3 pos)
{
	_transform.SetPos(pos);
}

void K3D12::GameObject::SetRotation(const Quaternion rotation)
{
	_transform.SetRotation(rotation);
}

void K3D12::GameObject::SetEulerAngles(const Vector3 euler)
{
	_transform.SetEulerAngles(euler);
}

void K3D12::GameObject::SetScale(const Vector3 scale)
{
	_scale = scale;
}

Vector3 K3D12::GameObject::GetPos()
{
	return _transform.GetPos();
}

Quaternion K3D12::GameObject::GetRotation()
{
	return _transform.GetRotation();
}

Vector3 K3D12::GameObject::GetEulerAngles()
{
	return _transform.GetEulerAngles();
}

Vector3 K3D12::GameObject::GetScale()
{
	return _scale;
}

void K3D12::GameObject::Enable()
{
	_isEnable = true;
}

void K3D12::GameObject::Disable()
{
	_isEnable = false;
}

bool K3D12::GameObject::IsEnable()
{
	return _isEnable;
}

Matrix K3D12::GameObject::GetSRTMatrix()
{
	auto mat = std::move(Matrix::CreateFromQuaternion(_transform.GetRotation()));

	mat = std::move(Matrix::CreateScaleMatrix(_scale) * mat);
	mat.axisW.x = _transform.GetPos().x;
	mat.axisW.y = _transform.GetPos().y;
	mat.axisW.z = _transform.GetPos().z;
	mat.axisW.w = 1.0f;
	return mat;
}

void K3D12::GameObject::Translate(const Vector3 & velocity)
{
	_transform.Translate(velocity);
}

void K3D12::GameObject::Move(const Vector3 & velocity)
{
	_transform.Move(velocity);

}

void K3D12::GameObject::Rotation(const Quaternion & rot)
{
	_transform.Rotation(rot);
}

void K3D12::GameObject::RotationEulerAngles(const Vector3 & eulerAngles)
{
	_transform.RotationEulerAngles(eulerAngles);
}

void K3D12::GameObject::RotationAxisAngles(const Vector3 & axis, float rad)
{
	_transform.RotationAxisAngles(axis, rad);
}

void K3D12::GameObject::RotateAround(const Vector3 & point, const  Quaternion & rot)
{
	_transform.RotateAround(point, rot);
}

void K3D12::GameObject::RotateAround(const Vector3 & point, const Vector3 & axis, float rad)
{
	_transform.RotateAround(point, axis, rad);
}

void K3D12::GameObject::LookAt(const Vector3 & point, const Vector3 & up)
{
	_transform.LookAt(point, up);
}

OrthonormalBasis K3D12::GameObject::GetLocalAxis()
{
	return _transform.GetLocalAxis();
}

Matrix K3D12::GameObject::GetView()
{
	return _transform.GetView();
}


K3D12::GameObject::GameObject() :
	_transform({}), _isEnable(true), _scale(Vector3::one)
{

}

K3D12::GameObject::GameObject(K3D12::Transform transform) :
	_transform(transform)
{
}


K3D12::GameObject::~GameObject()
{
	_transformBuffer.Discard();
}