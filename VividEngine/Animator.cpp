#include "pch.h"
#include "Animator.h"
#include "Bone.h"
#include "Animation.h"
#include "BoneInfo.h"
#include <unordered_set>
Animator::Animator(Animation* anim) {

	m_CurrentTime = 0.0;
	m_CurrentAnim = anim;
	m_Animations.push_back(anim);

	for (int i = 0; i < 100; i++) {
		m_FinalBones.push_back(float4x4::Identity());
	}



}

void Animator::AddAnimation(Animation* animation) {

	m_Animations.push_back(animation);

}

void Animator::SetAnimation(Animation* animation) {

	m_CurrentAnim = animation;
	m_CurrentTime = 0;

}

void Animator::SetTime(float time) {

	m_CurrentTime = time;
	//CalcBones
	CalculateBoneTransform(m_CurrentAnim->GetRootNode(), float4x4::Identity());

}

void Animator::UpdateAnimation(float dt) {

	SetTime(dt);

}

void Animator::CalculateBoneTransform(AssimpNodeData* node, float4x4 transform) {

	std::string nodeName = node->name;
	float4x4 nodeTransform = node->transformation;

	Bone* bone = m_CurrentAnim->FindBone(nodeName);

	if (bone != nullptr) {

		bone->Update(m_CurrentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	float4x4 globalTransform = nodeTransform * transform;
	auto boneInfo = m_CurrentAnim->GetBoneIDMap();
	if (boneInfo.count(nodeName) > 0)
	{
		int index = boneInfo[nodeName]->ID;
		float4x4 offset = boneInfo[nodeName]->Offset;
		m_FinalBones[index] = offset*globalTransform;// *offset;// nodeTransform; // float4x4::Identity();// offset* globalTransform;
	}

	for (int i = 0; i < node->childrenCount; i++) {
		CalculateBoneTransform(node->children[i],globalTransform);
	}

}

float Animator::GetLength() {
	return m_CurrentAnim->GetLength();
}

float Animator::GetTicksPerSecond() {

	return m_CurrentAnim->GetTicksPerSecond();

}

std::vector<Animation*> RemoveDuplicates(const std::vector<Animation*>& animations) {
	std::vector<Animation*> uniqueAnimations;
	std::unordered_set<std::string> seenNames;

	for (Animation* anim : animations) {
		if (anim != nullptr) { // Check for null pointers
			std::string name = anim->GetName();
			if (seenNames.find(name) == seenNames.end()) {
				// Name not found in the set, add to unique list and mark as seen
				uniqueAnimations.push_back(anim);
				seenNames.insert(name);
			}
		}
	}

	return uniqueAnimations;
}


void Animator::ClearCopies() {

	
	m_Animations = RemoveDuplicates(m_Animations);
}