// perkunas_point.h

#ifndef INCLUDED_PERKUNAS_POINT_H
#define INCLUDED_PERKUNAS_POINT_H

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace geometry
	{
		template<common::concepts::Arithmethic ArithmethicType>
		struct Point
		{
			ArithmethicType m_x;
			ArithmethicType m_y;

			template<common::concepts::Arithmethic _Other_type>
			operator Point<_Other_type>() noexcept
			{
				return { static_cast<_Other_type>(m_x), static_cast<_Other_type>(m_y) } ;
			}
		};
	}
}
#endif // !INCLUDED_PERKUNAS_POINT_H
