/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Sync/Event.hpp>

namespace d3d11_renderer
{
	class Event
		: public renderer::Event
	{
	public:
		Event( Device const & device );
		~Event();
		/**
		*\copydoc	renderer::Event::getStatus
		*/
		renderer::EventStatus getStatus()const override;
		/**
		*\copydoc	renderer::Event::getStatus
		*/
		void set()const override;
		/**
		*\copydoc	renderer::Event::getStatus
		*/
		void reset()const override;

	private:
		mutable renderer::EventStatus m_status{ renderer::EventStatus::eReset };
	};
}