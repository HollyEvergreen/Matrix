#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <bitset>
#include <memory>

#include <imgui.h>
#include "defines.h"
#include "../RenderDemon/VulkanContext.hpp"
#include "../RenderDemon/VulkanInstance.hpp"
#include "RenderContext_t.hpp"
class RenderContext //: public Serializable
{
private:
	using context_t = rcontext_t;
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
		return std::get<T>(context[key]._data);
	}
	static void Insert(const std::string& key, context_t value) {
		context.insert_or_assign(key, value);
	}
	static void Remove(const std::string& key) {
		context.erase(key);
	}
};
