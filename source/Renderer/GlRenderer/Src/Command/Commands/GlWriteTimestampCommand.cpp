/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlWriteTimestampCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	WriteTimestampCommand::WriteTimestampCommand( renderer::PipelineStageFlag pipelineStage
		, renderer::QueryPool const & pool
		, uint32_t query )
		: m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
	{
	}

	void WriteTimestampCommand::apply()const
	{
		glLogCall( glQueryCounter, m_query, GL_TIMESTAMP );
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
