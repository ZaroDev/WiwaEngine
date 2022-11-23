#include <wipch.h>
#include "FileSystem.h"
#include <fstream>
#include <Windows.h>
#include <string>

size_t FileSystem::ReadAll(const char* file, sbyte* memblock)
{
	std::ifstream f(file, std::ios::in | std::ios::binary | std::ios::ate);

	size_t size = 0;

	if (f.is_open()) {
		size = f.tellg();
		memblock = new sbyte[size];
		f.seekg(0, std::ios::beg);
		f.read(memblock, size);
		f.close();
	}

	return size;
}

File FileSystem::Open(const char* file, int openmode)
{
	File f;
	f.fstream->open(file, openmode);
	f.is_open = f.fstream->is_open();

	return f;
}

FileMapping FileSystem::CreateMemoryMappedFile(const char* name, size_t size)
{
	FileMapping fmap;
	fmap.mapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
	fmap.is_open = true;

	if (!fmap.mapFile) {
		fmap.is_open = false;
	}
	else {
		fmap.mapView = (char*)MapViewOfFile(fmap.mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);

		if (!fmap.mapView) {
			fmap.is_open = false;
		}
	}

	return fmap;
}

FileMapping FileSystem::OpenMemoryMappedFile(const char* name, size_t size)
{
	FileMapping fmap;
	fmap.mapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);
	fmap.is_open = true;

	if (!fmap.mapFile) {
		fmap.is_open = false;
	}
	else {
		fmap.mapView = (char*)MapViewOfFile(fmap.mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);

		if (!fmap.mapView) {
			fmap.is_open = false;
		}
	}

	return fmap;
}

File::File()
{
	fstream = new std::fstream();
	is_open = false;
}

File::~File()
{
	
}

bool File::GetLine(std::string& str) const
{
	return (bool)std::getline(*fstream, str);
}

void File::Read(sbyte* memblock, size_t count)
{
	fstream->read(memblock, count);
}

void File::Write(const sbyte* memblock, size_t count)
{
	fstream->write(memblock, count);
}

void File::Read(void* memblock, size_t count)
{
	fstream->read((sbyte*)memblock, count);
}

void File::Write(const void* memblock, size_t count)
{
	fstream->write((const sbyte*)memblock, count);
}

bool File::Eof() const
{
	return fstream->eof();
}

size_t File::Size()
{
	size_t beg = fstream->tellg();
	fstream->seekg(0, std::ios::end);
	size_t end = fstream->tellg();
	fstream->seekg(0, std::ios::beg);

	return end - beg;
}

void File::Close()
{
	fstream->close();
	delete fstream;
}

void FileMapping::Write(sbyte* data, size_t offset, size_t count)
{
	memcpy(mapView + offset, data, count);
}

void FileMapping::Read(sbyte* data, size_t offset, size_t count)
{
	memcpy(data, mapView + offset, count);
}

void FileMapping::Close()
{
	UnmapViewOfFile(mapView);
	CloseHandle(mapFile);
}

File& operator<<(File& file, std::string& str)
{
	(*file.fstream) << str;

	return file;
}

File& operator<<(File& file, const char* str)
{
	(*file.fstream) << str;

	return file;
}

File& operator<<(File& file, char ch)
{
	(*file.fstream) << ch;

	return file;
}

void operator>>(File& file, std::string& str)
{
	(*file.fstream) >> str;
}

void operator>>(File& file, char& ch)
{
	(*file.fstream) >> ch;
}