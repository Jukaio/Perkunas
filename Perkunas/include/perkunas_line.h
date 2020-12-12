// perkunas_line.h

#ifndef INCLUDED_PERKUNAS_LINE_H
#define INCLUDED_PERKUNAS_LINE_H

#include "perkunas_point.h"

namespace perkunas
{
	template<_Arithmethic_concept _Type>
	struct _Line
	{
		Point<_Type> m_start;
		Point<_Type> m_end;
	};
}
#endif // !INCLUDED_PERKUNAS_LINE_H
