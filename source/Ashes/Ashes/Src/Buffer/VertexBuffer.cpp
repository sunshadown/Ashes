﻿/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/VertexBuffer.hpp"

namespace ashes
{
	VertexBufferBase::VertexBufferBase( Device const & device
		, uint32_t size
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: m_device{ device }
		, m_size{ size }
		, m_buffer{ m_device.createBuffer( size
			, target | BufferTarget::eVertexBuffer
			, flags ) }
	{
	}
}
