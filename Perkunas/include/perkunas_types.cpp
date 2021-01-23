#include "perkunas_types.h"

namespace perkunas
{
	namespace video
	{

	}

	namespace chrono
	{
		Timer::Timer(Interval time, Callback that)
			: callback(that)
			, time(time)
		{
		}
		Timer::~Timer()
		{

		}
		void Timer::set(Callback that)
		{
			callback = that;
		}
		void Timer::set(Interval t)
		{
			this->time = t;
		}
		void Timer::operator()()
		{
			if(is_valid())
				callback(*this);
		}
		Timer::operator Interval() const
		{
			return time;
		}

		bool Timer::is_valid() const
		{
			return static_cast<bool>(callback) && id != INVALID;
		}
	}
}

