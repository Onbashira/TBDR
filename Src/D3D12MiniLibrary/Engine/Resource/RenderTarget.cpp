#include "RenderTarget.h"
#include "../System/D3D12System.h"
#include "../Factory/Factory.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/CommandQueue.h"
#include "../Window/Window.h"


constexpr float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

K3D12::RenderTarget::RenderTarget()
{
}


K3D12::RenderTarget::~RenderTarget()
{
	Discard();
}


HRESULT K3D12::RenderTarget::CreateSwapChain(CommandQueue & commandQueue, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{


	DXGI_SWAP_CHAIN_DESC1	swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = windowWidth;
	swapChainDesc.Height = windowHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	Microsoft::WRL::ComPtr<IDXGISwapChain1>	swapChain;
	if (FAILED(factory.GetFactory()->CreateSwapChainForHwnd(commandQueue.GetQueue().Get(), window.GetWindowHandle(), &swapChainDesc, nullptr, nullptr, &swapChain)))
		return FALSE;
	// フルスクリーンのサポートなし
	if (FAILED(factory.GetFactory()->MakeWindowAssociation(window.GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER)))
		return FALSE;

	if (FAILED(swapChain.As(&_swapChain)))
		return FALSE;

	_currentIndex = _swapChain->GetCurrentBackBufferIndex();
	return S_OK;
}

HRESULT K3D12::RenderTarget::CreateRenderTargets(unsigned int bufferNum)
{
	this->_bufferNum = bufferNum;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = bufferNum;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_rtResource.resize(bufferNum);
	//レンダーターゲットビュー用のヒープの作成
	{
		if (FAILED(_rtHeap.Create(&desc)))
			return E_FAIL;
		_rtHeap.SetName("RendertargetsHeap ");
	}
	//レンダーターゲットの作成
	{
		for (UINT i = 0; i < bufferNum; i++) {
			//バッファの取得
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(this->_rtResource[i].GetAddressOf()))))
				return FALSE;
			//レンダーターゲットビューの取得
			GET_DEVICE->CreateRenderTargetView(_rtResource[i].GetResource(), nullptr, _rtHeap.GetCPUHandle(i));
			_rtResource[i].SetResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
			_rtResource[i].SetName("RenderTargetResource ");
		}
	}
	return S_OK;
}

HRESULT K3D12::RenderTarget::Create(CommandQueue & commandQueue, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{
	auto hr = CreateSwapChain(commandQueue, factory, window, windowWidth, windowHeight, bufferNum);
	CHECK_RESULT(hr);
	hr = CreateRenderTargets(bufferNum);
	CHECK_RESULT(hr);
	return hr;
}

unsigned int K3D12::RenderTarget::GetBufferNum()
{
	return _bufferNum;
}

unsigned int K3D12::RenderTarget::GetCurrentBuckBuffer()
{
	return _currentIndex;
}

HRESULT K3D12::RenderTarget::SetStatePresent(std::shared_ptr<GraphicsCommandList> list)
{
	auto hr = _rtResource[_currentIndex].ResourceTransition(list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D12::RenderTarget::SetStateRenderTarget(std::shared_ptr<GraphicsCommandList> list)
{
	auto hr = _rtResource[_currentIndex].ResourceTransition(list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D12::RenderTarget::SetStateCopyDest(std::shared_ptr<GraphicsCommandList> list)
{
	auto hr =_rtResource[_currentIndex].ResourceTransition(list->GetCommandList().Get(),D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D12::RenderTarget::SetStateGenericRead(std::shared_ptr<GraphicsCommandList> list)
{
	auto hr = _rtResource[_currentIndex].ResourceTransition(list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D12::RenderTarget::CopyToRenderTarget(std::shared_ptr<GraphicsCommandList> list,Resource* pSrc)
{
	list->GetCommandList()->CopyResource(this->_rtResource[_currentIndex].GetResource(),pSrc->GetResource());
	return S_OK;
}

void K3D12::RenderTarget::ClearScreen(std::shared_ptr<GraphicsCommandList> list)
{
	SetStateRenderTarget(list);
	list->GetCommandList()->ClearRenderTargetView(this->_rtHeap.GetCPUHandle(_currentIndex), clearColor, 0, nullptr);

}

void K3D12::RenderTarget::FlipScreen()
{
	_currentIndex = _swapChain->GetCurrentBackBufferIndex();
}

HRESULT K3D12::RenderTarget::Present(unsigned int sysncInterval, unsigned int flags)
{
	auto hr = _swapChain->Present(sysncInterval, flags);
	CHECK_RESULT(hr);
	FlipScreen();
	return hr;
}

void K3D12::RenderTarget::Discard()
{
	_swapChain.Reset();
	for (auto& res : _rtResource) {
		res.Discard();
	}
	_rtHeap.Discard();
}