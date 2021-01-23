// perkunas_chrono_system.cpp

#include "perkunas.h"
#include "perkunas_internal.h"

namespace perkunas
{
	namespace chrono
	{
		class System::Impl : public internal::init::Timer
		{
		public:
			Impl() = default;
			~Impl() = default;
		};
		System::System()
			: m_impl(std::make_unique<Impl>())
		{

		}
		System::~System()
		{

		}
		void System::add(Timer& that) const
		{
			auto callback = [](Uint32, void* param)
			{
				auto& user_timer = (*static_cast<Timer*>(param));
				auto time = static_cast<Timer::Interval>(user_timer).count();
				user_timer();
				return static_cast<Uint32>(time);
			};

			if(that.is_valid())
				remove(that);

			auto time = static_cast<Timer::Interval>(that).count();
			SDL_TimerID id = SDL_AddTimer(static_cast<Uint32>(time), callback, &that);
			if(id == 0)
				internal::Error::print_error_message("To throw: Cannot add timer"); // throw error

			that.id = id;
		}

		void System::remove(Timer& that) const
		{
			if(!SDL_RemoveTimer(that.id))
				internal::Error::print_error_message("To throw: Cannot remove timer"); // throw error
			that.id = 0;
		}

		void System::delay(Milliseconds time) const
		{
			SDL_Delay(static_cast<Uint32>(time.count()));
		}
		Milliseconds System::now() const
		{
			return Milliseconds(SDL_GetTicks());
		}
	}
}

