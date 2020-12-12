
#include "perkunas.h"
#include "perkunas_system.h"
#include "perkunas_handle_implementation.h"

namespace perkunas
{
	class VideoSubSystem::_VideoSubSystemImpl : public _Video_init
	{
	public:
		_VideoSubSystemImpl(const WindowTitle& p_title, const WindowRectangle& p_rect)
			: m_window(p_title,
						static_cast<uint16_t>(p_rect.m_x),
						static_cast<uint16_t>(p_rect.m_y),
						static_cast<uint16_t>(p_rect.m_w),
						static_cast<uint16_t>(p_rect.m_h))
			, m_renderer(m_window)
		{

		}

		_Window m_window;
		_Renderer m_renderer;
	};

	VideoSubSystem::VideoSubSystem(WindowTitle p_title, const WindowRectangle& p_rect)
		: m_impl(std::make_unique<_VideoSubSystemImpl>(p_title, p_rect))
	{

	}
	VideoSubSystem::~VideoSubSystem() // Required for the pointer to implementation
	{

	}

	void VideoSubSystem::set_window_title(WindowTitle p_title)
	{
		m_impl->m_window.set_title(p_title);
	}

	WindowTitle VideoSubSystem::get_window_title()
	{
		return m_impl->m_window.get_title();
	}

	void VideoSubSystem::set_window_position(const WindowPosition& p_position)
	{
		m_impl->m_window.set_position(static_cast<uint16_t>(p_position.m_x),
									   static_cast<uint16_t>(p_position.m_y));
	}

	void VideoSubSystem::set_window_size(const WindowSize& p_size)
	{
		m_impl->m_window.set_size(static_cast<uint16_t>(p_size.m_x),
													  static_cast<uint16_t>(p_size.m_y));

	}

	WindowPosition VideoSubSystem::get_window_position()
	{
		window_position_t position = m_impl->m_window.get_position();
		return WindowPosition{ position.m_x, position.m_y };
	}

	WindowSize VideoSubSystem::get_window_size()
	{
		window_size_t size = m_impl->m_window.get_size();
		return WindowSize{ size.m_x, size.m_y };
	}

	void VideoSubSystem::set_window_rectangle(const WindowRectangle& p_rect)
	{
		m_impl->m_window.set_rectangle(static_cast<uint16_t>(p_rect.m_x),
														   static_cast<uint16_t>(p_rect.m_y),
														   static_cast<uint16_t>(p_rect.m_w),
														   static_cast<uint16_t>(p_rect.m_h));
	}

	WindowRectangle VideoSubSystem::get_window_rectangle()
	{
		window_rectangle_t rect = m_impl->m_window.get_rectangle();
		return WindowRectangle{ rect.m_x, rect.m_y, rect.m_w, rect.m_h };
	}

	WindowID VideoSubSystem::get_window_id()
	{
		return m_impl->m_window.get_window_id();
	}

	void VideoSubSystem::render_clear()
	{
		m_impl->m_renderer.clear();
	}
	void VideoSubSystem::render_present()
	{
		m_impl->m_renderer.present();
	}
	void VideoSubSystem::set_color(red_t r, green_t g, blue_t b, alpha_t a)
	{
		m_impl->m_renderer.set_color(r, g, b, a);
	}
	void VideoSubSystem::draw_rectangle(const DrawRectangle& p_rect, RectangleStyle p_style)
	{
		m_impl->m_renderer.draw(p_rect, static_cast<bool>(p_style));
	}
	void VideoSubSystem::draw_pixel(const Pixel& p_pixel)
	{
		m_impl->m_renderer.draw(p_pixel);
	}
	void VideoSubSystem::draw_line(const Line& p_line)
	{
		m_impl->m_renderer.draw(p_line);
	}
}