/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "RendererPrerequisites.hpp"
#include "RenderingResources.hpp"

#include <VkLib/SwapChain.hpp>

namespace renderer
{
	class SwapChain
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, utils::IVec2 const & size );
		/**
		*\brief
		*	D�finit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		void setClearColour( utils::RgbaColour const & value );
		/**
		*\return
		*	R�cup�re les ressources de rendu actives.
		*/
		RenderingResources * getResources();
		/**
		*\return
		*	R�cup�re les ressources de rendu par d�faut.
		*/
		RenderingResources const & getDefaultResources()const
		{
			return *m_renderingResources[0];
		}
		/**
		*\return
		*	Pr�sente les ressources de rendu.
		*/
		void present( RenderingResources & resources );
		/**
		*\return
		*	Les dimensions de la swap chain.
		*/
		inline utils::IVec2 getDimensions()const
		{
			return { m_swapChain->getWidth(), m_swapChain->getHeight() };
		}
		/**
		*\return
		*	La swap chain de vulkan.
		*/
		inline vk::SwapChain const & getSwapChain()const
		{
			return *m_swapChain;
		}

	private:
		bool doCheckNeedReset( VkResult errCode
			, bool acquisition
			, char const * const action );
		void SwapChain::doResetSwapChain();

	private:
		Device const & m_device;
		utils::IVec2 m_dimensions;
		vk::SwapChainPtr m_swapChain;
		//! Les ressources de rendu li�es � la swap chain.
		std::vector< RenderingResourcesPtr > m_renderingResources;
		//! L'indice de la ressource de rendu active.
		mutable size_t m_resourceIndex{ 0 };
	};
}