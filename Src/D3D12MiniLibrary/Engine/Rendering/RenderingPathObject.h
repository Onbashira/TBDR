#pragma once
#include <memory>
#include <functional>
#include "Math.h"
#include "../Util/D3D12Common.h"


namespace K3D12 {
	class RenderingManager;
	class PipelineStateObject;
	class GraphicsCommandList;
	class CommandQueue;
	class Fence;

	//レンダリング時に描画命令を積むコマンドコンテキストを隔離するためのクラス
	class RenderingPathObject
	{
		friend class  RenderingManager;
	private:

		unsigned int  _pathIndex;
		std::string   _pathName;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onStartInitializerFunc;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onBeforExcuteInitializerFunc;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onAfterExcuteInitializerFunc;
		std::shared_ptr<GraphicsCommandList> _masterCommandList;
		bool _isEnable;

	public:

	private:

	public:

		//各フレームの一番最初に呼ばれる初期化関数
		void OnStartInitializer(std::weak_ptr<GraphicsCommandList> list);

		//コマンドの実行前に呼ばれる初期化関数
		void OnBeforExcuteInitializer(std::weak_ptr<GraphicsCommandList> list);

		//コマンドの実行後に呼ばれる初期化関数（OnStartとは統合できるがわざと分離している）
		void OnAfterExcuteInitializer(std::weak_ptr<GraphicsCommandList> list);

		//各イニシャライザのセット
		void SetOnStartInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//各イニシャライザのセット
		void SetOnBeforExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//各イニシャライザのセット
		void SetOnAfterExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//このパスの有効無効の切り替え
		void SetEnable(bool isEnable);

		RenderingPathObject();
		~RenderingPathObject();
	};
}