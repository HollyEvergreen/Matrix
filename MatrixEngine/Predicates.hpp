#include <string>

namespace Matrix {
	class Predicates {
	public:
		static const bool All = true;
		template <typename T>
		static bool In(std::vector<T> array, T object) {
			for (auto& _object : array) {
				if (std::strcmp(_object, object) == 0) return true;
			}
			return false;
		}
	};
}