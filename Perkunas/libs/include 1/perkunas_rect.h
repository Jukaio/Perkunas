// perkunas_rect.h

#ifndef INCLUDED_PERKUNAS_RECT_H
#define INCLUDED_PERKUNAS_RECT_H

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace geometry
	{
		template<common::concepts::_Arithmethic_concept ArithmethicType>
		struct Rect
		{
			ArithmethicType m_x, m_y;
			ArithmethicType m_w, m_h;

			template<common::concepts::_Arithmethic_concept OtherArithmethicType>
			operator Rect<OtherArithmethicType>() noexcept
			{
				return { static_cast<OtherArithmethicType>(m_x),
						 static_cast<OtherArithmethicType>(m_y),
						 static_cast<OtherArithmethicType>(m_w),
						 static_cast<OtherArithmethicType>(m_h) };
			}
		};
	}
}

#endif // !INCLUDED_PERKUNAS_RECT_H