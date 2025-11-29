#pragma once
#include <array>
#include <unordered_map>
typedef std::array<int, 3> Version;

#define to_vk_native(t, x) static_cast<t::NativeType>(x)
#define to_vk_native_ptr(t, x) static_cast<t::NativeType*>(x)

#define vk_ops(vk_type, member) \
operator vk_type() const { return member; } \
operator vk_type::NativeType() const { return to_vk_native(vk_type, member); }

#define sptr(t) std::shared_ptr<t>

enum ConsoleColour {
	// Regular colors
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,

	// Bright colors
	BRIGHT_BLACK,
	BRIGHT_RED,
	BRIGHT_GREEN,
	BRIGHT_YELLOW,
	BRIGHT_BLUE,
	BRIGHT_MAGENTA,
	BRIGHT_CYAN,
	BRIGHT_WHITE,

	// Background colors
	BG_BLACK,
	BG_RED,
	BG_GREEN,
	BG_YELLOW,
	BG_BLUE,
	BG_MAGENTA,
	BG_CYAN,
	BG_WHITE,

	// Bright background colors
	BG_BRIGHT_BLACK,
	BG_BRIGHT_RED,
	BG_BRIGHT_GREEN,
	BG_BRIGHT_YELLOW,
	BG_BRIGHT_BLUE,
	BG_BRIGHT_MAGENTA,
	BG_BRIGHT_CYAN,
	BG_BRIGHT_WHITE,
};

static std::unordered_map<ConsoleColour, const char*> ColourCodes = {
	// Regular colors
	{BLACK, "\033[30m"},
	{RED, "\033[31m"},
	{GREEN, "\033[32m"},
	{YELLOW, "\033[33m"},
	{BLUE, "\033[34m"},
	{MAGENTA, "\033[35m"},
	{CYAN, "\033[36m"},
	{WHITE, "\033[37m"},

	// Bright colors
	{BRIGHT_BLACK, "\033[90m"},
	{BRIGHT_RED, "\033[91m"},
	{BRIGHT_GREEN, "\033[92m"},
	{BRIGHT_YELLOW, "\033[93m"},
	{BRIGHT_BLUE, "\033[94m"},
	{BRIGHT_MAGENTA, "\033[95m"},
	{BRIGHT_CYAN, "\033[96m"},
	{BRIGHT_WHITE, "\033[97m"},

	// Background colors
	{BG_BLACK, "\033[40m"},
	{BG_RED, "\033[41m"},
	{BG_GREEN, "\033[42m"},
	{BG_YELLOW, "\033[43m"},
	{BG_BLUE, "\033[44m"},
	{BG_MAGENTA, "\033[45m"},
	{BG_CYAN, "\033[46m"},
	{BG_WHITE, "\033[47m"},

	// Bright background colors
	{BG_BRIGHT_BLACK, "\033[100m"},
	{BG_BRIGHT_RED, "\033[101m"},
	{BG_BRIGHT_GREEN, "\033[102m"},
	{BG_BRIGHT_YELLOW, "\033[103m"},
	{BG_BRIGHT_BLUE, "\033[104m"},
	{BG_BRIGHT_MAGENTA, "\033[105m"},
	{BG_BRIGHT_CYAN, "\033[106m"},
	{BG_BRIGHT_WHITE, "\033[107m"}
};

enum ConsoleStyling {
	// Text styling
	RESET,
	BOLD,
	DIM,
	ITALIC,
	UNDERLINE,
	BLINK,
	REVERSE,
	HIDDEN
};

static std::unordered_map<ConsoleStyling, const char*> StylingCodes = {
	// Text styling
	{ RESET, "\033[0m" },
	{ BOLD, "\033[1m" },
	{ DIM, "\033[2m" },
	{ ITALIC, "\033[3m" },
	{ UNDERLINE, "\033[4m" },
	{ BLINK, "\033[5m" },
	{ REVERSE, "\033[7m" },
	{ HIDDEN, "\033[8m" }
};

#define ResetStyling std::cout<<StylingCodes[RESET]<<ColourCodes[GREEN];

#define style(str, col) std::format("{}{}{}", ColourCodes[col], str, ColourCodes[GREEN])
#define style_static(str, col) ColourCodes[col] << str << ColourCodes[GREEN]

#define class_name typeid(*this).name()

#define fchain return *this;