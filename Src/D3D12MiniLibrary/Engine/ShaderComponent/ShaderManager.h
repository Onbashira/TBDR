#pragma once
#include <d3dcommon.h>
#include <string>
#include <wrl.h>

#include <d3d12.h>
#include "../Util/ManagerComponentBase.h"
#include "../Util/NonCopyable.h"

namespace K3D12 {

	class ShaderManager : private NonCopyable
	{
	private:
		NonRaiiManagerComponentBase<Microsoft::WRL::ComPtr<ID3DBlob>> _pool;

	public:
	private:
		ShaderManager();

	public:

		static ShaderManager& GetInstance();
		bool IsLoaded(std::string name);
		Microsoft::WRL::ComPtr<ID3DBlob> Get(std::string name);
		void Set(std::string name, Microsoft::WRL::ComPtr<ID3DBlob> shader);
		void Erase(std::string name);
		void Discard();
		~ShaderManager();
	};

}
