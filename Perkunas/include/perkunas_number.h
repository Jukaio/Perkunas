// perkunas_number.h
// experimental!

#ifndef INCLUDED_PERKUNAS_NUMBER
#define INCLUDED_PERKUNAS_NUMBER

#include "perkunas_template_concepts.h"

namespace perkunas
{
	namespace common
	{
		template <common::concepts::Arithmethic Type>
		constexpr Type max_value()
		{
			return Type(~0);
		}

		template<common::concepts::Arithmethic Type>
		struct Number
		{
			Type m_value;

			template<common::concepts::Arithmethic OtherNumber>
			operator OtherNumber() noexcept
			{
				return static_cast<OtherNumber>(m_value);
			}
			Number()
				: m_value(max_value<Type>())
			{
			}

		};
	}
}
#endif // !INCLUDED_PERKUNAS_NUMBER
