/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ClearAttachment_HPP___
#define ___Ashes_ClearAttachment_HPP___
#pragma once

#include "RenderPass/ClearValue.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies clear attachment.
	*\~french
	*\brief
	*	Définit le nettoyage d'une attache.
	*/
	struct ClearAttachment
	{
		/**
		*\~english
		*\brief
		*	Selects the colour, depth and/or stencil aspects of the attachment to be cleared.
		*	\p aspectMask can include ashes::ImageAspectFlag::eColour for colour attachments, ashes::ImageAspectFlag::eDepth for depth/stencil attachments with a depth component, and ashes::ImageAspectFlag::eStencil for depth/stencil attachments with a stencil component.
		*	If the subpass’s depth/stencil attachment is ashes::AttachmentUnused, then the clear has no effect.
		*\~french
		*\brief
		*	Sélectionne l'aspect couleur, profondeur et/ou stencil de l'attache à nettoyer.
		*	\p aspectMask peut inclure ashes::ImageAspectFlag::eColour pour les attaches couleur, ashes::ImageAspectFlag::eDepth pour les attaches profondeur, et ashes::ImageAspectFlag::eStencil pour les attaches stencil.
		*	Si l'attache profondeur/stencil de la sous-passe vaut ashes::AttachmentUnused, alors le nettoyage n'a aucun effet.
		*/
		ImageAspectFlags aspectMask;
		/**
		*\~english
		*\brief
		*	Only meaningful if ashes::ImageAspectFlag::eColour is set in \p aspectMask, in which case it is an index to the \p colourAttaches array in the ashes::Subpass constructor of the current subpass which selects the colour attachment to clear.
		*	If \p colourAttachment is ashes::AttachmentUnused then the clear has no effect.
		*\~french
		*\brief
		*	Seulement utile si ashes::ImageAspectFlag::eColour est défini dans \p aspectMask, auquel cas c'est un indice dans le tableau \p colourAttaches du constructeur de render::Subpass pour la sous-passe courante, qui sélectionne l'attache couleur à nettoyer.
		*	Si \p colourAttachment vaut ashes::AttachmentUnused, alors le nettoyage n'a aucun effet.
		*/
		uint32_t colourAttachment;
		/**
		*\~english
		*\brief
		*	The colour or depth/stencil value to clear the attachment to.
		*\~french
		*\brief
		*	La couleur ou valeur profondeur/stencil avec laquelle l'attache est nettoyée.
		*/
		ClearValue clearValue;
	};
}

#endif
