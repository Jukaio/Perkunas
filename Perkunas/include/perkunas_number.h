
#ifndef INCLUDED_PERKUNAS_NUMBER
#define INCLUDED_PERKUNAS_NUMBER

#include "perkunas_template_concepts.h"

namespace perkunas
{
	template <_Arithmethic_concept _Type>
	constexpr _Type max_value()
	{
		return _Type(~0);
	}

	template<_Arithmethic_concept _Type>
	struct Number
	{
		_Type m_value;

		Number()
			: m_value(max_value<_Type>())
		{
		}

	};
}
#endif // !INCLUDED_PERKUNAS_NUMBER
