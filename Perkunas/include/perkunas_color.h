// perkunas_color.h

#ifndef INCLUDED_PERKUNAS_COLOR_H
#define INCLUDED_PERKUNAS_COLOR_H

#include <cinttypes>
#include "perkunas_template_concepts.h"
#include <algorithm>

namespace perkunas
{
	namespace video
	{
		// TODO: Generic colour and colour channels using the upper concept
		class Color
		{
		public:
			static const Color Red;
			static const Color Green;
			static const Color Blue;
			static const Color Yellow;
			static const Color Magenta;
			static const Color Cyan;
			static const Color White;
			static const Color Black;

			typedef uint8_t uintx_t;
			class Channel
			{
				static const uintx_t channel_bit_size = static_cast<uintx_t>(~0);
			public:

				Channel() noexcept
				{ } 
				template<common::concepts::Integer NumberType>
				Channel(NumberType value) noexcept
				{
					assign(static_cast<uintx_t>(value));
				}
				Channel(double value) noexcept
				{
					assign(value);
				}

				template<common::concepts::Integer NumberType>
				Channel& operator=(NumberType value) noexcept
				{
					assign(value);
					return *this;
				}
				template<common::concepts::DecimalNumber NumberType>
				Channel& operator=(NumberType value) noexcept
				{
					assign(value);
					return *this;
				}
				template<common::concepts::Arithmethic NumberType>
				operator NumberType() const noexcept
				{
					if(std::is_floating_point<NumberType>::value)
						return static_cast<NumberType>(m_decimal);
					else
						return static_cast<NumberType>(m_integer);
				}

				Channel& operator++() noexcept
				{
					assign(m_integer + 1);
					return *this;
				}
				Channel& operator--() noexcept
				{
					assign(m_integer - 1);
					return *this;
				}
				Channel operator++(int) noexcept
				{
					assign(m_integer + 1);
					return *this;
				}
				Channel operator--(int) noexcept
				{
					assign(m_integer - 1);
					return *this;
				}

			private:
				double opengl_clamp(double value)
				{
					return std::clamp(value, 0.0, 1.0);
				}

				template<common::concepts::Integer NumberType>
				void assign(NumberType value) noexcept
				{
					m_integer = static_cast<uintx_t>(value);
					m_decimal = opengl_clamp(value / static_cast<double>(channel_bit_size));
				}
				template<common::concepts::DecimalNumber NumberType>
				void assign(NumberType value) noexcept
				{
					m_decimal = opengl_clamp(value);
					m_integer = static_cast<uintx_t>(value * channel_bit_size);
				}

				// Cache integer and decimal instead of 
				// calculating during conversion
				// to reduce unneeded calculations
				uintx_t m_integer = 0;
				double m_decimal = 0.0;
			};

			operator uint32_t()
			{
				uint32_t to_return{0};
				to_return |= (static_cast<uint32_t>(blue) << 0);
				to_return |= (static_cast<uint32_t>(green) << 8);
				to_return |= (static_cast<uint32_t>(red) << 16);
				to_return |= (static_cast<uint32_t>(alpha) << 24);
				return to_return;
			}
			static Color create(uint32_t hex)
			{
				Color color{};
				color.alpha = static_cast<uintx_t>((0xFF000000 & hex) >> 24);
				color.red	= static_cast<uintx_t>((0x00FF0000 & hex) >> 16);
				color.green = static_cast<uintx_t>((0x0000FF00 & hex) >> 8);
				color.blue	= static_cast<uintx_t>((0x000000FF & hex) >> 0);
				return color;
			}

			Channel red{};
			Channel green{};
			Channel blue{};
			Channel alpha{};
		};

		inline const Color Color::Red{ 255, 0, 0, 255 };
		inline const Color Color::Green{ 0, 255, 0, 255 };
		inline const Color Color::Blue{ 0, 0, 255, 255 };
		inline const Color Color::Yellow{ 255, 255, 0, 255};
		inline const Color Color::Magenta{ 255, 0, 255, 255 };
		inline const Color Color::Cyan{ 0, 255, 255, 255 };
		inline const Color Color::White{ 255, 255, 255, 255 };
		inline const Color Color::Black{ 0, 0, 0, 255 };

		inline bool operator==(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) == static_cast<Color::uintx_t>(rhs);
		}
		inline bool operator<(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) < static_cast<Color::uintx_t>(rhs);
		}
		inline bool operator>(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) > static_cast<Color::uintx_t>(rhs);
		}
		inline bool operator<=(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) <= static_cast<Color::uintx_t>(rhs);
		}
		inline bool operator>=(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) >= static_cast<Color::uintx_t>(rhs);
		}
		inline bool operator!=(const Color::Channel& lhs, const Color::Channel& rhs) noexcept
		{
			return static_cast<Color::uintx_t>(lhs) != static_cast<Color::uintx_t>(rhs);
		}

		inline bool operator==(const Color& lhs, const Color& rhs)
		{
			return lhs.red == rhs.red &&
				   lhs.green == rhs.green &&
				   lhs.blue == rhs.blue &&
				   lhs.alpha == rhs.alpha;
		}
		inline bool operator!=(const Color& lhs, const Color& rhs)
		{
			return !(lhs == rhs);
		}
	}
}

#endif // !INCLUDED_PERKUNAS_COLOR_H
