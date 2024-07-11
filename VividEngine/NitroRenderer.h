#pragma once
#include "RendererBase.h"
#include <vector>

class NodeEntity;
class NodeActor;

//Nitro renderer is a non rtx renderer. 
class NitroRenderer : public RendererBase
{
public:

	NitroRenderer();
	void PreRender() override;
	void Render(bool no_render) override;


	//Rended Shadows
	// 
	void RenderShadows();
	// 
	//renderers
	void RenderEntityPBR(NodeEntity* entity);
	void RenderActor(NodeActor* actor);

	//depth renderers
	void RenderEntityDepth(NodeEntity* entity);
	void RenderActorDepth(NodeActor* actor);

	//creation
	void CreateMeshPBRPipeline();
	void CreateActorPipeline();
	void CreateMeshDepthPipeline();
	void CreateActorDepthPipeline();

private:

	//Entities
	std::vector<NodeEntity*> m_Entities;
	std::vector<NodeActor*> m_Actors;

	//MeshPBR
	RefCntAutoPtr<IBuffer> m_EntityPBRConstants;
	RefCntAutoPtr<IPipelineState> m_PS_EntityPBR;
	RefCntAutoPtr<IShaderResourceBinding> m_EntityPBR_SRB;

	//Mesh Depth
	RefCntAutoPtr<IBuffer> m_EntityDepthConstants;
	RefCntAutoPtr<IPipelineState> m_PS_EntityDepth;
	RefCntAutoPtr<IShaderResourceBinding> m_EntityDepth_SRB;
	//Actor Depth
	RefCntAutoPtr<IBuffer> m_ActorDepthConstants;
	RefCntAutoPtr<IPipelineState> m_PS_ActorDepth;
	RefCntAutoPtr<IShaderResourceBinding> m_ActorDepth_SRB;

	//Actor
	RefCntAutoPtr<IBuffer> m_ActorConstants;
	RefCntAutoPtr<IPipelineState> m_PS_Actor;
	RefCntAutoPtr<IShaderResourceBinding> m_Actor_SRB;

	//

};

