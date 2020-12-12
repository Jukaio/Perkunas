
#include "pch.h"
#include <perkunas.h>


using namespace std;
using namespace ::testing;

namespace perkunas
{
	TEST(Perkunas, Window)
	{

	}

}

namespace playground
{
	string convert(uint32_t arabic)
	{
		const auto arabic_to_roman_conversions = { make_pair(1000u, "M"),
												   make_pair(900u, "CM"),
												   make_pair(500u, "D"),
												   make_pair(400u, "CD"),
												   make_pair(100u, "C"),
												   make_pair(90u, "XC"),
												   make_pair(50u, "L"),
												   make_pair(40u, "XL"),
												   make_pair(10u, "X"),
												   make_pair(9u, "IX"),
												   make_pair(5u, "V"),
												   make_pair(4u, "IV"),
												   make_pair(1u, "I") };

		string roman("");
		for(auto arabic_to_roman : arabic_to_roman_conversions)
		{
			while(arabic >= arabic_to_roman.first)
			{
				roman += arabic_to_roman.second;
				arabic -= arabic_to_roman.first;
			}
		}
		return roman;
	}

	string fizz_buzz(uint32_t integer)
	{
		if(!integer)
			return "0";

		const static uint32_t cases = 5;
		const static string word_lookup[cases] = { "", "Fizz", "Buzz", "Whizz", "Bang"};
		const static uint32_t devider_lookup[cases] = { 0, 3, 5, 7, 13 };

		string to_return{ "" };
		for(int i = 1; i < cases; i++)
		{
			bool is_any = !(integer % devider_lookup[i]);
			to_return += word_lookup[i * is_any];
		}

		if(to_return.empty())
			return to_string(integer);
		return to_return;
	}

	TEST(FizzBuzz, Number)
	{
		EXPECT_EQ(fizz_buzz(0), "0");
		EXPECT_EQ(fizz_buzz(1), "1");
		EXPECT_EQ(fizz_buzz(2), "2");
		EXPECT_EQ(fizz_buzz(4), "4");
		EXPECT_EQ(fizz_buzz(8), "8");
		EXPECT_EQ(fizz_buzz(11), "11");
	}

	TEST(FizzBuzz, Fizz)
	{
		EXPECT_EQ(fizz_buzz(3), "Fizz");
		EXPECT_EQ(fizz_buzz(6), "Fizz");
		EXPECT_EQ(fizz_buzz(9), "Fizz");
	}

	TEST(FizzBuzz, Buzz)
	{
		EXPECT_EQ(fizz_buzz(5), "Buzz");
		EXPECT_EQ(fizz_buzz(10), "Buzz");
		EXPECT_EQ(fizz_buzz(20), "Buzz");
	}

	TEST(FizzBuzz, Whizz)
	{
		EXPECT_EQ(fizz_buzz(7), "Whizz");
		EXPECT_EQ(fizz_buzz(14), "Whizz");
		EXPECT_EQ(fizz_buzz(28), "Whizz");
	}

	TEST(FizzBuzz, FizzWhizz)
	{
		EXPECT_EQ(fizz_buzz(21), "FizzWhizz");
		EXPECT_EQ(fizz_buzz(42), "FizzWhizz");
	}

	TEST(FizzBuzz, BuzzWhizz)
	{
		EXPECT_EQ(fizz_buzz(35), "BuzzWhizz");
		EXPECT_EQ(fizz_buzz(70), "BuzzWhizz");
	}

	TEST(FizzBuzz, FizzBuzz)
	{
		for(int i = 15; i < 101; i += 15)
		{
			EXPECT_EQ(fizz_buzz(i), "FizzBuzz");
		}
	}

	TEST(FizzBuzz, FizzBuzzWhizz)
	{
		EXPECT_EQ(fizz_buzz(105), "FizzBuzzWhizz");
	}




	TEST(TestArabicToRomanDigitConverter, I_Digits) {
		EXPECT_EQ(convert(1), "I");
		EXPECT_EQ(convert(2), "II");
		EXPECT_EQ(convert(3), "III");
	}

	TEST(TestArabicToRomanDigitConverter, V_Digits)
	{
		EXPECT_EQ(convert(4), "IV");
		EXPECT_EQ(convert(5), "V");
		EXPECT_EQ(convert(6), "VI");
		EXPECT_EQ(convert(7), "VII");
	}

	TEST(TestArabicToRomanDigitConverter, X_Digits)
	{
		EXPECT_EQ(convert(9), "IX");
		EXPECT_EQ(convert(10), "X");
		EXPECT_EQ(convert(21), "XXI");
	}

	TEST(TestArabicToRomanDigitConverter, L_Digits)
	{
		EXPECT_EQ(convert(50), "L");
		EXPECT_EQ(convert(51), "LI");
		EXPECT_EQ(convert(55), "LV");
	}

	TEST(TestArabicToRomanDigitConverter, C_Digits)
	{
		EXPECT_EQ(convert(100), "C");
		EXPECT_EQ(convert(101), "CI");
		EXPECT_EQ(convert(156), "CLVI");
	}


	TEST(TestArabicToRomanDigitConverter, D_Digits)
	{
		EXPECT_EQ(convert(500), "D");
		EXPECT_EQ(convert(506), "DVI");
		EXPECT_EQ(convert(400), "CD");
	}

	TEST(TestArabicToRomanDigitConverter, M_Digits)
	{
		EXPECT_EQ(convert(1000), "M");
		EXPECT_EQ(convert(900), "CM");
		EXPECT_EQ(convert(2335), "MMCCCXXXV");
	}
}