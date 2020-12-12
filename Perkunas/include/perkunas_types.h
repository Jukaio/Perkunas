
#ifndef INCLUDED_PERKUNAS_TYPES
#define INCLUDED_PERKUNAS_TYPES

#include "perkunas_number.h"
#include "perkunas_line.h"
#include "perkunas_point.h"
#include "perkunas_rect.h"

#include <memory>
#include <string>

namespace perkunas
{
	/* VIDEO TYPES */
	typedef uint8_t red_t;
	typedef uint8_t green_t;
	typedef uint8_t blue_t;
	typedef uint8_t alpha_t;

	typedef Point<int> WindowSize;
	typedef Point<int> WindowPosition;
	typedef Point<int> Pixel;

	typedef Rectangle<int> WindowRectangle;
	typedef Rectangle<int> DrawRectangle;

	typedef std::string WindowTitle;

	typedef _Line<int> Line;
	
	typedef uint32_t WindowID;
	/* !VIDEO TYPES */

	/* INPUT EVENT TYPES */
	/* !INPUT EVENT TYPES */
}

#endif // !INCLUDED_PERKUNAS_TYPES

