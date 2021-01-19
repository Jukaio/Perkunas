// perkunas_point.h

#ifndef INCLUDED_PERKUNAS_POINT_H
#define INCLUDED_PERKUNAS_POINT_H

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace geometry
	{
		template<common::concepts::Arithmethic Number>
		struct Point
		{
			Number m_x;
			Number m_y;

			template<common::concepts::Arithmethic OtherNumber>
			operator Point<OtherNumber>() noexcept
			{
				return { static_cast<OtherNumber>(m_x), 
						 static_cast<OtherNumber>(m_y) } ;
			}
		};
	}
}
#endif // !INCLUDED_PERKUNAS_POINT_H
