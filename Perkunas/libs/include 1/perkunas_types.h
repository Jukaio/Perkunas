// perkunas_types.h

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
	namespace video
	{
		typedef geometry::Point<int> WindowSize;
		typedef geometry::Point<int> WindowPosition;
		typedef geometry::Point<int> Pixel;
		typedef geometry::Rect<int> WindowRectangle;
		typedef geometry::Rect<int> Rectangle;
		typedef geometry::Line<int> Line;
		typedef std::string WindowTitle;
		typedef uint32_t WindowID;

		enum class RectangleStyle
		{
			NOT_FILLED,
			FILLED
		};
	}
	/* !VIDEO TYPES */

}
namespace prk = perkunas;

#endif // !INCLUDED_PERKUNAS_TYPES

