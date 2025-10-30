#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <bitset>
#include "defines.h"

static class AppContext //: public Serializable
{
private:
	using context_t = std::variant<
		std::string, 
		int, 
		float,
		std::bitset<8>,
		std::bitset<16>,
		std::bitset<32>,
		std::bitset<64>,
		Version
	>;
	static std::unordered_map<std::string, context_t> context;
public:
	static void Init(std::initializer_list<std::pair<std::string, context_t>> init) {
		context = std::unordered_map<std::string, context_t>();
		for (auto& pair : init) {
			context.insert(pair);
		}
	}
	template <typename T>
	static const T& Get(const std::string& key) {
		return std::get<T>(context[key]);
	}
	static void Insert(const std::string& key, context_t value) {
		context.insert_or_assign(key, value);
	}
	static void Remove(const std::string& key) {
		context.erase(key);
	}
} _AppContext;
