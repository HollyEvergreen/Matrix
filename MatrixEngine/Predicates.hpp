#include <string>
#include <functional>

namespace Matrix {
	class Predicates {
	public:
		static const bool TRUE = true;
		template <typename T>
		static bool In(std::vector<T> array, T object) {
			for (auto& _object : array) {
				if (std::strcmp(_object, object) == 0) return true;
			}
			return false;
		}

		template <typename T>
		static bool All(std::vector<T> array, std::function<bool(T)> action) {
			bool all = true;
			for (T& obj : array) {
				all &= action(obj);
			}
			return all;
		}
	};
}