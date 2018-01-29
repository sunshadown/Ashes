/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Pipeline/PipelineLayout.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Layout de pipeline.
	*/
	class PipelineLayout
		: public renderer::PipelineLayout
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent LogicalDevice.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] setLayouts
		*	Les layouts des descripteurs du pipeline.
		*\param[in] pushConstantRanges
		*	Les intervalles de push constants.
		*/
		PipelineLayout( Device const & device
			, renderer::DescriptorSetLayoutCRefArray const & setLayouts
			, renderer::PushConstantRangeCRefArray const & pushConstantRanges );
		/**
		*\copydoc		renderer::PipelineLayout::createPipeline
		*/
		renderer::PipelinePtr createPipeline( renderer::ShaderProgram const & program
			, renderer::VertexLayoutCRefArray const & vertexLayouts
			, renderer::RenderPass const & renderPass
			, renderer::PrimitiveTopology topology
			, renderer::RasterisationState const & rasterisationState
			, renderer::ColourBlendState const & colourBlendState )const override;

	private:
		Device const & m_device;
	};
}
