/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11EndRenderPassCommand.hpp"

#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

namespace d3d11_renderer
{
	EndRenderPassCommand::EndRenderPassCommand( Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
	{
	}

	void EndRenderPassCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( 0u, nullptr );
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}