
#include "application.h"
#include <algorithm>
#include <cstring>


void Application::set_state(State state)
{
	m_state = state;
}

Application::Application()
	: m_video("", {})
{
	set_state(State::Init);
	m_events.add_callbacks(*this);

	prk::video::window::Rectangle window_rect;
	prk::video::window::Title window_title;
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
	m_video.set(window_rect);
	m_video.set(window_title);

	auto title = m_video.get<prk::video::WindowTitle>();

	m_gomoku.center_board(m_video);
}

void Application::run()
{
	set_state(State::Run);

	auto texture = m_video.create_texture("sprite.bmp");

	while(is_running())
	{
		update_input();
		m_events.poll();

		m_gomoku.update(m_mouse);

		m_video.set(prk::video::Color{ 125, 125, 125, 255 });
		m_video.clear();

		m_gomoku.draw(m_video);
		m_video.present();

		std::this_thread::sleep_for(Milliseconds{16});
	}
}

bool Application::is_running()
{
	return m_state == State::Run;
}

bool Application::Gomoku::Board::is_empty(Position at)
{
	return not m_state[State::Type::PlacedStones][at];
}

Application::Gomoku::Content Application::Gomoku::Board::has(Position at) const
{
	if(m_state[State::Type::PlacedStones][at])
	{
		bool colour = m_state[State::Type::PlacedColour][at];
		return static_cast<Content>(colour);
	}
	return Content::Nothing;
}

Application::Gomoku::Content Application::Gomoku::Board::has(const BitMatrix& with) const
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

bool Application::Gomoku::Board::is_full() const
{
	
	return m_state[State::Type::PlacedStones].all();
}

size_t Application::Gomoku::Board::count(bool value)
{
	uint64_t count = 0;
	auto counter = [this, &count, &value](BitMatrix::PointInt x, BitMatrix::PointInt y)
	{
		if(not is_empty({x, y}) == value)
			count++;
	};
	for_each(counter);

	return count;
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

void Application::Gomoku::Board::display(State::Type type)
{
	m_state[type].display();
}

void Application::Gomoku::Board::display()
{
	auto print = [this](BitMatrix::PointInt x, BitMatrix::PointInt y)
	{
		auto content = has({x, y});
		switch(content)
		{
			case Content::Black:	printf("B");	break;
			case Content::White:	printf("W");	break;
			case Content::Nothing:	printf("0");	break;
		}
		if(x == rows - 1)
			printf("\n");
	};
	for_each(print);
}


inline void Application::Gomoku::Board::State::check_out_of_range(Type at) const
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

const Application::Gomoku::Board::State::Layer& Application::Gomoku::Board::State::operator[](Type at) const
{
	check_out_of_range(at);
	int index = static_cast<int>(at);
	return m_data[index];
}

bool Application::Gomoku::Board::for_each(std::function<Bool(BitMatrix::PointInt, BitMatrix::PointInt)> act, bool if_nothing)
{
	for(BitMatrix::PointInt y = 0; y < columns; y++)
	{
		for(BitMatrix::PointInt x = 0; x < rows; x++)
		{
			auto what = act(x, y);
			if(what != Bool::KeepRunning)
				return static_cast<bool>(what);
		}
	}
	return if_nothing;
}

void Application::Gomoku::Board::for_each(std::function<void(BitMatrix::PointInt, BitMatrix::PointInt)> act)
{
	for(BitMatrix::PointInt y = 0; y < columns; y++)
	{
		for(BitMatrix::PointInt x = 0; x < rows; x++)
		{
			act(x, y);
		}
	}
}

void Application::Gomoku::reset()
{
	m_board.reset();
}



Application::Gomoku::Result Application::Gomoku::check_outcome(const Board& board)
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
	const uint8_t match_pos = match_length - 1;
	BitMatrix::Scanner scans[scan_type_to_index(ScanType::Count)]
	{
		{ {rows, columns}, { 0, 0 }, { 0, match_pos } },
		{ {rows, columns}, { 0, 0 }, { match_pos, 0 } },
		{ {rows, columns}, { 0, 0 }, { match_pos, match_pos } },
		{ {rows, columns}, { 0, match_pos }, { match_pos, 0 } },
	};


	auto found_match = Content::Nothing;
	for(int i = 0; i < scan_type_to_index(ScanType::Count); i++)
	{
		auto& current = scans[i];
		auto scanning = [&](const BitMatrix& to_scan)
		{
			auto test = board.has(to_scan);
			if(test != Content::Nothing)
			{
				found_match = test;
			}
		};
		current.for_each(scanning);
		if(found_match != Content::Nothing)
		{
			switch(found_match)
			{
				case Content::Black: return Result::BlackWins;
				case Content::White: return Result::WhiteWins;
			}
		}
	}
	if(board.is_full()) 
		return Result::Draw;
	return Result::None;
}

bool Application::Gomoku::game_over(Board& board)
{
	auto outcome = check_outcome(board);
	if(outcome != Result::None)
	{
		switch(outcome)
		{
			case Result::BlackWins: printf("Black Won!\n"); break;
			case Result::WhiteWins: printf("White Won!\n"); break;
			case Result::Draw:		printf("Draw!\n");		break;
		}
		return true;
	}
	return false;
}

Application::Gomoku::Position Application::Gomoku::screen_to_board(MouseState::Motion::Coordinate position)
{
	position.m_x -= m_draw_offset.m_x;
	position.m_y -= m_draw_offset.m_y;

	position.m_x /= cell_size;
	position.m_y /= cell_size;

	return position;
}

bool Application::Gomoku::is_position_valid(MouseState::Motion::Coordinate position)
{
	if(position.m_x < rows &&
	   position.m_y < columns &&
	   position.m_x >= 0 &&
	   position.m_y >= 0 &&
	   is_empty(position))
	{
		return true;
	}
	return false;
}

Application::Gomoku::PossibleMoves Application::Gomoku::generate_moves(Board board)
{
	PossibleMoves moves;
	if(board.count(true) == 0)
	{
		moves.push_back({ (rows - 1) / 2, (columns - 1) / 2 });
		return moves;
	}

	for(BitMatrix::PointInt y = 0; y < columns; y++)
	{
		for(BitMatrix::PointInt x = 0; x < rows; x++)
		{
			Content content = board.has({x, y});
			if(content != Content::Nothing)
			{
				const int up = y + 1;
				const int down= y - 1;
				const int left = x - 1;
				const int right = x + 1;

				const int max_up = columns - 1;
				const int max_down = 0;
				const int max_right = rows - 1;
				const int max_left = 0;

				const bool has_up = up <= max_up;
				const bool has_down = down >= max_down;
				const bool has_left = left >= max_left;
				const bool has_right = right <= max_right;

				auto check_and_push_back = [&moves, &board](int x, int y)
				{
					const prk::geometry::Point<int> position{ x, y };
					if(board.has(position) == Content::Nothing)
					{
						for(auto& move : moves)
							if(move.m_x == position.m_x &&
							   move.m_y == position.m_y)
								return;
						moves.push_back(position);
					}
				};

				if(has_up)					check_and_push_back(x, up);
				if(has_down)				check_and_push_back(x, down);
				if(has_left)				check_and_push_back(left, y);
				if(has_right)				check_and_push_back(right, y);
				if(has_left && has_up)		check_and_push_back(left, up);
				if(has_left && has_down)	check_and_push_back(left, down);
				if(has_right && has_down)	check_and_push_back(right, down);
				if(has_right && has_up)		check_and_push_back(right, up);;
			}

			//Position position{x, y};
			//if(board.has(position) == Content::Nothing)
			//	moves.push_back(position);
		}
	}

	return moves;
}

float Application::Gomoku::goal_function(Board board, Player player)
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
	const uint8_t max_match_size = match_length - 1; // Last size = win/lose
	BitMatrix::Scanner all_scans[scan_type_to_index(ScanType::Count)][max_match_size];

	for(BitMatrix::PointInt match_size_index = 0; match_size_index < max_match_size; match_size_index++)
	{
		BitMatrix::PointInt target = match_size_index + 1;
		all_scans[scan_type_to_index(ScanType::Vertical)][match_size_index] = {{ rows, columns }, { 0, 0 }, { 0, target }};
		all_scans[scan_type_to_index(ScanType::Horizontal)][match_size_index] = { { rows, columns }, { 0, 0 }, { target, 0} };
		all_scans[scan_type_to_index(ScanType::TopLeftToBottomRight)][match_size_index] = { { rows, columns }, { 0, 0 }, { target, target } };
		all_scans[scan_type_to_index(ScanType::TopRightToBottomLeft)][match_size_index] = { { rows, columns }, { 0, target }, { target, 0 } };
	}

	prk::geometry::Line<int> offsets[scan_type_to_index(ScanType::Count)]
	{
		{ { 0, -1 }, { 0, 1 } },
		{ { -1, 0 }, { 1, 0 } },
		{ { -1, -1 }, { 1, 1 } },
		{ { -1, 1 }, { 1, -1 } }
	};
	
	// Scan and evaluate
	float score = 0.0f;
	for(int i = 0; i < scan_type_to_index(ScanType::Count); i++)
	{
		for(int match_size_index = max_match_size - 1; match_size_index >= 0; match_size_index--) //from big to small
		{
			auto& current = all_scans[i][match_size_index];
			auto& offset = offsets[i];

			auto scanning = [&](const BitMatrix& to_scan, Position at)
			{
				auto scan_check = board.has(to_scan);
				if(scan_check != Content::Nothing)
				{
					auto is_me = player.m_uses == scan_check;
					auto is_enemy = player.m_uses != scan_check;
					float multiplier = (is_me * 1.0f) + (is_enemy * -1.0f);
					const float unblocked = (std::powf((2.0f * (match_size_index + 1)), 2) * multiplier);

					auto block_lower = at;
					auto block_upper = current.get_dimension();
					
					// Check the blocks
					const auto off_start_x = offset.m_start.m_x;
					const auto off_start_y = offset.m_start.m_y;
					const auto off_end_x = offset.m_end.m_x;
					const auto off_end_y = offset.m_end.m_y;

					prk::geometry::Point<int> lower{ block_lower.m_x + off_start_x, block_lower.m_y + off_start_y };
					prk::geometry::Point<int> upper{ block_upper.m_x + off_end_x, block_upper.m_y + off_end_y };

					bool lower_blocked = false;
					bool upper_blocked = false;
					bool out_of_lower_bounds = !(lower.m_x >= 0 && lower.m_y >= 0);
					bool out_of_upper_bounds = !(upper.m_x >= 0 && upper.m_y >= 0);

					if(!out_of_lower_bounds)
						lower_blocked = board.has(lower) != Content::Nothing;
					if(!out_of_upper_bounds)
						upper_blocked = board.has(upper) != Content::Nothing;

					float punish = 0.0f;
					if(lower_blocked || out_of_lower_bounds)
						punish += unblocked / 2;
					if(upper_blocked || out_of_upper_bounds)
						punish += unblocked / 2;

					score += (unblocked - punish);
				}
			};
			current.for_each(scanning);
		}
	}
	return score;
}

float Application::Gomoku::minimax(Board board, int depth, bool my_turn) // true == Maximise
{
	auto result = check_outcome(board);
	switch(result)
	{
		case Result::BlackWins: return -10.0f;
		case Result::WhiteWins: return 10.0f;
		case Result::Draw:		return 0.0f;
	}
	 
	Content place = my_turn ? Content::White : Content::Black; // Correct
	float score = my_turn ? -INFINITY : INFINITY;	// other version has swapped

	auto moves = generate_moves(board);
	for(auto& move : moves)
	{
		board.set(place, move);
		switch(my_turn)
		{
			case true:	score = std::max(score, minimax(board, depth + 1, !my_turn)); break;
			case false:	score = std::min(score, minimax(board, depth + 1, !my_turn)); break;
		}
		board.set(Content::Nothing, move);
	}
	return score;
}

float Application::Gomoku::alphabeta(Board board, int depth, float alpha, float beta, bool my_turn)
{
	auto result = check_outcome(board);
	switch(result)
	{
		case Result::BlackWins: return -99999.0f;
		case Result::WhiteWins: return 99999.0f;
		case Result::Draw:		return 0.0f;
	}

	if(depth > depth_limit)
	{
		if(my_turn)
			return goal_function(board, m_white);
		else
			return goal_function(board, m_white);
	}

	Content place = my_turn ? Content::White : Content::Black; 

	auto moves = generate_moves(board);
	auto sort_by_goal = [&](const Move& lhs, const Move& rhs)
	{
		board.set(Content::Black, lhs);
		float lhs_score = goal_function(board, m_black);
		board.set(Content::Nothing, lhs);

		board.set(Content::Black, rhs);
		float rhs_score = goal_function(board, m_black);
		board.set(Content::Nothing, rhs);

		return lhs_score > rhs_score;
	};
	std::sort(moves.begin(), moves.end(), sort_by_goal);
	
	size_t resize = moves.size() > move_cache ? move_cache : moves.size();
	moves.resize(resize);

	for(auto& move : moves)
	{
		board.set(place, move);
		float score = alphabeta(board, depth + 1, alpha, beta, !my_turn);
		switch(my_turn)
		{
			case true:	alpha = std::max(alpha, score); break;
			case false:	beta = std::min(beta, score);	break;
		}
		board.set(Content::Nothing, move);

		if(alpha >= beta)
			break;
	}
	if(my_turn) 
		return alpha; 
	return beta;
}



Application::Gomoku::Move Application::Gomoku::get_best_move(Board board, Player)
{
	auto moves = generate_moves(board);
	if(moves.empty())
		return { -1, -1 };

	Move best_move { -1, -1 };
	float best_score = -INFINITY;
	if(moves.size() > 1)
	{
		for(auto& move : moves)
		{
			board.set(Content::White, move);
			float score = algo_mode == AlgoMode::Minimax ?
						  minimax(board, 0, false) : 
						  alphabeta(board, 0, -INFINITY, INFINITY, false);
			board.set(Content::Nothing, move);
			if(score > best_score)
			{
				best_move = move;
				best_score = score;
			}
		}
	} 
	else 
		best_move = moves[0];
	return best_move;
}


bool Application::Gomoku::decide_turn(Player player, const MouseState& mouse)
{
	switch(player.m_type)
	{
		case Player::Type::Human:	 return human_turn(mouse, player);
		case Player::Type::Computer: return computer_turn(player);
	}
	return false;
}

bool Application::Gomoku::human_turn(const MouseState& mouse, Player player)
{
	typedef MouseState::Button::Code MouseCode;
	
	if(mouse.just_pressed(MouseCode::Left))
	{
		auto position = screen_to_board(mouse.get_position());
		if(is_position_valid(position))
		{
			place(player.m_uses, position);
			return true;
		}
		return false;
	}
	return false;
}
bool Application::Gomoku::computer_turn(Player player)
{
	auto move = get_best_move(m_board, player);
	place(player.m_uses, move);
	return true;
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
	switch(m_turn)
	{
		case Turn::Black:
			if(decide_turn(m_black, mouse))
			{
				printf("Turn: %d\n", (int)m_board.count(true));
				if(game_over(m_board))
				{	
					reset();
				}
				set_state(Turn::White);
			}
			break;

		case Turn::White:
			if(decide_turn(m_white, mouse))
			{
				printf("Turn: %d\n", (int)m_board.count(true));
				if(game_over(m_board))
				{
					reset();
				}
				set_state(Turn::Black);
				float some_score = goal_function(m_board, m_white);
				printf("Score: %f\n", some_score);
			}
			break;
	}
}

void Application::Gomoku::center_board(prk::video::System& video)
{
	auto window_size = video.get<prk::video::window::Size>();
	Position origin{ window_size.m_x / 2, window_size.m_y / 2 };

	origin.m_x -= (rows * cell_size) / 2;
	origin.m_y -= (columns * cell_size) / 2;

	m_draw_offset = origin;
}

void Application::Gomoku::draw_background(prk::video::System& video)
{
	video.set(prk::video::Color{ 255, 0, 0, 255 });


	for(BitMatrix::PointInt y = 0; y < columns; y++)
	{
		for(BitMatrix::PointInt x = 0; x < rows; x++)
		{
			const int half_cell = (cell_size / 2);

			const int consider_offset_x = half_cell + m_draw_offset.m_x;
			const int consider_offset_y = half_cell + m_draw_offset.m_y;

			const int outer_x = rows * cell_size + consider_offset_x;
			const int outer_y = columns * cell_size + consider_offset_y;

			prk::geometry::Point<int> start_vert{ (x * cell_size) + consider_offset_x, (y * cell_size) + consider_offset_y - cell_size };
			prk::geometry::Point<int> end_vert{ x * cell_size + consider_offset_x, outer_y };

			prk::geometry::Point<int> start_hori{ (x * cell_size) + consider_offset_x - cell_size, (y * cell_size) + consider_offset_y };
			prk::geometry::Point<int> end_hori{ outer_x, y * cell_size + consider_offset_y };

			video.draw({ start_vert, end_vert });
			video.draw({ start_hori, end_hori });
		}
	}
}

void Application::Gomoku::draw(prk::video::System& video)
{
	draw_background(video);

	typedef prk::video::RectangleStyle RectStyle;
	const RectStyle style = RectStyle::FILLED;

	for(Board::State::Layer::PointInt y = 0; y < columns; y++)
		for(Board::State::Layer::PointInt x = 0; x < rows; x++)
		{
			auto content = m_board.has({ x, y });
			if(content != Content::Nothing)
			{
				const int consider_offset_x = m_draw_offset.m_x;
				const int consider_offset_y = m_draw_offset.m_y;

				prk::video::Rectangle to_draw{ (x * cell_size) + (stone_size / 4.0f) + consider_offset_x,
											   (y * cell_size) + (stone_size / 4.0f) + consider_offset_y,
											   stone_size, 
											   stone_size };
				switch(content)
				{
					case Content::Black:
						video.set(prk::video::Color{ 0, 0, 0, 255 });
						video.draw(to_draw);
						break;

					case Content::White:
						video.set(prk::video::Color{ 1.0, 1.0, 1.0, 1.0 });
						video.draw(to_draw);
						break;
				}
			}
		}
}

bool BitMatrix::all() const
{
	const auto& me = *this;
	for(PointInt y = 0; y < get_dimension().m_y; y++)
	{
		for(PointInt x = 0; x < get_dimension().m_x; x++)
		{
			if(not me[{ x, y }])
				return false;
		}
	}
	return true;
}
