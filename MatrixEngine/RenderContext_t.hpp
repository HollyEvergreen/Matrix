#include <vulkan/vulkan.hpp>

struct rcontext_t {
	enum T : size_t{
		ImCtx = 6884846640886027091,
		VkCtx = 12179018493042638006,
		VkInst = 2402733210811619884,
		GLFWwin_ptr = 10835721306568818330,
		CharVector = 2272760371463628997,
		Int = 12638232278978672507,
		Float = 12638226781420530164,
		Bit8 = 18139419958109338083,
		Bit16 = 1595286131316390077,
		Bit32 = 9757118343466150806,
		Bit64 = 14501082883272283677,
		version = 8772101540053042101
	};
	using DT = std::variant<
		std::weak_ptr<ImGuiContext>,
		std::weak_ptr<VulkanContext>,
		std::weak_ptr<VulkanInstance>,
		GLFWwindow*,
		std::vector<const char*>,
		int,
		float,
		std::bitset<8>,
		std::bitset<16>,
		std::bitset<32>,
		std::bitset<64>,
		Version>;
	T _type;
	DT _data;

	template<typename Type>
	rcontext_t(Type data) {
		_data = data;
		_type = (T)typeid(data).hash_code();
	}
	rcontext_t(){}
};