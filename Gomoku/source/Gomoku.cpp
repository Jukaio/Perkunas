// Gomoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <perkunas.h>
#include <vector>
#include <Windows.h>
#include <chrono>

#include <fstream>

template <typename _Type>
class Vector
{
public:
	using iterator = _Type*;
	using const_iterator = const _Type*;
	using pointer = _Type*;
	using const_pointer = const _Type*;
	using reference = _Type&;
	using const_reference = const _Type&;
	using value_type = _Type;
	using size_type = size_t; // 64-bit system: unsigned long

private:
	pointer _safe_allocation(size_type size)	// Allocating an array of T with count 0 is undefined behaviour
	{										// To avoid this, we use this little function to allow passing 0 as count
		if(size > 0)
			return new value_type[size];
		return nullptr;
	}

	void _invalidate()
	{
		m_count = 0;
		m_capacity = 0;
		m_data = nullptr;
	}

	void _tidy()
	{
		delete m_data;
		_invalidate();
	}

	void _swap(const Vector& rhs)
	{
		using std::swap;

		swap(m_data,	 rhs.m_data);
		swap(m_count,	 rhs.m_count);
		swap(m_capacity, rhs.m_capacity);
	}


public:
	explicit Vector(size_type size = 0)
		: m_data(_safe_allocation(size))	// Memory allocation of value_type * passed size
		, m_count(size)						// Assignment of passed size to count
		, m_capacity(size)					// Assignment of passed size to capacity
	{
		std::fill(begin(),					// data[0]	
				  end(),					// data[count] index AFTER the last element
				  value_type());			// Default constructor TODO: new()[] 
	}

	Vector(const Vector& rhs)
		: m_data(_safe_allocation(rhs.m_capacity))	// Memory allocation of value_type with passed size
		, m_count(rhs.m_count)						// Assignment of passed size to count
		, m_capacity(rhs.m_capacity)				// Assignment of passed size to capacity
	{
		std::copy(rhs.begin(), 
				  rhs.end(), 
				  begin());
	}

	Vector(Vector&& rhs)
		: m_data(rhs.m_data)			// Allocation happened in rhs already, take its values
		, m_count(rhs.m_count)			// Assignment of passed size to count
		, m_capacity(rhs.m_capacity)	// Assignment of passed size to capacity
	{
		rhs._invalidate();				// Invalidate rhs
	}

	Vector& operator=(const Vector& rhs)
	{
		if(this != &rhs)	// (If rhs == this, we would swap invalid data with invalid data (nullptr, 0, 0)
		{
			_tidy();
			m_data = _safe_allocation(rhs.m_capacity);
			m_count = rhs.m_count;
			m_capacity = rhs.m_capacity;
			std::copy(rhs.begin(), 
					  rhs.end(), 
					  begin());
		}
		return *this;
	}

	Vector& operator=(Vector&& rhs)
	{
		if(this != &rhs)	// (If rhs == this, we would swap invalid data with invalid data (nullptr, 0, 0)
		{
			_tidy();		// Delete data and invalidate in this
			_swap(rhs);		// Take contents from rhs
		}
		return *this; 
	}	

	~Vector() { delete m_data; }
		

	iterator begin() const noexcept { return &m_data[0]; }
	iterator end() const noexcept { return &m_data[m_count]; }

private:
	pointer m_data		= nullptr;	// Storage of the data
	size_type m_count	= 0;		// Current size of the data
	size_type m_capacity= 0;		// Maximum size of the data until reallocation
};

class Application : public perkunas::InputCallback, public perkunas::WindowCallback
{
public:

	// Inherited via InputCallback
	virtual void on_keyboard(perkunas::events::input::Keyboard&) override
	{

	}

	virtual void on_mouse_wheel(perkunas::events::input::mouse::Wheel&) override
	{

	}

	virtual void on_mouse_button(perkunas::events::input::mouse::Button&) override
	{

	}

	virtual void on_mouse_motion(perkunas::events::input::mouse::Motion&) override
	{

	}

	// Inherited via WindowCallback
	virtual void on_window_move(const perkunas::WindowMoveEvent& p_event) override
	{
		printf("Testing Move: %d, %d\n", p_event.m_position.m_x, p_event.m_position.m_y);
	}

	virtual void on_window_resize(const perkunas::WindowResizeEvent&) override
	{
	}

	virtual void on_window_focus(const perkunas::WindowFocusEvent&) override
	{
	}

	virtual void on_window_status(const perkunas::WindowStatusEvent&) override
	{
	}

	virtual void on_window_visibility(const perkunas::WindowVisibilityEvent&) override
	{
	}

};

int main(int, char**)
{
	prk::VideoSubSystem video("Hello", { 120, 120, 460, 460 });
	prk::EventSubSystem events;

	Application app;
	events.add_callbacks(app);


	video.set_window_rectangle({ 100, 100, 480, 480 });
	video.set_window_title("Hello World!");

	prk::Rectangle<int> point1{ 2, 4, 8, 9 };
	prk::Rectangle<float> point2 = point1;


	constexpr int width = 10;
	constexpr int height = 10;
	/*
	
	// replacement for enum - in reality use enum class or something
	const int PREPARE = 0;
	const int PROCESS = 1;
	const int EXIT = 2;
	const int STATE_COUNT = 3;
	int (*function[STATE_COUNT])(int&); // Create array of function pointers - 
										// return-type (*variable-name)(parameter-types)
	std::fill(&function[0], &function[STATE_COUNT], nullptr); // Fill the array of function pointers with nullptrs

	// Assign lambdas to function pointers in array
	function[PREPARE] = [](int& timer)
	{
		timer = 10;
		printf("Prepare: %d\n", timer);
		return 1;
	};	 
	function[PROCESS] = [](int& timer)
	{
		printf("Process: %d\n", timer);
		timer--;
		if(timer >= 0) 
			return 1;
		return 2;
	};
	function[EXIT] = [](int& timer)
	{
		timer = 0;
		printf("Exit: %d\n", timer);
		return 3;
	};

	int context = 0; // The context for the timer - a simple int
	int index = 0;	 // The index for the state machine to orient itself
	while(index < STATE_COUNT && index > -1) // State machine -> Goes from state to state until it breaks out
		index = function[index](context);    // Acces the function which corrosponds to index - Lookup 
		*/
	while(true)
	{
		events.poll();

		video.set_color(prk::red_t{255}, 
						prk::green_t{255}, 
						prk::blue_t{ 255 }, 
						prk::alpha_t{ 255 });
		video.render_clear();
		
		video.set_color(prk::red_t{ 255 }, 
						prk::green_t{ 0 }, 
						prk::blue_t{ 0 }, 
						prk::alpha_t{ 255 });

		for(int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++)
			{
				const int pos_x = x * 32;
				const int pos_y = y * 32;
				video.draw_rectangle({ pos_x, pos_y, 32, 32}, prk::RectangleStyle::NOT_FILLED);
			}
		}

		video.render_present();
	}
	
	return 0;
}