
#ifndef INCLUDED_PERKUNAS_INTERNAL_H
#define INCLUDED_PERKUNAS_INTERNAL_H

//#define WIN32_LEAN_AND_MEAN             
// Exclude rarely-used stuff from Windows headers

/*
	implementation classes and types:
	underscore prefix and between words
	only letter of first word is uppercase 

	Typedefs and aliases:
	snake_case with _t as suffix

*/

/*
#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_AUDIO          0x00000010u
#define SDL_INIT_VIDEO          0x00000020u
#define SDL_INIT_JOYSTICK       0x00000200u 
#define SDL_INIT_HAPTIC         0x00001000u
#define SDL_INIT_GAMECONTROLLER 0x00002000u 
#define SDL_INIT_EVENTS         0x00004000u
#define SDL_INIT_SENSOR         0x00008000u
#define SDL_INIT_NOPARACHUTE    0x00100000u 
Add handles for each of those
*/
#include <stdio.h>
#include <cassert>
#include <typeinfo>
#include <type_traits>
#include <string>

#pragma warning(push)
#pragma warning(disable: 26494)
#pragma warning(disable: 26812)
#pragma warning(disable: 26818)
#pragma warning(disable: 26819)
#include <SDL2/SDL.h>
#pragma warning(pop)

#include "perkunas_color.h"
#include "perkunas_types.h"
#include "perkunas_events.h"

namespace perkunas
{
	namespace internal
	{
		enum class SystemType
		{
			Video = SDL_INIT_VIDEO,
			Event = SDL_INIT_EVENTS
		};

		enum class SDL_InitState : int
		{
			Success,
			Failure
		};

		template<typename SuccessType, SuccessType value>
		constexpr bool is_successful(int x) noexcept
		{
			return x == static_cast<int>(value);
		}

		struct Error 
		{
			static void print_error_message(const std::string& p_message) noexcept
			{
				printf("ERROR:\t %s\n", p_message.c_str());
			}
		};

		template<typename Type>
		struct TypeCounter
		{
			static void increment() noexcept
			{
				m_counter++;
			}

			static void decrement() noexcept
			{
				m_counter--;
			}

			int count() noexcept
			{
				return m_counter;
			}

		private:
			inline static int m_counter = 0;
		};
		template<typename Type>
		class UniqueExistor : TypeCounter<UniqueExistor<Type>>
		{
			typedef TypeCounter<UniqueExistor<Type>> My_TypeCounter;

		public:
			UniqueExistor() noexcept
			{
				if(My_TypeCounter::count())
				{
					std::string type_name = (typeid(Type).name());
					std::string error_message = "_One_time_system_existor->too many instances of type :" + type_name;
					Error::print_error_message(error_message);
					assert(!"ERROR: ONLY ONE INSTANCE OF TYPENAME _TYPE IS ALLOWED TO EXIST!");
				}
				My_TypeCounter::increment();
			}
			~UniqueExistor() noexcept
			{
				My_TypeCounter::decrement();
			}
		
		private:
			UniqueExistor(const UniqueExistor&) = delete;
			UniqueExistor(UniqueExistor&&) = delete;
			UniqueExistor& operator=(const UniqueExistor&) = delete;
			UniqueExistor& operator=(UniqueExistor&&) = delete;
		};

		namespace init
		{
			class Timer : UniqueExistor<Timer>
			{
			public:
				Timer();
				virtual ~Timer() noexcept;

			private:
				Timer(const Timer&) = delete;
				Timer(Timer&&) = delete;
				Timer& operator=(const Timer&) = delete;
				Timer& operator=(Timer&&) = delete;
			};

			class Audio : UniqueExistor<Audio>
			{
			public:
				Audio();
				virtual ~Audio() noexcept;

			private:
				Audio(const Audio&) = delete;
				Audio(Audio&&) = delete;
				Audio& operator=(const Audio&) = delete;
				Audio& operator=(Audio&&) = delete;
			};

			class Video
			{
			public:
				Video();
				virtual ~Video() noexcept;
			};

			class Events : UniqueExistor<Events>
			{
			public:
				Events();
				virtual ~Events() noexcept;

			private:
				Events(const Events&) = delete;
				Events(Events&&) = delete;
				Events& operator=(const Events&) = delete;
				Events& operator=(Events&&) = delete;
			};
		}

		class Surface
		{
		public:
			Surface(const common::FilePath&);
			~Surface();

			operator SDL_Surface*() const
			{
				return m_data;
			}

		private:
			bool is_vaild();
			void check_valid();

			SDL_Surface* m_data = nullptr;
		};

		class Renderer;
		class Texture
		{
		public:
			Texture() = default;
			Texture(const Renderer&, const Surface&);
			~Texture();

			operator SDL_Texture*() const
			{
				return m_data;
			}

		protected:
			bool is_vaild();
			void check_valid();

		private:
			SDL_Texture* m_data = nullptr;
		};

		class Window
		{
			friend Renderer;
		public:
			Window(const video::window::Title& title, uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept;
			~Window() noexcept;

			Window(Window&& p_other) noexcept;
			Window& operator=(Window&& p_other) noexcept;

		private:
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;

		public:
			void set_title(const video::window::Title& p_title) noexcept;
			video::window::Title get_title();

			void set_position(uint16_t x, uint16_t y) noexcept;
			void set_size(uint16_t w, uint16_t h) noexcept;
			video::window::Position get_position() const noexcept;
			video::window::Size get_size() const noexcept;
			void set_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept;
			video::window::Rectangle get_rectangle() const noexcept;
			video::window::ID get_id() const noexcept;

		private:
			SDL_Window* m_window;
		};

		class Renderer : TypeCounter<Renderer>
		{
			typedef TypeCounter<Renderer> __Renderer_counter;
		public:
			Renderer(const Window&);
			~Renderer() noexcept;

			Renderer(Renderer&& p_other) noexcept;
			Renderer& operator=(Renderer&& p_other) noexcept;
		private:
			Renderer(const Renderer& p_other) = delete;
			Renderer& operator=(const Renderer& p_other) = delete;
		public:
			operator SDL_Renderer*() const
			{
				return m_data;
			}

			void set_color(video::Color::uintx_t r, 
						   video::Color::uintx_t g,
						   video::Color::uintx_t b,
						   video::Color::uintx_t a) noexcept;
			video::Color get_color() noexcept;
			void clear() noexcept;
			void present() noexcept;

			template<common::concepts::Arithmethic Type>
			void draw(geometry::Rect<Type> p_rect, bool p_filled = false) noexcept
			{
				// a bool has two values, thus the draw lookup has the size of two
				static void (*draw_lookup[2])(SDL_Renderer*, SDL_FRect&) =
				{
					[](SDL_Renderer* p_renderer, SDL_FRect& p_rect) { SDL_RenderDrawRectF(p_renderer, &p_rect); },
					[](SDL_Renderer* p_renderer, SDL_FRect& p_rect) { SDL_RenderFillRectF(p_renderer, &p_rect); }
				};
				SDL_FRect rect = { static_cast<float>(p_rect.m_x),
								   static_cast<float>(p_rect.m_y),
								   static_cast<float>(p_rect.m_w),
								   static_cast<float>(p_rect.m_h) };
				draw_lookup[p_filled](m_data, rect);
			}
			template<common::concepts::Arithmethic Type>
			void draw(geometry::Point<Type> p_point) noexcept
			{
				SDL_RenderDrawPointF(m_data,
									 static_cast<float>(p_point.m_x),
									 static_cast<float>(p_point.m_y));
			}
			template<common::concepts::Arithmethic Type>
			void draw(geometry::Line<Type> p_line) noexcept
			{ 
				SDL_RenderDrawLineF(m_data,
									static_cast<float>(p_line.m_start.m_x),
									static_cast<float>(p_line.m_start.m_y),
									static_cast<float>(p_line.m_end.m_x),
									static_cast<float>(p_line.m_end.m_y));
			}
			void draw(const Texture& that) noexcept
			{
				SDL_Rect rect{0, 0, 0, 0};
				SDL_QueryTexture(that, nullptr, nullptr, &rect.w, &rect.h);
				SDL_RenderCopy(m_data, that, nullptr, &rect);
			}
			template<common::concepts::Arithmethic SrcType, common::concepts::Arithmethic DstType>
			void draw(const Texture& that, geometry::Rect<SrcType> src, geometry::Rect<DstType> dst) noexcept
			{
				SDL_Rect s = { static_cast<int>(src.m_x),
							   static_cast<int>(src.m_y),
							   static_cast<int>(src.m_w),
							   static_cast<int>(src.m_h) };
				SDL_FRect d = { static_cast<float>(dst.m_x),
								static_cast<float>(dst.m_y),
								static_cast<float>(dst.m_w),
								static_cast<float>(dst.m_h) };
				SDL_RenderCopyF(m_data, that, &s, &d);
			}


		private:
			SDL_Renderer* m_data;
		};

	}

}

#endif // !INCLUDED_PERKUNAS_INTERNAL_H