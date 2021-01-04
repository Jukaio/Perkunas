// perkunas_line.h

#ifndef INCLUDED_PERKUNAS_LINE_H
#define INCLUDED_PERKUNAS_LINE_H

#include "perkunas_point.h"

namespace perkunas
{
	namespace geometry
	{
		template<common::concepts::_Arithmethic_concept ArithmethicType>
		struct Line 
		{
			Point<ArithmethicType> m_start;
			Point<ArithmethicType> m_end;

			template<common::concepts::_Arithmethic_concept _Other_type>
			operator Line<_Other_type>() noexcept
			{
				return { { Point<_Other_type>(m_start) },
						 { Point<_Other_type>(m_end) } };
			}
		};
	}
}
#endif // !INCLUDED_PERKUNAS_LINE_H
