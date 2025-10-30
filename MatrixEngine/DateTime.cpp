#pragma once
#include "DateTime.hpp"
#include <time.h>

namespace Matrix {
	int* DateTime::now() const {
		time_t* t = new time_t();
		time(t);
		return (int*)gmtime(t);
	}
	const std::string& DateTime::ToString(DateTimeFormat fmt)
	{
		auto t = now();
		switch (fmt)
		{
		case Matrix::DateTime::ddmmyyyy:
			this->t = std::format(
				"{0}/{1}/{2}",
				t[3],
				t[4],
				t[5]
			);
			break;
		case Matrix::DateTime::ddmmyy_hh_mm_ss:
			this->t = std::format(
				"{3}/{4}/{5} {0}:{1}:{2}",
				t[0],
				t[1],
				t[2],
				t[3],
				t[4],
				std::to_string(t[5]).substr(2)
			);
			break;
		default:
			this->t = "invalid format specifier";
			break;
		}
		return this->t;
	}
}
