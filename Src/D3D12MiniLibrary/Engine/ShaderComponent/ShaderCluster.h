#pragma once
#include <wrl.h>
#include <vector>
#include <d3dcommon.h>
#include <string>
#include <d3d12.h>



//シェーダーにパスに対応した
class ShaderCluster
{
private:	

	std::string	_clusterName;
	std::vector<D3D_SHADER_MACRO> _shaderMacro;
	Microsoft::WRL::ComPtr<ID3DBlob> _vs;
	Microsoft::WRL::ComPtr<ID3DBlob> _ps;
	Microsoft::WRL::ComPtr<ID3DBlob> _gs;
	Microsoft::WRL::ComPtr<ID3DBlob> _ds;
	Microsoft::WRL::ComPtr<ID3DBlob> _hs;
public :

private:

public:
	void SetVS(Microsoft::WRL::ComPtr<ID3DBlob> vs);
	void SetPS(Microsoft::WRL::ComPtr<ID3DBlob> ps);
	void SetGS(Microsoft::WRL::ComPtr<ID3DBlob> gs);
	void SetDS(Microsoft::WRL::ComPtr<ID3DBlob> ds);
	void SetHS(Microsoft::WRL::ComPtr<ID3DBlob> hs);

	Microsoft::WRL::ComPtr<ID3DBlob> GetVS();
	Microsoft::WRL::ComPtr<ID3DBlob> GetPS();
	Microsoft::WRL::ComPtr<ID3DBlob> GetGS();
	Microsoft::WRL::ComPtr<ID3DBlob> GetDS();
	Microsoft::WRL::ComPtr<ID3DBlob> GetHS();

	void AddShaderMacro(std::string name, std::string definition);
	void EraseShaderMacro(std::string name);
	const std::vector<D3D_SHADER_MACRO>& GetShaderMacro()const;

	ShaderCluster();
	~ShaderCluster();
};

