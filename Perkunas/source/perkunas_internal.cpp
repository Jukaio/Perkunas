// Perkunas.cpp 

#include "perkunas_internal.h"

#include <exception>
#include <algorithm>


namespace perkunas
{
    namespace internal
    {
        // Ignored Warning 26455: Default Constructors have to throw due to the SDL2 initialization
        #pragma warning(push)
        #pragma warning(disable: 26455)
        namespace init
        {
            // -- BASE SDL INITIALISER -- // 
            constexpr bool is_init_successful(int x) noexcept
            {
                return is_successful<SDL_InitState, SDL_InitState::Success>(x);
            }

            Timer::Timer()
            {
                if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_TIMER)))
                    throw std::exception(SDL_GetError());
            }
            Timer::~Timer() noexcept
            {
                SDL_QuitSubSystem(SDL_INIT_TIMER);
            }
            Audio::Audio()
            {
                if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_AUDIO)))
                    throw std::exception(SDL_GetError());
            }
            Audio::~Audio() noexcept
            {
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
            }
            Video::Video() 
            {
                if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_VIDEO)))
                    throw std::exception(SDL_GetError());
            }
            Video::~Video() noexcept
            {
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
            }
            Events::Events() 
            {
                if(not is_init_successful(SDL_InitSubSystem(SDL_INIT_EVENTS)))
                    throw std::exception(SDL_GetError());
            }
            Events::~Events() noexcept
            {
                SDL_QuitSubSystem(SDL_INIT_EVENTS);
            }
        }
        #pragma warning(pop) 
        // -- END BASE SDL INITIALISER -- // 
    
        // -- VIDEO SYSTEMS -- //

        Window::Window(const video::window::Title& title, uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept
        {
            m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
        }
        Window::~Window() noexcept
        {
            SDL_DestroyWindow(m_window);
        }
        Window::Window(Window&& p_other) noexcept
        {
            m_window = p_other.m_window;
        }
        Window& Window::operator=(Window&& p_other) noexcept
        {
            m_window = p_other.m_window;
            return *this;
        }
        void Window::set_title(const video::window::Title& p_title) noexcept
        {
            SDL_SetWindowTitle(m_window, p_title.c_str());
        }
        video::window::Title Window::get_title()
        {
            return video::window::Title{ SDL_GetWindowTitle(m_window) };
        }
        void Window::set_position(uint16_t x, uint16_t y) noexcept
        {
            SDL_SetWindowPosition(m_window, x, y);
        }
        void Window::set_size(uint16_t w, uint16_t h) noexcept
        {
            SDL_SetWindowSize(m_window, w, h);
        }
        video::window::Position Window::get_position() const noexcept
        {
            int x, y;
            SDL_GetWindowPosition(m_window, &x, &y);
            return video::window::Position{x, y};
        }
        video::window::Size Window::get_size() const noexcept
        {
            int w,h;
            SDL_GetWindowSize(m_window, &w, &h);
            return video::window::Size{w, h};
        }
        void Window::set_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) noexcept
        {
            set_position(x, y);
            set_size(w, h);
        }
        video::window::Rectangle Window::get_rectangle() const noexcept
        {
            video::window::Position pos = get_position();
            video::window::Size size = get_size();
            return video::window::Rectangle{ pos.m_x, pos.m_y, size.m_x, size.m_y };
        }

        video::window::ID Window::get_id() const noexcept
        {
            return SDL_GetWindowID(m_window);
        }

        Renderer::Renderer(const Window& p_window_base)
        {
            m_data = SDL_CreateRenderer(p_window_base.m_window, __Renderer_counter::count(), SDL_RENDERER_SOFTWARE);
            if(not m_data)
                throw std::exception(SDL_GetError());
            __Renderer_counter::increment();
        }
        Renderer::~Renderer() noexcept
        {
            __Renderer_counter::decrement();
            SDL_DestroyRenderer(m_data);
        }
        Renderer::Renderer(Renderer&& p_other) noexcept
        {
            m_data = p_other.m_data;
        }
        Renderer& Renderer::operator=(Renderer&& p_other) noexcept
        {
            m_data = p_other.m_data;
            return *this;
        }
  
        void Renderer::set_color(video::Color::uintx_t r,
                                 video::Color::uintx_t g,
                                 video::Color::uintx_t b,
                                 video::Color::uintx_t a) noexcept
        {
            SDL_SetRenderDrawColor(m_data, r, g, b, a);
        }
        video::Color Renderer::get_color() noexcept
        {
            video::Color::uintx_t r, g, b, a;
            SDL_GetRenderDrawColor(m_data, &r, &g, &b, &a);
            return video::Color{r, g, b, a};
        }
        void Renderer::clear() noexcept
        {
            SDL_RenderClear(m_data);
        }
        void Renderer::present() noexcept
        {
            SDL_RenderPresent(m_data);
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

        Surface::Surface(const common::FilePath& at)
        {
            m_data = SDL_LoadBMP(at.string().c_str());
        }

        Surface::~Surface()
        {
            check_valid();
            SDL_FreeSurface(m_data);
        }

        bool Surface::is_vaild()
        {
            return m_data != nullptr;
        }

        void Surface::check_valid()
        {
            if(not is_vaild())
                throw std::runtime_error("Surface not valid\n");
        }

        Texture::Texture(const Renderer& with, const Surface& that)
        {
            m_data = SDL_CreateTextureFromSurface(with, that);
        }

        Texture::~Texture()
        {
            check_valid();
            SDL_DestroyTexture(m_data);
        }

        bool Texture::is_vaild()
        {
            return m_data != nullptr;
        }

        void Texture::check_valid()
        {
            if(not is_vaild())
                throw std::runtime_error("Texture not valid\n");
        }

}
}

