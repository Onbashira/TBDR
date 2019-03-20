#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "VMD.h"


namespace K3D12 {

	class StateMachineController;
	class TransitionConditions;

	class StateNode
	{
		friend class StateMachineController;
	private:
		//遷移先名前、遷移条件
		std::map<std::string, std::pair<std::weak_ptr<K3D12::StateNode>, K3D12::TransitionConditions>>	_transitionMap;
		//このステートの名前
		std::string															_stateName;
		//モーションデータへの弱参照
		std::weak_ptr<K3D12::MotionData>									_animation;
	public:
	private:
	public:
		void SetMotionData(std::weak_ptr<K3D12::MotionData> data);
		std::weak_ptr<K3D12::MotionData> GetMotionData();
		void SetTransitionConditions(const std::pair<std::string, std::weak_ptr<K3D12::StateNode>>& destination, K3D12::TransitionConditions consitions);

		StateNode();
		~StateNode();
	};
}