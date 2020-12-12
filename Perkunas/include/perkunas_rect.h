// perkunas_rect.h

#ifndef INCLUDED_PERKUNAS_RECT_H
#define INCLUDED_PERKUNAS_RECT_H

#include "perkunas_template_concepts.h"

namespace perkunas
{
	template<_Arithmethic_concept _Type>
	struct Rectangle
	{
		_Type m_x, m_y;
		_Type m_w, m_h;

		template<_Arithmethic_concept _Other_type>
		operator Rectangle<_Other_type>() noexcept
		{
			return { static_cast<_Other_type>(m_x), 
					 static_cast<_Other_type>(m_y),
					 static_cast<_Other_type>(m_w), 
					 static_cast<_Other_type>(m_h) };
		}
	};
}

#endif // !INCLUDED_PERKUNAS_RECT_H