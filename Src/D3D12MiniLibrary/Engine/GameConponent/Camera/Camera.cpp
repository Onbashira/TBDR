#include <memory>
#include "../../Resource/ConstantBuffer.h"
#include "Camera.h"
#include "../../CommandContext/GraphicsCommandList.h"
#include "../../Resource/DepthStencilBuffer.h"
#include "../../Util/Utility.h"

K3D12::Camera::Camera() :
	GameObject(), _type(CameraType::Perspective)
{
}


K3D12::Camera::~Camera()
{

}


void K3D12::Camera::InitializeCamera(CameraType type, const float width, const float height, const float nearClip, const float farClip,const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	switch (type)
	{
	case CameraType::Perspective:
		initializePerspective(width, height, nearClip, farClip, position, target, upWard);
		break;
	case CameraType::Orthogonal:
		InitializeOrthogonal(width, height, nearClip, farClip, position, target, upWard);
		break;
	default:
		break;
	}
}

void K3D12::Camera::InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	_fov = DegToRad(fov);
	_type = CameraType::Perspective;
	_aspectRatio = height / width;
	auto mat = Matrix::ExtractRotationMatrix(Matrix::CreateLookAt(position, target, upWard));
	SetRotation(Quaternion::CreateFromRotationMatrix(mat));
	this->_projection = Matrix::CreatePerspectiveFOV(fov, _aspectRatio, nearClip, farClip);
}

HRESULT K3D12::Camera::InitializeCameraDepthStencill(DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight)
{
	auto hr = _depthStencillRersource.Create(windowWidth, windowHeight, depthFormat, depthFormat);
	_depthStencillRersource.SetName("CameraDepthStencill");
	return hr;

}

void K3D12::Camera::SetConstantBuffer(GraphicsCommandList * list, UINT paramaterIndex)
{
	list->GetCommandList()->SetGraphicsRootConstantBufferView(paramaterIndex, _cameraMatrixBuffer.GetResource()->GetGPUVirtualAddress());
}

void K3D12::Camera::Discard()
{

	_depthStencillRersource.Discard();
	_cameraMatrixBuffer.Discard();

}

HRESULT K3D12::Camera::CreateBuffer()
{
	auto hr = _cameraMatrixBuffer.Create(Util::Alignment256Bytes(sizeof(CameraInfo)));
	if (FAILED(hr)) {
		return hr;
	}
	_cameraMatrixBuffer.SetName("CameraMatrixBuffer");
	return hr;
}


HRESULT K3D12::Camera::InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	_fov = 0;
	_type = CameraType::Orthogonal;
	_aspectRatio = width / height;
	SetScale(Vector3::one);
	SetPos(position);

	auto mat = Matrix::ExtractRotationMatrix(Matrix::CreateLookAt(position, target, upWard));
	SetRotation(Quaternion::CreateFromRotationMatrix(mat));
	this->_projection = Matrix::CreateOrthographic(width, height, nearClip, farClip);


	if (FAILED(CreateBuffer())) {
		return E_FAIL;
	}
	Update();

	return S_OK;

}

HRESULT K3D12::Camera::initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	_fov = 0;
	_type = CameraType::Perspective;
	this->_near = nearClip;
	this->_far = farClip;
	this->_windowHeight = height;
	this->_windowWidth = width;

	_aspectRatio = width / height;
	SetScale(Vector3::one);
	SetPos(position);
	auto mat = Matrix::CreateLookAt(position, target, upWard);

	SetRotation(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix(mat)));

	mat = Matrix::Invert(mat);

	this->_projection = Matrix::CreatePerspectiveFOV(DegToRad(60), _aspectRatio, nearClip, farClip);

	this->_info.projection = this->_projection;
	this->_info.view = mat;
	this->_info.windowHeight = this->_windowHeight;
	this->_info.windowWidth = this->_windowWidth;

	if (FAILED(CreateBuffer())) {
		return E_FAIL;
	}

	Update();
	return S_OK;
}

K3D12::CameraType K3D12::Camera::GetType()
{
	return _type;
}

const Matrix & K3D12::Camera::GetProjection()
{
	return this->_projection;
}

const Matrix  K3D12::Camera::GetViewProjection()
{
	return Matrix::Multiply(GetView(), _projection);
}

K3D12::CameraInfo K3D12::Camera::GetCameraInfo()
{
	return this->_info;
}

K3D12::ConstantBuffer & K3D12::Camera::GetCameraBuffer()
{
	return this->_cameraMatrixBuffer;
}

K3D12::DepthStencil & K3D12::Camera::GetDepthStencil()
{
	return _depthStencillRersource;
}

float K3D12::Camera::GetFov()
{
	return _fov;
}

float K3D12::Camera::GetNearClip()
{
	return -(_projection._43 / _projection._33);
}

float K3D12::Camera::GetFarClip()
{
	return -(_projection._43 / (_projection._33 - 1.0f));
}

void K3D12::Camera::Update()
{
	CameraInfo cameraMat{};
	//std::cout << "POSITION : X = " << GetPos().x << std::endl;
	//std::cout << "POSITION : Y = " << GetPos().y << std::endl;
	//std::cout << "POSITION : Z = " << GetPos().z << std::endl;

	//std::cout << "ANGLE : X = " << GetEulerAngles().x << " ";
	//std::cout << "ANGLE : Y = " << GetEulerAngles().y << " ";
	//std::cout << "ANGLE : Z = " << GetEulerAngles().z << std::endl;

	////std::cout << "Rot : X = " << GetRotation().x << " ";
	////std::cout << "Rot : Y = " << GetRotation().y << " ";
	////std::cout << "Rot : Z = " << GetRotation().z << " ";
	////std::cout << "Rot : W = " << GetRotation().w << std::endl;




	this->_info.projection = this->_projection;
	this->_info.view = this->GetView();
	this->_info.windowHeight = this->_windowHeight;
	this->_info.windowWidth = this->_windowWidth;

	_cameraMatrixBuffer.Update(&this->_info, sizeof(CameraInfo), 0);
}

void K3D12::Camera::DebugMove(K3D12::InputManager & input)
{
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_UP)) {
		Move(Vector3::forward);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_DOWN)) {
		Move(Vector3::back);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_RIGHT)) {
		Move(Vector3::right);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_LEFT)) {
		Move(Vector3::left);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_SPACE)) {
		Move(Vector3::up);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_SHIFT)) {
		Move(Vector3::down);
	}
}

void K3D12::Camera::DebugRotate(InputManager & input)
{
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_W)) {
		RotationAxisAngles(GetLocalAxis().u, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_S)) {
		RotationAxisAngles(GetLocalAxis().u, DegToRad(-1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_D)) {
		RotationAxisAngles(GetLocalAxis().v, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_A)) {
		RotationAxisAngles(GetLocalAxis().v, DegToRad(-1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_Q)) {
		RotationAxisAngles(GetLocalAxis().w, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_E)) {
		RotationAxisAngles(GetLocalAxis().w, DegToRad(-1.0f));
	}
}




