// application.h

#ifndef INCLUDED_APPLICATION_H
#define INCLUDED_APPLICATION_H

#include <stdio.h>
#include <vector>
#include <perkunas>
#include <array>
#include "FileReader.hpp"
#include <bitset>
#include <functional>
#include <cmath>

class BitMatrix
{
public:
	class Scanner;

	static const size_t max_size = 256;
	typedef uint8_t PointInt;
	typedef std::bitset<max_size> MyLayer;
	typedef prk::geometry::Point<PointInt> MyPoint;
	typedef MyPoint Position;
	typedef MyPoint Dimension;

	class DimensionsTooBigForBuffer : public std::out_of_range
	{
	public:
		DimensionsTooBigForBuffer()
			: std::out_of_range("The buffer of the BitMatrix is not big enough\n")
		{
		}
	};
	class PositionOutOfDimensions : public std::out_of_range
	{
	public:
		PositionOutOfDimensions(const Position& at)
			: std::out_of_range(std::string("Position: ") +
								std::to_string(at.m_x) +
								" " +
								std::to_string(at.m_y) +
								std::string(" is out of Dimensions"))
		{
		}
	};


private:
	void out_of_dimensions_check(const MyPoint& at) const
	{
		if((at.m_x > this->m_dims.m_x ||
		   at.m_y > this->m_dims.m_y))
		{
			throw PositionOutOfDimensions{at};
		}
	}
	void out_of_buffer_check(const MyPoint& at) const
	{
		size_t size = static_cast<size_t>(at.m_x) * at.m_y;
		if(size > max_size)
			throw DimensionsTooBigForBuffer{};
	}
	size_t position_to_index(const Position& at) const
	{
		out_of_dimensions_check(at);
		return (static_cast<size_t>(at.m_y) * m_dims.m_x) + at.m_x;
	}

public:
	BitMatrix() noexcept : m_dims{ 0, 0 }
	{
	}
	BitMatrix(Dimension dims) : m_dims(dims)
	{
		out_of_buffer_check(dims);
	}

	bool operator[](Position at) const
	{
		auto index = position_to_index(at);
		return m_data[index];
	}

	void set()
	{
		m_data.set();
	}
	void reset()
	{
		m_data.reset();
	}

	void set(Position at, bool value)
	{
		auto index = position_to_index(at);
		m_data.set(index, value);
	}

	void resize(Dimension dims)
	{
		out_of_buffer_check(dims);
		m_dims = dims;
	}
	size_t size() const
	{
		return position_to_index(m_dims);
	}
	Dimension get_dimension() const
	{
		return m_dims;
	}


	void display()
	{
		for(PointInt y = 0; y < m_dims.m_y; y++)
		{ 
			for(PointInt x = 0; x < m_dims.m_x; x++)
			{
				auto index = position_to_index({x, y});
				printf("%d", static_cast<int>(m_data[index]));
			}
			printf("\n");
		}
	}

	BitMatrix& operator&=(const BitMatrix& other)
	{
		if(other.size() > this->size())
			this->m_dims = other.m_dims;

		this->m_data &= other.m_data;
		return *this;
	}

	bool any()
	{
		return this->m_data.any();
	}
	bool all()
	{
		return this->m_data.all();
	}
	bool none()
	{
		return this->m_data.none();
	}

	bool operator==(const BitMatrix& other) const
	{
		return this->m_data == other.m_data;
	}
	bool operator!=(const BitMatrix& other) const
	{
		return !(*this == other);
	}

	BitMatrix operator&(const BitMatrix& other)
	{
		BitMatrix to_return = *this;
		return to_return &= other;
	}
	BitMatrix& operator|=(const BitMatrix& other)
	{
		if(other.size() > this->size())
			this->m_dims = other.m_dims;

		this->m_data |= other.m_data;
		return *this;
	}
	BitMatrix operator|(const BitMatrix& other)
	{
		BitMatrix to_return = *this;
		return to_return |= other;
	}
	BitMatrix& operator<<=(size_t pos)
	{
		this->m_data <<= pos;
		return *this;
	}
	BitMatrix& operator>>=(size_t pos)
	{
		this->m_data >>= pos;
		return *this;
	}
	BitMatrix operator<<(size_t pos)
	{
		BitMatrix to_return = *this;
		return to_return <<= pos;
	}
	BitMatrix operator>>(size_t pos)
	{
		BitMatrix to_return = *this;
		return to_return >>= pos;
	}

private:
	MyLayer m_data;
	Dimension m_dims;
};

class BitMatrix::Scanner
{
public:
	typedef BitMatrix::PointInt PointInt;
	typedef prk::geometry::Point<PointInt> MyPoint;
	typedef MyPoint Position;
	typedef MyPoint Dimension;

	Scanner(const Dimension& dimension, Position from, Position to)
		: m_bits(dimension)
		, m_dims(calculate_line_dimension(from, to))
	{
		make_bit_line(from, to);
	}

	Scanner(const BitMatrix& to_scan, Position from, Position to)
		: m_bits({to_scan.get_dimension()})
		, m_dims(calculate_line_dimension(from, to))
	{
		make_bit_line(from, to);
	}


private:
	Dimension calculate_line_dimension(const Position& from, const Position& to)
	{
		int x = abs(to.m_x - from.m_x);
		int y = abs(to.m_y - from.m_y);

		return prk::geometry::Point<int>{ x, y };
	}
	void make_bit_line(const Position& from, const Position& to)
	{
		int dx = to.m_x - from.m_x;
		int dy = to.m_y - from.m_y;
		int step = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);
		
		dx /= step;
		dy /= step;

		int x = from.m_x;
		int y = from.m_y;

		for(int i = 0; i <= step; i++)
		{
			m_bits.set(prk::geometry::Point<int>{ x, y }, true);
			x += dx;
			y += dy;
		}
	}

public:
	Dimension get_dimension() const
	{
		return m_dims;
	}

	void display()
	{
		m_bits.display();
	}

	void for_each(std::function<void(const BitMatrix&)> act)
	{
		BitMatrix scanner = m_bits;
		MyPoint full_dim = scanner.get_dimension();
		for(PointInt y = 0; y < full_dim.m_y - get_dimension().m_y; y++)
		{
			for(PointInt x = 0; x < full_dim.m_x - get_dimension().m_x; x++)
			{
				act(scanner);
				scanner <<= 1;
			}
			scanner <<= get_dimension().m_x;
		}
	}

private:
	BitMatrix m_bits;
	Dimension m_dims;
};



class Application
	: public prk::event::callback::Input
	, public prk::event::callback::Window
{
private:
	class KeyboardState
	{
	public:
		using Keyboard = prk::event::input::Keyboard;
		using KeyState = Keyboard::State;
		using KeyCode = Keyboard::Key;

	private:
		constexpr static size_t key_count = static_cast<size_t>(KeyCode::CodeCount);
		std::array<KeyState, key_count> m_current_states;
		std::array<KeyState, key_count> m_prev_states;

	public:
		bool is_down(KeyCode p_key);
		bool is_pressed(KeyCode p_key);
		void update_key_state(KeyCode p_key, KeyState p_state);
		void update_previous_states();
	};
	class MouseState
	{
	public:
		using Button = prk::event::input::mouse::Button;
		using ButtonCode = Button::Code;
		using ButtonState = Button::State;
		using Wheel = prk::event::input::mouse::Wheel;
		using Motion = prk::event::input::mouse::Motion;

		bool is_down(ButtonCode p_key) const;
		bool just_pressed(ButtonCode p_key) const;
		void update_button_state(ButtonCode p_key, ButtonState p_state);
		void update_previous_states();
		void update_position(Motion::Coordinate p_position);
		Motion::Coordinate get_position() const;

	private:
		constexpr static size_t key_count = static_cast<size_t>(ButtonCode::Count);
		std::array<ButtonState, key_count> m_current_states;
		std::array<ButtonState, key_count> m_prev_states;
		Motion::Coordinate m_position;
	};
	struct Color
	{
		int r, g, b, a;
	};
	class Grid
	{
	public:
		typedef prk::geometry::Point<int> position_t;
		typedef prk::geometry::Point<int> dimension_t;
		typedef prk::video::System video_system_t;

	private:
		struct Data
		{
			position_t pos;
			dimension_t dim;
			dimension_t node_dim;
		};

		struct Node
		{
			Color m_color{ 0, 0, 0, 255 };
		};
		typedef std::vector<Node> NodeContainer;

		const dimension_t& get_dimension() const;
		const dimension_t& get_node_dimension() const;
		const position_t& get_position() const;
		int get_size() const;

	public:
		Grid(video_system_t& video, Data data);
		void draw();

	private:
		video_system_t& video;
		Data data;
		NodeContainer nodes;
	};

	class Gomoku
	{
	public:
		static const uint8_t rows = 15;
		static const uint8_t columns = 15;

		enum class State
		{
			PlayerTurn,
			AITurn
		};

		enum class Content
		{
			Nothing = -1,		// Nothing = -1
			Black = false,		// Black = 0
			White = true,		// White = 1
		};
		typedef prk::geometry::Point<int> Position;

		class Board
		{
		public:
			typedef uint32_t Index;

			class State
			{
			public:
				enum class Type
				{
					PlacedStones,
					PlacedColour,
					Count
				};
				typedef BitMatrix Layer;
				static const uint8_t bitfield_count = uint8_t(Type::Count);

			private:
				typedef std::array<Layer, bitfield_count> Layers;

			public:
				class OutOfRange : public std::out_of_range
				{
				public:
					OutOfRange() : std::out_of_range("Do not pass in Type::Count") 
					{ }
				};

			private:
				void check_out_of_range(Type at);

			public:
				State();
				Layer& operator[](Type at);

			private:
				Layers m_data;
			};

			Content has(Position at);
			Content has(const BitMatrix& with);
			void reset();
			void set(Content that, Position at);
			void display();

		private:
			State m_state;
		};

		void set_state(Gomoku::State state)
		{
			m_state = state;
		}

		void reset();
		bool check_win();
		bool ai_turn(const MouseState& mouse);
		bool is_empty(Position at);
		void place(Content that, Position at);
		bool player_turn(const MouseState& mouse);
	public:
		void update(const MouseState& mouse);
		void draw(prk::video::System& video);

	private:
		State m_state = State::PlayerTurn;
		Board m_board;
	};



	enum class State
	{
		Undefined = -1,
		Init,
		Run,
		Quit
	};

public:
	// Inherited via InputCallback
	virtual void on_keyboard(const prk::event::input::Keyboard& event) final;
	virtual void on_mouse_button(const prk::event::input::mouse::Button& event) final;
	virtual void on_mouse_wheel(const prk::event::input::mouse::Wheel& event) final;
	virtual void on_mouse_motion(const prk::event::input::mouse::Motion& event) final;

	// Inherited via WindowCallback
	virtual void on_window_move(const prk::event::window::Move& event) final;
	virtual void on_window_resize(const prk::event::window::Resize& event) final;
	virtual void on_window_focus(const prk::event::window::Focus& event) final;
	virtual void on_window_status(const prk::event::window::Status& event) final;
	virtual void on_window_visibility(const prk::event::window::Visiblity& event) final;

	Application();
	void run();

private:
	void update_input();
	void set_state(State state);
	bool is_running();
	

	State m_state = State::Undefined;

	prk::video::System m_video;
	prk::event::System m_events;

	KeyboardState m_keyboard{};
	MouseState m_mouse{};

	Gomoku m_gomoku{};
	Grid m_grid;
};
#endif // !INCLUDED_APPLICATION_H
