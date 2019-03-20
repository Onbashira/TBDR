#include <wrl.h>
#include <vector>
#include <d3dcommon.h>
#include <string>
#include <d3d12.h>
#include <algorithm>
#include "ShaderCluster.h"


ShaderCluster::ShaderCluster()
{
}


ShaderCluster::~ShaderCluster()
{
	_vs.Reset();
	_ps.Reset();
	_gs.Reset();
	_ds.Reset();
	_hs.Reset();
}

void ShaderCluster::SetVS(Microsoft::WRL::ComPtr<ID3DBlob> vs)
{
	_vs = vs;
}

void ShaderCluster::SetPS(Microsoft::WRL::ComPtr<ID3DBlob> ps)
{
	_ps = ps;
}

void ShaderCluster::SetGS(Microsoft::WRL::ComPtr<ID3DBlob> gs)
{
	_gs = gs;
}

void ShaderCluster::SetDS(Microsoft::WRL::ComPtr<ID3DBlob> ds)
{
	_ds = ds;

}

void ShaderCluster::SetHS(Microsoft::WRL::ComPtr<ID3DBlob> hs)
{
	_hs = hs;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCluster::GetVS()
{
	return _vs;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCluster::GetPS()
{
	return _ps;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCluster::GetGS()
{
	return _gs;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCluster::GetDS()
{
	return _ds;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCluster::GetHS()
{
	return _hs;
}

void ShaderCluster::AddShaderMacro(std::string name, std::string definition)
{
	D3D_SHADER_MACRO macro = { name.c_str(),definition.c_str() };
	_shaderMacro.push_back(macro);
}

void ShaderCluster::EraseShaderMacro(std::string name)
{
	auto itr = std::find_if(_shaderMacro.begin(), _shaderMacro.end(), 
		[name](const D3D_SHADER_MACRO& value)->bool {
			if (std::string(value.Name) == name) {
				return true; 
			}
			return false; 
		}
	);
	this->_shaderMacro.erase(itr);
}

const std::vector<D3D_SHADER_MACRO>& ShaderCluster::GetShaderMacro() const
{
	return _shaderMacro;
}
