// perkunas_number.h
// experimental!

#ifndef INCLUDED_PERKUNAS_NUMBER
#define INCLUDED_PERKUNAS_NUMBER

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace common
	{
		template <common::concepts::Arithmethic ArithmethicType>
		constexpr ArithmethicType max_value()
		{
			return ArithmethicType(~0);
		}

		template<common::concepts::Arithmethic ArithmethicType>
		struct Number
		{
			ArithmethicType m_value;

			template<common::concepts::Arithmethic _Other_type>
			operator _Other_type() noexcept
			{
				return static_cast<_Other_type>(m_value);
			}
			Number()
				: m_value(max_value<ArithmethicType>())
			{
			}

		};
	}
}
#endif // !INCLUDED_PERKUNAS_NUMBER
