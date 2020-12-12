// Perkunas.h
#ifndef INCLUDED_PERKUNAS_H
#define INCLUDED_PERKUNAS_H

#include <memory>
#include <stdint.h>
#include <string>
#include <limits>

#include "perkunas_events.h"

/*
	Linker Error caused by C runtime libraries
	Can TODO: remove "inherit include directories from parent"in VC++ Directories -> Include Directories
*/


/*
	user facing classes/types: 
	Always CamelCase with first letter uppercase
*/
namespace perkunas
{
	typedef events::window::Move WindowMoveEvent;
	typedef events::window::Resize WindowResizeEvent;
	typedef events::window::Focus WindowFocusEvent;
	typedef events::window::Status WindowStatusEvent;
	typedef events::window::Visiblity WindowVisibilityEvent;

	class WindowCallback 
	{
	public:
		virtual void on_window_move(const perkunas::WindowMoveEvent&) = 0;
		virtual void on_window_resize(const perkunas::WindowResizeEvent&) = 0;
		virtual void on_window_focus(const perkunas::WindowFocusEvent&) = 0;
		virtual void on_window_status(const perkunas::WindowStatusEvent&) = 0;
		virtual void on_window_visibility(const perkunas::WindowVisibilityEvent&) = 0;
	};

	class InputCallback
	{
	public:
		virtual void on_keyboard(perkunas::events::input::Keyboard&) = 0;
		virtual void on_mouse_wheel(perkunas::events::input::mouse::Wheel&) = 0;
		virtual void on_mouse_button(perkunas::events::input::mouse::Button&) = 0;
		virtual void on_mouse_motion(perkunas::events::input::mouse::Motion&) = 0;
	};

	template<typename _Type>
	concept HasWindowCallback = std::is_base_of <WindowCallback, _Type>::value;
	template<typename _Type>
	concept HasInputCallback = std::is_base_of <InputCallback, _Type>::value;
	template<typename _Type>
	concept IsCallback = HasWindowCallback<_Type> &&
						 HasInputCallback<_Type>;

	struct SubSystemCounter
	{
	protected:
		static uint32_t next()
		{
			static uint32_t index_ = 0;
			return index_++;
		}
	};

	template <typename _Type>
	struct UniqueIdentifier : SubSystemCounter
	{
		static uint32_t get_unique_index()
		{
			static const uint32_t identifier = SubSystemCounter::next();
			return identifier;
		}
	};

	enum class RectangleStyle
	{
		NOT_FILLED,
		FILLED
	};

	class VideoSubSystem : public UniqueIdentifier<VideoSubSystem>
	{
	public:
		VideoSubSystem(WindowTitle, const WindowRectangle&);
		~VideoSubSystem();

		// Window Methods
		void set_window_title(WindowTitle);
		void set_window_position(const WindowPosition&);
		void set_window_size(const WindowSize&);
		void set_window_rectangle(const WindowRectangle&);
		WindowTitle get_window_title();
		WindowPosition get_window_position();
		WindowSize get_window_size();
		WindowRectangle get_window_rectangle();
		WindowID get_window_id();

		// Render Methods
		void set_color(red_t, green_t, blue_t, alpha_t); // TODO: Colour type
		void draw_rectangle(const DrawRectangle&, RectangleStyle);
		void draw_pixel(const Pixel&);
		void draw_line(const Line&);
		void render_clear();
		void render_present();

	private:
		class _VideoSubSystemImpl;
		std::unique_ptr<_VideoSubSystemImpl> m_impl;
	};


	class EventSubSystem : public UniqueIdentifier<EventSubSystem>
	{
	public:
		EventSubSystem();
		~EventSubSystem();

	private:
		void add_window_callbacks(WindowCallback&);
		void add_input_callbacks(InputCallback&);
	
	public:
		template<IsCallback _Type>
		void add_callbacks(_Type& p_callback)
		{
			if(HasInputCallback<_Type>) 
				add_input_callbacks(p_callback);
			if(HasWindowCallback<_Type>)
				add_window_callbacks(p_callback);
		}

		void poll();

	private:
		class _EventSubSystemImpl;
		std::unique_ptr<_EventSubSystemImpl> m_impl;
	};




	class AudioSubSystem : public UniqueIdentifier<AudioSubSystem>
	{
	public:
		AudioSubSystem();
		~AudioSubSystem();


	private:
		class _AudioSubSystemImpl;
		std::unique_ptr<_AudioSubSystemImpl> m_core_implementation;
	};
}
namespace prk = perkunas;
#endif // !INCLUDED_PERKUNAS_H

