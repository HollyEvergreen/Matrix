#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <format>
#include <DateTime.hpp>
//#include <Debug.hpp>
#include <Defines.h>

namespace Matrix { 
	namespace Filesys {
		// 
		// $ prefix indicates the component is an environment var that should be resolved immediately
		// # prefix indicates the component is stored in the global app context
		// - prefix indicates the component is a drive name
		//
		class Path {
		public:
			inline Path(std::initializer_list<std::string> _path) {
				path_components = _path;
				simplify();
				path();
			}
			inline Path() {
				path_components = { "-C" };
				simplify();
				path();  
			}
			const std::string& path();
			Path& append(std::string& component);
			Path& append(const char* component);
			inline Path& join(std::initializer_list<const char*> ilist) {
				Extend(ilist.size());
				path_components.insert(path_components.cend(), ilist.begin(), ilist.end());
				_dirty = true;
				fchain
			}
			static inline Path GetHomePath() {
				return Path({"-C", "Users", "$User", "AppData", "Roaming", "#AppName"});
			}
			inline void Extend(size_t n) { path_components.reserve(n); _dirty = true; }
			inline void invalidate_cache(){ _dirty = true; }

			operator const char* () { return path().c_str(); }
			operator std::string () { return path(); }

		private:
			void simplify(void);
			char path_sep = '/';
			bool _dirty = true;
			std::vector<std::string> path_components;
			std::string _path;
		};

		class Item {
		public:
			Item() = default;
			Path _path = {"-C"};
			
		};

		class Directory : public Item {
		public:
			
		};

		enum FileType : int8_t{
			unknown = -1,
			Material,
			Shader,
			Model_obj,
			Log,
		};

		enum OpenMode : uint8_t {
			WriteOnly,
			ReadWrite,
			Append,
			ReadAndAppend,
			BinaryWriteOnly,
			BinaryReadWrite,
			BinaryAppend,
			BinaryReadAndAppend
		};
		static const char* modes[] = {
			"w",    // WriteOnly
			"r+",   // ReadWrite
			"a",    // Append
			"a+",   // ReadAndAppend
			"wb",   // BinaryWriteOnly
			"rb+",  // BinaryReadWrite
			"ab",   // BinaryAppend
			"ab+"   // BinaryReadAndAppend
		};

		class File : public Item {
		public:
			inline File(Path path, OpenMode mode) {
				fopen_s(&_file, path, modes[mode]);
				_type = FileType::unknown;
				Size();
			}
			inline File(Path path, OpenMode mode, FileType _type) {
				fopen_s(&_file, path, modes[mode]);
				this->_type = _type;
				Size();
			}
			inline size_t Size() { _size = fseek(_file, 0, 2 /*SEEK_END*/); fseek(_file, 0, 1 /*SEEK_CUR*/); return _size; }
			inline FileType GetType() const { return _type; }
			static inline File* Create(FileType _type, Path _path){
				return new File(_path, ReadWrite, _type);
			}

			template<class T>
			T* Load(size_t count);
			template<class T>
			File& Write(T* obj, int count = 1);
			template<class T>
			static File& Write(File* f, T* obj, int count=1);
			std::string loadAllText();
		private:
			FileType _type;
			FILE* _file;
			size_t _size;
		};

		class FileManager {
		public:
			friend class File;
			friend class Directory;
			friend class Item;
			friend class Path;

			inline FileManager(Path path, bool isExtern = false) {
				this->HomePath = path;
				this->isExtern = isExtern;
			}

			Path HomePath = Path::GetHomePath();
			bool isExtern = false;

			File LoadFile(Path path, OpenMode mode);
			void RemoveFile(Path path);
			void RemoveDir(Path path);
			void RemoveFile(File file);
			void RemoveDir(Directory dir);

			// Pass in subdirectories one at a time with function training
			// e.g
			// fileManager.cd("sub1")
			//	     .cd("sub2")
			//
			inline FileManager& cd(const char* subdir) {
				HomePath.append(subdir);
				fchain
			}

			inline FileManager& cd(std::initializer_list<const char*> subdirs) {
				HomePath.Extend(subdirs.size());
				for(auto& subdir : subdirs)
					HomePath.append(subdir);
				fchain
			}

			static inline File* GetLogFile() {
				const std::string Now = DateTime().ToString(DateTime::ddmmyyyy);
				Path LogPath = Path::GetHomePath().join({ "Log", std::format("{0}.Log", Now).c_str()});
				return File::Create(FileType::Log, LogPath);
			}
		private:
		};
	} 
}