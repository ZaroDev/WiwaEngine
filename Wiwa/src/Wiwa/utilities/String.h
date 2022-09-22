#pragma once
#include "Empathy/Core.h"

#include <iostream>

namespace EM {
	class EM_API String
	{
	private:
		size_t m_Length;
		char* m_Text;
	public:
		String(size_t len = 1);

		String(const char* text);
		String(const String& str);

		~String();

		char* data() const { return m_Text; }
		const char* c_str() const { return m_Text; }

		size_t length() const { return m_Length; }
		size_t end() const { return m_Length; }

		// Indexing
		char& operator[](int index);
		char& operator[](int index) const;

		// Operations
		void setLength(size_t len);

		void append(const char* text);
		void append(const String& str);

		void append(char c);

		void set(const char* text);
		void set(const String& str);

		size_t find(char c, size_t startIndex = 0) const;
		size_t find(const char* text, size_t startIndex = 0) const;
		size_t find(const String& str, size_t startIndex = 0) const;

		size_t findb(char c, size_t startIndex = 0) const;
		size_t findb(const char* text, size_t startIndex = 0) const;
		size_t findb(const String& str, size_t startIndex = 0) const;

		String substr(size_t index, size_t count) const;
		String substr(size_t index) const;

		size_t replace(const char* text1, const char* text2, size_t startIndex = 0);
		void replaceAll(const char* text1, const char* text2);

		void toLower();
		void toUpper();

		// Operators
		void operator=(const char* text);
		void operator=(const String& str);

		void operator+=(const char* text);
		void operator+=(const String& str);
		void operator+=(char c);
	};

	// Output
	EM_API std::ostream& operator<< (std::ostream& out, const String& str);

	// Comparators
	EM_API bool operator==(const String& str1, const String& str2);
	EM_API bool operator==(const String& str, const char* text);

	// Operators
	EM_API String operator+(const String& str1, const String& str2);
}