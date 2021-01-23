
#include "perkunas.h"
#include "perkunas_internal.h"
#include <map>
#include <typeinfo>

namespace perkunas
{
	namespace video
	{
		class Texture::Impl : public internal::Texture
		{
		public:
			Impl() = default;
			Impl(const internal::Renderer& sys, const common::FilePath& at)
				: internal::Texture(sys, internal::Surface(at))
			{ }
		};

		Texture::Texture()
		{
		}

		Texture::~Texture() // Required for the pointer to implementation
		{
		}

		class System::Impl : public internal::init::Video
		{
		public:
			Impl(const video::window::Title& p_title, const video::window::Rectangle& p_rect)
				: m_window(p_title,
							static_cast<uint16_t>(p_rect.m_x),
							static_cast<uint16_t>(p_rect.m_y),
							static_cast<uint16_t>(p_rect.m_w),
							static_cast<uint16_t>(p_rect.m_h))
				, m_renderer(m_window)
			{

			}

			RectangleStyle m_rect_style{RectangleStyle::FILLED};
			internal::Window m_window;
			internal::Renderer m_renderer;

		};

		Sprite::Sprite(const Texture& use, const Rectangle& at)
			: m_image(use)
			, m_source(at)
		{ }

		System::System(const video::window::Title& p_title, const video::window::Rectangle& p_rect)
			: m_impl(std::make_unique<Impl>(p_title, p_rect))
		{

		}
		System::~System() // Required for the pointer to implementation
		{

		}
		Texture System::create_texture(const common::FilePath& at) const
		{
			Texture texture;
			texture.m_impl = std::make_shared<Texture::Impl>(m_impl->m_renderer, at);
			return texture;
		}

		void System::set(const video::window::Title& p_title) const
		{
			m_impl->m_window.set_title(p_title);
		}

		void System::set(const video::window::Position& p_position) const
		{
			m_impl->m_window.set_position(static_cast<uint16_t>(p_position.m_x),
										   static_cast<uint16_t>(p_position.m_y));
		}

		void System::set(const video::window::Size& p_size) const
		{
			m_impl->m_window.set_size(static_cast<uint16_t>(p_size.m_x),
									  static_cast<uint16_t>(p_size.m_y));

		}

		void System::set(const video::window::Rectangle& p_rect) const
		{
			m_impl->m_window.set_rectangle(static_cast<uint16_t>(p_rect.m_x),
										   static_cast<uint16_t>(p_rect.m_y),
										   static_cast<uint16_t>(p_rect.m_w),
										   static_cast<uint16_t>(p_rect.m_h));
		}

		void System::clear()
		{
			m_impl->m_renderer.clear();
		}
		void System::present()
		{
			m_impl->m_renderer.present();
		}
		void System::set(const video::Color& p_color) const
		{
			m_impl->m_renderer.set_color(p_color.red, p_color.green, p_color.blue, p_color.alpha);
		}
		void System::set(video::RectangleStyle that) const
		{
			m_impl->m_rect_style = that;
		}
		void System::draw(const Rectangle& p_rect) const
		{
			m_impl->m_renderer.draw(p_rect, m_impl->m_rect_style == RectangleStyle::FILLED);
		}
		void System::draw(const Pixel& p_pixel) const
		{
			m_impl->m_renderer.draw(p_pixel);
		}
		void System::draw(const Line& p_line) const
		{
			m_impl->m_renderer.draw(p_line);
		}

		void System::draw(const Texture& that) const
		{
			m_impl->m_renderer.draw(*that.m_impl);
		}

		void System::draw(const Sprite& that, const Sprite::Target& at) const
		{
			auto& texture = that.m_image;
			auto& source = that.m_source;
			m_impl->m_renderer.draw(*texture.m_impl, source, at);
		}

		template<>
		video::window::Title System::get() const
		{
			return m_impl->m_window.get_title();
		}
		template<>
		video::window::ID System::get() const
		{
			return m_impl->m_window.get_id();
		}
		template<>
		video::window::Position System::get() const
		{
			return m_impl->m_window.get_position();
		}
		template<>
		video::window::Size System::get() const
		{
			return m_impl->m_window.get_size();
		}
		template<>
		video::window::Rectangle System::get() const
		{
			return m_impl->m_window.get_rectangle();
		}
		template<> 
		video::RectangleStyle System::get() const
		{
			return m_impl->m_rect_style;
		}
		template<> video::Color System::get() const
		{
			return m_impl->m_renderer.get_color();
		}
	}
}