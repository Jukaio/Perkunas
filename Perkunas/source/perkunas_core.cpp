
#include "perkunas.h"
#include "perkunas_system.h"
#include "perkunas_handle_implementation.h"

#include <algorithm>
#include <vector>

namespace perkunas
{
	/*
		Four Layers:
		Layer 1: SDL Layer
		Layer 2: _base Layer
		Layer 3: Core (connects layer 2 to layer 4 while everything behind and incuding layer 3 stays hidden)
		Layer 4: User Layer
	*/

	class EventSubSystem::_EventSubSystemImpl : public _Event_sub_system
	{
	public:
		_EventSubSystemImpl() = default;
		~_EventSubSystemImpl() = default;
	};

	EventSubSystem::EventSubSystem()
		: m_impl(std::make_unique<_EventSubSystemImpl>())
	{

	}

	EventSubSystem::~EventSubSystem()
	{
	}


	typedef std::function <void(const SDL_Event&)> ConvertSDLFunction;
	class EventCallbackSystem
	{
	private:
		class Window
		{
		private:
			std::vector<WindowCallback*> m_window_callbacks;
			const ConvertSDLFunction callback_lookup[static_cast<int>(events::window::Common::Type::Count)]
			{
				[&](const SDL_Event& p_event)
				{
					auto event = prepare_visibility_event(p_event);
					std::for_each(std::begin(m_window_callbacks),
								  std::end(m_window_callbacks),
								  [&](WindowCallback* callback) { callback->on_window_visibility(event); });
				},
				[&](const SDL_Event& p_event)
				{
					auto event = prepare_move_event(p_event);
					std::for_each(std::begin(m_window_callbacks),
								  std::end(m_window_callbacks),
								  [&](WindowCallback* callback) { callback->on_window_move(event); });
				},
				[&](const SDL_Event& p_event)
				{
					auto event = prepare_resize_event(p_event);
					std::for_each(std::begin(m_window_callbacks),
								  std::end(m_window_callbacks),
								  [&](WindowCallback* callback) { callback->on_window_resize(event); });
				},
				[&](const SDL_Event& p_event)
				{
					auto event = prepare_status_event(p_event);
					std::for_each(std::begin(m_window_callbacks),
								  std::end(m_window_callbacks),
								  [&](WindowCallback* callback) { callback->on_window_status(event); });
				},
				[&](const SDL_Event& p_event)
				{
					auto event = prepare_focus_event(p_event);
					std::for_each(std::begin(m_window_callbacks),
								  std::end(m_window_callbacks),
								  [&](WindowCallback* callback) { callback->on_window_focus(event); });
				},
			};

			static events::window::Move prepare_move_event(const SDL_Event& p_external_event) 
			{
				typedef events::window::Move MoveEvent;
				typedef MoveEvent::Position Position;
		
				return MoveEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					Position
					{
						p_external_event.window.data1,
						p_external_event.window.data2
					}
				};
			}
			static events::window::Resize prepare_resize_event(const SDL_Event& p_external_event) 
			{
				typedef events::window::Resize ResizeEvent;
				typedef ResizeEvent::Size Size;

				return ResizeEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					Size
					{
						p_external_event.window.data1,
						p_external_event.window.data2
					}
				};
			}
			static events::window::Visiblity prepare_visibility_event(const SDL_Event& p_external_event) 
			{
				typedef events::window::Visiblity VisibilityEvent;
				typedef VisibilityEvent::ID TypeID;

				return VisibilityEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					static_cast<TypeID>(p_external_event.window.event)
				};
			}
			static events::window::Status prepare_status_event(const SDL_Event& p_external_event)
			{
				typedef events::window::Status StatusEvent;
				typedef StatusEvent::ID TypeID;

				return StatusEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					static_cast<TypeID>(p_external_event.window.event)
				};
			}
			static events::window::Focus prepare_focus_event(const SDL_Event& p_external_event)
			{
				typedef events::window::Focus FocusEvent;
				typedef FocusEvent::ID TypeID;

				return FocusEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					static_cast<TypeID>(p_external_event.window.event)
				};
			}


		public:
			const ConvertSDLFunction m_event_forwarder
			{ 
				[&](const SDL_Event& p_event) 
				{
					auto index = events::window::id_lookup[p_event.window.event];
					if(index > 0) callback_lookup[index](p_event);
				} 
			};
			const std::function<void(WindowCallback*)> m_callback_adder
			{
				[&](WindowCallback* p_callback)
				{ m_window_callbacks.push_back(p_callback); }
			};
		};
		class Input
		{
		private:
			static events::input::Keyboard prepare_keyboard_event(const SDL_Event& p_external_event)
			{
				typedef events::input::Keyboard KeyboardEvent;
				typedef KeyboardEvent::Key Key;
				typedef KeyboardEvent::State State;
				typedef KeyboardEvent::Modifier Mod;

				return KeyboardEvent
				{ 
					static_cast<WindowID>(p_external_event.window.windowID),
					KeyboardEvent::KeyAndState
					{ 
						static_cast<Key>(p_external_event.key.keysym.scancode),
						static_cast<State>(p_external_event.key.state) 
					},
					static_cast<Mod>(p_external_event.key.keysym.mod)
				};
			}
			static events::input::mouse::Button prepare_mouse_button_event(const SDL_Event& p_external_event)
			{
				typedef events::input::mouse::Button MouseButtonEvent;
				typedef MouseButtonEvent::Code Code;
				typedef MouseButtonEvent::State State;
		
				return MouseButtonEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					static_cast<Code>(p_external_event.button.button),
					static_cast<State>(p_external_event.button.state)
				};
			}
			static events::input::mouse::Motion prepare_mouse_motion_event(const SDL_Event& p_external_event)
			{
				typedef events::input::mouse::Motion MouseMotionEvent;
				typedef MouseMotionEvent::Coordinate Coordinate;
				typedef MouseMotionEvent::TravelDistance TravelDistance;

				return MouseMotionEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					Coordinate 
					{ 
						p_external_event.motion.x, 
						p_external_event.motion.y 
					},
					TravelDistance
					{ 
						p_external_event.motion.xrel, 
						p_external_event.motion.yrel 
					},
				};
			}
			static events::input::mouse::Wheel prepare_mouse_wheel_event(const SDL_Event& p_external_event)
			{
				typedef events::input::mouse::Wheel MouseWheelEvent;
				typedef MouseWheelEvent::ScrollAmount ScrollAmount;

				return MouseWheelEvent
				{
					static_cast<WindowID>(p_external_event.window.windowID),
					ScrollAmount 
					{ 
						p_external_event.wheel.x, 
						p_external_event.wheel.y 
					}
				};
			}

			// TODO: Do the same for window callbacks
			std::vector<InputCallback*> m_input_callbacks;
		public:
			const std::function <void(const SDL_Event&)> callback_lookup[static_cast<int>(events::input::Common::Type::Count)]
			{
				[&](const SDL_Event& p_event) 
				{ 
					auto event = prepare_keyboard_event(p_event);
					std::for_each(std::begin(m_input_callbacks), 
								  std::end(m_input_callbacks), 
								  [&](InputCallback* callback) { callback->on_keyboard(event); } );
				},
				[&](const SDL_Event& p_event) 
				{ 
					auto event = prepare_mouse_motion_event(p_event);
					std::for_each(std::begin(m_input_callbacks),
								  std::end(m_input_callbacks),
								  [&](InputCallback* callback) { callback->on_mouse_motion(event); });
				},
				[&](const SDL_Event& p_event) 
				{ 
					auto event = prepare_mouse_wheel_event(p_event); 
					std::for_each(std::begin(m_input_callbacks),
								  std::end(m_input_callbacks),
								  [&](InputCallback* callback) { callback->on_mouse_wheel(event); });
				},
				[&](const SDL_Event& p_event) 
				{ 
					auto event = prepare_mouse_button_event(p_event); 
					std::for_each(std::begin(m_input_callbacks),
								  std::end(m_input_callbacks),
								  [&](InputCallback* callback) { callback->on_mouse_button(event); });
				},
			};

			const std::function<void(InputCallback*)> m_callback_adder
			{
				[&](InputCallback* p_callback)
				{
				m_input_callbacks.push_back(p_callback); }
			};

		};

		Window m_window_callback_system;
		Input m_input_callback_system;

		const ConvertSDLFunction (&generate_lookup())[SDL_EventType::SDL_LASTEVENT]
		{
			typedef events::input::Common::Type EventType;

			static ConvertSDLFunction convert_functions[SDL_EventType::SDL_LASTEVENT];
			convert_functions[SDL_KEYDOWN] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::Keyboard)];
			convert_functions[SDL_KEYUP] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::Keyboard)];
			convert_functions[SDL_MOUSEBUTTONDOWN] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::MouseButton)];
			convert_functions[SDL_MOUSEBUTTONUP] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::MouseButton)];
			convert_functions[SDL_MOUSEMOTION] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::MouseMotion)];
			convert_functions[SDL_MOUSEWHEEL] = m_input_callback_system.callback_lookup[static_cast<int>(EventType::MouseWheel)];
			convert_functions[SDL_WINDOWEVENT] = m_window_callback_system.m_event_forwarder;
			
			return convert_functions;
		}
		const ConvertSDLFunction (&event_conversion_lookup)[SDL_EventType::SDL_LASTEVENT] = generate_lookup();

	public:
		void add_window_callbacks(WindowCallback* p_callback)
		{
			m_window_callback_system.m_callback_adder(p_callback);
		}
		void add_input_callbacks(InputCallback* p_callback)
		{
			m_input_callback_system.m_callback_adder(p_callback);
		}

		const void operator()(const SDL_Event& p_external_event)
		{
			auto& function = event_conversion_lookup[p_external_event.type];
			if(function) function(p_external_event);
		}
	};

	EventCallbackSystem event_callback_system;
	void EventSubSystem::poll()
	{	
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			event_callback_system(ev);
		}
	}
	void EventSubSystem::add_window_callbacks(WindowCallback& p_callback)
	{
		event_callback_system.add_window_callbacks(&p_callback);
	}
	void EventSubSystem::add_input_callbacks(InputCallback& p_callback)
	{
		event_callback_system.add_input_callbacks(&p_callback);
	}
}