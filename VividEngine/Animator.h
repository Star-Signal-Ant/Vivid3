#pragma once
#include <vector>
#include <string>
#include "BasicMath.hpp"

using namespace Diligent;

class NodeActor;
class Animation;
class AssimpNodeData;

class Animator
{

public:

	Animator(Animation* anim);
	void SetTime(float time);
	void UpdateAnimation(float dt);
	void AddAnimation(Animation* animation);
	void SetAnimation(Animation* animation);
	void CalculateBoneTransform(AssimpNodeData* node, float4x4 transform);
	float GetLength();
	void ClearCopies();
	std::vector<float4x4> GetBones() {
		return m_FinalBones;
	}
	float GetTicksPerSecond();
	std::vector<Animation*> GetAnimations() {
		return m_Animations;
	}
private:


	NodeActor* m_Actor;
	
	std::vector<float4x4> m_FinalBones;
	float m_Speed = 0.025f;
	Animation* m_CurrentAnim = nullptr;
	std::vector<Animation*> m_Animations;
	float m_CurrentTime = 0.0;
	float m_DeltaTime;

};

