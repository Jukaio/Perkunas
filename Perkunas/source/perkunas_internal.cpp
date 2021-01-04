// Perkunas.cpp 

#include "perkunas_internal.h"

#include <exception>
#include <algorithm>


namespace perkunas
{
    namespace internal
    {
        // -- BASE SDL INITIALISER -- // 

        constexpr bool is_init_successful(int x) noexcept
        {
            return is_successful<SDL_InitState, SDL_InitState::Success>(x);
        }

    // Ignored Warning 26455: Default Constructors have to throw due to the SDL2 initialization
    #pragma warning(push)
    #pragma warning(disable: 26455)
        _Timer_init::_Timer_init()
        {
            if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_TIMER)))
                throw std::exception(SDL_GetError());
        }
        _Timer_init::~_Timer_init() noexcept
        {
            SDL_QuitSubSystem(SDL_INIT_TIMER);
        }
        _Audio_init::_Audio_init()
        {
            if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_AUDIO)))
                throw std::exception(SDL_GetError());
        }
        _Audio_init::~_Audio_init() noexcept
        {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }
        _Video_init::_Video_init() 
        {
            if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_VIDEO)))
                throw std::exception(SDL_GetError());
        }
        _Video_init::~_Video_init() noexcept
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
        _Events_init::_Events_init() 
        {
            if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_EVENTS)))
                throw std::exception(SDL_GetError());
        }
        _Events_init::~_Events_init() noexcept
        {
            SDL_QuitSubSystem(SDL_INIT_EVENTS);
        }
    #pragma warning(pop) 

        // -- END BASE SDL INITIALISER -- // 
    
        // -- VIDEO SYSTEMS -- //

        _Window::_Window(const window_title_t& title, uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept
        {
            m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
        }
        _Window::~_Window() noexcept
        {
            SDL_DestroyWindow(m_window);
        }
        _Window::_Window(_Window&& p_other) noexcept
        {
            m_window = p_other.m_window;
        }
        _Window& _Window::operator=(_Window&& p_other) noexcept
        {
            m_window = p_other.m_window;
            return *this;
        }
        void _Window::set_title(window_title_t p_title) noexcept
        {
            SDL_SetWindowTitle(m_window, p_title.c_str());
        }
        window_title_t _Window::get_title()
        {
            return window_title_t{ SDL_GetWindowTitle(m_window) };
        }
        void _Window::set_position(uint16_t x, uint16_t y) noexcept
        {
            SDL_SetWindowPosition(m_window, x, y);
        }
        void _Window::set_size(uint16_t w, uint16_t h) noexcept
        {
            SDL_SetWindowSize(m_window, w, h);
        }
        window_position_t _Window::get_position() const noexcept
        {
            int x, y;
            SDL_GetWindowPosition(m_window, &x, &y);
            return window_position_t{x, y};
        }
        window_size_t _Window::get_size() const noexcept
        {
            int w,h;
            SDL_GetWindowSize(m_window, &w, &h);
            return window_size_t{w, h};
        }
        void _Window::set_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept
        {
            set_position(x, y);
            set_size(w, h);
        }
        window_rectangle_t _Window::get_rectangle() const noexcept
        {
            window_position_t pos = get_position();
            window_size_t size = get_size();
            return window_rectangle_t{ pos.m_x, pos.m_y, size.m_x, size.m_y };
        }

        video::WindowID _Window::get_window_id() const noexcept
        {
            return SDL_GetWindowID(m_window);
        }

        _Renderer::_Renderer(const _Window& p_window_base)
        {
            m_renderer = SDL_CreateRenderer(p_window_base.m_window, __Renderer_counter::count(), SDL_RENDERER_SOFTWARE);
            if(not m_renderer)
                throw std::exception(SDL_GetError());
            __Renderer_counter::increment();
        }
        _Renderer::~_Renderer() noexcept
        {
            __Renderer_counter::decrement();
            SDL_DestroyRenderer(m_renderer);
        }
        _Renderer::_Renderer(_Renderer&& p_other) noexcept
        {
            m_renderer = p_other.m_renderer;
        }
        _Renderer& _Renderer::operator=(_Renderer&& p_other) noexcept
        {
            m_renderer = p_other.m_renderer;
            return *this;
        }
        void _Renderer::set_color(common::Color::red_t r, 
                                  common::Color::green_t g, 
                                  common::Color::blue_t b, 
                                  common::Color::alpha_t a) noexcept
        {
            SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
        }
        void _Renderer::clear() noexcept
        {
            SDL_RenderClear(m_renderer);
        }
        void _Renderer::present() noexcept
        {
            SDL_RenderPresent(m_renderer);
        }

        // -- !VIDEO SYSTEMS -- //

        void handle()
        {
            SDL_Event ev;
            while(SDL_PollEvent(&ev))
            {
            
                /*
                SDL_QUIT = 0x100, /**< User-requested quit 
                                        
                // Display events 
                SDL_DISPLAYEVENT = 0x150,  

                // Window events 
                SDL_WINDOWEVENT = 0x200, 
                SDL_SYSWMEVENT,             

                // Keyboard events 
                SDL_KEYDOWN = 0x300, 
                SDL_KEYUP,                 
                SDL_TEXTEDITING,           
                SDL_TEXTINPUT,             
                SDL_KEYMAPCHANGED,         
                                       
                // Mouse events 
                SDL_MOUSEMOTION = 0x400, 
                SDL_MOUSEBUTTONDOWN,       
                SDL_MOUSEBUTTONUP,         
                SDL_MOUSEWHEEL,            

                // Joystick events 
                SDL_JOYAXISMOTION = 0x600, 
                SDL_JOYBALLMOTION,         
                SDL_JOYHATMOTION,          
                SDL_JOYBUTTONDOWN,         
                SDL_JOYBUTTONUP,           
                SDL_JOYDEVICEADDED,        
                SDL_JOYDEVICEREMOVED,      

                // Game controller events 
                SDL_CONTROLLERAXISMOTION = 0x650, 
                SDL_CONTROLLERBUTTONDOWN,         
                SDL_CONTROLLERBUTTONUP,           
                SDL_CONTROLLERDEVICEADDED,        
                SDL_CONTROLLERDEVICEREMOVED,      
                SDL_CONTROLLERDEVICEREMAPPED,     

                // Touch events 
                SDL_FINGERDOWN = 0x700,
                SDL_FINGERUP,
                SDL_FINGERMOTION,

                // Gesture events 
                SDL_DOLLARGESTURE = 0x800,
                SDL_DOLLARRECORD,
                SDL_MULTIGESTURE,

                // Clipboard events 
                SDL_CLIPBOARDUPDATE = 0x900,

                // Drag and drop events 
                SDL_DROPFILE = 0x1000,
                SDL_DROPTEXT,                
                SDL_DROPBEGIN,      
                SDL_DROPCOMPLETE,          

                // Audio hotplug events 
                SDL_AUDIODEVICEADDED = 0x1100,
                SDL_AUDIODEVICEREMOVED,     

                // Sensor events 
                SDL_SENSORUPDATE = 0x1200,
            
                // Render events 
                SDL_RENDER_TARGETS_RESET = 0x2000,
                SDL_RENDER_DEVICE_RESET,
                SDL_USEREVENT = 0x8000,
                SDL_LASTEVENT = 0xFFFF
                */
            }
        }

    }
}

