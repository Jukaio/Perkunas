// perkunas_types.h

#ifndef INCLUDED_PERKUNAS_TYPES
#define INCLUDED_PERKUNAS_TYPES

#include <memory>
#include <string>
#include <filesystem>
#include <functional>
#include <chrono>

#include "perkunas_number.h"
#include "perkunas_line.h"
#include "perkunas_point.h"
#include "perkunas_rect.h"
#include "perkunas_color.h"


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

		class System;
		class Texture
		{
			friend System;
		public:
			Texture();
			~Texture();

		private:
			class Impl;
			std::shared_ptr<Impl> m_impl = nullptr;
		};
		class Sprite
		{
			friend System;
		public:
			typedef geometry::Rect<int> Rectangle;
			typedef geometry::Rect<float> Target;

			Sprite() = default;
			Sprite(const Texture&, const Rectangle&);
		private:
			Texture m_image{};
			Rectangle m_source{};
		};
	}
	/* !VIDEO TYPES */

	/* CHRONO TYPES */
	namespace chrono
	{
		typedef std::chrono::milliseconds Milliseconds;
		typedef std::chrono::seconds Seconds;
		typedef std::chrono::minutes Minutes;
		typedef std::chrono::hours Hours;

		class System;
		class Timer final
		{
		private:
			friend System;
		public:

			typedef Milliseconds Interval;
			typedef std::function<void(Timer&)> Callback;

			explicit Timer(Interval time, Callback that);
			~Timer();
			void set(Callback that);
			void set(Interval t);

		private:
			operator Interval() const;
			void operator()();
			bool is_valid() const;

			static const int INVALID = 0;
			int id = INVALID;
			Callback callback;
			Interval time{};
		};
	}
	/* !CHRONO TYPES */

}
namespace prk = perkunas;

#endif // !INCLUDED_PERKUNAS_TYPES

