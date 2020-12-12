// perkunas_point.h

#ifndef INCLUDED_PERKUNAS_POINT_H
#define INCLUDED_PERKUNAS_POINT_H

#include "perkunas_template_concepts.h"

namespace perkunas
{
	template<_Arithmethic_concept _Type>
	struct Point
	{
		_Type m_x;
		_Type m_y;

		template<_Arithmethic_concept _Other_type>
		operator Point<_Other_type>() noexcept
		{
			return { static_cast<_Other_type>(m_x), static_cast<_Other_type>(m_y) } ;
		}
	};
}
#endif // !INCLUDED_PERKUNAS_POINT_H
