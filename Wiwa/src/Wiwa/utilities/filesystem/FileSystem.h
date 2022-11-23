#pragma once
#include <fstream>
#include <string>

typedef char sbyte;

// Normal file
class File {
	friend class FileSystem;
private:
	std::fstream* fstream;

	bool is_open;
public:
	File();
	~File();

	bool GetLine(std::string& str) const;

	void Read(sbyte* memblock, size_t count);
	void Write(const sbyte* memblock, size_t count);

	void Read(void* memblock, size_t count);
	void Write(const void* memblock, size_t count);

	bool IsOpen() const { return is_open; }

	bool Eof() const;
	size_t Size();

	void Close();

	friend File& operator<<(File& file, std::string& str);
	friend File& operator<<(File& file, const char* str);
	friend File& operator<<(File& file, char ch);

	friend void operator>>(File& file, std::string& str);
	friend void operator>>(File& file, char& ch);
};

// Memory mapped file
class FileMapping {
	friend class FileSystem;
private:
	void* mapFile;
	sbyte* mapView;

	bool is_open;
public:
	void Write(sbyte* data, size_t offset, size_t count);
	void Read(sbyte* data, size_t offset, size_t count);

	sbyte* data() { return mapView; }

	bool IsOpen() const { return is_open; }
	void Close();
};

// FileSystem
class FileSystem
{
private:
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

	static size_t ReadAll(const char* file, sbyte* memblock);

	static File Open(const char* file, int openMode);

	static File OpenI(const char* file) { return Open(file, OM_IN); }
	static File OpenO(const char* file) { return Open(file, OM_OUT); }
	static File OpenIO(const char* file) { return Open(file, OM_IN | OM_OUT); }

	static File OpenIB(const char* file) { return Open(file, OM_IN | OM_BINARY); }
	static File OpenOB(const char* file) { return Open(file, OM_OUT | OM_BINARY); }
	static File OpenIOB(const char* file) { return Open(file, OM_IN | OM_OUT | OM_BINARY); }

	static FileMapping CreateMemoryMappedFile(const char* name, size_t size);
	static FileMapping OpenMemoryMappedFile(const char* name, size_t size);
};