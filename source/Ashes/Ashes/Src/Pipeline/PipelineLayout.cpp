/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Pipeline/PipelineLayout.hpp"

#include "Core/Device.hpp"
#include "Core/RenderingResources.hpp"
#include "Descriptor/DescriptorSetLayout.hpp"

namespace ashes
{
	PipelineLayout::PipelineLayout( Device const & device
		, DescriptorSetLayoutCRefArray const & layouts
		, PushConstantRangeArray const & pushConstantRanges )
		: m_device{ device }
		, m_pushConstantRanges{ pushConstantRanges }
	{
		registerObject( m_device, "PipelineLayout", this );
	}

	PipelineLayout::~PipelineLayout()
	{
		unregisterObject( m_device, this );
	}
}
