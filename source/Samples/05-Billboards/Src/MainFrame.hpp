#pragma once

#include "Prerequisites.hpp"

#include <Core/Renderer.hpp>

#include <MainFrame.hpp>

namespace vkapp
{
	class MainFrame
		: public common::MainFrame
	{
	public:
		MainFrame( wxString const & rendererName
			, common::RendererFactory & factory );

	private:
		common::RenderPanel * doCreatePanel( wxSize const & size, ashes::Renderer const & renderer )override;
	};
}
