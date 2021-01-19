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

			template<event::concepts::IsCallback CallbackType>
			void add_callbacks(CallbackType& p_callback)
			{
				if(event::concepts::HasInputCallback<CallbackType>)
					add_callback(dynamic_cast<event::callback::Input&>(p_callback));
				if(event::concepts::HasWindowCallback<CallbackType>)
					add_callback(dynamic_cast<event::callback::Window&>(p_callback));
			}

			void poll();

		private:
			void add_callback(event::callback::Window&);
			void add_callback(event::callback::Input&);

			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

	namespace video
	{
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
			typedef geometry::Rect<int> Rect;

			Sprite() = default;
			Sprite(const Texture&, const Rectangle&);
		private:
			Texture m_image{};
			Rect m_source{};
		};

		class System
		{
		public:
			System(const video::window::Title&, const video::window::Rectangle&);
			~System();

			Texture create_texture(const common::FilePath&);

			// Setters
			// Window
			void set(const video::window::Title&);
			void set(const video::window::Position&);
			void set(const video::window::Size&);
			void set(const video::window::Rectangle&);
			// Render
			void set(const video::Color&);
			void set(video::RectangleStyle);
			// !Setters

			// Generic getter template
			template<video::GetterTypes ToGet>
			ToGet get() const
			{
				// TODO: Throw - if it throws it is library error, never user error
				// Unless the user changed the WindowTypes concept
				return ToGet{};
			}
			// !Generic getters

			void draw(const Rectangle&);
			void draw(const Pixel&);
			void draw(const Line&);
			void draw(const Texture&);
			void draw(const Sprite&, const TargetRectangle&);

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
}
#endif // !INCLUDED_PERKUNAS_H

