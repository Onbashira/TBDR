#pragma once
#include <string>
#include <d3dcompiler.h>

namespace K3D12 {

	class HLSLIncluder : public ID3DInclude
	{
	private:
		std::string _relativePath;
		std::string _nowString;
	public:

	private:
	public:

		HRESULT STDMETHODCALLTYPE	Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		HRESULT	STDMETHODCALLTYPE	Close(LPCVOID pData);

		void SetRelativePath(std::string relativePath);
		HLSLIncluder(std::string relativePath);
		HLSLIncluder() {};
		~HLSLIncluder() {};
	};
}

