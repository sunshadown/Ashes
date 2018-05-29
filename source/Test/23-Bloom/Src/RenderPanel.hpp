#pragma once

#include "Prerequisites.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

#include <ObjLoader.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, renderer::Renderer const & renderer );
		~RenderPanel();

	private:
		/**
		*\name
		*	Initialisation.
		*/
		/**@{*/
		void doCleanup();
		void doUpdateProjection();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateTexture();
		void doCreateUniformBuffer();
		void doCreateStagingBuffer();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenVertexBuffer();
		void doCreateOffscreenPipeline();
		void doPrepareOffscreenFrame();
		void doPrepareHiPass();
		void doPrepareBlurXPass();
		void doPrepareBlurYPass();
		void doPrepareCombinePass();
		void doCreateMainDescriptorSet();
		void doCreateMainRenderPass();
		void doCreateMainVertexBuffer();
		void doCreateMainPipeline();
		void doPrepareMainFrames();
		/**@}*/
		/**
		*\name
		*	Rendering.
		*/
		/**@{*/
		void doUpdate();
		void doDraw();
		void doResetSwapChain();
		/**@}*/
		/**
		*\name
		*	Events.
		*/
		/**@{*/
		void onTimer( wxTimerEvent & event );
		void onSize( wxSizeEvent & event );
		/**@}*/

	private:
		wxTimer * m_timer{ nullptr };
		renderer::Mat4 m_rotate;
		/**
		*\name
		*	Global.
		*/
		/**@{*/
		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::TextureViewPtr m_view;
		renderer::SamplerPtr m_sampler;
		renderer::TexturePtr m_renderTargetColour;
		renderer::TextureViewPtr m_renderTargetColourView;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::UniformBufferPtr< renderer::Mat4 > m_objectUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		/**@}*/
		/**
		*\name
		*	Offscreen.
		*/
		/**@{*/
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::RenderPassPtr m_offscreenRenderPass;
		renderer::PipelineLayoutPtr m_offscreenPipelineLayout;
		renderer::PipelinePtr m_offscreenPipeline;
		renderer::VertexBufferPtr< NonTexturedVertexData > m_offscreenVertexBuffer;
		renderer::BufferPtr< uint16_t > m_offscreenIndexBuffer;
		renderer::VertexLayoutPtr m_offscreenVertexLayout;
		renderer::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_offscreenDescriptorPool;
		renderer::DescriptorSetPtr m_offscreenDescriptorSet;
		std::vector< NonTexturedVertexData > m_offscreenVertexData;
		renderer::UInt16Array m_offscreenIndexData;
		renderer::QueryPoolPtr m_queryPool;
		/**@}*/
		/**
		*\name
		*	Bloom passes.
		*/
		/**@{*/
		struct Pass
		{
			renderer::CommandBufferPtr commandBuffer;
			renderer::DescriptorSetLayoutPtr descriptorLayout;
			renderer::DescriptorSetPoolPtr descriptorPool;
			renderer::DescriptorSetPtr descriptorSet;
			renderer::PipelineLayoutPtr pipelineLayout;
			renderer::RenderPassPtr renderPass;
			renderer::FrameBufferPtr frameBuffer;
			renderer::PipelinePtr pipeline;
			renderer::TexturePtr image;
			std::vector< renderer::TextureViewPtr > views;
			renderer::SemaphorePtr semaphore;
		};
		struct
		{
			Pass hi;
			std::array< Pass, 4u > blurX;
			std::array< Pass, 4u > blurY;
			Pass combine;
		} m_passes;
		renderer::TextureViewPtr m_blurMipView;
		renderer::SamplerPtr m_mipSampler;
		std::array< renderer::SamplerPtr, 4 > m_blurSamplers;
		struct Configuration
		{
			utils::Vec2 textureSize;
			uint32_t coefficientsCount;
			uint32_t dump;
			std::array< renderer::Vec4, 15u > coefficients;
		};
		renderer::UniformBufferPtr< Configuration > m_blurConfiguration;
		/**@}*/
		/**
		*\name
		*	Main.
		*/
		/**@{*/
		renderer::RenderPassPtr m_mainRenderPass;
		renderer::PipelineLayoutPtr m_mainPipelineLayout;
		renderer::PipelinePtr m_mainPipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
		renderer::VertexLayoutPtr m_mainVertexLayout;
		renderer::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_mainDescriptorPool;
		renderer::DescriptorSetPtr m_mainDescriptorSet;
		std::vector< TexturedVertexData > m_mainVertexData;
		/**@}*/
		/**
		*\name
		*	Swapchain.
		*/
		/**@{*/
		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		std::vector< renderer::CommandBufferPtr > m_commandBuffers;
		renderer::SignalConnection< renderer::SwapChain::OnReset > m_swapChainReset;
		/**@}*/
	};
}