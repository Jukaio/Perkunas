// perkunas_template_concepts.h

#ifndef INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H
#define INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H

#include <type_traits>

namespace perkunas
{
	template<typename _Number_type>
	concept _Arithmethic_concept = std::is_arithmetic<_Number_type>::value;

	template<typename _Number_type>
	concept _Signed_integer_concept = std::is_integral<_Number_type>::value && 
									  std::is_unsigned<_Number_type>::value;
}
#endif // !INCLUDED_PERKUNAS_TEMPLATE_CONCEPTS_H
