/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Pipeline_HPP___
#define ___D3D11Renderer_Pipeline_HPP___
#pragma once

#include "Shader/D3D11ShaderDesc.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Pipeline/Pipeline.hpp>

#include <optional>

namespace d3d11_renderer
{
	class Pipeline
		: public ashes::Pipeline
	{
	public:
		Pipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::GraphicsPipelineCreateInfo && createInfo );
		~Pipeline();
		PushConstantsBuffer findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;

		inline ashes::InputAssemblyState const & getInputAssemblyState()const
		{
			return m_createInfo.inputAssemblyState;
		}

		inline bool hasVertexLayout()const
		{
			return !m_createInfo.vertexInputState.vertexBindingDescriptions.empty()
				&& !m_createInfo.vertexInputState.vertexAttributeDescriptions.empty();
		}

		inline std::vector< PushConstantsBuffer > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

		inline ID3D11DepthStencilState * getDSState()const
		{
			return m_dsState;
		}

		inline ID3D11RasterizerState * getRSState()const
		{
			return m_rsState;
		}

		inline ID3D11InputLayout * getIAState()const
		{
			return m_iaState;
		}

		inline ID3D11BlendState * getBDState()const
		{
			return m_bdState;
		}

		inline ashes::VertexInputState const & getVertexInputState()const
		{
			return this->m_createInfo.vertexInputState;
		}

		inline bool hasViewport()const
		{
			return ( bool )m_viewport;
		}

		inline bool hasScissor()const
		{
			return ( bool )m_scissor;
		}

		inline std::array< FLOAT, 4u > getBlendFactor()const
		{
			return m_createInfo.colourBlendState.blendConstants;
		}

		inline UINT getSampleMask()const
		{
			return m_createInfo.colourBlendState.attachs.empty()
				? 0u
				: m_createInfo.colourBlendState.attachs.begin()->colorWriteMask;
		}

		inline UINT getStencilRef()const
		{
			return m_createInfo.depthStencilState
				? m_createInfo.depthStencilState.value().front.reference
				: 0u;
		}

		inline D3D11_VIEWPORT const & getViewport()const
		{
			assert( m_viewport );
			return *m_viewport;
		}

		inline RECT const & getScissor()const
		{
			assert( m_scissor );
			return *m_scissor;
		}

		inline std::list< CompiledShaderModule > const & getShaderStages()const
		{
			return m_programModules;
		}

		inline ProgramLayout const & getProgramLayout()const
		{
			return m_programLayout;
		}

		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}

		inline bool hasDynamicState( ashes::DynamicState state )const
		{
			return m_createInfo.dynamicStates.end() != std::find( m_createInfo.dynamicStates.begin()
				, m_createInfo.dynamicStates.end()
				, state );
		}

	private:
		void doCreateBlendState( Device const & device );
		void doCreateRasterizerState( Device const & device );
		void doCreateDepthStencilState( Device const & device );
		void doCompileProgram( Device const & device );
		void doCreateInputLayout( Device const & device );

	private:
		ID3D11DepthStencilState * m_dsState{ nullptr };
		ID3D11RasterizerState * m_rsState{ nullptr };
		ID3D11InputLayout * m_iaState{ nullptr };
		ID3D11BlendState * m_bdState{ nullptr };
		std::optional< RECT > m_scissor;
		std::optional< D3D11_VIEWPORT > m_viewport;
		std::vector< PushConstantsBuffer > m_constantsPcbs;
		ProgramLayout m_programLayout;
		std::list< CompiledShaderModule > m_programModules;
		size_t m_vertexInputStateHash;
	};
}

#endif
