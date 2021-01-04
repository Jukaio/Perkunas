
#include "application.h"

void Application::update_input()
{
	m_keyboard.update_previous_states();
	m_mouse.update_previous_states();
}

void Application::on_keyboard(const prk::event::input::Keyboard& event)
{
	auto key_and_state = event.m_key_and_state;
	[[maybe_unused]] auto modifier = event.m_modifier;

	m_keyboard.update_key_state(key_and_state.m_code, key_and_state.m_state);
}

void Application::on_mouse_button(const prk::event::input::mouse::Button& event)
{
	m_mouse.update_button_state(event.m_code, event.m_state);
}

void Application::on_mouse_wheel(const prk::event::input::mouse::Wheel&)
{
}

void Application::on_mouse_motion(const prk::event::input::mouse::Motion& event)
{
	m_mouse.update_position(event.m_coordinate);
}

void Application::on_window_move(const prk::event::window::Move&)
{
}

void Application::on_window_resize(const prk::event::window::Resize&)
{
}

void Application::on_window_focus(const prk::event::window::Focus&)
{
}

void Application::on_window_status(const prk::event::window::Status& event)
{
	typedef prk::event::window::Status status_t;
	if(event.m_id == status_t::ID::Close)
	{
		set_state(State::Quit);
	}
}

void Application::on_window_visibility(const prk::event::window::Visiblity&)
{
}

bool Application::KeyboardState::is_down(KeyCode p_key)
{
	auto index = static_cast<size_t>(p_key);
	return m_current_states[index] == KeyState::Pressed;
}

bool Application::KeyboardState::is_pressed(KeyCode p_key)
{
	auto index = static_cast<size_t>(p_key);
	return m_current_states[index] == KeyState::Pressed &&
		   m_prev_states[index] == KeyState::Released;
}

void Application::KeyboardState::update_key_state(KeyCode p_key, KeyState p_state)
{
	auto index = static_cast<size_t>(p_key);
	m_current_states[index] = p_state;
}

void Application::KeyboardState::update_previous_states()
{
	std::copy(std::begin(m_current_states), std::end(m_current_states), std::begin(m_prev_states));
}

bool Application::MouseState::is_down(ButtonCode p_key) const
{
	auto index = static_cast<size_t>(p_key);
	return m_current_states[index] == ButtonState::Pressed;
}

bool Application::MouseState::just_pressed(ButtonCode p_key) const
{
	auto index = static_cast<size_t>(p_key);
	return m_current_states[index] == ButtonState::Pressed &&
		m_prev_states[index] == ButtonState::Released;
}

void Application::MouseState::update_button_state(ButtonCode p_key, ButtonState p_state)
{
	auto index = static_cast<size_t>(p_key);
	m_current_states[index] = p_state;
}

void Application::MouseState::update_previous_states()
{
	std::copy(std::begin(m_current_states), std::end(m_current_states), std::begin(m_prev_states));
}

void Application::MouseState::update_position(Motion::Coordinate p_position)
{
	m_position = p_position;
}

Application::MouseState::Motion::Coordinate Application::MouseState::get_position() const
{
	return m_position;
}
