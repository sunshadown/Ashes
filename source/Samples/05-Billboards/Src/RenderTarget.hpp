#pragma once

#include "Prerequisites.hpp"

#include <Camera.hpp>
#include <RenderTarget.hpp>
#include <Miscellaneous/Offset2D.hpp>

namespace vkapp
{
	class RenderTarget
		: public common::RenderTarget
	{
	public:
		RenderTarget( ashes::Device const & device
			, ashes::Extent2D const & size
			, common::Scene && scene
			, common::ImagePtrArray && images );

		inline void enableMoveCamera( bool enable )
		{
			m_moveCamera = enable;
		}

		inline void updateMousePosition( ashes::Offset2D const & position )
		{
			m_currentMousePosition = position;
		}

	private:
		void doUpdateProjection( ashes::Extent2D const & size );
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( ashes::Extent2D const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;

	private:
		ashes::UniformBufferPtr< common::SceneData > m_sceneUbo;
		ashes::Offset2D m_previousMousePosition;
		ashes::Offset2D m_currentMousePosition;
		common::Camera m_camera;
		bool m_moveCamera{ false };
	};
}
