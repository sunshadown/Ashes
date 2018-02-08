/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Buffer_HPP___
#define ___Renderer_Buffer_HPP___
#pragma once

#include "Core/Device.hpp"

namespace renderer
{
	/**
	*\brief
	*	Classe de tampon GPU.
	*/
	class BufferBase
	{
	protected:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] size
		*	La taille du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		BufferBase( Device const & device
			, uint32_t size
			, BufferTargets target
			, MemoryPropertyFlags flags );

	public:
		/**
		*\brief
		*	Destructeur.
		*/
		virtual ~BufferBase() = default;
		/**
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		virtual uint8_t * lock( uint32_t offset
			, uint32_t size
			, MemoryMapFlags flags )const = 0;
		/**
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*\param[in] modified
		*	Dit si le tampon a été modifié, et donc si la VRAM doit être mise à jour.
		*/
		virtual void unlock( uint32_t size
			, bool modified )const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		virtual BufferMemoryBarrier makeTransferDestination()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		virtual BufferMemoryBarrier makeTransferSource()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un vertex shader.
		*\return
		*	La barrière mémoire.
		*/
		virtual BufferMemoryBarrier makeVertexShaderInputResource()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		virtual BufferMemoryBarrier makeUniformBufferInput()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout mémoire donné.
		*\param[in] dstAccess
		*	Les indicateurs d'accès voulus après la transition.
		*\return
		*	La barrière mémoire.
		*/
		virtual BufferMemoryBarrier makeMemoryTransitionBarrier( AccessFlags dstAccess )const = 0;
		/**
		*\return
		*	La taille du tampon.
		*/
		inline uint32_t getSize()const
		{
			return m_size;
		}
		/**
		*\return
		*	Les cibles du tampon.
		*/
		inline BufferTargets getTargets()const
		{
			return m_target;
		}

	protected:
		Device const & m_device;
		uint32_t m_size;
		BufferTargets m_target;
	};
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	template< typename T >
	class Buffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		Buffer( Device const & device
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\return
		*	Le nombre d'éléments.
		*/
		inline uint32_t getCount()const
		{
			return uint32_t( m_buffer->getSize() / sizeof( T ) );
		}
		/**
		*\return
		*	Le tampon.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		inline T * lock( uint32_t offset
			, uint32_t size
			, MemoryMapFlags flags )const
		{
			reinterpret_cast< T * >( m_buffer->lock( uint32_t( offset * sizeof( T ) )
				, uint32_t( size * sizeof( T ) )
				, flags ) );
		}
		/**
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*\param[in] modified
		*	Dit si le tampon a été modifié, et donc si la VRAM doit être mise à jour.
		*/
		inline void unlock( uint32_t size
			, bool modified )const
		{
			m_buffer->unlock( size * sizeof( T ), modified );
		}

	private:
		BufferBasePtr m_buffer;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\param[in] device
	*	Les périphérique logique.
	*\param[in] count
	*	Le nombre d'éléments du tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	BufferPtr< T > makeBuffer( Device const & device
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< Buffer< T > >( device
			, count
			, target
			, flags );
	}
}

#include "Buffer.inl"

#endif
