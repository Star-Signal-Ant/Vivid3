#include "pch.h"
#include "NitroRenderer.h"
#include "SceneGraph.h"
#include "Engine.h"
#include <vector>
#include "NodeLight.h"
#include "RenderTargetCube.h"
#include "Texture2D.h"

#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/Buffer.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Common/interface/RefCntAutoPtr.hpp>
#include <MapHelper.hpp> // Add this line
#include "TextureCube.h"
#include "NodeEntity.h"
#include "NodeActor.h"
#include "Mesh3D.h"
#include "MeshBuffer.h"
#include "MaterialBase.h"
using namespace Diligent;

//Constants

struct MeshPBRConstants {

    float4x4 g_MVP;
    float4x4 g_Proj;
    float4x4 g_Model;
    float4x4 g_View;
    float4x4 g_ModelInv;
    float4 v_CameraPosition;
    float4 v_LightPosition;
    float4 v_ExtraProp;
    float4 v_LightDiffuse;
    int4 v_LightMode;
    float4 v_LightDir;
    float4 v_ViewDir;
    float4 v_LightCone;


};


struct ActorConstants
{

    float4x4 v_MVP;
    float4x4 v_Proj;
    float4x4 v_Model;
    float4x4 v_View;
    float4x4 v_ModelInv;
    float4 v_CameraPos;
    float4 v_LightPos;
    float4 v_LightProp;
    float4 v_LightDiff;
    float4 v_LightSpec;
    float4 v_LenderProps;
    int4 v_LightMode;
    float4 v_LightDir;
    float4 v_LightCone;
    float4 v_MatDiff;
    float4 v_MatSpec;
    float4 v_CameraExt;
    float4x4 bones[100];
};


NitroRenderer::NitroRenderer() {

	CreateMeshPBRPipeline();
    CreateActorPipeline();


}

void NitroRenderer::PreRender()
{
    if (GetSceneGraph()->IsUpdated()) {

        int b = 5;
        m_Entities.clear();
        for (auto entity : GetSceneGraph()->GetEntities()) {

            m_Entities.push_back(entity);

        }
        m_Actors.clear();
        for (auto actor : GetSceneGraph()->GetActors()) {
            m_Actors.push_back(actor);
        }

    }
	//GetSceneGraph()->RenderShadows();

}

void NitroRenderer::Render(bool no_render) {

	//GetSceneGraph()->Render();
    for (auto actor : m_Actors) {

        RenderActor(actor);

    }
    for (auto entity : m_Entities) {

        RenderEntityPBR(entity);

    }



}

//Renderers

void NitroRenderer::RenderEntityPBR(NodeEntity* entity) {

    
    auto camera = GetSceneGraph()->GetCamera();
    auto lights = GetSceneGraph()->GetLights();
    auto light = lights[0];

    float FOVRadians = 45.0f * (3.14159265358979323846f / 180.0f);

    float4x4 mvp = camera->GetProjection(); //float4x4::Projection(FOVRadians, 1024.0f / 760.0f,0.01,1001,false);


    float4x4 view = camera->GetWorldMatrix();  //float4x4::Translation(float3(0,1.0f,-5)).Inverse();

    float4x4 model = entity->GetWorldMatrix();

    float4x4 id = float4x4::Identity().Inverse();

    //mvp = mvp*id;

    //mvp.Transpose();


    mvp = model * view * mvp;

    MeshPBRConstants dd;

   dd.g_MVP = mvp.Transpose();
  dd.g_Model = model.Transpose();
  dd.g_ModelInv = model.Inverse().Transpose();
  dd.g_Proj = camera->GetProjection().Transpose();
  dd.g_View = view.Transpose();
  dd.v_LightDiffuse = light->GetDiffuse();
  dd.v_LightPosition = float4(light->GetPosition(), 1.0f);
//  dd.vlightSpec = float4(light->GetSpecular(), 1.0f);


  dd.v_CameraPosition = float4(camera->GetPosition(), 1.0);



  dd.v_ExtraProp = float4(light->GetRange(), 0, 0, 0);
  dd.v_ViewDir = float4(camera->TransformVector(float3(0, 0, -1.0)), 1.0);


  
    {

        
        printf("Mapping!\n");
        MapHelper<MeshPBRConstants> map_data(Engine::m_pImmediateContext, m_EntityPBRConstants,MAP_TYPE::MAP_WRITE, MAP_FLAG_DISCARD);
       





        *map_data = dd;

        //map_data[0].g_MVP = mvp.Transpose();
        //map_data[0].g_Model = model.Transpose();
        //map_data[0].g_ModelInv = model.Inverse().Transpose();
        //map_data[0].g_Proj = camera->GetProjection().Transpose();
        //map_data[0].g_View = view.Transpose();
        //map_data[0].v_LightDiffuse = light->GetDiffuse();
        //map_data[0].v_LightPosition = float4(light->GetPosition(), 1.0f);
        //  map_data[0].lightSpec = float4(light->GetSpecular(), 1.0f);

        //map_data[0].v_CameraPosition = float4(camera->GetPosition(), 1.0);



        //map_data[0].v_ExtraProp = float4(light->GetRange(), 0, 0, 0);
        //map_data[0].v_ViewDir = float4(camera->TransformVector(float3(0, 0, -1.0)), 1.0);

        //map_data.Unmap();
        printf("Unampping!\n");
        //return;


    }
    
    //return;



    for (auto mesh : entity->GetMeshes())
    {

        auto material = mesh->GetMaterial();

        auto albedo = material->GetDiffuse();
        auto norm = material->GetNormal();
        auto rough = material->GetRough();
        auto metal = material->GetMetal();

  //      return;
        //Bind Textures

        m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(albedo->GetView(),SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureNorm")->Set(norm->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureRough")->Set(rough->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureMetal")->Set(metal->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
      
        //m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "v_Shadow")->Set(albedo->GetView());

        //

        //

        int b = 5;

        Uint64 offsets = 0;

        IBuffer* pBuffs[] = { mesh->GetBuffer()->GetVertexBuffer() };

        RESOURCE_STATE_TRANSITION_MODE flags = RESOURCE_STATE_TRANSITION_MODE::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

   //     return;
        Engine::m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offsets,flags);
        Engine::m_pImmediateContext->SetIndexBuffer(mesh->GetBuffer()->GetIndexBuffer(), 0, flags );

     //   return;


        Engine::m_pImmediateContext->SetPipelineState(m_PS_EntityPBR);

        Engine::m_pImmediateContext->CommitShaderResources(m_EntityPBR_SRB, flags);

        DrawIndexedAttribs attrib;
        attrib.IndexType = VALUE_TYPE::VT_UINT32;

        attrib.NumIndices = mesh->GetTris().size() * 3;

        attrib.Flags = DRAW_FLAG_VERIFY_ALL;
        Engine::m_pImmediateContext->DrawIndexed(attrib);

    }

    Engine::m_pImmediateContext->Flush();

}

void NitroRenderer::RenderActor(NodeActor* actor) {

    auto camera = GetSceneGraph()->GetCamera();
    auto lights = GetSceneGraph()->GetLights();
    auto light = lights[0];

    float4x4 mvp = camera->GetProjection(); //float4x4::Projection(FOVRadians, 1024.0f / 760.0f,0.01,1001,false);


    float4x4 view = camera->GetWorldMatrix();  //float4x4::Translation(float3(0,1.0f,-5)).Inverse();

    float4x4 model = actor->GetWorldMatrix();

    float4x4 id = float4x4::Identity().Inverse();

    //mvp = mvp*id;

    //mvp.Transpose();


    mvp = model * view * mvp;

    auto m_Bones = actor->GetBones();


    ActorConstants dd;

    dd.v_MVP = mvp.Transpose();
    dd.v_Model = model.Transpose();
    dd.v_ModelInv = model.Inverse().Transpose();
    dd.v_Proj = camera->GetProjection().Transpose();
    dd.v_View = view.Transpose();
    dd.v_LightDiff = light->GetDiffuse();
    dd.v_LightPos = float4(light->GetPosition(), 1.0f);
    dd.v_LightSpec = float4(light->GetSpecular(), 1.0f);
    dd.v_MatDiff = float4(1, 1, 1, 1);
    dd.v_MatSpec = float4(1, 1, 1, 1);
    dd.v_CameraPos = float4(camera->GetPosition(), 1.0);
    dd.v_LightProp = float4(light->GetRange(), 0, 0, 0);
    for (int i = 0; i < 100; i++) {
        dd.bones[i] = m_Bones[i].Transpose();

    }



    {
        printf("Mapping!\n");
        MapHelper<ActorConstants> map_data(Engine::m_pImmediateContext, m_ActorConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        float FOVRadians = 45.0f * (3.14159265358979323846f / 180.0f);






        *map_data = dd;
        //map_data.Unmap();
        printf("Unmapping.");
    }


    for (auto mesh : actor->GetMeshes())
    {

        auto material = mesh->GetMaterial();

        auto albedo = material->GetDiffuse();
        auto norm = material->GetNormal();
        auto spec = material->GetSpecular();


        //Bind Textures

        m_Actor_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "v_Texture")->Set(albedo->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        m_Actor_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "v_TextureNorm")->Set(norm->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        m_Actor_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "v_TextureSpec")->Set(spec->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        //m_Actor_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureMetal")->Set(metal->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
        //m_EntityPBR_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "v_Shadow")->Set(albedo->GetView());


        //
      

        //

        int b = 5;

        Uint64 offsets = 0;

        IBuffer* pBuffs[] = { mesh->GetBuffer()->GetVertexBuffer() };

        Engine::m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        Engine::m_pImmediateContext->SetIndexBuffer(mesh->GetBuffer()->GetIndexBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        Engine::m_pImmediateContext->SetPipelineState(m_PS_Actor);

        Engine::m_pImmediateContext->CommitShaderResources(m_Actor_SRB,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs attrib;
        attrib.IndexType = VALUE_TYPE::VT_UINT32;

        attrib.NumIndices = mesh->GetTris().size() * 3;

        attrib.Flags = DRAW_FLAG_VERIFY_ALL;
        Engine::m_pImmediateContext->DrawIndexed(attrib);

    }

}

RefCntAutoPtr<IBuffer> CreateUniform(int size, std::string path) {

    BufferDesc desc;
    desc.Name = path.c_str();
    desc.Size = (Uint64)size;
    desc.Usage = USAGE_DYNAMIC;
    desc.BindFlags = BIND_UNIFORM_BUFFER;
    desc.CPUAccessFlags = CPU_ACCESS_WRITE;

    RefCntAutoPtr<IBuffer> buffer;

    Engine::m_pDevice->CreateBuffer(desc, nullptr, &buffer);

    return buffer;

}


//MESH PBR PIPELINE

void NitroRenderer::CreateMeshPBRPipeline() {

    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "EntityPBR PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Engine::m_pSwapChain->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat = Engine::m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.Desc.UseCombinedTextureSamplers = true;

  
    // Create a shader source stream factory to load shaders from files.
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    //m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = Engine::m_pShaderFactory;
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "MeshPBR VS";
        ShaderCI.FilePath = "pbr_lit.vsh";
        Engine::m_pDevice->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        m_EntityPBRConstants = CreateUniform(sizeof(MeshPBRConstants), "Mesh PBR");
    }



    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "MeshPBR PS";
        ShaderCI.FilePath = "pbr_lit.psh";
        Engine::m_pDevice->CreateShader(ShaderCI, &pPS);
    }


    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        LayoutElement{1, 0, 4, VT_FLOAT32, False},
        LayoutElement{2, 0, 3, VT_FLOAT32, False},
        LayoutElement{3, 0, 3, VT_FLOAT32, False},
        LayoutElement{4, 0, 3, VT_FLOAT32, False},
        LayoutElement{5, 0, 3, VT_FLOAT32, False},
        LayoutElement(6,0,4,VT_FLOAT32,False),
        LayoutElement(7,0,4,VT_FLOAT32,False)

    };
    // clang-format on

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // clang-format off
    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    ShaderResourceVariableDesc Vars[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        {SHADER_TYPE_PIXEL, "g_TextureNorm", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        { SHADER_TYPE_PIXEL, "g_TextureRough", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC },
        { SHADER_TYPE_PIXEL, "g_TextureMetal", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC },
        {SHADER_TYPE_PIXEL, "v_Shadow", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    };

    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    // clang-format off
    // Define immutable sampler for g_Texture. Immutable samplers should be used whenever possible
    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "g_TextureNorm", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "g_TextureRough", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "g_TextureMetal", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "v_Shadow", SamLinearClampDesc}

    };
    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    Engine::m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PS_EntityPBR);

    // Since we did not explicitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables
    // never change and are bound directly through the pipeline state object.
    m_PS_EntityPBR->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_EntityPBRConstants);


    // Since we are using mutable variable, we must create a shader resource binding object
    // http://diligentgraphics.com/2016/03/23/resource-binding-model-in-diligent-engine-2-0/
    m_PS_EntityPBR->CreateShaderResourceBinding(&m_EntityPBR_SRB, true);


}


//Actor Pipeline
void NitroRenderer::CreateActorPipeline() {

    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Actor PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Engine::m_pSwapChain->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat = Engine::m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.Desc.UseCombinedTextureSamplers = true;


    // Create a shader source stream factory to load shaders from files.
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    //m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = Engine::m_pShaderFactory;
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Actor VS";
        ShaderCI.FilePath = "mesh_actorlit.vsh";
        Engine::m_pDevice->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        m_ActorConstants = CreateUniform(sizeof(ActorConstants), "Actor Constants");
    }



    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Actor PS";
        ShaderCI.FilePath = "mesh_actorlit.psh";
        Engine::m_pDevice->CreateShader(ShaderCI, &pPS);
    }


    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        LayoutElement{1, 0, 4, VT_FLOAT32, False},
        LayoutElement{2, 0, 3, VT_FLOAT32, False},
        LayoutElement{3, 0, 3, VT_FLOAT32, False},
        LayoutElement{4, 0, 3, VT_FLOAT32, False},
        LayoutElement{5, 0, 3, VT_FLOAT32, False},
        LayoutElement(6,0,4,VT_FLOAT32,False),
        LayoutElement(7,0,4,VT_FLOAT32,False)

    };
    // clang-format on

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // clang-format off
    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    ShaderResourceVariableDesc Vars[] =
    {
        {SHADER_TYPE_PIXEL, "v_Texture", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        {SHADER_TYPE_PIXEL, "v_TextureNorm", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        { SHADER_TYPE_PIXEL, "v_TextureSpec", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC },

    };

    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    // clang-format off
    // Define immutable sampler for g_Texture. Immutable samplers should be used whenever possible
    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "v_Texture", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "v_TextureNorm", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "v_TextureSpec", SamLinearClampDesc},


    };
    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    Engine::m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PS_Actor);

    // Since we did not explicitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables
    // never change and are bound directly through the pipeline state object.
    m_PS_Actor->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_ActorConstants);


    // Since we are using mutable variable, we must create a shader resource binding object
    // http://diligentgraphics.com/2016/03/23/resource-binding-model-in-diligent-engine-2-0/
    m_PS_Actor->CreateShaderResourceBinding(&m_Actor_SRB, true);


}

