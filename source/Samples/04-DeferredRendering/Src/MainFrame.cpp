#include "MainFrame.hpp"

#include "RenderPanel.hpp"

namespace vkapp
{
	MainFrame::MainFrame( wxString const & rendererName
		, common::RendererFactory & factory )
		: common::MainFrame{ AppName, rendererName, factory }
	{
	}

	common::RenderPanel * MainFrame::doCreatePanel( wxSize const & size, ashes::Renderer const & renderer )
	{
		return new RenderPanel( this, size );
	}
}
