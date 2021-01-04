// perkunas_color.h

#ifndef INCLUDED_PERKUNAS_COLOR_H
#define INCLUDED_PERKUNAS_COLOR_H

#include <cinttypes>

namespace perkunas
{
	namespace common
	{
		struct Color
		{
			typedef uint8_t red_t;
			typedef uint8_t green_t;
			typedef uint8_t blue_t;
			typedef uint8_t alpha_t;

			red_t red = 0;
			green_t green = 0;
			blue_t blue = 0;
			alpha_t alpha = 0;
		};
	}
}

#endif // !INCLUDED_PERKUNAS_COLOR_H
