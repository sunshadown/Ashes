/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include <Core/Renderer.hpp>

#include <iostream>
#include <regex>

namespace gl_renderer
{
	namespace
	{
		std::string doRetrieveCompilerLog( Device const & device
			, GLuint shaderName )
		{
			auto context = device.getContext();
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( context
				, glGetShaderiv
				, shaderName
				, GL_INFO_LOG_LENGTH
				, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( context
					, glGetShaderInfoLog
					, shaderName
					, infologLength
					, &charsWritten
					, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

		bool doCheckCompileErrors( Device const & device
			, bool compiled
			, GLuint shaderName )
		{
			auto compilerLog = doRetrieveCompilerLog( device
				, shaderName );

			if ( !compilerLog.empty() )
			{
				if ( !compiled )
				{
					renderer::Logger::logError( compilerLog );
				}
				else
				{
					renderer::Logger::logWarning( compilerLog );
				}
			}
			else if ( !compiled )
			{
				renderer::Logger::logError( "Shader compilation failed" );
			}

			return compiled;
		}
	}

	ShaderModule::ShaderModule( Device const & device
		, renderer::ShaderStageFlag stage )
		: renderer::ShaderModule{ device, stage }
		, m_device{ device }
		, m_shader{ m_device.getContext()->glCreateShader( convert( stage ) ) }
		, m_isSpirV{ false }
	{
	}

	ShaderModule::~ShaderModule()
	{
		// Shader object is destroyed by the ShaderProgram.
	}

	void ShaderModule::loadShader( std::string const & shader )
	{
		std::string source = shader;

		if ( m_stage == renderer::ShaderStageFlag::eVertex )
		{
			std::regex regex{ R"(void[ ]*main)" };
			source = std::regex_replace( shader.data()
				, regex
				, R"(vec4 rendererScalePosition(vec4 pos)
{
	mat4 scale;
	scale[0] = vec4( 1.0, 0.0, 0.0, 0.0 );
	scale[1] = vec4( 0.0, -1.0, 0.0, 0.0 );
	scale[2] = vec4( 0.0, 0.0, 1.0, 0.0 );
	scale[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
	return scale * pos;
}

$&)" );
		}

		auto length = int( source.size() );
		char const * data = source.data();
		auto context = m_device.getContext();
		glLogCall( context
			, glShaderSource
			, m_shader
			, 1
			, &data
			, &length );
		glLogCall( context
			, glCompileShader
			, m_shader );
		int compiled = 0;
		glLogCall( context
			, glGetShaderiv
			, m_shader
			, GL_INFO_COMPILE_STATUS
			, &compiled );

		if ( !doCheckCompileErrors( m_device, compiled != 0, m_shader ) )
		{
			throw std::runtime_error{ "Shader compilation failed." };
		}
	}

	void ShaderModule::loadShader( renderer::ByteArray const & fileData )
	{
		if ( !m_device.getRenderer().isSPIRVSupported() )
		{
			throw std::runtime_error{ "Shader compilation from SPIR-V is not supported." };
		}

		auto context = m_device.getContext();
		context->glShaderBinary( 1u, &m_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, fileData.data(), GLsizei( fileData.size() ) );
		m_isSpirV = true;
	}
}