#include "FileSystem.hpp"
namespace Matrix {
	namespace Filesys {
		const std::string& Path::path() {
			if (!_dirty)
				return _path;
			_path.clear();
			simplify();
			for (auto& component : path_components)
				_path += component + path_sep;
			_dirty = false;
			return _path;
		}
		inline Path& Path::append(std::string& component)
		{
			path_components.push_back(std::move(component));
			invalidate_cache();
			fchain
		}
		inline Path& Path::append(const char* component)
		{
			path_components.emplace_back(component);
			invalidate_cache();
			fchain
		}
		void Path::simplify(void)
		{
		}

		template<class T>
		T* File::Load(size_t count)
		{
			auto data = malloc(count);
			if (!data) return nullptr;
			fread_s(data, count, 1, count, _file);
			return (T*)data;
		}
		template<class T>
		inline File& File::Write(T* obj, int count)
		{
			fwrite(obj, sizeof(T), count, _file);
		}
		inline std::string File::loadAllText()
		{
			return std::string(Load<std::string::value_type>(Size()));
		}
		inline File FileManager::LoadFile(Path path, OpenMode mode)
		{
			return File(path, mode);
		}
		inline void FileManager::RemoveFile(Path path)
		{
			remove(path.path().c_str());
		}
		inline void FileManager::RemoveDir(Path path)
		{
			remove(path.path().c_str());
		}
		inline void FileManager::RemoveFile(File file)
		{
			RemoveFile(file._path);
		}
		inline void FileManager::RemoveDir(Directory dir)
		{
			RemoveDir(dir._path);
		}
	}
}