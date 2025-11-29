#define __case(name) \
	case _##name: \
		_type = name;
#define GenTypeSwitch switch ((THC)typeid(data).hash_code()){__case(Path)__case(String)__case(Int)__case(Float)__case(Bit8)__case(Bit16)__case(Bit32)__case(Bit64)__case(version)};

struct acontext_t {
	enum THC {
		_Path = 8559957141500673660,
		_String = 10283618467285603348,
		_Int = 12638232278978672507,
		_Float = 12638226781420530164,
		_Bit8 = 18139419958109338083,
		_Bit16 = 1595286131316390077,
		_Bit32 = 9757118343466150806,
		_Bit64 = 14501082883272283677,
		_version = 8772101540053042101
	}; 
	enum T {
		Path,
		String,
		Int,
		Float,
		Bit8,
		Bit16,
		Bit32,
		Bit64,
		version 
	};
	using DT =
		std::variant<
		Matrix::Filesys::Path,
		std::string,
		int,
		float,
		std::bitset<8>,
		std::bitset<16>,
		std::bitset<32>,
		std::bitset<64>,
		Version
		>;
	T _type;
	std::variant<
		Matrix::Filesys::Path,
		std::string,
		int,
		float,
		std::bitset<8>,
		std::bitset<16>,
		std::bitset<32>,
		std::bitset<64>,
		Version
	> _data;

	template<typename Type>
	acontext_t(Type data) {
		_data = data;
		switch ((THC)typeid(data).hash_code()) {
		case _Path: _type = Path; 
		case _String: _type = String; 
		case _Int: _type = Int; 
		case _Float: _type = Float; 
		case _Bit8: _type = Bit8; 
		case _Bit16: _type = Bit16; 
		case _Bit32: _type = Bit32; 
		case _Bit64: _type = Bit64; 
		case _version: _type = version;
		}; ()
	}
	acontext_t(){
		_type = (T)- 1;
	}
};

