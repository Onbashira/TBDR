#include <map>
#include <iostream>
#include "TransitionConditions.h"
#include "StateNode.h"
#include "Math.h"

K3D12::StateNode::StateNode()
{

}


K3D12::StateNode::~StateNode()
{
}


void K3D12::StateNode::SetMotionData(std::weak_ptr<K3D12::MotionData> data)
{
	this->_animation = data;
}

std::weak_ptr<K3D12::MotionData> K3D12::StateNode::GetMotionData()
{
	return this->_animation;
}

void K3D12::StateNode::SetTransitionConditions(const std::pair<std::string, std::weak_ptr<K3D12::StateNode>>& destination, K3D12::TransitionConditions consitions)
{
	if (this->_transitionMap.find(destination.first) != this->_transitionMap.end()) {
		this->_transitionMap.find(destination.first)->second.second = consitions;
		this->_transitionMap.find(destination.first)->second.first = destination.second;
	}
}


