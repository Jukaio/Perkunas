// Perkunas.h

#ifndef INCLUDED_PERKUNAS_H
#define INCLUDED_PERKUNAS_H

#include <memory>
#include <stdint.h>
#include <string>
#include <limits>

#include "perkunas_events.h"
#include "perkunas_color.h"

namespace perkunas
{
	namespace event
	{
		class System 
		{
		public:
			System();
			~System();

			// TODO: Does not work due to compile time
			// Using pointers is a must do, so the compiler
			// Won't cry about classes
			// They all could also share a base callback class
		private:
			void add_window_callbacks(event::callback::Window&);
			void add_input_callbacks(event::callback::Input&);

		public:
			template<event::concepts::IsCallback ArithmethicType>
			void add_callbacks(ArithmethicType& p_callback)
			{
			
				if(event::concepts::HasInputCallback<ArithmethicType>)
					add_input_callbacks(dynamic_cast<event::callback::Input&>(p_callback));
				if(event::concepts::HasWindowCallback<ArithmethicType>)
					add_window_callbacks(dynamic_cast<event::callback::Window&>(p_callback));
			}
			
			void poll();

		private:
			class _EventImpl;
			std::unique_ptr<_EventImpl> m_impl;
		};
	}

	namespace video
	{
		class System
		{
		public:
			System(WindowTitle, const WindowRectangle&);
			~System();

			// Window Methods
			void set_window_title(WindowTitle);
			void set_window_position(const WindowPosition&);
			void set_window_size(const WindowSize&);
			void set_window_rectangle(const WindowRectangle&);
			WindowTitle get_window_title() const;
			WindowPosition get_window_position() const;
			WindowSize get_window_size() const;
			WindowRectangle get_window_rectangle() const;
			WindowID get_window_id() const;

			// Render Methods
			void set_color(common::Color); // TODO: Colour type
			void draw_rectangle(const Rectangle&, RectangleStyle);
			void draw_pixel(const Pixel&);
			void draw_line(const Line&);


			void render_clear();
			void render_present();

		private:
			class _VideoImpl;
			std::unique_ptr<_VideoImpl> m_impl;
		};
	}
}
#endif // !INCLUDED_PERKUNAS_H

