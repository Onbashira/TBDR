#include "TransitionConditions.h"



K3D12::TransitionConditions::TransitionConditions()
{
}

K3D12::TransitionConditions::TransitionConditions(std::function<bool()> condition, bool hasExitTime, float exitTime, bool isLoop) :
	_hasExitTime(hasExitTime),_isLoop(isLoop),_exitTime(exitTime),_conditionFunction(condition)
{

}


K3D12::TransitionConditions::~TransitionConditions()
{
}


void K3D12::TransitionConditions::EnableLoop()
{
	_isLoop = true;
}

void K3D12::TransitionConditions::DisableLoop()
{
	_isLoop = false;
}

void K3D12::TransitionConditions::EnableHasExitTime()
{
	_hasExitTime = true;
}

void K3D12::TransitionConditions::DisableHasExitTime()
{
	_hasExitTime = false;

}

void K3D12::TransitionConditions::SetExitTime(float exitTime)
{
	_exitTime = exitTime;
}

float K3D12::TransitionConditions::GetExitTime()
{
	return _exitTime;
}

bool K3D12::TransitionConditions::CheckCondition()
{
	return _conditionFunction();
}

void K3D12::TransitionConditions::ResetCondiiton(std::function<bool()> newCondition)
{
	_conditionFunction = newCondition;
}
