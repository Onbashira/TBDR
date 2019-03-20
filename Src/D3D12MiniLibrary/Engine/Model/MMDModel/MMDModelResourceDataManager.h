#pragma once
#include <map>
#include <vector>
#include <memory>
#include <Windows.h>
#include "../../Util/ManagerComponentBase.h"

namespace K3D12 {
	struct MMDModelResourceData;

	//コンバート済みモデルのデータを保持するクラス
	class MMDModelResourceDataManager
	{
	private:
		ManagerComponentBase< MMDModelResourceData>	_pool;

	public:

		std::weak_ptr<MMDModelResourceData>	GetConvertedModelData(std::string path);
		void								SetConvertedModelData(std::string key, std::shared_ptr<MMDModelResourceData> modelData);
		void								EraseConvertedModelData(std::string key);
		bool								IsLoadedModel(std::string path);
		void								Discard();
		
		MMDModelResourceDataManager();
		~MMDModelResourceDataManager();
	private:
	public:

	};
}