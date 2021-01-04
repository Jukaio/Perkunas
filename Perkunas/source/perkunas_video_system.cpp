
#include "perkunas.h"
#include "perkunas_internal.h"

namespace perkunas
{
	namespace video
	{
		class System::_VideoImpl : public internal::_Video_init
		{
		public:
			_VideoImpl(const WindowTitle& p_title, const WindowRectangle& p_rect)
				: m_window(p_title,
							static_cast<uint16_t>(p_rect.m_x),
							static_cast<uint16_t>(p_rect.m_y),
							static_cast<uint16_t>(p_rect.m_w),
							static_cast<uint16_t>(p_rect.m_h))
				, m_renderer(m_window)
			{

			}

			internal::_Window m_window;
			internal::_Renderer m_renderer;
		};

		System::System(WindowTitle p_title, const WindowRectangle& p_rect)
			: m_impl(std::make_unique<_VideoImpl>(p_title, p_rect))
		{

		}
		System::~System() // Required for the pointer to implementation
		{

		}

		void System::set_window_title(WindowTitle p_title)
		{
			m_impl->m_window.set_title(p_title);
		}

		WindowTitle System::get_window_title() const
		{
			return m_impl->m_window.get_title();
		}

		void System::set_window_position(const WindowPosition& p_position) 
		{
			m_impl->m_window.set_position(static_cast<uint16_t>(p_position.m_x),
										   static_cast<uint16_t>(p_position.m_y));
		}

		void System::set_window_size(const WindowSize& p_size)
		{
			m_impl->m_window.set_size(static_cast<uint16_t>(p_size.m_x),
														  static_cast<uint16_t>(p_size.m_y));

		}

		WindowPosition System::get_window_position() const
		{
			internal::window_position_t position = m_impl->m_window.get_position();
			return WindowPosition{ position.m_x, position.m_y };
		}

		WindowSize System::get_window_size() const
		{
			internal::window_size_t size = m_impl->m_window.get_size();
			return WindowSize{ size.m_x, size.m_y };
		}

		void System::set_window_rectangle(const WindowRectangle& p_rect)
		{
			m_impl->m_window.set_rectangle(static_cast<uint16_t>(p_rect.m_x),
															   static_cast<uint16_t>(p_rect.m_y),
															   static_cast<uint16_t>(p_rect.m_w),
															   static_cast<uint16_t>(p_rect.m_h));
		}

		WindowRectangle System::get_window_rectangle() const
		{
			internal::window_rectangle_t rect = m_impl->m_window.get_rectangle();
			return WindowRectangle{ rect.m_x, rect.m_y, rect.m_w, rect.m_h };
		}
		WindowID System::get_window_id() const
		{
			return m_impl->m_window.get_window_id();
		}

		void System::render_clear()
		{
			m_impl->m_renderer.clear();
		}
		void System::render_present()
		{
			m_impl->m_renderer.present();
		}
		void System::set_color(common::Color p_color)
		{
			m_impl->m_renderer.set_color(p_color.red, p_color.green, p_color.blue, p_color.alpha);
		}
		void System::draw_rectangle(const Rectangle& p_rect, RectangleStyle p_style)
		{
			m_impl->m_renderer.draw(p_rect, static_cast<bool>(p_style));
		}
		void System::draw_pixel(const Pixel& p_pixel)
		{
			m_impl->m_renderer.draw(p_pixel);
		}
		void System::draw_line(const Line& p_line)
		{
			m_impl->m_renderer.draw(p_line);
		}
	}
}