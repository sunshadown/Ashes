/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Image/TestStagingTexture.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Image/TestTextureView.hpp"

#include <Core/Exception.hpp>
#include <Miscellaneous/Offset2D.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

namespace test_renderer
{
	namespace
	{
		uint32_t getSize( ashes::Extent2D const & extent )
		{
			return std::max( 1u, extent.width )
				* std::max( 1u, extent.height );
		}
	}

	StagingTexture::StagingTexture( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & extent )
		: ashes::StagingTexture{ device, extent }
		, m_device{ device }
		, m_buffer{ device, getSize( extent ) * getSize( format ), ashes::BufferTarget::eTransferDst | ashes::BufferTarget::eTransferSrc }
	{
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, ashes::MemoryMapFlag::eWrite | ashes::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw ashes::Exception{ ashes::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::TextureView const & texture )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eTransfer
			, m_buffer.makeTransferSource() );
		commandBuffer.copyToImage( ashes::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				ashes::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, m_buffer
			, texture.getTexture() );
	}

	void StagingTexture::doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::TextureView const & texture )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.copyToBuffer( ashes::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				ashes::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, texture.getTexture()
			, m_buffer );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, ashes::MemoryMapFlag::eWrite | ashes::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw ashes::Exception{ ashes::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, buffer
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}
}
