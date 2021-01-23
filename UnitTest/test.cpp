
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
			video.set(new_size);

			return new_size.m_x == video.get<window::Size>().m_x &&
				   new_size.m_y == video.get<window::Size>().m_y;
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
			event.add(class_with_callbacks);

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
	namespace video
	{
		TEST(Perkunas, ColorDifferentInitialisationIsSame)
		{
			Color color_decimal;
			color_decimal.red = 1.0;
			color_decimal.green = 1.0;
			color_decimal.blue = 1.0;
			color_decimal.alpha = 1.0;

			Color color_integer;
			color_integer.red = 255;
			color_integer.green = 255;
			color_integer.blue = 255;
			color_integer.alpha = 255;

			Color color_dec{255, 255, 255, 255};
			Color color_int{ 1.0, 1.0, 1.0, 1.0 };
			bool test = color_decimal == color_integer;
			bool test1 = color_dec == color_int;
			bool test2 = color_decimal == color_int;
			bool test3 = color_dec == color_integer;
			EXPECT_TRUE(test && test1 && test2 && test3);
		}

		TEST(Perkunas, ColorCopyIsSame)
		{
			Color color{ 125, 125, 125, 125 };
			auto copy = color;
			EXPECT_EQ(color, copy);
		}

		TEST(Perkunas, ColorIntAndDoubleAsParameterBothValid)
		{
			Color color_int{ 127, 127, 127, 127 };
			Color color_dec{ 0.5, 0.5, 0.5, 0.5 };

			EXPECT_EQ(color_int, color_dec);
		}

		TEST(Perkunas, ColourChannelReceiveCorrectConversion)
		{
			for(Color::Channel i = 0; i < Color::Channel{ 255 }; i++)
			{
				int test_int = i;
				double test_double = i;
				double to_test_with = static_cast<int>(test_int) / 255.0;
				EXPECT_EQ(test_int, i);
				EXPECT_EQ(test_double, to_test_with);
			}
		}

		TEST(Perkunas, ColourRedHex)
		{
			Color color{ 255, 0, 0, 0 };
			uint32_t hex = color;
			const uint32_t red_hex = 0x00FF0000;
			EXPECT_EQ(hex, red_hex);
		}
		TEST(Perkunas, ColourGreenHex)
		{
			Color color{ 0, 255, 0, 0 };
			uint32_t hex = color;
			const uint32_t green_hex = 0x0000FF00;
			EXPECT_EQ(hex, green_hex);
		}
		TEST(Perkunas, ColourBlueHex)
		{
			Color color{ 0, 0, 255, 0 };
			uint32_t hex = color;
			const uint32_t blue_hex = 0x000000FF;
			EXPECT_EQ(hex, blue_hex);
		}
		TEST(Perkunas, ColourAlphaHex)
		{
			Color color{ 0, 0, 0, 255 };
			uint32_t hex = color;
			const uint32_t alpha_hex = 0xFF000000;
			EXPECT_EQ(hex, alpha_hex);
		}

		TEST(Perkunas, ColorAllMaxHex)
		{
			Color color{ 255, 255, 255, 255 };
			uint32_t hex = color;
			const uint32_t white_hex = 0xFFFFFFFF;
			EXPECT_EQ(hex, white_hex);
		}

		TEST(Perkunas, ColourChannelToHex)
		{
			Color color{ 137, 224, 160, 255 };
			uint32_t hex = color;
			
			const uint32_t some_hex = 0xFF89e0a0;
			EXPECT_EQ(hex, some_hex);
		}

		// A dummy namespace to "immitate" passing stuff to and receiving stuff from SDL
		// Unneeded or irrelevant parameters got excluded
		namespace SDL_Dummy
		{
			uint32_t g_color_key{0};
			void SDL_SetColorKey(uint32_t key)
			{
				g_color_key = key;
			}
			uint32_t SDL_GetColorKey()
			{
				return g_color_key;
			}

			uint8_t g_red;
			uint8_t g_green;
			uint8_t g_blue;
			uint8_t g_alpha;
			void SDL_SetRenderDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				g_red = r;
				g_green = g;
				g_blue = b;
				g_alpha = a;
			}
			void SDL_GetRendererDrawColor(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
			{
				(*r) = g_red;
				(*g) = g_green;
				(*b) = g_blue;
				(*a) = g_alpha;
			}

			TEST(Perkunas, SDLColourParamTest)
			{
				Color color{ 125, 126, 127, 128 };
				SDL_Dummy::SDL_SetColorKey(color);
				auto color_key = SDL_Dummy::SDL_GetColorKey();

				EXPECT_EQ(color, Color::create(color_key));

				uint8_t r{};
				uint8_t g{};
				uint8_t b{};
				uint8_t a{};
				SDL_Dummy::SDL_SetRenderDrawColor(color.red, color.green, color.blue, color.alpha);
				SDL_Dummy::SDL_GetRendererDrawColor(&r, &g, &b, &a);
				EXPECT_EQ(r, color.red);
				EXPECT_EQ(g, color.green);
				EXPECT_EQ(b, color.blue);
				EXPECT_EQ(a, color.alpha);

				Color renderer_color{ r, g, b, a };
				EXPECT_EQ(color, renderer_color);
			}
		}
		TEST(Perkunas, ColourStaticCreate)
		{
			auto color = Color::create(0xFFFFFFFF);
			const Color white{ 255, 255, 255, 255 };
			EXPECT_EQ(color, white);

			color = Color::create(0xFF0000FF);
			const Color blue{ 0, 0, 255, 255};
			EXPECT_EQ(color, blue);
		}
	}
	
}
