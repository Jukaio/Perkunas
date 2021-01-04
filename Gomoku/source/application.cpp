
#include "application.h"

Application::Grid::Grid(video_system_t& video, Data data)
	: data{ data.pos, data.dim, data.node_dim }
	, video(video)
{
	nodes.resize(get_size());
}
const Application::Grid::dimension_t& Application::Grid::get_dimension() const { return data.dim; }
const Application::Grid::dimension_t& Application::Grid::get_node_dimension() const { return data.node_dim; }
const Application::Grid::position_t& Application::Grid::get_position() const { return data.pos; }
int Application::Grid::get_size() const { return (data.dim.m_x * data.dim.m_y); }

void Application::Grid::draw()
{
	typedef prk::video::RectangleStyle RectStyle;

	const auto dimension = get_dimension();
	const auto position = get_position();
	const auto node_dimensions = get_node_dimension();

	for(int x = 0; x < dimension.m_x; x++)
		for(int y = 0; y < dimension.m_y; y++)
		{
			const position_t draw_at
			{
				position.m_x + (x * node_dimensions.m_x),
				position.m_y + (y * node_dimensions.m_y)
			};
			video.draw_rectangle(prk::geometry::Rect<int64_t>
			{
				draw_at.m_x,
					draw_at.m_y,
					node_dimensions.m_x,
					node_dimensions.m_y
			},
								 RectStyle::NOT_FILLED);
		}
}

void Application::set_state(State state)
{
	m_state = state;
}

Application::Application()
	: m_video("", {})
	, m_grid(m_video, { {120, 60}, {15, 15}, {32, 32} })
{
	set_state(State::Init);
	m_events.add_callbacks(*this);

	prk::video::WindowRectangle window_rect;
	prk::video::WindowTitle window_title;
	try
	{
		ini::FileReader reader("settings.ini");
		window_rect.m_x = reader.GetInt("x");
		window_rect.m_y = reader.GetInt("y");
		window_rect.m_w = reader.GetInt("width");
		window_rect.m_h = reader.GetInt("height");
		window_title = reader.GetString("title");
	}
	catch(const ini::FileReaderException& exc)
	{
		printf(exc.what());
	}
	catch(const std::exception& exc)
	{
		printf(exc.what());
	}
	m_video.set_window_rectangle(window_rect);
	m_video.set_window_title(window_title);

	//typedef Gomoku::Board Board;

	/*
	Gomoku::Board board;
	BitMatrix scanner({15, 15});
	for(Board::State::Layer::PointInt i = 0; i < 5; i++)
	{
		scanner.set( { i, 0 }, true);
		board.set(Content::Black, { i, 0 });
	}
	[[maybe_unused]]auto does_it = board.has(scanner);
	board.display();
	*/
}

void Application::run()
{
	set_state(State::Run);
	while(is_running())
	{
		update_input();
		m_events.poll();

		m_gomoku.update(m_mouse);

		m_video.set_color(prk::common::Color{ 255, 0, 0, 255 });
		m_video.render_clear();

		m_gomoku.draw(m_video);

		m_video.render_present();
	}
}

bool Application::is_running()
{
	return m_state == State::Run;
}

Application::Gomoku::Content Application::Gomoku::Board::has(Position at)
{
	if(m_state[State::Type::PlacedStones][at])
	{
		bool colour = m_state[State::Type::PlacedColour][at];
		return static_cast<Content>(colour);
	}
	return Content::Nothing;
}

Application::Gomoku::Content Application::Gomoku::Board::has(const BitMatrix& with)
{
	auto placed = m_state[State::Type::PlacedStones] & with;
	if(placed == with)
	{
		auto color = placed & m_state[State::Type::PlacedColour];
		if(color == placed)
		{
			return Content::White;
		}
		else if(color.none())
		{
			return Content::Black;
		}
	}
	return Content::Nothing;
}

void Application::Gomoku::Board::reset()
{
	for(int i = 0; i < State::bitfield_count; i++)
		m_state[static_cast<State::Type>(i)].reset();
}

void Application::Gomoku::Board::set(Content that, Position at)
{
	const auto content_colour_to_bool = [](Content that)
	{
		return static_cast<bool>(that);
	};
	m_state[State::Type::PlacedStones].set(at, that != Content::Nothing);
	m_state[State::Type::PlacedColour].set(at, content_colour_to_bool(that));
}

void Application::Gomoku::Board::display()
{
	for(int i = 0; i < static_cast<int>(State::Type::Count); i++)
	{
		m_state[static_cast<State::Type>(i)].display();
		printf("\n");
	}
}

inline void Application::Gomoku::Board::State::check_out_of_range(Type at)
{
	if(at == Type::Count)
		throw OutOfRange{};
}

Application::Gomoku::Board::State::State()
{
	std::fill(std::begin(m_data), std::end(m_data), Layer({ rows, columns }));
}

Application::Gomoku::Board::State::Layer& Application::Gomoku::Board::State::operator[](Type at)
{
	check_out_of_range(at);
	int index = static_cast<int>(at);
	return m_data[index];
}

void Application::Gomoku::reset()
{
	m_board.reset();
}

bool Application::Gomoku::check_win()
{
	enum class ScanType
	{
		Vertical,
		Horizontal,
		TopLeftToBottomRight,
		TopRightToBottomLeft,
		Count
	};
	auto scan_type_to_index = [](ScanType type) { return static_cast<size_t>(type); };
	BitMatrix::Scanner scans[scan_type_to_index(ScanType::Count)]
	{
		{{rows, columns}, { 0, 0 }, { 0, 4 } },
		{{rows, columns}, { 0, 0 }, { 4, 0 } },
		{{rows, columns}, { 0, 0 }, { 4, 4 } },
		{{rows, columns}, { 0, 4 }, { 4, 0 } },
	};

	bool found_match = false;
	for(int i = 0; i < scan_type_to_index(ScanType::Count); i++)
	{
		auto& current = scans[i];
		static auto scanning = [&](const BitMatrix& to_scan)
		{
			auto test = m_board.has(to_scan);
			if(test != Content::Nothing)
			{
				found_match = true;
			}
		};
		current.for_each(scanning);
		if(found_match)
			return true;
	}
	return false;
}

bool Application::Gomoku::player_turn(const MouseState& mouse)
{
	typedef MouseState::Button::Code MouseCode;
	
	if(mouse.just_pressed(MouseState::Button::Code::Left))
	{
		auto position = mouse.get_position();
		position.m_x /= 32;
		position.m_y /= 32;
		if(position.m_x < rows && 
		   position.m_y < columns &&
		   is_empty(position))
		{
			place(Gomoku::Content::Black, position);
			return true;
		}
	}
	return false;
}

bool Application::Gomoku::ai_turn(const MouseState& mouse)
{
	if(mouse.just_pressed(MouseState::Button::Code::Left))
	{
		auto position = mouse.get_position();
		position.m_x /= 32;
		position.m_y /= 32;

		if(position.m_x < rows &&
		   position.m_y < columns &&
		   is_empty(position))
		{
			place(Gomoku::Content::White, position);
			return true;
		}
	}
	return false;
}

bool Application::Gomoku::is_empty(Position at)
{
	return m_board.has(at) == Content::Nothing;
}

void Application::Gomoku::place(Content that, Position at)
{
	m_board.set(that, at);
}

void Application::Gomoku::update(const MouseState& mouse)
{
	switch(m_state)
	{
		case State::PlayerTurn:
			if(player_turn(mouse))
			{
				set_state(State::AITurn);
				if(check_win())
				{
					printf("Player won!!\n");
					reset();
				}
			}
			break;

		case State::AITurn:
			if(ai_turn(mouse))
			{
				set_state(State::PlayerTurn);;
				if(check_win())
				{
					printf("AI won!!\n");
					reset();
				}
			}
			break;
	}
}

void Application::Gomoku::draw(prk::video::System& video)
{
	typedef prk::video::RectangleStyle RectStyle;
	const RectStyle style = RectStyle::FILLED;

	for(Board::State::Layer::PointInt y = 0; y < columns; y++)
		for(Board::State::Layer::PointInt x = 0; x < rows; x++)
		{
			prk::video::Rectangle to_draw{ x * 32, y * 32, 32, 32 };
			switch(m_board.has({ x, y }))
			{
				case Content::Nothing:
					video.set_color({ 125, 125, 125, 255 });
					break;
				case Content::Black:
					video.set_color({ 0, 0, 0, 255 });
					break;

				case Content::White:
					video.set_color({ 255, 255, 255, 255 });
					break;
			}
			video.draw_rectangle(to_draw, style);
		}
}
