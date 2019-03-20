#include <string>
#include "Factory.h"
#include "../Util/Logger.h"


K3D12::Factory::Factory() :
	_factory()
{
}


K3D12::Factory::~Factory()
{
	_factory.Reset();
}


HRESULT K3D12::Factory::Create(UINT createFlags)
{
	UINT flag = 0;
#if defined(_DEBUG)
	// DirectX12のデバッグレイヤーを有効にする
	{
		Microsoft::WRL::ComPtr<ID3D12Debug>	debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

		}
	}
	createFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DirectX12がサポートする利用可能なハードウェアアダプタを取得
	if (FAILED(CreateDXGIFactory2(createFlags, IID_PPV_ARGS(&_factory))))
		return FALSE;
	return S_OK;
}

IDXGIFactory4 * K3D12::Factory::GetFactory()const
{
	return _factory.Get();
}

void K3D12::Factory::Discard()
{
	if (_factory != nullptr) {
		_factory.Reset();
		SystemLogger::GetInstance().Log(LogLevel::Debug,"Factory is Reset\n");
	}
}

