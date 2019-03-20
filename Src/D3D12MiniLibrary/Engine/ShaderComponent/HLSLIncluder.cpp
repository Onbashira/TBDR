#include "HLSLIncluder.h"


K3D12::HLSLIncluder::HLSLIncluder(std::string relativePath) :
	_relativePath(relativePath)
{

}


HRESULT K3D12::HLSLIncluder::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	FILE* fp;
	std::string path(_relativePath + pFileName);
	fopen_s(&fp, path.data(), "r");
	if (fp == nullptr) return E_FAIL;

	_nowString.clear();
	UINT bytes = 0;
	char data = 0;
	size_t check = 0;
	while (true)
	{
		check = fread(&data, static_cast<unsigned int>(sizeof(data)), static_cast<size_t>(1), fp);
		if (check == 0) break;
		_nowString.push_back(data);
		bytes += sizeof(data);
	}

	fclose(fp);

	*ppData = static_cast<LPCVOID>(_nowString.data());
	*pBytes = bytes;

	return S_OK;
}

HRESULT K3D12::HLSLIncluder::Close(LPCVOID pData)
{
	return S_OK;
}

void K3D12::HLSLIncluder::SetRelativePath(std::string relativePath)
{
	_relativePath = relativePath;
}
