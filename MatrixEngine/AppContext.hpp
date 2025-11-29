#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <bitset>
#include "defines.h"
#include "FileSystem.hpp"
#include "AppContext_t.hpp"
#include <any>
static class AppContext
{
private:
	static std::unordered_map<std::string, acontext_t> context;
public:
	static void Init(std::initializer_list<std::pair<std::string, acontext_t>> init) {
		context = std::unordered_map<std::string, acontext_t>();
		for (auto& pair : init) {
			context.insert(pair);
		}
	}
	template <typename T>
	static const T& Get(const std::string& key) {
		return std::get<T>(context[key]._data);
	}
	template <typename T>
	static const T& Get(const std::string& key) {
		using ctxT = acontext_t::T;
		switch (context[key]._type)
		{
		case ctxT::Path:
			return Get<Matrix::Filesys::Path>(key);
		case ctxT::String:
			return Get<std::string>(key);
		case ctxT::Int:
			return Get<int>(key);
		case ctxT::Float:
			return Get<float>(key);
		case ctxT::Bit8:
			return Get<std::bitset<8>>(key);
		case ctxT::Bit16:
			return Get<std::bitset<16>>(key);
		case ctxT::Bit32:
			return Get<std::bitset<32>>(key);
		case ctxT::Bit64:
			return Get<std::bitset<64>>(key);
		case ctxT::version:
			return Get<Version>(key);
		default:
			break;
		}
		
	}
	static void Insert(const std::string& key, acontext_t value) {
		context.insert_or_assign(key, value);
	}
	static void Remove(const std::string& key) {
		context.erase(key);
	}
} _AppContext;

