// Perkunas.h

#ifndef INCLUDED_PERKUNAS_H
#define INCLUDED_PERKUNAS_H

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

			template<event::concepts::IsCallback Callback>
			void add(Callback& that) const
			{
				if(event::concepts::HasInputCallback<Callback>)
					add_callback(dynamic_cast<event::callback::Input&>(that));
				if(event::concepts::HasWindowCallback<Callback>)
					add_callback(dynamic_cast<event::callback::Window&>(that));
			}

			void poll() const;

		private:
			void add_callback(event::callback::Window&) const;
			void add_callback(event::callback::Input&) const;

			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}
	namespace video
	{
		class System
		{
		public:
			System(const video::window::Title&, 
				   const video::window::Rectangle&);
			~System();

			Texture create_texture(const common::FilePath&) const;

			void set(const video::window::Title&) const;
			void set(const video::window::Position&) const;
			void set(const video::window::Size&) const;
			void set(const video::window::Rectangle&) const;
			void set(const video::Color&) const;
			void set(video::RectangleStyle) const;

			template<video::GetterTypes ToGet>
			ToGet get() const
			{
				// TODO: Throw - if it throws it is library error, never user error
				// Unless the user changed the WindowTypes concept
				return ToGet{};
			}

			void draw(const Rectangle&) const;
			void draw(const Pixel&) const;
			void draw(const Line&) const;
			void draw(const Texture&) const;
			void draw(const Sprite&, const Sprite::Target&) const;

			void clear();
			void present();

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};

		// Explicit getter instantiations
		extern template video::window::Title System::get() const;
		extern template video::window::Position System::get() const;
		extern template video::window::Size System::get() const;
		extern template video::window::Rectangle System::get() const;
		extern template video::window::ID System::get() const;
		extern template video::RectangleStyle System::get() const;
		extern template video::Color System::get() const;
	}

	namespace chrono
	{
		class System
		{
		public:
			System();
			~System();

			void add(Timer&) const;
			void remove(Timer&) const;
			void delay(Milliseconds) const;
			Milliseconds now() const;

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;

		};
	}
}
#endif // !INCLUDED_PERKUNAS_H

