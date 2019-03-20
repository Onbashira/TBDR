#pragma once
#include <string>
#include "StateMachineController.h"
#include "StateNode.h"
#include "../Model/MMDModel/MMDModelResourceData.h"


namespace K3D12 {

	class Animator
	{
	private:
		StateMachineController _statemachineController;
	public:
		std::weak_ptr<MMDBoneTree> _skelton;
		unsigned int _animationFrame;
	private:

	public:
		void SetModelSkelton(std::weak_ptr<MMDBoneTree> skelton);
		void AttachAnimation(std::string animationName, std::string animationFilePath);
		void PlayAnimation(std::string animation);
		void SetTransitionCondition(std::string from, std::string to, TransitionConditions newCondition);
		void ResetSkelton();
		Animator();
		~Animator();
	};

}
