#pragma once
#include <fstream>
#include <string>

#include <Wiwa/core/Core.h>

typedef char sbyte;

namespace Wiwa {
	// Normal file
	class WI_API File {
		friend class FileSystem;
	private:
		std::fstream* fstream;

		bool is_open;
	public:
		File();
		~File();

		// Get line from file
		bool GetLine(std::string& str) const;

		// Read bytes of memory from file
		void Read(sbyte* memblock, size_t count);

		// Write bytes of memory to file
		void Write(const sbyte* memblock, size_t count);

		// Read bytes of memory from file
		void Read(void* memblock, size_t count);

		// Write bytes of memory to file
		void Write(const void* memblock, size_t count);

		// Check whether a file is open
		bool IsOpen() const { return is_open; }

		// Check if reached end of file
		bool Eof() const;

		// Check file size
		size_t Size();

		// Close file
		void Close();

		friend File& operator<<(File& file, std::string& str);
		friend File& operator<<(File& file, const char* str);
		friend File& operator<<(File& file, char ch);

		friend void operator>>(File& file, std::string& str);
		friend void operator>>(File& file, char& ch);
	};

	// Memory mapped file
	class WI_API FileMapping {
		friend class FileSystem;
	private:
		void* mapFile;
		sbyte* mapView;

		bool is_open;
	public:
		// Write bytes of data into the memory mapped file
		void Write(sbyte* data, size_t offset, size_t count);

		// Read bytes of data from the memory mapped file
		void Read(sbyte* data, size_t offset, size_t count);

		// Take raw data from the memory mapped file
		sbyte* data() { return mapView; }

		// Check if the memory mapped file is open
		bool IsOpen() const { return is_open; }

		// Close memory mapped file
		void Close();
	};

	// FileSystem
	class WI_API FileSystem
	{
	private:
		static void _toLower(std::string& str);

		FileSystem();
	public:
		enum OpenMode {
			OM_IN = 1,
			OM_OUT = 2,
			OM_ATE = 4,
			OM_APP = 8,
			OM_TRUNC = 16,
			OM_BINARY = 32
		};

		// Path utils
		static std::string RemoveFolderFromPath(std::string folder, std::string path);
		
		// Get filename from path
		static std::string GetFileName(std::string path);

		//======== File system utils ========
		// Check whether the file or directory exists
		static bool Exists(const char* path);

		// Check last write time of a file or directory
		static time_t LastWriteTime(const char* path);

		// Take file size in bytes
		static size_t FileSize(const char* file);

		// Get current working directory
		static std::string CurrentPath();

		// Set current working directory
		static void CurrentPath(const char* path);

		// Checks whether the given path refers to an empty file or directory
		static bool IsEmpty(const char* path);

		// Checks whether the two paths refer to the same file system object
		static bool IsEquivalent(const char* path1, const char* path2);

		// Create directory
		static bool CreateDir(const char* path);

		// Create directories recursively
		static bool CreateDirs(const char* path);

		// Create directory symlink
		static void CreateDirSymlink(const char* to, const char* symlink);

		// Create directory hardlink
		static void CreateHardlink(const char* to, const char* hardlink);

		// Create symlink
		static void CreateSymlink(const char* to, const char* symlink);

		// Removes a file or an empty directory
		static bool Remove(const char* path);

		// Removes a file or a directory and all its contents, recursively
		static bool RemoveAll(const char* path);

		//======== File management ========
		/* Directly read all bytes from file
		* Generates a buffer of size [read_bytes] into memblock pointer variable
		* return read_bytes;
		*/
		static size_t ReadAll(const char* file, sbyte** memblock);

		// Open file using OpenMode enums with bitwise operator (<<)
		static File Open(const char* file, int openMode);

		// Open text file for input
		static File OpenI(const char* file) { return Open(file, OM_IN); }

		// Open text file for output
		static File OpenO(const char* file) { return Open(file, OM_OUT); }

		// Open text file for input and output
		static File OpenIO(const char* file) { return Open(file, OM_IN | OM_OUT); }

		// Open binary file for input
		static File OpenIB(const char* file) { return Open(file, OM_IN | OM_BINARY); }

		// Open binary file for output
		static File OpenOB(const char* file) { return Open(file, OM_OUT | OM_BINARY); }

		// Open binary file for input and output
		static File OpenIOB(const char* file) { return Open(file, OM_IN | OM_OUT | OM_BINARY); }

		// Create memory mapped file
		static FileMapping CreateMemoryMappedFile(const char* name, size_t size);

		// Open memory mapped file
		static FileMapping OpenMemoryMappedFile(const char* name, size_t size);
	};
}