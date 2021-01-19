// perkunas_types.h

#ifndef INCLUDED_PERKUNAS_TYPES
#define INCLUDED_PERKUNAS_TYPES

#include "perkunas_number.h"
#include "perkunas_line.h"
#include "perkunas_point.h"
#include "perkunas_rect.h"
#include "perkunas_color.h"

#include <memory>
#include <string>
#include <filesystem>

namespace perkunas
{
	namespace common
	{
		typedef std::filesystem::path FilePath;
	}

	/* VIDEO TYPES */
	namespace video
	{
		namespace window
		{
			typedef uint32_t ID;
			typedef std::string Title;
			struct Position : geometry::Point<int> {};
			struct Size : geometry::Point<int> {};
			struct Rectangle : geometry::Rect<int> {};
		}
		typedef window::ID WindowID;
		typedef window::Title WindowTitle;
		typedef window::Position WindowPosition;
		typedef window::Size WindowSize;
		typedef window::Rectangle WindowRectangle;

		typedef geometry::Point<float> Pixel;
		typedef geometry::Rect<float> Rectangle;
		typedef geometry::Rect<float> TargetRectangle;
		typedef geometry::Line<float> Line;

		enum class RectangleStyle
		{
			NOT_FILLED,
			FILLED
		};

		template <typename Type>
		concept GetterTypes = std::is_same<Type, window::ID>::value ||
							  std::is_same<Type, window::Title>::value ||
							  std::is_same<Type, window::Position>::value ||
							  std::is_same<Type, window::Size>::value ||
							  std::is_same<Type, window::Rectangle>::value ||
							  std::is_same<Type, RectangleStyle>::value ||
							  std::is_same<Type, Color>::value;
	}
	/* !VIDEO TYPES */

}
namespace prk = perkunas;

#endif // !INCLUDED_PERKUNAS_TYPES

