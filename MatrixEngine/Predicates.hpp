#include <string>
#include <functional>

namespace Matrix {
	class Predicates {
	public:
		static const bool TRUE = true;
		template <typename T>
		static bool In(std::vector<T> array, T object) {
			for (auto& _object : array) {
				if (Equivalent(_object, object)) 
					return true;
			}
			return false;
		}
		template <typename T, size_t size>
		static bool In(T* _array, T object) {
			for (size_t i = 0; i < size; i++) {
				if (Equivalent(_array[i], object))
					return true;
			}
			return false;
		}
		/*template <typename T, size_t size>
		static bool In(T _array[], T object) {
			for (size_t i = 0; i < size; i++) {
				if (Equivalent(_array[i], object))
					return true;
			}
			return false;
		}*/
		template <typename T>
		static bool Equivalent(T a, T b) {
			return a == b;
		}
		static bool Equivalent(char* a, char* b) {
			return std::strcmp(a, b) == 0;
		}
		static bool Is(void* a, void* b) {
			return a == b;
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