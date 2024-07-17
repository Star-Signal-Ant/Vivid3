#pragma once
#include "RendererBase.h"
#include <vector>

class NodeEntity;
class NodeActor;
class NodeTerrain;
class MeshLines;

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
	void RenderTerrain(NodeTerrain* terrain);
	//depth renderers
	void RenderEntityDepth(NodeEntity* entity);
	void RenderActorDepth(NodeActor* actor);
	void RenderTerrainDepth(NodeTerrain* terrain);
	void RenderGizmo(NodeEntity* entity);
	void RenderLines(MeshLines* lines);

	//creation
	void CreateMeshPBRPipeline();
	void CreateActorPipeline();
	void CreateMeshDepthPipeline();
	void CreateActorDepthPipeline();
	void CreateTerrainPipeline();
	void CreateTerrainDepthPipeline();
	void CreateGizmoPipeline();
	void CreateLinesPipeline();

	//gizmo
	void SetGizmo(NodeEntity* entity) {
		//m_Gizmo = entity;
		m_Gizmos.clear();
		if (entity != nullptr) {
			m_Gizmos.push_back(entity);
		}
	}
	void ClearGizmo() {
		m_Gizmos.clear();
	}
	void AddGizmo(NodeEntity* entity) {
		m_Gizmos.push_back(entity);
	}
	void AddLines(MeshLines* lines) {
		m_Lines.push_back(lines);
	}

private:

	//Entities
	std::vector<NodeEntity*> m_Entities;
	std::vector<NodeActor*> m_Actors;
	NodeTerrain* m_ActiveTerrain;
	std::vector<NodeEntity*> m_Gizmos;
	std::vector<MeshLines*> m_Lines;

	//MeshPBR
	RefCntAutoPtr<IBuffer> m_EntityPBRConstants;
	RefCntAutoPtr<IPipelineState> m_PS_EntityPBR;
	RefCntAutoPtr<IShaderResourceBinding> m_EntityPBR_SRB;

	//Gizmo
	RefCntAutoPtr<IBuffer> m_GizmoConstants;
	RefCntAutoPtr<IPipelineState> m_PS_Gizmo;
	RefCntAutoPtr<IShaderResourceBinding> m_Gizmo_SRB;

	//Lines
		//Gizmo
	RefCntAutoPtr<IBuffer> m_LinesConstants;
	RefCntAutoPtr<IPipelineState> m_PS_Lines;
	RefCntAutoPtr<IShaderResourceBinding> m_Lines_SRB;



	//Mesh Depth
	RefCntAutoPtr<IBuffer> m_EntityDepthConstants;
	RefCntAutoPtr<IPipelineState> m_PS_EntityDepth;
	RefCntAutoPtr<IShaderResourceBinding> m_EntityDepth_SRB;
	//Actor Depth
	RefCntAutoPtr<IBuffer> m_ActorDepthConstants;
	RefCntAutoPtr<IPipelineState> m_PS_ActorDepth;
	RefCntAutoPtr<IShaderResourceBinding> m_ActorDepth_SRB;
	//Terrain
	RefCntAutoPtr<IBuffer> m_TerrainConstants;
	RefCntAutoPtr<IPipelineState> m_PS_Terrain;
	RefCntAutoPtr<IShaderResourceBinding> m_Terrain_SRB;
	RefCntAutoPtr<IBuffer> m_TerrainDepthConstants;
	RefCntAutoPtr<IPipelineState> m_PS_TerrainDepth;
	RefCntAutoPtr<IShaderResourceBinding> m_TerrainDepth_SRB;
	//Actor
	RefCntAutoPtr<IBuffer> m_ActorConstants;
	RefCntAutoPtr<IPipelineState> m_PS_Actor;
	RefCntAutoPtr<IShaderResourceBinding> m_Actor_SRB;

	//

};

