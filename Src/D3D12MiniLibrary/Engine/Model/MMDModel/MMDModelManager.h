#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "../..//Util/ManagerComponentBase.h"

namespace K3D12 {
	class MMDModel;

	class MMDModelManager
	{
	private:
	public:

	private:
		ManagerComponentBase<MMDModel>					 _pool;
		std::map<std::string, std::shared_ptr<MMDModel>> _modelPool;

	public:

		void SetMMDModel(std::string modelPath, std::shared_ptr<MMDModel> model);
		std::weak_ptr<MMDModel> GetMMDModel(std::string modelPath);
		void EraseMMDModel(std::string modelPath);
		void Discard();

		MMDModelManager();
		~MMDModelManager();
	};

}