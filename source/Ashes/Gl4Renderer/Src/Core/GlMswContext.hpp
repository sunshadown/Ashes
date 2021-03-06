/*
See LICENSE file in root folder
*/
#pragma once

#include "Core/GlContext.hpp"

#if ASHES_WIN32
#	include <Windows.h>

namespace gl_renderer
{
	class MswContext
		: public Context
	{
	public:
		MswContext( PhysicalDevice const & gpu
			, ashes::Connection const & connection
			, Context const * mainContext );
		~MswContext();

		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;

		inline HDC getHDC()const
		{
			return m_hDC;
		}

		inline HGLRC getContext()const
		{
			return m_hContext;
		}

#define WGL_LIB_FUNCTION( fun )\
		PFN_wgl##fun m_wgl##fun = nullptr;\
		template< typename ... Params >\
		inline auto wgl##fun( Params... params )const\
		{\
			return m_wgl##fun( params... );\
		}
#define WGL_LIB_FUNCTION_OPT( fun )\
		PFN_wgl##fun m_wgl##fun = nullptr;\
		template< typename ... Params >\
		inline auto wgl##fun( Params... params )const\
		{\
			return m_wgl##fun( params... );\
		};
#include "Miscellaneous/OpenGLFunctionsList.inl"

	private:
		void doLoadBaseFunctions();
		void doLoadMswFunctions();
		HGLRC doCreateDummyContext();
		bool doSelectFormat();
		bool doCreateGl3Context( Context const * mainContext );

	private:
		HDC m_hDC;
		HGLRC m_hContext;
		HWND m_hWnd;
	};
}

#endif
