#include <memory>
#include "Animator.h"
#include "TransitionConditions.h"
#include "StateMachineController.h"
#include "../Model/MMDModel/MMDModel.h"
#include "VMDLoader.h"
#include "MotionManager.h"

K3D12::Animator::Animator():_animationFrame(0)
{
}


K3D12::Animator::~Animator()
{
}


void K3D12::Animator::SetModelSkelton(std::weak_ptr<K3D12::MMDBoneTree> skelton)
{
	_statemachineController.SetModelSkelton(skelton);
}

void K3D12::Animator::AttachAnimation(std::string animationName, std::string animationFilePath)
{
	_statemachineController.AttachAnimation(animationName, K3D12::MotionManager::LoadMotion(animationFilePath).lock());
}

void K3D12::Animator::PlayAnimation(std::string animation)
{
	_statemachineController.PlayAnimation(_animationFrame);
}

void K3D12::Animator::SetTransitionCondition(std::string from, std::string to, K3D12::TransitionConditions newCondition)
{
	_statemachineController.ConnectingAnimation(from, to, newCondition);
}

void K3D12::Animator::ResetSkelton()
{
	_statemachineController.ResetSkelton();
}


