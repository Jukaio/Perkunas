
#include "pch.h"
#include <perkunas.h>


using namespace std;
using namespace ::testing;

namespace perkunas
{
	namespace video
	{
		void test_init()
		{
			auto video = System("Test", { 100, 100, 100, 100 });
		}

		bool change_window()
		{
			auto video = System("Test", { 100, 100, 100, 100 });
			WindowSize new_size{ 100, 200 };
			video.set_window_size(new_size);

			return new_size.m_x == video.get_window_size().m_x &&
				   new_size.m_y == video.get_window_size().m_y;
		}

		TEST(Perkunas, Video)
		{
			EXPECT_NO_THROW(test_init());
			EXPECT_TRUE(change_window());
		}
	}
	namespace event
	{
		void test_init()
		{
			auto event = System();
		}
		TEST(Perkunas, Event)
		{
			EXPECT_NO_THROW(test_init());
		}

		class TestApplication : public callback::Window
		{
		public:
			bool& m_received_something;
			TestApplication(bool& received) : m_received_something(received) {}

			virtual void on_window_move(const prk::event::window::Move&) final
			{
				m_received_something = true;
			}
			virtual void on_window_resize(const prk::event::window::Resize&) final
			{
				m_received_something = true;
			}
			virtual void on_window_focus(const prk::event::window::Focus&) final
			{
				m_received_something = true;
			}
			virtual void on_window_status(const prk::event::window::Status&) final
			{
				m_received_something = true;
			}
			virtual void on_window_visibility(const prk::event::window::Visiblity&) final
			{
				m_received_something = true;
			}
		};
		void test_callback()
		{
			bool received_something = false;

			auto event = System();
			TestApplication class_with_callbacks(received_something);
			event.add_window_callbacks(class_with_callbacks);

			video::test_init();
			event.poll();

			EXPECT_TRUE(received_something);
		}
		TEST(Perkunas, EventPoll)
		{
			test_callback();
		}
	}
	namespace geometry
	{
		TEST(Perkunas, GeometryDecimalToInt)
		{
			Rect<int> test_int_rect{ 10, 20, 10, 10 };
			Rect<double> test_double_rect{ 10.2, 20.3, 10.02, 10.5 };

			static auto compare_int_rect = [](Rect<int> lhs, Rect<int> rhs)
			{
				return (lhs.m_x == rhs.m_x &&
						lhs.m_y == rhs.m_y &&
						lhs.m_w == rhs.m_w &&
						lhs.m_h == rhs.m_h);
			};
			EXPECT_TRUE(compare_int_rect(test_int_rect, test_double_rect));
		}

		TEST(Perkunas, GeometryHigherCast)
		{
			const int max_int = static_cast<int>(~0);
			const uint64_t max_uint64 = static_cast<uint64_t>(~0);

			Rect<int> test_int_rect{ max_int, max_int, max_int, max_int };
			Rect<uint64_t> test_uint64_rect{ max_uint64, max_uint64, max_uint64, max_uint64 };

			static auto compare_int_rect = [](Rect<int> lhs, Rect<int> rhs)
			{
				return (lhs.m_x == rhs.m_x &&
						lhs.m_y == rhs.m_y &&
						lhs.m_w == rhs.m_w &&
						lhs.m_h == rhs.m_h);
			};
			EXPECT_TRUE(compare_int_rect(test_int_rect, test_uint64_rect));
		}
	}

}
