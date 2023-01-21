#pragma once

// Slightly overengineered method of easy console coloring with inspiration from https://github.com/aafulei/color-console/
namespace dye {
	// 0x07 resets to the normal white text, black background
	void set_col(int color = 0x07) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	template <typename T>
	class con_col {
	public:
		con_col(T text, int col) : content(text), color(col) {};
		T content;
		int color;
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& os, con_col<T> input)
	{
		set_col(input.color);
		os << input.content;
		set_col();
		return os;
	}
	template<typename T>
	con_col<T> red(T text) {
		return con_col(text, 0x04);
	}
	template<typename T>
	con_col<T> green(T text) {
		return con_col(text, 0x02);
	}
}