/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Core/VkConnection.hpp"

#include <Core/Device.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
		: public ashes::Device
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] vk_renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Renderer const & renderer
			, ashes::ConnectionPtr && connection );
		/**
		*\brief
		*	Destructeur.
		*/
		~Device();
		void updateSurfaceCapabilities()const;
		/**
		*\copydoc	ashes::Device::createStagingTexture
		*/
		ashes::StagingTexturePtr createStagingTexture( ashes::Format format
			, ashes::Extent2D const & extent )const;
		/**
		*\copydoc	ashes::Device::createRenderPass
		*/
		ashes::RenderPassPtr createRenderPass( ashes::RenderPassCreateInfo createInfo )const override;
		/**
		*\copydoc	ashes::Device::createPipelineLayout
		*/
		ashes::PipelineLayoutPtr createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const override;
		/**
		*\copydoc	ashes::Device::createDescriptorSetLayout
		*/
		ashes::DescriptorSetLayoutPtr createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray && bindings )const override;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		ashes::DescriptorPoolPtr createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes )const override;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		ashes::DeviceMemoryPtr allocateMemory( ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags )const override;
		/**
		*\copydoc	ashes::Device::createTexture
		*/
		ashes::TexturePtr createTexture( ashes::ImageCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( ashes::Texture const & image
			, ashes::ImageSubresource const & subresource
			, ashes::SubresourceLayout & layout )const override;
		/**
		*\copydoc	ashes::Device::createSampler
		*/
		ashes::SamplerPtr createSampler( ashes::SamplerCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::createBuffer
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, ashes::BufferTargets target )const override;
		/**
		*\copydoc	ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, ashes::Format format
			, uint32_t offset
			, uint32_t range )const override;
		/**
		*\copydoc	ashes::Device::createUniformBuffer
		*/
		ashes::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, ashes::BufferTargets target
			, ashes::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc	ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( ashes::Extent2D const & size )const override;
		/**
		*\copydoc	ashes::Device::createSemaphore
		*/
		ashes::SemaphorePtr createSemaphore()const override;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		ashes::FencePtr createFence( ashes::FenceCreateFlags flags )const override;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		ashes::EventPtr createEvent()const override;
		/**
		*\copydoc	ashes::Device::createCommandPool
		*/
		ashes::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, ashes::CommandPoolCreateFlags const & flags )const override;
		/**
		*\copydoc	ashes::Device::createShaderProgram
		*/
		virtual ashes::ShaderModulePtr createShaderModule( ashes::ShaderStageFlag stage )const override;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( ashes::QueryType type
			, uint32_t count
			, ashes::QueryPipelineStatisticFlags pipelineStatistics )const override;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		void waitIdle()const override;
		/**
		*\~french
		*\brief
		*	Récupère les propriétés mémoire requises pour le tampon donné.
		*\param[in] buffer
		*	Le tampon.
		*return
		*	Les propriétés mémoire.
		*\~english
		*\brief
		*	Retrieves the memory requirements for given buffer.
		*\param[in] buffer
		*	The buffer.
		*return
		*	The memory requirements.
		*/
		ashes::MemoryRequirements getBufferMemoryRequirements( VkBuffer buffer )const;
		/**
		*\~french
		*\brief
		*	Récupère les propriétés mémoire requises pour le tampon donné.
		*\param[in] image
		*	L'image.
		*return
		*	Les propriétés mémoire.
		*\~english
		*\brief
		*	Retrieves the memory requirements for given buffer.
		*\param[in] image
		*	The image.
		*return
		*	The memory requirements.
		*/
		ashes::MemoryRequirements getImageMemoryRequirements( VkImage image )const;
		/**
		*\~french
		*\return
		*	L'API de rendu.
		*\~english
		*\return
		*	The rendering API.
		*/
		inline Renderer const & getRenderer()const
		{
			return m_renderer;
		}
		/**
		*\~french
		*\return
		*	La connection à l'application.
		*\~english
		*\return
		*	The connection to the application.
		*/
		inline Connection const & getConnection()const
		{
			return *m_connection;
		}
		/**
		*\~french
		*\return
		*	La surface de présentation.
		*\~english
		*\return
		*	The presentation surface.
		*/
		inline auto getPresentSurface()const
		{
			return m_connection->getPresentSurface();
		}
		/**
		*\~french
		*\return
		*	Les capacités de la surface de présentation.
		*\~english
		*\return
		*	The presentation surface's capabilities.
		*/
		inline auto & getSurfaceCapabilities()const
		{
			return m_connection->getSurfaceCapabilities();
		}
		/**
		*\brief
		*	Le VkDevice.
		*/
		inline operator VkDevice const &()const
		{
			return m_device;
		}

#define VK_LIB_DEVICE_FUNCTION( fun ) PFN_##fun fun;
#	include "Miscellaneous/VulkanFunctionsList.inl"

	private:
		Renderer const & m_renderer;
		PhysicalDevice const & m_gpu;
		ConnectionPtr m_connection;
		VkPhysicalDeviceFeatures m_enabledFeatures;
		VkDevice m_device{ VK_NULL_HANDLE };
	};
}
