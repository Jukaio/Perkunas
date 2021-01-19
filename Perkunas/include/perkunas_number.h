// perkunas_number.h
// experimental!

#ifndef INCLUDED_PERKUNAS_NUMBER
#define INCLUDED_PERKUNAS_NUMBER

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace common
	{
		template <common::concepts::Arithmethic Number>
		constexpr Number max_value()
		{
			return Number(~0);
		}

		template<common::concepts::Arithmethic Number>
		struct Number
		{
			Number m_value;

			template<common::concepts::Arithmethic OtherNumber>
			operator OtherNumber() noexcept
			{
				return static_cast<OtherNumber>(m_value);
			}
			Number()
				: m_value(max_value<Number>())
			{
			}

		};
	}
}
#endif // !INCLUDED_PERKUNAS_NUMBER
