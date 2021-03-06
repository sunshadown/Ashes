﻿/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/ImageMemoryBarrier.hpp"

#include "Image/ImageSubresourceRange.hpp"
#include "Image/Texture.hpp"

namespace ashes
{
	ImageMemoryBarrier::ImageMemoryBarrier( AccessFlags srcAccessMask
		, AccessFlags dstAccessMask
		, ImageLayout oldLayout
		, ImageLayout newLayout
		, uint32_t srcQueueFamilyIndex
		, uint32_t dstQueueFamilyIndex
		, Texture const & image
		, ImageSubresourceRange const & subresourceRange )
		: m_srcAccessMask{ srcAccessMask }
		, m_dstAccessMask{ dstAccessMask }
		, m_oldLayout{ oldLayout }
		, m_newLayout{ newLayout }
		, m_srcQueueFamilyIndex{ srcQueueFamilyIndex }
		, m_dstQueueFamilyIndex{ dstQueueFamilyIndex }
		, m_image{ image }
		, m_subresourceRange{ subresourceRange }
	{
	}
}
