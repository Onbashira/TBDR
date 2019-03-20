#pragma once
#include <memory>
#include <vector>


namespace K3D12 {
	class GamePad;

	class GamePadManager
	{
	private:
		//システム側で決める接続を許すゲームパッド最大数
		int _maxConectionCount;
		//現在の接続されたゲームパッド
		std::vector<std::shared_ptr<GamePad>> _gamePads;
	public:

	private:
		void AllSetUp();
	public:
		//各パッドをアップデートする
		void Update();
		//IDからゲームパッドを取得する
		std::shared_ptr<GamePad> GetPad(int padID = 0);
		//パッド全体の初期化
		void Initialize(int macConectionCount = 4);
		//最大接続数をシステムで決めてイニシャライズする
		GamePadManager();
		~GamePadManager();
	};
}
