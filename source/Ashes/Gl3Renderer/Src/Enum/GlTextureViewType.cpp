#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GlTextureType convert( ashes::TextureViewType const & value )
	{
		switch ( value )
		{
		case ashes::TextureViewType::e1D:
			return GL_TEXTURE_1D;

		case ashes::TextureViewType::e2D:
			return GL_TEXTURE_2D;

		case ashes::TextureViewType::e3D:
			return GL_TEXTURE_3D;

		case ashes::TextureViewType::eCube:
			return GL_TEXTURE_CUBE;

		case ashes::TextureViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case ashes::TextureViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case ashes::TextureViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported TextureViewType" );
			return GL_TEXTURE_2D;
		}
	}

	GlTextureType convert( ashes::TextureViewType const & mode
		, uint32_t layers )
	{
		switch ( mode )
		{
		case ashes::TextureViewType::e1D:
			return layers > 1u
				? GL_TEXTURE_1D_ARRAY
				: GL_TEXTURE_1D;

		case ashes::TextureViewType::e2D:
			return layers > 1u
				? GL_TEXTURE_2D_ARRAY
				: GL_TEXTURE_2D;

		case ashes::TextureViewType::e3D:
			return GL_TEXTURE_3D;

		case ashes::TextureViewType::eCube:
			return layers > 6u
				? GL_TEXTURE_CUBE_ARRAY
				: GL_TEXTURE_CUBE;

		case ashes::TextureViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case ashes::TextureViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case ashes::TextureViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported TextureViewType" );
			return GL_TEXTURE_2D;
		}
	}
}
