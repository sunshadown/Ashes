#include "SceneRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"
#include "Texture.h"

#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/RenderingResources.hpp>
//#include <Renderer/RenderPass.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

#include <algorithm>

namespace render
{
	//*************************************************************************

	namespace
	{
		renderer::DescriptorSetLayout doCreateUboDescriptorLayout( renderer::Device const & device
			, ObjectType type )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( 0u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex );
			bindings.emplace_back( 1u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment );

			if ( type == ObjectType::eBillboard
				|| type == ObjectType::ePolyLine )
			{
				bindings.emplace_back( 0u
					, renderer::DescriptorType::eUniformBuffer
					, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );
			}

			return renderer::DescriptorSetLayout{ device, bindings };
		}

		renderer::DescriptorSet doCreateMaterialDescriptor( renderer::DescriptorSetPool const & pool
			, Material const & material )
		{
			renderer::DescriptorSet result{ pool };
			uint32_t index = 0u;

			if ( material.hasDiffuseMap() )
			{
				result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
					, material.diffuseMap().texture()
					, material.diffuseMap().sampler() );
			}

			if ( material.hasOpacityMap() )
			{
				result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
					, material.opacityMap().texture()
					, material.opacityMap().sampler() );
			}

			return result;
		}
	}

	//*************************************************************************

	SceneRenderer::RenderNode::RenderNode( renderer::Device const & device
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: m_program{ std::move( program ) }
		, m_mtxUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_matUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_uboDescriptorLayout{ std::move( layout ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout }
		, m_uboDescriptor{ m_uboDescriptorPool.createDescriptorSet() }
	{
		m_uboDescriptor.createBinding( { 0u
			, renderer::DescriptorType::eUniformBuffer
			, renderer::ShaderStageFlag::eVertex }
		, m_mtxUbo );
		m_uboDescriptor.createBinding( { 1u
			, renderer::DescriptorType::eUniformBuffer
			, renderer::ShaderStageFlag::eFragment }
		, m_matUbo );
	}

	//*************************************************************************

	SceneRenderer::ObjectNode::ObjectNode( renderer::Device const & device
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ), type }
		, m_pipelineLayout{ device, nullptr }
		, m_posLayout{ 0u }
		, m_nmlLayout{ 1u }
		, m_texLayout{ 2u }
	{
		//m_pipeline = std::make_shared< renderer::Pipeline >( device
		//	, m_pipelineLayout
		//	, *m_program
		//	, { m_posLayout, m_nmlLayout, m_texLayout }
		//	, vk::RenderPass{ resources.getDevice()
		//		, { VK_FORMAT_R8G8B8A8_UNORM }
		//		, {}
		//		, vk::RenderPassState{}
		//		, vk::RenderPassState{}
		//		, true
		//		, VK_SAMPLE_COUNT_32_BIT }
		//	, renderer::PrimitiveTopology::eTriangleList );
		m_posLayout.createAttribute< utils::Vec3 >( 0u, 0u );
		m_nmlLayout.createAttribute< utils::Vec3 >( 1u, 0u );
		m_texLayout.createAttribute< utils::Vec2 >( 2u, 0u );
	}

	//*************************************************************************

	SceneRenderer::BillboardNode::BillboardNode( renderer::Device const & device
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ), type }
		, m_billboardUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_pipelineLayout{ device, nullptr }
		, m_layout{ 0u }
	{
		m_uboDescriptor.createBinding( { 2u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment }
			, m_billboardUbo );
		//m_pipeline = std::make_shared< renderer::Pipeline >( device
		//	, m_pipelineLayout
		//	, *m_program
		//	, { m_posLayout, m_nmlLayout, m_texLayout }
		//	, vk::RenderPass{ resources.getDevice()
		//		, { VK_FORMAT_R8G8B8A8_UNORM }
		//		, {}
		//		, vk::RenderPassState{}
		//		, vk::RenderPassState{}
		//		, true
		//		, VK_SAMPLE_COUNT_32_BIT }
		//	, renderer::PrimitiveTopology::eTriangleFan );
		m_layout.createAttribute< utils::Vec3 >( 0u, offsetof( BillboardData, center ) );
		m_layout.createAttribute< utils::Vec2 >( 1u, offsetof( BillboardData, scale ) );
		m_layout.createAttribute< utils::Vec2 >( 2u, offsetof( BillboardBuffer::Vertex, texture ) );
	}

	//*************************************************************************

	SceneRenderer::PolyLineNode::PolyLineNode( renderer::Device const & device
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ), type }
		, m_lineUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_pipelineLayout{ device, nullptr }
		, m_layout{ 0u }
	{
		m_uboDescriptor.createBinding( { 2u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment }
			, m_lineUbo );
		//m_pipeline = std::make_shared< renderer::Pipeline >( device
		//	, m_pipelineLayout
		//	, *m_program
		//	, { m_posLayout, m_nmlLayout, m_texLayout }
		//	, vk::RenderPass{ resources.getDevice()
		//		, { VK_FORMAT_R8G8B8A8_UNORM }
		//		, {}
		//		, vk::RenderPassState{}
		//		, vk::RenderPassState{}
		//		, true
		//		, VK_SAMPLE_COUNT_32_BIT }
		//	, renderer::PrimitiveTopology::eTriangleFan );
		m_layout.createAttribute< utils::Vec3 >( 0u, offsetof( PolyLine::Vertex, m_position ) );
		m_layout.createAttribute< utils::Vec2 >( 1u, offsetof( PolyLine::Vertex, m_normal ) );
	}

	//*************************************************************************

	SceneRenderer::SceneRenderer( renderer::Device const & device )
		: m_device{ device }
	{
	}

	void SceneRenderer::initialise()
	{
		uint32_t i = 0u;
		for ( auto & node : m_objectNodes )
		{
			node = std::make_unique< ObjectNode >( m_device
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eObject )
				, UberShader::createShaderProgram( m_device
					, RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eObject )
				, NodeType( i ) );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			node = std::make_unique< BillboardNode >( m_device
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eBillboard )
				, UberShader::createShaderProgram( m_device
					, RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eBillboard )
				, NodeType( i ) );
			++i;
		}

		m_lineNode = std::make_unique< PolyLineNode >( m_device
			, doCreateUboDescriptorLayout( m_device
				, ObjectType::ePolyLine )
			, UberShader::createShaderProgram( m_device
				, RenderType::eScene
				, TextureFlag::eNone
				, OpacityType::eOpaque
				, ObjectType::ePolyLine )
			, NodeType( i ) );
	}

	void SceneRenderer::cleanup()
	{
		for ( auto & node : m_objectNodes )
		{
			node.reset();
		}

		for ( auto & node : m_billboardNodes )
		{
			node.reset();
		}

		m_lineNode.reset();
	}

	void SceneRenderer::draw( renderer::RenderingResources const & resources
		, Camera const & camera
		, float zoomScale
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, RenderPolyLineArray const & lines )const
	{
		doRenderObjects( resources
			, camera
			, NodeType::eOpaqueNoTex
			, *m_objectNodes[size_t( NodeType::eOpaqueNoTex )]
			, objects[size_t( NodeType::eOpaqueNoTex )] );
		doRenderObjects( resources
			, camera
			, NodeType::eOpaqueDiff
			, *m_objectNodes[size_t( NodeType::eOpaqueDiff )]
			, objects[size_t( NodeType::eOpaqueDiff )] );
		doRenderBillboards( resources
			, camera
			, NodeType::eOpaqueNoTex
			, *m_billboardNodes[size_t( NodeType::eOpaqueNoTex )]
			, billboards[size_t( NodeType::eOpaqueNoTex )] );
		doRenderBillboards( resources
			, camera
			, NodeType::eOpaqueDiff
			, *m_billboardNodes[size_t( NodeType::eOpaqueDiff )]
			, billboards[size_t( NodeType::eOpaqueDiff )] );
		doRenderLines( resources
			, camera
			, zoomScale
			, *m_lineNode
			, lines[size_t( NodeType::eOpaqueNoTex )] );
		doRenderTransparent( resources
			, camera
			, NodeType::eAlphaTest
			, OpacityType::eAlphaTest
			, objects
			, billboards
			, lines );
		doRenderTransparent( resources
			, camera
			, NodeType::eAlphaBlend
			, OpacityType::eAlphaBlend
			, objects
			, billboards
			, lines );
	}

	void SceneRenderer::doRenderTransparent( renderer::RenderingResources const & resources
		, Camera const & camera
		, NodeType type
		, OpacityType opacity
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, RenderPolyLineArray const & lines )const
	{
		size_t nodeType{ size_t( type )
			+ size_t( TransparentNodeType::eNoTex ) };
		doRenderObjects( resources
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderObjects( resources
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderObjects( resources
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderObjects( resources
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eNoTex );
		doRenderBillboards( resources
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderBillboards( resources
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderBillboards( resources
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderBillboards( resources
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
	}

	void SceneRenderer::doRenderObjects( renderer::RenderingResources const & resources
		, Camera const & camera
		, NodeType type
		, ObjectNode & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			resources.getCommandBuffer().bindPipeline( *node.m_pipeline );
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					node.m_mtxUbo.getData().model = object.m_object->transform();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					node.m_matUbo.getData().ambient = object.m_material->ambient();
					node.m_matUbo.getData().diffuse = object.m_material->diffuse();
					node.m_matUbo.getData().specular = object.m_material->specular();
					node.m_matUbo.getData().emissive = object.m_material->emissive();
					node.m_matUbo.getData().exponent = object.m_material->exponent();
					node.m_matUbo.getData().opacity = object.m_material->opacity();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_matUbo.getDatas()
						, node.m_matUbo );
					resources.getCommandBuffer().bindDescriptorSet( object.m_materialDescriptor
						, node.m_pipelineLayout );
					resources.getCommandBuffer().bindVertexBuffers( { std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getPositions() ) )
						, std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getNormals() ) )
						, std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getTexCoords() ) ) }
					, { 0u, 0u, 0u } );
					resources.getCommandBuffer().bindIndexBuffer( object.m_submesh->getIbo()
						, 0u
						, renderer::IndexType::eUInt16 );
					//node.m_scale->bind();
					resources.getCommandBuffer().draw( object.m_submesh->getIndexCount()
						, 1u
						, 0u
						, 0u );
				}
			}
		}
	}

	void SceneRenderer::doRenderBillboards( renderer::RenderingResources const & resources
		, Camera const & camera
		, NodeType type
		, BillboardNode & node
		, RenderBillboardVector const & billboards )const
	{
		if ( !billboards.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			utils::Vec3 const & position = camera.position();
			resources.getCommandBuffer().bindPipeline( *node.m_pipeline );
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_billboardUbo.getData().camera = position;

			for ( auto & billboard : billboards )
			{
				if ( billboard.m_billboard->visible()
					&& billboard.m_billboard->buffer().count() )
				{
					node.m_mtxUbo.getData().model = billboard.m_billboard->transform();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					node.m_billboardUbo.getData().dimensions = utils::Vec2{ billboard.m_billboard->dimensions() };
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_billboardUbo.getDatas()
						, node.m_billboardUbo );
					node.m_matUbo.getData().ambient = billboard.m_billboard->material().ambient();
					node.m_matUbo.getData().diffuse = billboard.m_billboard->material().diffuse();
					node.m_matUbo.getData().specular = billboard.m_billboard->material().specular();
					node.m_matUbo.getData().emissive = billboard.m_billboard->material().emissive();
					node.m_matUbo.getData().exponent = billboard.m_billboard->material().exponent();
					node.m_matUbo.getData().opacity = billboard.m_billboard->material().opacity();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_matUbo.getDatas()
						, node.m_matUbo );
					resources.getCommandBuffer().bindDescriptorSet( billboard.m_materialDescriptor
						, node.m_pipelineLayout );
					resources.getCommandBuffer().bindVertexBuffer( billboard.m_billboard->buffer().vbo()
						, 0u );
					resources.getCommandBuffer().draw( billboard.m_billboard->buffer().count() * 6
						, 1u
						, 0u
						, 0u );
				}
			}
		}
	}

	void SceneRenderer::doRenderLines( renderer::RenderingResources const & resources
		, Camera const & camera
		, float zoomScale
		, PolyLineNode & node
		, RenderPolyLineVector const & lines )const
	{
		if ( !lines.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			utils::Vec3 const & position = camera.position();
			resources.getCommandBuffer().bindPipeline( *node.m_pipeline );
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_lineUbo.getData().lineScale = zoomScale;
			node.m_lineUbo.getData().camera = position;

			for ( auto & line : lines )
			{
				if ( line.m_line->visible()
					&& line.m_line->count() )
				{
					node.m_mtxUbo.getData().model = line.m_line->transform();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					node.m_lineUbo.getData().lineWidth = line.m_line->width();
					node.m_lineUbo.getData().lineFeather = line.m_line->feather();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_lineUbo.getDatas()
						, node.m_lineUbo );
					node.m_matUbo.getData().ambient = line.m_line->material().ambient();
					node.m_matUbo.getData().diffuse = line.m_line->material().diffuse();
					node.m_matUbo.getData().specular = line.m_line->material().specular();
					node.m_matUbo.getData().emissive = line.m_line->material().emissive();
					node.m_matUbo.getData().exponent = line.m_line->material().exponent();
					node.m_matUbo.getData().opacity = line.m_line->material().opacity();
					resources.getStagingBuffer().copyUniformData( resources.getCommandBuffer()
						, node.m_matUbo.getDatas()
						, node.m_matUbo );
					resources.getCommandBuffer().bindDescriptorSet( line.m_materialDescriptor
						, node.m_pipelineLayout );
					//m_resources.getCommandBuffer().bindVertexBuffer( line.m_line->buffer()
					//	, 0u );
					resources.getCommandBuffer().draw( line.m_line->count() * 6
						, 1u
						, 0u
						, 0u );
				}
			}
		}
	}

	//*************************************************************************
}
