#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>
#include "../System/D3D12System.h"
#include "../Resource/RenderTarget.h"
#include "../CommandContext/CommandQueue.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "RenderingPathObject.h"

namespace K3D12 {

	class RenderingManager
	{
	private:
		//<Key =  pair<MasterPathIndex , MasterPathName> , value = <key = childPathName, value = PathOBJ>
		std::map < std::pair<unsigned int, std::string>, std::map<std::string, RenderingPathObject >> _renderingPathMap;
		std::map < std::string, bool> _masterPathEnableMap;
		RenderTarget _rendertarget;
	public:
	private:
	public:

		HRESULT IntializeRenderingManager(CommandQueue& commandQueue, Factory& factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		//レンダリングの実行
		void ExcutionRendering();

		//各フレームの一番最初に呼ばれる初期化関数
		void InitializeOnStart();

		//コマンドの実行前に呼ばれる初期化関数
		void InitializeOnBeforExcute();

		//コマンドの実行後に呼ばれる初期化関数（OnStartとは統合できるがわざと分離している）
		void InitializeOnAfterExcute();

		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(std::string pathName, std::string childPathName = "");
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(unsigned int pathIndex, unsigned int childPathIndex = 0);
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(unsigned int pathIndex, std::string childPathName = "");
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(std::string pathName, unsigned int childPathIndex = 0);

		unsigned int GetRenderingPathIndex(std::string pathName, std::string childPathName = "");
		unsigned int GetRenderingPathIndex(std::string pathName, unsigned int childPathIndex = 0);

		std::string  GetRenderingPathName(unsigned int pathIndex, std::string childPathName = "");
		std::string  GetRenderingPathName(unsigned int pathIndex, unsigned int childPathIndex = 0);

		void SetActiveMasterPath(std::string masterPathName, bool isEnable);
		void SetActiveChildPath(std::string masterPathName, std::string childPathName, bool isEnable);


		//レンダリングパスを登録する関数、返却地はStructBindingでレンダリングパスが使うコマンドリストの弱参照とインデックス番号を返す
		std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>  RegistRenderingPath(std::string name);
		//新規レンダリングパスをpathIndex番にインサートする
		std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>  InsertRenderingPath(std::string name, unsigned int insertPathIndex);

		RenderingManager();
		~RenderingManager();
	};
}