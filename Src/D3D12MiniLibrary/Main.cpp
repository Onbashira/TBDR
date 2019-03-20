#include <vector>
#include "./Engine/System/D3D12System.h"
#include "./Engine/Input/GamePad.h"
#include "./Engine/GameConponent/Camera/Camera.h"
#include "./Engine/Input/InputManager.h"
#include "./Engine/GameConponent/Primitive/Cube.h"
#include "./Engine/Util/Math.h"
#include "./Engine/Resource/UnorderdAccessView.h"
#include "Engine/Model/MMDModel/MMDModel.h"
#include "Engine/GameConponent/Primitive/Plane.h"
int main() {

	K3D12::D3D12System::SetWindowSize(1280, 720);
	K3D12::D3D12System::SetWindowName(L"1601279_¬—ÑŠJl_D3D12");
	K3D12::D3D12System::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraType::Perspective, 10.0f * Vector3(0.0f, 2.0f, -2.0f), Vector3(0.0f, 10.0f, 0.0f), Vector3::up, 0.1f, 500.0f, 0.0f);
	K3D12::D3D12System::InitializeController();

	auto model = K3D12::D3D12System::LoadModel("./ModelData/Model/‚Ü‚ñ‚¼‚­ˆ©‚¿‚á‚ñ_ver1.03/‚Ü‚ñ‚¼‚­ˆ©‚¿‚á‚ñ_ver1.03.pmx");
	model.lock()->Animator().AttachAnimation("Walk", "./ModelData/Motion/Walk.vmd");
	model.lock()->SetModelSkelton();
	model.lock()->RotationEulerAngles(Vector3(0.0f, 0.0f, 0.0f));
	K3D12::Plane* plane = K3D12::D3D12System::CreatePlane();
	plane->SetScale(Vector3(200.0f, 1.0f, 200.0f));
	plane->Update();
	float t = 0.0f;
	while (K3D12::D3D12System::MessageLoop() == 0) {
		K3D12::D3D12System::GetInput().InputUpdate();
		K3D12::D3D12System::ClearScreen();
		K3D12::D3D12System::GetCamera().DebugMove(K3D12::D3D12System::GetInput());
		K3D12::D3D12System::GetCamera().DebugRotate(K3D12::D3D12System::GetInput());
		K3D12::D3D12System::GetCamera().Update();
		//model.lock()->Animator().PlayAnimation("Walk");
		model.lock()->RotationEulerAngles(Vector3(0.0f,1.0f,0.0f));
		model.lock()->Update();
		//model.lock()->LateUpdate();
		model.lock()->Draw();
		plane->Draw();
		K3D12::D3D12System::ScreenFlip();
	}
	delete plane;
	K3D12::D3D12System::Destroy();
	return 0;
};
