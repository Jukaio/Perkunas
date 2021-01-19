// perkunas_line.h

#ifndef INCLUDED_PERKUNAS_LINE_H
#define INCLUDED_PERKUNAS_LINE_H

#include "perkunas_point.h"

namespace perkunas
{
	namespace geometry
	{
		template<common::concepts::Arithmethic Number>
		struct Line 
		{
			Point<Number> m_start;
			Point<Number> m_end;

			template<common::concepts::Arithmethic OtherNumber>
			operator Line<OtherNumber>() noexcept
			{
				return { { Point<OtherNumber>(m_start) },
						 { Point<OtherNumber>(m_end) } };
			}
		};
	}
}
#endif // !INCLUDED_PERKUNAS_LINE_H
