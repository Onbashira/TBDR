#include <string>
#include "../Util/D3D12Common.h"
#include "D3D12Device.h"
#include "../Factory/Factory.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"

//対応フィーチャーレベル
const D3D_FEATURE_LEVEL FeatureLevels[] = 
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0 
};
const std::string GPU_CARD_SPEC_LIST[] = {
	"NVIDIA",
	"INTEL"

};


K3D12::D3D12Device::D3D12Device() :
	_device(),_featureLevel(),_useWarpDevice(false)
{
}


K3D12::D3D12Device::~D3D12Device()
{
	Discard();
}


HRESULT K3D12::D3D12Device::Create(Factory* factory,bool useWarpDevice)
{
	_useWarpDevice = useWarpDevice;
	if (_useWarpDevice) {
		Microsoft::WRL::ComPtr<IDXGIAdapter>	warpAdapter;

		if (FAILED(factory->GetFactory()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)))) {
			return E_FAIL;
		}
		for (auto i : FeatureLevels) {
			if (SUCCEEDED(D3D12CreateDevice(warpAdapter.Get(), i, IID_PPV_ARGS(&_device)))) {
				_featureLevel = i;
				break;
			}
		}

	}
	else {
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	hardwareAdapter;
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
		hardwareAdapter = nullptr;

		for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->GetFactory()->EnumAdapters1(i, &adapter); i++) {
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
				continue;

			for (auto i : FeatureLevels) {
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), i, _uuidof(ID3D12Device3), nullptr))) {
					_featureLevel = i;
					hardwareAdapter = adapter;
					break;
				}
			}
			break;
		}
		adapter.Reset();
		
		auto hr = D3D12CreateDevice(hardwareAdapter.Get(), _featureLevel, IID_PPV_ARGS(&_device));
		if (FAILED(hr)) {
			return hr;
		};
	}
	auto hr = _device->GetDeviceRemovedReason();
	_device->SetName(L"Device");
	

	return hr;
}

ID3D12Device * K3D12::D3D12Device::GetDevice()
{
	return _device.Get();
}

const D3D_FEATURE_LEVEL& K3D12::D3D12Device::GetFeatureLevel()
{
	return this->_featureLevel;
}

void K3D12::D3D12Device::Discard()
{
	_device.Reset();
}

