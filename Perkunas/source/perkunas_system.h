
#ifndef INCLUDED_PERKUNAS_SYSTEM_H
#define INCLUDED_PERKUNAS_SYSTEM_H

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

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
#include <functional>
#include <array>

#pragma warning(push)
#pragma warning(disable: 26494)
#pragma warning(disable: 26812)
#pragma warning(disable: 26818)
#pragma warning(disable: 26819)
#include <SDL2/SDL.h>
#pragma warning(pop)

#include "perkunas_types.h"
#include "perkunas_events.h"

namespace perkunas
{
	namespace experimental
	{
		constexpr uint8_t BOOL_FALSE_INDEX = 0;
		constexpr uint8_t BOOL_TRUE_INDEX = 1;
		constexpr uint8_t BOOL_VALUE_SIZE = 2;
		struct StatementFunction
		{
			void operator()(bool expression, const std::function<void(void)>& function, const std::function<void(void)>& false_function = []() {})
			{
				static std::function<void(void)> lookup[BOOL_VALUE_SIZE] =
				{
					false_function,
					function
				};
// Ignored warning 26446: expression will never be out of bounds, no need for .at() 
// Ignored warning 26482: bool expression never bigger than 1
#pragma warning(push)
#pragma warning(disable: 26446)
#pragma warning(disable: 26482)
				lookup[expression]();
#pragma warning(pop) 
			}
		};

		struct BranchlessIfElse
		{
			BranchlessIfElse(std::function<void(void)>(&functions)[BOOL_VALUE_SIZE])
				: m_functions{ functions[BOOL_FALSE_INDEX], functions[BOOL_TRUE_INDEX] }
			{
			
			}
			std::function<void(void)> m_functions[BOOL_VALUE_SIZE];

			void operator()(bool expression)
			{
#pragma warning(push)
#pragma warning(disable: 26446)
#pragma warning(disable: 26482)
				m_functions[expression]();
#pragma warning(pop) 
			}
		};
		/*
		BranchlessIfElse test { if_else };
		test(p_filled); */

		class Branchless
		{ 
		public:
			Branchless() = default;

			static void IF(bool expression, const std::function<void(void)>& function)
			{
				StatementFunction if_statement;
				if_statement(expression, function);
			}

			static void IF_ELSE(bool expression, 
								const std::function<void(void)>& true_function, 
								const std::function<void(void)>& false_function)
			{
				StatementFunction if_statement;
				if_statement(expression, true_function, false_function);
			}
			static void IF_ELSE(bool expression,
								const std::function<void(void)> (&functions)[BOOL_VALUE_SIZE])
			{
				StatementFunction if_statement;
				if_statement(expression, functions[BOOL_TRUE_INDEX], functions[BOOL_FALSE_INDEX]);
			}
		}; /* Small idea for a branchless if using a lookup table */
		//Branchless::IF(p_filled, [this]() { printf("Oh, Hello\n"); });
	}

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

	template<typename _Type>
	struct _Type_counter
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
	template<typename _Type>
	class _Unique_existor : _Type_counter<_Unique_existor<_Type>>
	{
		typedef _Type_counter<_Unique_existor<_Type>> __This_type_counter;

	public:
		_Unique_existor() noexcept
		{
			if(__This_type_counter::count())
			{
				std::string type_name = (typeid(_Type).name());
				std::string error_message = "_One_time_system_existor->too many instances of type :" + type_name;
				Error::print_error_message(error_message);
				assert(!"ERROR: ONLY ONE INSTANCE OF TYPENAME _TYPE IS ALLOWED TO EXIST!");
			}
			__This_type_counter::increment();
		}
		~_Unique_existor() noexcept
		{
			__This_type_counter::decrement();
		}
		
	private:
		_Unique_existor(const _Unique_existor&) = delete;
		_Unique_existor(_Unique_existor&&) = delete;
		_Unique_existor& operator=(const _Unique_existor&) = delete;
		_Unique_existor& operator=(_Unique_existor&&) = delete;
	};

	class _Timer_init : _Unique_existor<_Timer_init>
	{
	public:
		_Timer_init();
		~_Timer_init() noexcept;

	private:
		_Timer_init(const _Timer_init&) = delete;
		_Timer_init(_Timer_init&&) = delete;
		_Timer_init& operator=(const _Timer_init&) = delete;
		_Timer_init& operator=(_Timer_init&&) = delete;
	};

	class _Audio_init : _Unique_existor<_Audio_init>
	{
	public:
		_Audio_init();
		~_Audio_init() noexcept;

	private:
		_Audio_init(const _Audio_init&) = delete;
		_Audio_init(_Audio_init&&) = delete;
		_Audio_init& operator=(const _Audio_init&) = delete;
		_Audio_init& operator=(_Audio_init&&) = delete;
	};

	class _Video_init
	{
	public:
		_Video_init();
		~_Video_init() noexcept;
	};

	class _Events_init : _Unique_existor<_Events_init>
	{
	public:
		_Events_init();
		~_Events_init() noexcept;

	private:
		_Events_init(const _Events_init&) = delete;
		_Events_init(_Events_init&&) = delete;
		_Events_init& operator=(const _Events_init&) = delete;
		_Events_init& operator=(_Events_init&&) = delete;
	};

	typedef std::string window_title_t;
	typedef const Point<int> window_size_t;
	typedef const Point<int> window_position_t;
	typedef const Rectangle<int> window_rectangle_t;

	class _Renderer;
	class _Window
	{
		friend _Renderer;
	public:
		_Window(const window_title_t& title, uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept;
		~_Window() noexcept;

		_Window(_Window&& p_other) noexcept;
		_Window& operator=(_Window&& p_other) noexcept;

	private:
		_Window(const _Window&) = delete;
		_Window& operator=(const _Window&) = delete;

	public:
		void set_title(window_title_t p_title) noexcept;
		window_title_t get_title();

		void set_position(uint16_t x, uint16_t y) noexcept;
		void set_size(uint16_t w, uint16_t h) noexcept;
		window_position_t get_position() const noexcept;
		window_size_t get_size() const noexcept;
		void set_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept;
		window_rectangle_t get_rectangle() const noexcept;
		WindowID get_window_id() const noexcept;

	private:
		SDL_Window* m_window;
	};

	/*
	struct _Window_data
	{
		Uint32 id;
		char *title;
		int x, y;
		int w, h;
		int min_w, min_h;
		int max_w, max_h;
		Uint32 flags;
		Uint32 last_fullscreen_flags;
		SDL_Rect windowed;

		SDL_DisplayMode fullscreen_mode;

		float opacity;
		float brightness;
		Uint16* gamma;
		Uint16* saved_gamma;

		SDL_bool is_hiding;
		SDL_bool is_destroying;
		SDL_bool is_dropping;      


		SDL_Window* prev;
		SDL_Window* next;
	};*/

	class _Renderer : _Type_counter<_Renderer>
	{
		typedef _Type_counter<_Renderer> __Renderer_counter;
	public:
		_Renderer(const _Window&);
		~_Renderer() noexcept;

		_Renderer(_Renderer&& p_other) noexcept;
		_Renderer& operator=(_Renderer&& p_other) noexcept;
	private:
		_Renderer(const _Renderer& p_other) = delete;
		_Renderer& operator=(const _Renderer& p_other) = delete;

	public:
		void set_color(red_t r, green_t g, blue_t b, alpha_t a) noexcept;
		void clear() noexcept;
		void present() noexcept;

		template<_Arithmethic_concept _Type>
		void draw(Rectangle<_Type> p_rect, bool p_filled = false) noexcept
		{
			// a bool has two values, thus the draw lookup has the size of two
			static void (*draw_lookup[2])(SDL_Renderer*, SDL_Rect&) =
			{
				[](SDL_Renderer* p_renderer, SDL_Rect& p_rect) { SDL_RenderDrawRect(p_renderer, &p_rect); },
				[](SDL_Renderer* p_renderer, SDL_Rect& p_rect) { SDL_RenderFillRect(p_renderer, &p_rect); }
			};

			SDL_Rect rect = { static_cast<int>(p_rect.m_x),
							  static_cast<int>(p_rect.m_y),
							  static_cast<int>(p_rect.m_w),
							  static_cast<int>(p_rect.m_h) };

			std::function<void(void)> if_else[2] =
			{
				[this, &rect]() { SDL_RenderDrawRect(m_renderer, &rect); },
				[this, &rect]() { SDL_RenderFillRect(m_renderer, &rect); }
			};

			//Branchless::IF_ELSE(p_filled, if_else);
			draw_lookup[p_filled](m_renderer, rect);
		}
		template<_Arithmethic_concept _Type>
		void draw(Point<_Type> p_point) noexcept
		{
			SDL_RenderDrawPointF(m_renderer,
								 static_cast<float>(p_point.m_x),
								 static_cast<float>(p_point.m_y));
		}
		template<_Arithmethic_concept _Type>
		void draw(_Line<_Type> p_line) noexcept
		{ 
			SDL_RenderDrawLineF(m_renderer,
								static_cast<float>(p_line.m_start.m_x),
								static_cast<float>(p_line.m_start.m_y),
								static_cast<float>(p_line.m_end.m_x),
								static_cast<float>(p_line.m_end.m_y));
		}

	private:
		SDL_Renderer* m_renderer;
	};




	typedef std::function<void(const SDL_Event&)> event_function_t;
	
	class _Event_sub_system : _Events_init
	{
	public:
		_Event_sub_system();
		~_Event_sub_system();

	};


}

#endif // !INCLUDED_PERKUNAS_SYSTEM_H