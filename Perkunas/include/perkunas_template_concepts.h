// perkunas_template_concepts.h

#ifndef INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H
#define INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H

#include <stdint.h>

namespace perkunas
{
	namespace common
	{
		namespace concepts
		{
			template<typename NumberType>
			concept Arithmethic = std::is_arithmetic<NumberType>::value ||
								  std::is_same<NumberType, std::nullptr_t>::value;

			template<typename NumberType>
			concept Integer = std::is_integral<NumberType>::value &&
							  !std::is_floating_point<NumberType>::value;

			template<typename NumberType>
			concept UnsignedInteger = concepts::Integer<NumberType> && 
									  std::is_unsigned<NumberType>::value;

			template <class NumberType>
			concept DecimalNumber = std::is_floating_point<NumberType>::value && 
									!std::is_integral<NumberType>::value;

			template<typename NumberType>
			concept ColorBaseInt = concepts::UnsignedInteger<NumberType> &&
								   (sizeof(NumberType) % (sizeof(uint8_t) * 4)) == 0;
		}
	}
}
#endif // !INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H
