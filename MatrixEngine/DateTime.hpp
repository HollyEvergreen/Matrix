#pragma once
#include <string>
#include <chrono>
#include "debug.hpp"
namespace Matrix {
	class DateTime : public DebugView<std::string> {
	public:
		enum DateTimeFormat {
			ddmmyy,
			ddmmyyyy,
			mmyy,
			ddmmyy_hh_mm_ss,
		};
		const std::string& ToString(DateTimeFormat fmt);
		const std::string& ToString(int fmt) { return ToString((DateTimeFormat)fmt); }
		operator const char*() { return ToString(ddmmyyyy).c_str(); }
	private:
		std::chrono::system_clock clk;
		int* now() const;
		std::string t = "";
	};
}