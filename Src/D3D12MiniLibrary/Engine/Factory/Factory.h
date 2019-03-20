#pragma once
#include"../Util/D3D12Common.h"


namespace K3D12 {
	class Factory
	{
	private:
		Microsoft::WRL::ComPtr<IDXGIFactory4>	_factory;
	public:

	private:

	public:
		HRESULT	Create(UINT createFlags = 0);
		IDXGIFactory4* GetFactory()const;
		void Discard();
		Factory();
		~Factory();

	};

}